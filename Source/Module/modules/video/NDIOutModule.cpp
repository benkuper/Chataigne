#include "NDIOutModule.h"
#include <algorithm>
#include <cmath>

using namespace juce;

namespace
{
    std::atomic<uint64_t> nextNDIRouteId { 1 };
    std::mutex ndiModulesLock;
    std::vector<NDIOutModule*> ndiModules;
    constexpr int ndiAudioRate = 48000;
}

NDIOutput::NDIOutput() : BaseItem("NDI Output")
{
    routeId = nextNDIRouteId.fetch_add(1, std::memory_order_relaxed);
    streamName = addStringParameter("Stream Name", "NDI source name", "Chataigne");
    width = addIntParameter("Width", "Output width in pixels", 1280, 16, 8192);
    height = addIntParameter("Height", "Output height in pixels", 720, 16, 8192);
    fps = addFloatParameter("FPS", "Output frame rate", 30.0f, 1.0f, 120.0f);
    testCard = addBoolParameter("Test Card", "Show a test card on this output", false);
}

void NDIOutput::onContainerParameterChangedInternal(Parameter* p)
{
    if (module != nullptr) module->updateOutput();
}

void NDIOutput::onContainerNiceNameChanged()
{
    BaseItem::onContainerNiceNameChanged();
    if (module != nullptr) module->updateOutput();
}

NDIOutputManager::NDIOutputManager(NDIOutModule* owner) :
    BaseManager("Outputs"), owner(owner)
{
    selectItemWhenCreated = false;
    autoReorderOnAdd = false;
    itemDataType = "NDI Output";
}

NDIOutput* NDIOutputManager::createItem()
{
    auto* result = new NDIOutput();
    result->module = owner;
    int number = 1;
    for (;; ++number)
    {
        const String proposed = number == 1 ? "Chataigne" : "Chataigne " + String(number);
        bool taken = false;
        for (auto* existing : items)
            if (existing != nullptr && existing->streamName->stringValue() == proposed)
                taken = true;
        if (!taken)
        {
            result->setNiceName("Output " + String(number));
            result->streamName->setValue(proposed);
            return result;
        }
    }
}

NDIOutModule::NDIOutModule() :
    Module(getTypeString()), Thread("NDI out sender"), outputs(this)
{
    setupIOConfiguration(false, false);
    hideScripts();
    outputs.addBaseManagerListener(this);
    moduleParams.addChildControllableContainer(&outputs);

    // A fresh module starts with one output. The manager adds/removes the rest.
    outputs.addItem(nullptr, var(), false);

    {
        const std::lock_guard<std::mutex> guard(ndiModulesLock);
        ndiModules.push_back(this);
    }
    updateOutput();
    startThread(Priority::low);
}

NDIOutModule::~NDIOutModule()
{
    shutdown();
}

void NDIOutModule::shutdown()
{
    if (cleanedUp) return;
    cleanedUp = true;

    {
        const std::lock_guard<std::mutex> guard(ndiModulesLock);
        ndiModules.erase(std::remove(ndiModules.begin(), ndiModules.end(), this), ndiModules.end());
    }

    outputs.removeBaseManagerListener(this);
    signalThreadShouldExit();
    stateSignal.notify_all();
    stopThread(-1); // Destroys senders before surfaces or the module disappear.

    const std::lock_guard<std::mutex> guard(stateLock);
    for (auto& entry : surfaces)
        entry.second->hideGL();
    surfaces.clear();
    desiredOutputs.clear();
}

