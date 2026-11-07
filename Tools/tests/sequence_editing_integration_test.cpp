#include "MainIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include "StateMachine/StateMachineIncludes.h"
#include "Module/ModuleIncludes.h"
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
    // Keep native hit testing independent of other desktop windows.
    host.setAlwaysOnTop(true);
    host.setVisible(true);
    host.toFront(false);
    drainMessages();
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


static void viewFollowing()
{
    Sequence sequence;
    sequence.totalTime->setValue(60);
    for (int mode = 0; mode < 4; ++mode)
        for (bool playing : { false, true })
            for (auto kind : { Sequence::TimeChangeKind::Playback, Sequence::TimeChangeKind::Seek,
                               Sequence::TimeChangeKind::Cue, Sequence::TimeChangeKind::Loop })
            {
                sequence.isPlaying->setValue(false, true);
                sequence.setCurrentTime(0, true, true);
                sequence.viewStartTime->setValue(0); sequence.viewEndTime->setValue(10);
                sequence.followViewRange = 10;
                sequence.viewFollowTime->setValueWithData(mode);
                sequence.isPlaying->setValue(playing, true);
                sequence.setCurrentTime(20, true, kind != Sequence::TimeChangeKind::Playback, kind);
                const bool follows = mode != 0 && (playing || mode == 3 || (mode == 2 && kind == Sequence::TimeChangeKind::Cue));
                check((sequence.viewStartTime->floatValue() > 0) == follows, "follow policy covers playback, cues, seeks and loops");
                check(almostEqual(sequence.viewEndTime->floatValue() - sequence.viewStartTime->floatValue(), 10), "following preserves zoom");
                if (follows && kind != Sequence::TimeChangeKind::Playback)
                    check(almostEqual(sequence.viewStartTime->floatValue(), 15), "jumps center immediately");
            }
    sequence.isPlaying->setValue(false, true);
    sequence.viewFollowTime->setValue("Playing and Cues");
    auto resetView = [&] { sequence.viewStartTime->setValue(0); sequence.viewEndTime->setValue(10); };
    auto* cue = sequence.cueManager->addItem(new TimeCue(35, sequence.cueManager.get()), var(), false);
    resetView();
    var params(new DynamicObject()); params.getDynamicObject()->setProperty("type", SequenceCommand::GOTO_CUE);
    SequenceCommand command(nullptr, ACTION, params);
    command.target->setRootContainer(&sequence);
    command.target->setValueFromTarget(cue);
    check(command.target->targetContainer == cue, "cue command resolves target");
    command.triggerInternal(0);
    check(almostEqual(sequence.viewStartTime->floatValue(), 30), "Go to cue command follows while paused");
    resetView(); sequence.setCurrentTime(0, true, true);
    sequence.nextCue->trigger();
    check(almostEqual(sequence.viewStartTime->floatValue(), 30), "next cue follows while paused");
    resetView(); sequence.setCurrentTime(50, true, true);
    sequence.prevCue->trigger();
    check(almostEqual(sequence.viewStartTime->floatValue(), 30), "previous cue follows while paused");
    resetView(); cue->playFromHere->trigger(); sequence.pauseTrigger->trigger();
    check(sequence.viewStartTime->floatValue() >= 30, "cue trigger follows before starting playback");
    sequence.viewFollowTime->setValue("Always");
    resetView(); sequence.currentTime->setValue(59);
    check(almostEqual(sequence.viewStartTime->floatValue(), 50) && almostEqual(sequence.viewEndTime->floatValue(), 60), "direct seeks clamp view at end");
    sequence.currentTime->setValue(1);
    check(almostEqual(sequence.viewStartTime->floatValue(), 0), "view clamps at start");
    for (bool oldValue : { false, true })
    {
        var data(new DynamicObject()), parameter(new DynamicObject());
        parameter.getDynamicObject()->setProperty("controlAddress", "/viewFollowTime");
        parameter.getDynamicObject()->setProperty("value", oldValue);
        data.getDynamicObject()->setProperty("parameters", var(Array<var>{ parameter }));
        sequence.loadJSONData(data);
        check(sequence.viewFollowTime->getValueKey() == (oldValue ? "Playing Only" : "Never"), "saved boolean migrates to equivalent mode");
    }
    sequence.viewFollowTime->setValue("Playing and Cues");
    Sequence restored; restored.loadJSONData(sequence.getJSONData());
    check(restored.viewFollowTime->getValueKey() == "Playing and Cues", "enum survives save and reload");
}

static void disabledSequenceVideo()
{
    auto* manager = ChataigneSequenceManager::getInstance();
    auto add = [&]()
    {
        auto* seq = manager->addItem(new Sequence(), var(), false);
        auto* video = static_cast<VideoLayer*>(seq->layerManager->addItem(new VideoLayer(seq, var()), var(), false));
        auto* clip = static_cast<VideoLayerClip*>(video->clipManager.addItem(new VideoLayerClip(), var(), false));
        video->currentClip = clip;
        video->moviePlayer->load("fixture.mp4");
        return video;
    };
    auto* top = add(); auto* bottom = add();
    check(CompositionRenderer::gatherActiveCues().size() == 2, "both enabled sequences contribute video");
    top->sequence->enabled->setValue(false);
    auto cues = CompositionRenderer::gatherActiveCues();
    check(cues.size() == 1 && cues[0].layer == bottom, "disabling top sequence reveals lower sequence video");
    check(CompositionRenderer::gatherActiveCues(top->sequence, top).isEmpty(), "filtered outputs also hide disabled sequences");
    top->sequence->enabled->setValue(true);
    check(CompositionRenderer::gatherActiveCues().size() == 2, "reenabling restores retained frame");
    top->enabled->setValue(false);
    check(CompositionRenderer::gatherActiveCues().size() == 1, "layer disabling still hides video");
    manager->removeItem(top->sequence, false); manager->removeItem(bottom->sequence, false);
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
    run("view following", viewFollowing);
    run("disabled sequence video", disabledSequenceVideo);
    run("audio clock media seeks", [] { externalMediaSeeks(true); });
    run("system clock media seeks", [] { externalMediaSeeks(false); });
    run("block handles and fades", blockHandles);
    run("audio browse filter", audioBrowseFilter);
    if (argc > 1) run("saved action reproduction", [&] { savedActionFixture(*app.engine, File(String::fromUTF8(argv[1]))); });
    app.engine.reset();
    std::cout << checks << " checks, " << failures << " failures\n";
    return failures == 0 ? 0 : 1;
}
