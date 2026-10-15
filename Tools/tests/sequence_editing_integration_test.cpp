#include "MainIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include "StateMachine/StateMachineIncludes.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#if JUCE_WINDOWS
#include <windows.h>
#endif
#undef main

class SequenceEditingTestApplication : public OrganicApplication
{
public:
    SequenceEditingTestApplication() : OrganicApplication("Sequence editing tests", false) {}
    void initialiseInternal(const String&) override {}
};

static int checks = 0;
static void check(bool condition, const char* label)
{
    ++checks;
    if (!condition) throw std::runtime_error(label);
}

static bool almostEqual(double a, double b) { return std::abs(a - b) < .0001; }

static void drainMessages()
{
#if JUCE_WINDOWS
    MSG message;
    while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
#endif
}

class TestSequence : public Sequence
{
public:
    explicit TestSequence(bool audioClock) : audioClock(audioClock) { setCurrentTime(0); }
    bool timeIsDrivenByAudio() override { return audioClock; }
    bool audioClock;
};

class SilenceSource : public PositionableAudioSource
{
public:
    int64 position = 0;
    void prepareToPlay(int, double) override {}
    void releaseResources() override {}
    void getNextAudioBlock(const AudioSourceChannelInfo& info) override
    { info.clearActiveBufferRegion(); position += info.numSamples; }
    void setNextReadPosition(int64 value) override { position = value; }
    int64 getNextReadPosition() const override { return position; }
    int64 getTotalLength() const override { return 48000 * 60; }
    bool isLooping() const override { return false; }
};

class RecordingVideoPlayer : public NullVideoPlayer
{
public:
    bool playing = false;
    double position = 0;
    int seeks = 0;
    bool isFileLoaded() const override { return getFilePath().isNotEmpty(); }
    bool isPlaying() const override { return playing; }
    void play() override { playing = true; }
    void pause() override { playing = false; }
    void stop() override { playing = false; }
    void setPosition(double value) override { position = value; ++seeks; }
    double getPosition() const override { return position; }
    double getDuration() const override { return 60; }
};

static void externalMediaSeeks(bool audioClock)
{
    SilenceSource source;
    TestSequence sequence(audioClock);
    AudioLayer audio(&sequence, var());
    auto* clip = static_cast<AudioLayerClip*>(audio.clipManager.addItem(new AudioLayerClip(), var(), false));
    clip->time->setValue(2);
    clip->coreLength->setValue(20);
    clip->clipStartOffset->setValue(.25);
    clip->stretchFactor->setValue(2);
    clip->transportSource.setSource(&source);
    clip->transportSource.prepareToPlay(256, 48000);

    VideoLayer video(&sequence, var());
    video.moviePlayer->removeListener(&video);
    auto* player = new RecordingVideoPlayer();
    video.moviePlayer.reset(player);
    player->addListener(&video);
    auto* movie = new VideoLayerClip();
    movie->time->setValue(2);
    movie->coreLength->setValue(20);
    movie->clipStartOffset->setValue(.5);
    movie->filePath->setValue("fixture.mp4", true);
    video.clipManager.addItem(movie, var(), false);

    struct Listener : Sequence::SequenceListener
    {
        bool seeking = false;
        Sequence::TimeChangeKind kind = Sequence::TimeChangeKind::Automatic;
        void sequenceTimeChanged(Sequence* s, const Sequence::TimeChange& change) override
        { seeking = s->isSeeking; kind = change.kind; }
    } listener;
    sequence.addSequenceListener(&listener);

    // Set the feedback silently to exercise playing callbacks without starting
    // a real device or the sequence worker thread.
    sequence.isPlaying->setValue(true, true);
    sequence.currentTime->setValue(4);
    video.syncPlaybackState();
    check(almostEqual(clip->transportSource.getCurrentPosition(), 1.25), "direct time write seeks stretched audio with source offset");
    check(almostEqual(player->position, 2.5) && player->playing, "direct time write seeks playing video with source offset");
    check(listener.seeking && listener.kind == Sequence::TimeChangeKind::Seek && !sequence.isSeeking,
        "direct time write exposes seek state to listeners and restores it afterwards");

    var params(new DynamicObject());
    params.getDynamicObject()->setProperty("action", GenericControllableCommand::SET_VALUE);
    params.getDynamicObject()->setProperty("root", (int64)&sequence);
    GenericControllableCommand command(nullptr, ACTION, params);
    command.target->setValueFromTarget(sequence.currentTime);
    check(command.value != nullptr, "generic action resolves sequence current time");
    command.value->setValue(6);
    const auto clockRevision = sequence.playbackClockRevision;
    command.triggerInternal(0);
    video.syncPlaybackState();
    check(almostEqual(clip->transportSource.getCurrentPosition(), 2.25), "generic Set Parameter Value action seeks playing audio");
    check(almostEqual(player->position, 4.5), "generic Set Parameter Value action seeks playing video");
    check(almostEqual(sequence.hiResAudioTime, 6) && almostEqual(sequence.targetTime, 6)
        && sequence.playbackClockRevision > clockRevision, "generic action reanchors both playback clocks");

    const int videoSeeks = player->seeks;
    sequence.setCurrentTime(6.02, true, false, Sequence::TimeChangeKind::Playback);
    video.syncPlaybackState();
    check(!listener.seeking && listener.kind == Sequence::TimeChangeKind::Playback, "ordinary playback stays distinct from seeking");
    check(player->seeks == videoSeeks, "ordinary playback does not repeatedly seek video");

    std::thread worker([&] { sequence.currentTime->setValue(8); });
    worker.join();
    sequence.setCurrentTime(6.04, true, false, Sequence::TimeChangeKind::Playback);
    drainMessages();
    video.syncPlaybackState();
    check(almostEqual(clip->transportSource.getCurrentPosition(), 3.25) && almostEqual(player->position, 6.5),
        "queued external parameter writes seek both media transports");

    sequence.currentTime->setValue(3);
    video.syncPlaybackState();
    check(almostEqual(clip->transportSource.getCurrentPosition(), .75) && almostEqual(player->position, 1.5),
        "backward panel-style time edits rewind both media transports");
    sequence.isPlaying->setValue(false, true);
    sequence.currentTime->setValue(5);
    video.syncPlaybackState();
    check(almostEqual(clip->transportSource.getCurrentPosition(), 1.75) && almostEqual(player->position, 3.5) && !player->playing,
        "paused parameter writes reposition both media transports");
    sequence.removeSequenceListener(&listener);
    clip->transportSource.setSource(nullptr);
}