void NDIOutModule::updateOutput()
{
    if (cleanedUp) return;
    std::vector<OutputConfig> wanted;
    if (enabled->boolValue())
    {
        for (auto* output : outputs.items)
        {
            if (output == nullptr || !output->enabled->boolValue()) continue;
            const String name = output->streamName->stringValue().trim();
            if (name.isEmpty()) continue;

            OutputConfig c;
            c.id = output->routeId;
            c.name = name;
            c.width = jlimit(16, 8192, output->width->intValue());
            c.height = jlimit(16, 8192, output->height->intValue());
            c.frameRate = jmax(1000, roundToInt(output->fps->floatValue() * 1000.0f));
            wanted.push_back(c);
        }
    }

    std::vector<uint64_t> activeIDs;
    for (const auto& c : wanted) activeIDs.push_back(c.id);

    {
        const std::lock_guard<std::mutex> guard(stateLock);
        for (auto it = surfaces.begin(); it != surfaces.end();)
        {
            const bool keep = std::any_of(wanted.begin(), wanted.end(),
                [id = it->first](const OutputConfig& c) { return c.id == id; });
            if (keep) { ++it; continue; }
            it->second->hideGL();
            it = surfaces.erase(it);
        }

        for (const auto& c : wanted)
        {
            auto& surface = surfaces[c.id];
            if (surface == nullptr)
            {
                surface.reset(new CompositionRenderer::CompositionSurface(VideoGLContext::getInstance()));
                surface->outputFilter = c.id;
                surface->setInterceptsMouseClicks(false, false);
                surface->setWantsKeyboardFocus(false);
            }
            surface->setSize(c.width, c.height);

            CompositionRenderer::SceneSettings settings;
            settings.blackBackground = true;
            settings.testCardMonitorWidth = c.width;
            settings.testCardMonitorHeight = c.height;
            for (auto* output : outputs.items)
                if (output != nullptr && output->routeId == c.id)
                {
                    settings.testCard = output->testCard->boolValue();
                    break;
                }
            surface->settings = settings;
            surface->showGL();
        }

        desiredOutputs = std::move(wanted);
        configDirty = true;
    }
    {
        const std::lock_guard<std::mutex> guard(audioLock);
        for (auto it = audioSources.begin(); it != audioSources.end();)
        {
            const bool stillEnabled = std::any_of(activeIDs.begin(), activeIDs.end(),
                [id = it->second.routeId](uint64_t active) { return active == id; });
            if (stillEnabled) ++it;
            else it = audioSources.erase(it);
        }
    }
    stateSignal.notify_one();
}

void NDIOutModule::itemAdded(NDIOutput* output)
{
    if (output != nullptr) output->module = this;
    updateOutput();
}

void NDIOutModule::itemsAdded(Array<NDIOutput*> added)
{
    for (auto* output : added) if (output != nullptr) output->module = this;
    updateOutput();
}

void NDIOutModule::itemRemoved(NDIOutput*) { updateOutput(); }
void NDIOutModule::itemsRemoved(Array<NDIOutput*>) { updateOutput(); }
void NDIOutModule::itemsReordered() { updateOutput(); }

void NDIOutModule::onContainerParameterChangedInternal(Parameter* p)
{
    Module::onContainerParameterChangedInternal(p);
    if (p == enabled) updateOutput();
}

void NDIOutModule::run()
{
    using Clock = std::chrono::steady_clock;
    while (!threadShouldExit())
    {
        std::vector<OutputConfig> wanted;
        bool dirty = false;
        {
            const std::lock_guard<std::mutex> guard(stateLock);
            dirty = configDirty;
            if (dirty)
            {
                wanted = desiredOutputs;
                configDirty = false;
            }
        }
        if (dirty) configureSenders(wanted);
        if (threadShouldExit()) break;

        const auto now = Clock::now();
        auto wake = now + std::chrono::milliseconds(100);
        for (auto& entry : senders)
        {
            auto& s = entry.second;
            if (s.sender == nullptr) continue;
            if (s.nextDue <= now)
            {
                sendLatestFrame(entry.first, s);
                sendLatestAudio(entry.first, s);
                const auto interval = std::chrono::duration_cast<Clock::duration>(
                    std::chrono::duration<double>(1000000.0 / s.frameRate / 1000.0));
                s.nextDue += interval;
                if (s.nextDue < Clock::now()) s.nextDue = Clock::now();
            }
            if (s.nextDue < wake) wake = s.nextDue;
        }
        std::unique_lock<std::mutex> lock(stateLock);
        stateSignal.wait_until(lock, wake, [this] { return configDirty || threadShouldExit(); });
    }
    for (auto& entry : senders) destroySender(entry.second);
    senders.clear();
}

void NDIOutModule::destroySender(SenderState& s)
{
    if (s.sender != nullptr)
        VideoNetworkRuntime::NDI::get().send_destroy(s.sender);
    s.sender = nullptr;
    s.name.clear();
    s.audioFraction = 0;
}

