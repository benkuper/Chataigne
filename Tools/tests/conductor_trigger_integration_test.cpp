#include "MainIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include <iostream>
#include <stdexcept>
#undef main

class ConductorTestApplication : public OrganicApplication
{
public:
    ConductorTestApplication() : OrganicApplication("Conductor tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

// Keep transport deterministic while exercising the real cue, sequence seek,
// action notifications and consequence dispatch code.
class TrackingSequence : public Sequence
{
public:
    int plays = 0, stops = 0;

    void onContainerTriggerTriggered(Trigger* t) override
    {
        if (t == playTrigger || t == stopTrigger)
        {
            const bool playing = t == playTrigger;
            if (playing) ++plays;
            else ++stops;
            const bool changed = isPlaying->boolValue() != playing;
            isPlaying->setValue(playing, true);
            if (changed) sequenceListeners.call(&SequenceListener::sequencePlayStateChanged, this);
            if (!playing) setCurrentTime(0, true, true);
        }
        else Sequence::onContainerTriggerTriggered(t);
    }
};

class CountingConsequence : public Consequence
{
public:
    int calls = 0;
    void triggerCommand(int = 0) override { ++calls; }
};

static CountingConsequence* addCounter(ConsequenceManager* manager)
{
    auto* counter = new CountingConsequence();
    manager->addItem(counter, var(), false);
    return counter;
}

static void exercise(bool directConsequences, bool cueSetsCurrent)
{
    TrackingSequence first, second;
    Conductor conductor;
    conductor.triggerConductorConsequencesOnDirect->setValue(directConsequences);
    conductor.cueTriggerSetCurrent->setValue(cueSetsCurrent);
    auto* a = static_cast<ConductorCue*>(conductor.processorManager.addItem(new ConductorCue(), var(), false));
    auto* b = static_cast<ConductorCue*>(conductor.processorManager.addItem(new ConductorCue(), var(), false));
    a->setLinkedSequence(&first);
    b->setLinkedSequence(&second);
    auto* aOn = addCounter(a->csmOn.get());
    auto* aOff = addCounter(a->csmOff.get());
    auto* bOn = addCounter(b->csmOn.get());
    auto* global = addCounter(conductor.csmOn.get());

    conductor.triggerOn->trigger();
    check(first.plays == 1 && first.stops == 0, "GO starts linked sequence once without stopping it");
    check(aOn->calls == 1 && global->calls == 1, "GO runs cue and conductor consequences once");
    check(conductor.currentCue == a && a->isCurrent && !b->isCurrent, "current cue after GO");
    check(conductor.nextCueIndex->intValue() == 2, "GO advances to next cue once");

    first.setCurrentTime(3, true, true);
    conductor.triggerCurrent->trigger();
    check(first.plays == 2 && first.stops == 0, "explicit retrigger has no intermediate Stop");
    check(first.currentTime->doubleValue() == 0, "Force Play from start seeks an already playing sequence");
    check(aOn->calls == 2 && aOff->calls == 0, "retrigger runs On once without Off");
    check(global->calls == (directConsequences ? 2 : 1), "retrigger respects conductor consequence option");

    a->forceStartFrom0->setValue(false);
    first.setCurrentTime(4, true, true);
    conductor.triggerCurrent->trigger();
    check(first.plays == 3 && first.stops == 0 && first.currentTime->doubleValue() == 4,
        "retrigger preserves playback position when Force Play from start is disabled");

    const int globalBefore = global->calls;
    conductor.triggerOn->trigger();
    check(first.stops == 1 && second.plays == 1 && second.stops == 0,
        "cue change stops previous sequence once and starts next once");
    check(aOff->calls == 1 && bOn->calls == 1 && global->calls == globalBefore + 1,
        "cue change executes Off, On and conductor consequences once");

    const int playsBefore = first.plays;
    const int secondStopsBefore = second.stops;
    const int beforeDirect = global->calls;
    a->triggerOn->trigger();
    check(first.plays == playsBefore + (cueSetsCurrent ? 1 : 0), "manual cue activation respects Cue Trigger Set Current");
    check(second.stops == secondStopsBefore + (cueSetsCurrent ? 1 : 0), "manual cue transition stops previous sequence once");
    check(global->calls == beforeDirect + (cueSetsCurrent && directConsequences ? 1 : 0),
        "manual cue executes conductor consequences at most once");
    if (cueSetsCurrent)
    {
        const int stopsBefore = first.stops;
        a->triggerOn->trigger();
        check(first.plays == playsBefore + 2 && first.stops == stopsBefore,
            "manual retrigger of current cue does not stop its sequence");
    }

    auto* currentBeforeOff = conductor.currentCue;
    const int firstPlaysBeforeOff = first.plays;
    const int secondPlaysBeforeOff = second.plays;
    b->notifyActionTriggered(false, 0);
    check(conductor.currentCue == currentBeforeOff
        && first.plays == firstPlaysBeforeOff && second.plays == secondPlaysBeforeOff,
        "Off notification never activates or restarts a cue");

    // Auto Start and Auto Stop remain independently configurable.
    a->autoStart->setValue(false);
    const int disabledStartPlays = first.plays;
    conductor.triggerCue(a, true);
    check(first.plays == disabledStartPlays, "Auto Start disabled prevents linked sequence launch");
    a->autoStop->setValue(false);
    const int disabledStopStops = first.stops;
    conductor.triggerCue(b, true);
    check(first.stops == disabledStopStops, "Auto Stop disabled preserves previous sequence playback");
}

static void exerciseAudioRetrigger()
{
    AudioBuffer<float> samples(1, 44100 * 10);
    for (int i = 0; i < samples.getNumSamples(); ++i) samples.setSample(0, i, 0.25f);
    MemoryAudioSource source(samples, false, false);
    class AudioDrivenSequence : public TrackingSequence
    {
        bool timeIsDrivenByAudio() override { return true; }
    } sequence;
    Conductor conductor;
    auto* cue = static_cast<ConductorCue*>(conductor.processorManager.addItem(new ConductorCue(), var(), false));
    cue->setLinkedSequence(&sequence);
    auto* layer = static_cast<AudioLayer*>(sequence.layerManager->addItem(new AudioLayer(&sequence, var()), var(), false));
    auto* clip = static_cast<AudioLayerClip*>(layer->clipManager.addItem(new AudioLayerClip(), var(), false));
    clip->transportSource.setSource(&source, 0, nullptr, 44100);
    clip->transportSource.prepareToPlay(256, 44100);

    conductor.triggerOn->trigger();
    check(sequence.plays == 1 && sequence.stops == 0 && clip->transportSource.isPlaying(),
        "GO activates real audio transport once without Stop");
    AudioBuffer<float> output(1, 256);
    AudioSourceChannelInfo block(&output, 0, 256);
    clip->transportSource.getNextAudioBlock(block);
    const double position = clip->transportSource.getCurrentPosition();
    cue->forceStartFrom0->setValue(false);
    conductor.triggerCurrent->trigger();
    check(clip->transportSource.isPlaying() && clip->transportSource.getCurrentPosition() == position,
        "current cue retrigger preserves real audio playback and position");
    clip->transportSource.getNextAudioBlock(block);
    check(output.getSample(0, 128) == 0.25f, "audio continues after current cue retrigger");

    cue->forceStartFrom0->setValue(true);
    conductor.triggerCurrent->trigger();
    check(clip->transportSource.isPlaying() && clip->transportSource.getCurrentPosition() == 0,
        "forced restart seeks real audio transport without Stop");
    check(sequence.plays == 3 && sequence.stops == 0, "audio retriggers never issue an intermediate Stop");

    const double stopBegin = Time::getMillisecondCounterHiRes();
    sequence.stopTrigger->trigger();
    check(Time::getMillisecondCounterHiRes() - stopBegin < 100.0,
        "direct sequence Stop at zero returns without audio callbacks");
    check(!clip->transportSource.isPlaying() && sequence.currentTime->doubleValue() == 0,
        "direct sequence Stop stops real audio and resets sequence time");
    sequence.playTrigger->trigger();
    sequence.setCurrentTime(0.1, true, true);
    const double pauseBegin = Time::getMillisecondCounterHiRes();
    sequence.pauseTrigger->trigger();
    check(Time::getMillisecondCounterHiRes() - pauseBegin < 100.0,
        "direct sequence Pause returns without audio callbacks");
    check(!clip->transportSource.isPlaying() && std::abs(sequence.currentTime->doubleValue() - 0.1) < 1.0e-6,
        "direct sequence Pause preserves sequence time");
}

static void exerciseAutoNext()
{
    TrackingSequence first, second;
    Conductor conductor;
    auto* a = static_cast<ConductorCue*>(conductor.processorManager.addItem(new ConductorCue(), var(), false));
    auto* b = static_cast<ConductorCue*>(conductor.processorManager.addItem(new ConductorCue(), var(), false));
    a->setLinkedSequence(&first);
    b->setLinkedSequence(&second);
    a->autoNext->setValue(true);
    auto* global = addCounter(conductor.csmOn.get());
    auto* next = addCounter(b->csmOn.get());
    conductor.triggerOn->trigger();
    first.finishTrigger->trigger();
    check(conductor.currentCue == b && second.plays == 1 && second.stops == 0,
        "Auto Next starts the following sequence once without Stop");
    check(global->calls == 2 && next->calls == 1, "Auto Next dispatches consequences once per cue");
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    ConductorTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        for (bool directConsequences : { false, true })
            for (bool cueSetsCurrent : { false, true })
                exercise(directConsequences, cueSetsCurrent);
        exerciseAudioRetrigger();
        exerciseAutoNext();
        std::cout << "Conductor trigger integration tests passed (4 option combinations, audio continuity, Auto Next)\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Conductor trigger integration failed: " << e.what() << "\n";
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