static void blockHandles()
{
    LayerBlock block;
    block.addFadeParameters();
    Component host;
    host.setOpaque(true);
    host.setBounds(20, 20, 600, 100);
    host.addToDesktop(ComponentPeer::windowIsTemporary | ComponentPeer::windowIgnoresKeyPresses);
    host.setVisible(true);
    host.toFront(false);
    LayerBlockUI ui(&block);
    host.addAndMakeVisible(ui);
    ui.setBounds(0, 0, 500, 80);
    // getXForTime truncates to integer pixels, so a visible block may have
    // fractional start/end times just outside its UI's view range.
    ui.setViewRange(.01f, 9.99f);
    auto* peer = host.getPeer();
    auto pointer = [&](Point<float> position, int mods = 0)
    {
        peer->handleMouseEvent(MouseInputSource::InputSourceType::mouse, position, ModifierKeys(mods),
            MouseInputSource::invalidPressure, MouseInputSource::invalidOrientation, Time::currentTimeMillis());
    };
    pointer({ 250, 20 });
    ui.updateFadeHandles();
    check(ui.grabber.isVisible() && ui.coreGrabber.isVisible(), "pixel rounding preserves visible trim handles");
    check(ui.fadeInHandle.isVisible() && ui.fadeOutHandle.isVisible(), "pixel rounding preserves zero-duration fade handles");

    pointer({ 496, 30 });
    ui.handlePaintTimerInternal();
    check(ui.coreGrabber.isVisible(), "trim handle stays visible while hovered across timer updates");
    pointer({ 496, 30 }, ModifierKeys::leftButtonModifier);
    ui.handlePaintTimerInternal();
    check(ui.coreGrabber.isVisible(), "trim handle stays visible while dragged");
    pointer({ 480, 30 }, ModifierKeys::leftButtonModifier);
    pointer({ 480, 30 });

    Automation automation;
    ui.setInlineEditor(&automation);
    pointer({ 250, 40 });
    ui.updateFadeHandles();
    check(ui.grabber.isVisible() && ui.coreGrabber.isVisible(), "hovering embedded editors keeps trim handles visible");
    check(ui.getComponentAt(496, 30) == &ui.coreGrabber, "trim handles receive clicks above embedded editors");

    // Start a real fade drag through the same peer event path as mouse input.
    ui.setViewRange(0, 10);
    pointer({ 495, 5 });
    pointer({ 495, 5 }, ModifierKeys::leftButtonModifier);
    pointer({ 395, 5 }, ModifierKeys::leftButtonModifier);
    pointer({ 395, 5 });
    check(block.blockFadeOut->enabled && almostEqual(block.blockFadeOut->doubleValue(), 2), "fade-out handle drag sets its duration");

    ui.setViewRange(1, 9);
    pointer({ 250, 20 });
    ui.updateFadeHandles();
    check(!ui.grabber.isVisible() && !ui.coreGrabber.isVisible(), "genuinely clipped endpoints do not expose trim handles");
    block.isUILocked->setValue(true);
    ui.setViewRange(0, 10);
    ui.updateFadeHandles();
    check(!ui.grabber.isVisible() && !ui.coreGrabber.isVisible() && !ui.fadeOutHandle.isVisible(), "locked blocks hide editing handles");
    host.removeFromDesktop();
}