void NDIOutModule::configureSenders(const std::vector<OutputConfig>& wanted)
{
    for (auto it = senders.begin(); it != senders.end();)
    {
        const bool keep = std::any_of(wanted.begin(), wanted.end(),
            [id = it->first](const OutputConfig& c) { return c.id == id; });
        if (keep) { ++it; continue; }
        destroySender(it->second);
        it = senders.erase(it);
    }
    if (wanted.empty()) return;
    if (!VideoNetworkRuntime::NDI::get().load())
    {
        setWarningMessage("NDI runtime unavailable or incompatible", "NDI");
        return;
    }
    setWarningMessage({}, "NDI");

    for (const auto& c : wanted)
    {
        auto& s = senders[c.id];
        s.frameRate = c.frameRate;
        if (s.sender != nullptr && s.name == c.name) continue;
        destroySender(s);
        const CharPointer_UTF8 nameUTF8 = c.name.toUTF8();
        NDIlib_send_create_t settings {};
        settings.p_ndi_name = nameUTF8.getAddress();
        settings.p_groups = nullptr;
        settings.clock_video = false; // The worker does its own timing per output.
        settings.clock_audio = false;
        s.sender = VideoNetworkRuntime::NDI::get().send_create(&settings);
        if (s.sender != nullptr)
        {
            s.name = c.name;
            NLOG("NDI", "Created " << c.name << " (route " << String((int64)c.id) << ")");
            s.nextDue = std::chrono::steady_clock::now();
        }
        else
            setWarningMessage("Cannot create NDI sender " + c.name, "NDI");
    }
}

void NDIOutModule::sendLatestFrame(uint64_t id, SenderState& s)
{
    if (s.sender == nullptr || VideoNetworkRuntime::NDI::get().send_get_no_connections(s.sender, 0) == 0)
        return;
    Image image;
    {
        const std::lock_guard<std::mutex> guard(stateLock);
        auto it = surfaces.find(id);
        if (it == surfaces.end()) return;
        image = it->second->getLatestImage();
    }
    if (!image.isValid()) return;
    const int w = image.getWidth(), h = image.getHeight();
    if (w <= 0 || h <= 0) return;

    Image::BitmapData bd(image, Image::BitmapData::readOnly);
    if (bd.lineStride <= 0) return;
    NDIlib_video_frame_v2_t frame(w, h, NDIlib_FourCC_video_type_BGRX,
        s.frameRate, 1000, (float)w / h,
        NDIlib_frame_format_type_progressive, NDIlib_send_timecode_synthesize,
        (uint8*)bd.getLinePointer(0), bd.lineStride);
    VideoNetworkRuntime::NDI::get().send_send_video_v2(s.sender, &frame);
}

void NDIOutModule::sendLatestAudio(uint64_t id, SenderState& s)
{
    // All NDI streams use 48 kHz, even with 44.1 kHz Sound Card playback.
    const double framesExact = (48000000.0 / s.frameRate) + s.audioFraction;
    const int frames = (int)std::floor(framesExact);
    s.audioFraction = framesExact - frames;
    if (frames <= 0) return;

    AudioBuffer<float> mix(2, frames), temp(2, frames);
    mix.clear();
    {
        const std::lock_guard<std::mutex> guard(audioLock);
        for (auto& entry : audioSources)
        {
            auto& source = entry.second;
            if (source.routeId != id || source.fifo == nullptr) continue;
            const int available = source.fifo->getFramesAvailable();
            // Prime from one chunk; always consume partial buffers after that.
            // Re-priming at 4096 frames on every underflow caused long silent gaps.
            if (!source.primed)
            {
                if (available < jmin(frames, 1024)) continue;
                source.primed = true;
            }
            temp.clear();
            source.fifo->pullData(temp, frames);
            for (int ch = 0; ch < 2; ++ch)
                mix.addFrom(ch, 0, temp, ch, 0, frames);
        }
    }
    if (s.sender == nullptr || VideoNetworkRuntime::NDI::get().send_get_no_connections(s.sender, 0) == 0)
        return; // FIFOs are still drained to avoid stale audio on reconnect.

    for (int ch = 0; ch < 2; ++ch)
        for (int i = 0; i < frames; ++i)
            mix.getWritePointer(ch)[i] = jlimit(-1.0f, 1.0f, mix.getReadPointer(ch)[i]);

    NDIlib_audio_frame_v2_t frame {};
    frame.sample_rate = ndiAudioRate;
    frame.no_channels = 2;
    frame.no_samples = frames;
    frame.timecode = NDIlib_send_timecode_synthesize;
    frame.p_data = mix.getWritePointer(0);
    frame.channel_stride_in_bytes = (int)((const char*)mix.getReadPointer(1) - (const char*)mix.getReadPointer(0));
    VideoNetworkRuntime::NDI::get().send_send_audio_v2(s.sender, &frame);
}