static void audioBrowseFilter()
{
    AudioLayerClip clip;
    AudioFormatManager formats;
    formats.registerBasicFormats();
    check(clip.filePath->fileTypeFilter == formats.getWildcardForAllFormats(), "audio inspector filter includes registered audio formats");
    check(clip.filePath->fileTypeFilter.contains("*.wav") && clip.filePath->fileTypeFilter.contains("*.mp3"),
        "audio browsing includes WAV and MP3");
}

static void clearMediaPaths(const var& data)
{
    if (data.isArray())
        for (auto& item : *data.getArray()) clearMediaPaths(item);
    else if (auto* object = data.getDynamicObject())
    {
        if (object->getProperty("controlAddress").toString() == "/filePath")
            object->setProperty("value", "");
        auto& properties = object->getProperties();
        for (int i = 0; i < properties.size(); ++i) clearMediaPaths(properties.getValueAt(i));
    }
}

static void savedActionFixture(Engine& engine, const File& file)
{
    var data = JSON::parse(file);
    check(data.isObject(), "saved reproduction project parses");
    // Keep the saved action and its target intact. Use a synthetic audio source
    // so this regression is independent of the user's local MP3 and hardware.
    clearMediaPaths(data);
    engine.isLoadingFile = true;
    StateManager::getInstance()->loadJSONData(data.getProperty("states", var()));
    ChataigneSequenceManager::getInstance()->loadJSONData(data.getProperty("sequences", var()));
    engine.engineListeners.call(&EngineListener::endLoadFile);
    engine.isLoadingFile = false;
    engine.engineListeners.call(&EngineListener::fileLoaded);

    auto* sequence = dynamic_cast<Sequence*>(engine.getControllableContainerForAddress("/sequences/sequence"));
    auto* action = dynamic_cast<Action*>(engine.getControllableContainerForAddress("/states/state/processors/action"));
    check(sequence && action && !action->csmOn->items.isEmpty(), "saved sequence and action restore");
    auto* consequence = dynamic_cast<Consequence*>(action->csmOn->items[0]);
    auto* command = consequence ? dynamic_cast<GenericControllableCommand*>(consequence->command.get()) : nullptr;
    check(command && command->value && command->target->target == sequence->currentTime, "saved action restores generic current-time target");
    auto* audio = dynamic_cast<AudioLayer*>(sequence->layerManager->items[0]);
    check(audio && !audio->clipManager.items.isEmpty(), "saved audio layer restores");
    auto* clip = static_cast<AudioLayerClip*>(audio->clipManager.items[0]);
    SilenceSource source;
    clip->transportSource.setSource(&source);
    struct DetachSource
    {
        TimelineAudioTransportSource& transport;
        ~DetachSource() { transport.setSource(nullptr); }
    } detach { clip->transportSource };
    clip->transportSource.prepareToPlay(256, 48000);
    sequence->isPlaying->setValue(true, true);
    sequence->setCurrentTime(8, true, true);
    const double expectedTime = sequence->currentTime->getStepSnappedValueFor(
        sequence->currentTime->getCroppedValue(command->value->getValue()));
    command->triggerInternal(0);
    check(almostEqual(sequence->currentTime->doubleValue(), expectedTime), "saved generic action writes sequence current time at its frame precision");
    check(almostEqual(clip->transportSource.getCurrentPosition(), expectedTime), "saved generic action seeks the playing audio transport");
    sequence->isPlaying->setValue(false, true);
}

int main(int argc, char** argv)
{
    ScopedJuceInitialiser_GUI gui;
    SequenceEditingTestApplication app;
    app.engine.reset(new ChataigneEngine());
    int failures = 0;
    auto run = [&](const char* label, auto test)
    {
        try { test(); std::cout << "PASS: " << label << std::endl; }
        catch (const std::exception& e) { ++failures; std::cout << "FAIL: " << label << ": " << e.what() << std::endl; }
    };
    run("audio clock media seeks", [] { externalMediaSeeks(true); });
    run("system clock media seeks", [] { externalMediaSeeks(false); });
    run("block handles and fades", blockHandles);
    run("audio browse filter", audioBrowseFilter);
    if (argc > 1) run("saved action reproduction", [&] { savedActionFixture(*app.engine, File(String::fromUTF8(argv[1]))); });
    app.engine.reset();
    std::cout << checks << " checks, " << failures << " failures\n";
    return failures == 0 ? 0 : 1;
}