void NDIOutModule::enqueueAudio(const void* source, uint64_t routeId,
    const float* samples, int frames, int sampleRate)
{
    if (source == nullptr || routeId == 0 || samples == nullptr || frames <= 0 || sampleRate <= 0)
        return;
    {
        const std::lock_guard<std::mutex> guard(stateLock);
        if (std::none_of(desiredOutputs.begin(), desiredOutputs.end(),
            [routeId](const OutputConfig& c) { return c.id == routeId; })) return;
    }
    const std::lock_guard<std::mutex> guard(audioLock);
    auto& a = audioSources[source];
    if (a.fifo == nullptr || a.routeId != routeId || a.sampleRate != sampleRate)
    {
        a.routeId = routeId;
        a.sampleRate = sampleRate;
        a.nextInputPosition = 0;
        a.hasPrevious = false;
        a.primed = false;
        a.fifo.reset(new AudioFIFO(2, ndiAudioRate + 1));
        NLOG("NDI", "PCM received on route " << String((int64)routeId)
            << " at " << sampleRate << " Hz");
    }

    // Persistent fractional position makes consecutive PCM chunks seamless.
    std::vector<float> resampled;
    resampled.reserve((size_t)(frames * ((double)ndiAudioRate / sampleRate) + 4) * 2);
    const double step = (double)sampleRate / ndiAudioRate;
    while (a.nextInputPosition < frames)
    {
        const double p = a.nextInputPosition;
        const int index = (int)std::floor(p);
        const double t = p - index;
        const int i0 = jlimit(0, frames - 1, index);
        const int i1 = jlimit(0, frames - 1, index + 1);
        const float l0 = index < 0 && a.hasPrevious ? a.lastLeft : samples[2 * i0];
        const float r0 = index < 0 && a.hasPrevious ? a.lastRight : samples[2 * i0 + 1];
        resampled.push_back((float)(l0 + (samples[2 * i1] - l0) * t));
        resampled.push_back((float)(r0 + (samples[2 * i1 + 1] - r0) * t));
        a.nextInputPosition += step;
    }
    a.nextInputPosition -= frames;
    a.lastLeft = samples[(frames - 1) * 2];
    a.lastRight = samples[(frames - 1) * 2 + 1];
    a.hasPrevious = true;

    const int produced = (int)resampled.size() / 2;
    if (produced > a.fifo->getFreeFrames())
    {
        a.fifo->clear();
        a.primed = false;
    }
    if (produced <= a.fifo->getFreeFrames())
        a.fifo->pushData(resampled.data(), produced);
}

void NDIOutModule::forgetAudio(const void* source)
{
    const std::lock_guard<std::mutex> guard(audioLock);
    audioSources.erase(source);
}

void NDIOutModule::publishAudio(const void* source, uint64_t routeId,
    const float* samples, int frames, int sampleRate)
{
    if (routeId == 0) return;
    const std::lock_guard<std::mutex> guard(ndiModulesLock);
    for (auto* module : ndiModules)
        module->enqueueAudio(source, routeId, samples, frames, sampleRate);
}

void NDIOutModule::removeAudioSource(const void* source)
{
    const std::lock_guard<std::mutex> guard(ndiModulesLock);
    for (auto* module : ndiModules)
        module->forgetAudio(source);
}

void NDIOutModule::loadJSONDataItemInternal(var data)
{
    Module::loadJSONDataItemInternal(data);

    // Convert pre-manager saves containing Stream Name / Width / Height / FPS.
    // New files have an Outputs container and are handled by BaseManager.
    const var params = data.getProperty("params", var());
    const var children = params.getProperty("containers", var());
    if (children.isObject() && children.hasProperty(Identifier(outputs.shortName)))
    {
        updateOutput();
        return;
    }

    if (outputs.items.isEmpty()) outputs.addItem(nullptr, var(), false);
    NDIOutput* first = outputs.items[0];
    if (first == nullptr) return;

    const var values = params.getProperty("parameters", var());
    if (auto* saved = values.getArray())
    {
        for (const auto& v : *saved)
        {
            const String address = v.getProperty("controlAddress", "").toString().toLowerCase().removeCharacters("-_ /\\");
            const var value = v.getProperty("value", var());
            if (value.isVoid()) continue;
            if (address.endsWith("streamname")) first->streamName->setValue(value);
            else if (address.endsWith("width")) first->width->setValue(value);
            else if (address.endsWith("height")) first->height->setValue(value);
            else if (address.endsWith("fps")) first->fps->setValue(value);
            else if (address.endsWith("testcard")) first->testCard->setValue(value);
        }
    }
    updateOutput();
}

void NDIOutModule::clearItem()
{
    shutdown();
    Module::clearItem();
}
