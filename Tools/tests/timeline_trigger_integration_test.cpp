#include "MainIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include <iostream>
#include <stdexcept>
#include <functional>
#include <thread>
#if JUCE_WINDOWS
#include <windows.h>
#endif
#undef main

class TriggerTestApplication : public OrganicApplication
{
public:
    TriggerTestApplication() : OrganicApplication("Timeline trigger tests", false) {}
    void initialiseInternal(const String&) override {}
};

static int checks = 0;
static void check(bool value, const char* label)
{
    ++checks;
    if (!value) throw std::runtime_error(label);
}

class CounterConsequence : public Consequence
{
public:
    int& count;
    std::function<void()> callback;
    CounterConsequence(int& c, std::function<void()> cb = {}) : count(c), callback(cb) {}
    void triggerCommand(int = 0) override { ++count; auto action = callback; if (action) action(); }
};

struct Fixture
{
    Sequence sequence;
    ChataigneTriggerLayer* layer;
    ChataigneTimeTrigger* trigger;
    ManualCondition* condition;
    int on = 0, off = 0;
    Fixture(bool duration = true)
    {
        sequence.evaluateOnSeek->setValueWithData(Sequence::ALWAYS);
        layer = static_cast<ChataigneTriggerLayer*>(sequence.layerManager->addItem(new ChataigneTriggerLayer(&sequence), var(), false));
        trigger = static_cast<ChataigneTimeTrigger*>(layer->ttm->addItem(nullptr, var(), false));
        trigger->time->setValue(1);
        condition = static_cast<ManualCondition*>(trigger->cdm->addItem(new ManualCondition(), var(), false));
        trigger->cdm->checkAllConditions(0);
        trigger->csm->addItem(new CounterConsequence(on), var(), false);
        trigger->untcsm->addItem(new CounterConsequence(off), var(), false);
        if (duration) trigger->length->setValue(2);
    }
    void valid(bool value) { condition->active->setValue(value); }
    void move(double time, Sequence::TimeChangeKind kind = Sequence::TimeChangeKind::Playback)
    { sequence.setCurrentTime(time, true, kind != Sequence::TimeChangeKind::Playback, kind); }
    void seek(double time) { move(time, Sequence::TimeChangeKind::Seek); }
    void mode(int value) { trigger->duration->evaluation->setValueWithData(value); }
};

static void durationBehavior()
{
    {
        Fixture f;
        f.move(1);
        check(f.trigger->duration->active && !f.trigger->duration->valid, "active and invalid are independent");
        check(f.on == 0 && f.off == 0, "invalid entry is deferred by default");
        f.move(3);
        check(f.off == 1 && !f.trigger->duration->active, "never valid defaults to FALSE at exit");
    }
    {
        Fixture f;
        f.trigger->duration->neverValidExit->setValueWithData(ChataigneTimeTrigger::action_triggerTrue);
        f.move(1); f.move(3);
        check(f.on == 1 && f.off == 0, "never-valid exit can force TRUE despite invalidity");
    }
    {
        Fixture f;
        f.move(1); f.valid(true); f.valid(false); f.valid(true); f.valid(false);
        check(f.on == 1 && f.trigger->duration->everValid, "Only once remembers transient validity");
        f.move(3);
        check(f.off == 0, "successful visit defaults to no FALSE at exit");
        f.move(4); f.seek(0); f.move(1); f.move(3);
        check(f.off == 1, "new visit resets ever-valid history");
    }
    {
        Fixture f;
        f.mode(ChataigneTimeTrigger::onlyEnterExit);
        f.move(1); f.valid(true); f.valid(false);
        check(f.on == 0 && f.trigger->duration->everValid, "Enter/exit tracks validity without executing TRUE");
        f.move(3);
        check(f.off == 0, "exit uses condition history rather than TRUE execution history");
    }
    {
        Fixture f;
        f.trigger->duration->everValidExit->setValueWithData(ChataigneTimeTrigger::action_triggerFalse);
        f.valid(true); f.move(1); f.move(2); f.move(3);
        check(f.on == 1 && f.off == 1, "ever-valid exit can explicitly run FALSE");
    }
    {
        Fixture f;
        f.mode(ChataigneTimeTrigger::always);
        f.trigger->duration->invalidHandling->setValueWithData(ChataigneTimeTrigger::immediately);
        f.move(1); f.move(1.5); f.valid(true); f.valid(false); f.valid(true);
        check(f.on == 2 && f.off == 2, "Always dispatches validity edges without repeating unchanged values");
        f.move(3);
        check(f.off == 2, "Always has no implicit exit release");
    }
    {
        Fixture f;
        f.trigger->duration->invalidHandling->setValueWithData(ChataigneTimeTrigger::immediately);
        f.move(1); f.move(1.5); f.valid(true); f.valid(false); f.move(3);
        check(f.on == 1 && f.off == 1, "Only once permits one initial FALSE before TRUE");
    }
    {
        Fixture f;
        f.move(1);
        f.layer->ttm->triggerAllConsequences(true);
        check(f.on == 1 && f.trigger->duration->active && !f.trigger->duration->everValid,
            "manual TRUE changes neither activity nor validity history");
        f.valid(true);
        check(f.on == 2, "manual TRUE does not consume the automatic once budget");
        f.layer->ttm->triggerAllConsequences(false);
        f.layer->ttm->triggerAllConsequences(false);
        check(f.off == 2 && f.trigger->duration->active, "manual FALSE repeats without leaving the duration");
    }
    for (int action : { ChataigneTimeTrigger::action_nothing, ChataigneTimeTrigger::action_triggerBoth })
    {
        Fixture f;
        f.trigger->duration->neverValidExit->setValueWithData(action);
        f.move(1); f.move(3);
        check(f.on == (action == ChataigneTimeTrigger::action_triggerBoth)
            && f.off == (action == ChataigneTimeTrigger::action_triggerBoth), "never-valid Nothing/Both actions");
    }
}

static void pointsAndSeeks()
{
    {
        Fixture f;
        f.valid(true); f.seek(4); f.move(3);
        check(!f.trigger->duration->active && f.on == 0, "reverse arrival at exclusive end stays inactive");
        f.move(2.5); f.move(1);
        check(f.trigger->duration->active && f.on == 1, "reverse entry and inclusive start boundary");
        f.move(0.5);
        check(!f.trigger->duration->active && f.off == 0, "reverse playback uses exit rules, not rewind actions");
    }
    {
        Fixture f(false);
        check(!f.trigger->duration, "point has no duration state");
        f.move(1); f.move(2);
        check(f.off == 1 && f.on == 0, "point executes FALSE exactly once");
        f.seek(1.5);
        check(f.off == 1, "small rewind does not affect an earlier point");
        f.seek(0); f.valid(true); f.move(1);
        check(f.off == 2 && f.on == 1, "crossed point rewinds and rearms");
        f.layer->ttm->sequencePlayStateChanged(&f.sequence);
        check(f.on == 1, "remaining on exact boundary does not duplicate a crossing");
    }
    {
        Fixture f;
        f.valid(true); f.move(1.5); f.seek(1.25); f.seek(0);
        check(f.on == 1 && f.off == 1, "within-block rewind preserves visit; later rewind runs FALSE");
    }
    {
        Fixture f;
        f.valid(true); f.seek(4);
        check(f.on == 0 && f.off == 0, "destination seek skips whole duration");
        f.seek(2);
        check(f.on == 1 && f.trigger->duration->active, "backward seek can enter destination duration");
        f.seek(4);
        check(f.off == 0 && !f.trigger->duration->active, "forward seek uses history-based exit");
    }
    {
        Fixture f;
        f.trigger->duration->forwardSeek->setValueWithData(1);
        f.seek(4);
        check(f.off == 1, "replay mode executes skipped duration exit");
    }
    {
        Fixture f;
        f.trigger->forwardSeek->setValueWithData(TimeTrigger::neverSeek);
        f.valid(true); f.seek(2);
        check(f.on == 0 && f.trigger->duration->active && f.trigger->duration->everValid,
            "suppressed seek still reconciles activity and validity");
        f.move(2.25);
        check(f.on == 1, "next playback evaluates silently initialized visit");
    }
    {
        Fixture f(false);
        int secondOn = 0, secondOff = 0;
        auto* second = static_cast<ChataigneTimeTrigger*>(f.layer->ttm->addItem(nullptr, var(), false));
        second->time->setValue(2);
        second->csm->addItem(new CounterConsequence(secondOn), var(), false);
        second->untcsm->addItem(new CounterConsequence(secondOff), var(), false);
        auto* condition = static_cast<ManualCondition*>(second->cdm->addItem(new ManualCondition(), var(), false));
        second->cdm->checkAllConditions(0);
        f.layer->forwardSeekPoints->setValueWithData(1);
        f.valid(true); f.seek(4);
        check(f.on == 0 && secondOn == 0 && secondOff == 1, "latest selection precedes condition evaluation");
        f.seek(0); second->enabled->setValue(false); f.seek(4);
        check(f.on == 1 && secondOff == 2, "disabled latest cue is excluded from selection");
        ignoreUnused(condition);
    }
    {
        Fixture f(false);
        int tied = 0;
        auto* second = new ChataigneTimeTrigger("Tied cue");
        second->time->setValue(1);
        f.layer->ttm->addItem(second, var(), false);
        second->csm->addItem(new CounterConsequence(tied), var(), false);
        f.layer->forwardSeekPoints->setValueWithData(1);
        f.seek(2);
        check(tied == 1 && f.off == 0, "latest timestamp ties use persisted item order");
        f.seek(0); f.layer->ttm->setItemIndex(second, 0, false); f.seek(2);
        check(tied == 1 && f.off == 2, "explicit order edits update latest-cue tie resolution");
        f.trigger->time->setValue(5); f.seek(0); f.seek(2);
        check(tied == 2, "moving cues updates boundary queries");
    }
    for (int setting : { TimeTrigger::inheritSeek, TimeTrigger::alwaysSeek, TimeTrigger::playingSeek,
                         TimeTrigger::stoppedSeek, TimeTrigger::neverSeek })
    {
        for (bool playing : { false, true })
        {
            Fixture f(false);
            f.sequence.isPlaying->setValue(playing, true);
            f.sequence.evaluateOnSeek->setValueWithData(Sequence::NEVER);
            f.trigger->forwardSeek->setValueWithData(setting);
            f.seek(2);
            bool expected = setting == TimeTrigger::alwaysSeek || (setting == TimeTrigger::playingSeek && playing)
                || (setting == TimeTrigger::stoppedSeek && !playing);
            check(f.off == (expected ? 1 : 0), "forward seek override playing/stopped matrix");
            f.trigger->backwardSeek->setValueWithData(setting);
            f.seek(0);
            check(f.off == (expected ? 2 : 0), "backward seek override playing/stopped matrix");
            f.sequence.isPlaying->setValue(false, true);
        }
    }
    {
        Fixture f(false);
        f.valid(true); f.seek(2); int before = f.on;
        f.move(0);
        check(f.on == before + 1, "reverse playback evaluates point instead of rewind action");
    }
    {
        Fixture f(false);
        f.valid(true); f.move(1); f.seek(0.5);
        check(f.off == 1, "rewinding from an exact point boundary rearms that cue");
        f.seek(1);
        check(f.on == 2, "rearmed exact-boundary cue executes on the next forward crossing");
    }
    {
        Fixture f(false);
        f.trigger->time->setValue(0);
        f.sequence.isPlaying->setValue(true, true);
        f.layer->ttm->sequencePlayStateChanged(&f.sequence);
        f.layer->ttm->sequencePlayStateChanged(&f.sequence);
        f.move(0.5);
        check(f.off == 1, "time-zero point fires once on start, not again on first frame");
        f.sequence.isPlaying->setValue(false, true);
    }
    {
        Fixture f;
        f.valid(true); f.move(2);
        f.move(0.5, Sequence::TimeChangeKind::Loop);
        check(!f.trigger->duration->active, "loop completes outgoing duration visit");
        f.move(1.5);
        check(f.on == 2, "loop starts fresh validity history and evaluation budget");
    }
    {
        Fixture f(false);
        f.trigger->time->setValue(0);
        f.move(0, Sequence::TimeChangeKind::Loop);
        f.move(0, Sequence::TimeChangeKind::Loop);
        check(f.off == 2, "whole-period loop updates execute even when the displayed time is unchanged");
    }
    {
        Fixture f;
        f.sequence.totalTime->setValue(4);
        f.trigger->time->setValue(0); f.trigger->length->setValue(5);
        f.trigger->duration->everValidExit->setValueWithData(ChataigneTimeTrigger::action_triggerFalse);
        f.valid(true); f.seek(0.5);
        f.sequence.playSpeed->setValue(-1);
        f.move(4, Sequence::TimeChangeKind::Loop);
        check(f.trigger->duration->active && f.on == 2 && f.off == 1,
            "reverse loop starts a fresh visit in a duration extending past the sequence end");
        f.move(3.5);
        check(f.on == 2 && f.off == 1, "reverse loop entry is not repeated on the following playback frame");
    }
    {
        Fixture f(false);
        f.trigger->time->setValue(0); f.valid(true); f.trigger->csm->clear();
        int later = 0;
        auto* block = new ChataigneTimeTrigger("Loop block");
        block->length->setValue(2);
        f.layer->ttm->addItem(block, var(), false);
        block->csm->addItem(new CounterConsequence(later), var(), false);
        f.trigger->csm->addItem(new CounterConsequence(f.on, [&f] { f.seek(3); }), var(), false);
        f.move(0, Sequence::TimeChangeKind::Loop);
        check(f.on == 1 && later == 0 && !block->duration->active,
            "time-zero cue seek cancels later duration entries during loop dispatch");
    }
}

static void loadingAndConversion()
{
    {
        Fixture loaded;
        const var initial = loaded.trigger->getJSONData();
        loaded.move(1); loaded.valid(true);
        loaded.trigger->loadJSONData(initial);
        check(loaded.trigger->duration->active && !loaded.trigger->duration->everValid
            && !loaded.trigger->duration->trueExecuted, "in-place loading resets old visit history without consequences");
        check(loaded.on == 1 && loaded.off == 0, "reloading controls does not execute exit or entry actions");
    }
    Fixture f;
    f.mode(ChataigneTimeTrigger::always);
    f.trigger->duration->neverValidExit->setValueWithData(ChataigneTimeTrigger::action_triggerTrue);
    f.trigger->length->setValue(0);
    check(!f.trigger->duration && f.trigger->getParameterByName("Evaluate conditions", true) == nullptr,
        "conversion removes duration parameter objects");
    var point = f.trigger->getJSONData();
    check(point.hasProperty("durationSettings"), "converted point stores compact nondefault choices");
    ChataigneTimeTrigger restored;
    restored.loadJSONData(point);
    check(!restored.duration, "loading converted point does not allocate duration controls");
    restored.length->setValue(2);
    check(restored.duration->evaluation->getValueDataAsEnum<ChataigneTimeTrigger::evaluateSetting>() == ChataigneTimeTrigger::always,
        "duration choices survive point save/load and restoration");
    check(restored.duration->neverValidExit->getValueDataAsEnum<ChataigneTimeTrigger::possibleActions>() == ChataigneTimeTrigger::action_triggerTrue,
        "restored never-valid exit choice");
    var longData = restored.getJSONData();
    auto* parameters = longData.getProperty("parameters", var()).getArray();
    check(parameters != nullptr, "duration parameter data exists");
    // Put length last so lazy loading cannot rely on parameter ordering.
    for (int i = 0; i < parameters->size(); ++i)
        if ((*parameters)[i].getProperty("controlAddress", "") == "/" + restored.length->shortName)
        { auto length = (*parameters)[i]; parameters->remove(i); parameters->add(length); break; }
    ChataigneTimeTrigger reordered;
    reordered.loadJSONData(longData);
    check(reordered.duration && reordered.duration->evaluation->getValueDataAsEnum<ChataigneTimeTrigger::evaluateSetting>() == ChataigneTimeTrigger::always,
        "duration settings load before length regardless of parameter order");
    check(f.on == 0 && f.off == 0, "conversion and loading execute no consequences");
    f.trigger->length->setValue(2);
    auto* undo = UndoMaster::getInstance();
    undo->clearUndoHistory();
    f.trigger->length->setUndoableValue(2, 0);
    check(!f.trigger->duration, "undoable length edit removes duration objects");
    check(undo->undo() && f.trigger->duration
        && f.trigger->duration->evaluation->getValueDataAsEnum<ChataigneTimeTrigger::evaluateSetting>() == ChataigneTimeTrigger::always,
        "undo restores duration settings without stale parameter pointers");
    check(undo->redo() && !f.trigger->duration, "redo converts back to a lightweight point");
    undo->clearUndoHistory();

    ChataigneTimeTrigger fresh;
    check(!fresh.getJSONData().hasProperty("durationSettings"), "fresh point has no duration payload");
    fresh.length->setValue(2);
    var legacy = fresh.getJSONData();
    var legacyParameters;
    auto parameter = [](const String& address, const String& value)
    {
        var p(new DynamicObject());
        p.getDynamicObject()->setProperty("controlAddress", address);
        p.getDynamicObject()->setProperty("value", value);
        return p;
    };
    legacyParameters.append(parameter("/" + StringUtil::toShortName("On exit (active)"), "Deactivate - Trigger FALSE"));
    legacyParameters.append(parameter("/" + StringUtil::toShortName("On exit (inactive)"), "Deactivate - Trigger TRUE"));
    var length(new DynamicObject()); length.getDynamicObject()->setProperty("controlAddress", "/" + fresh.length->shortName);
    length.getDynamicObject()->setProperty("value", 2);
    legacyParameters.append(length);
    legacy.getDynamicObject()->setProperty("parameters", legacyParameters);
    ChataigneTimeTrigger old;
    old.loadJSONData(legacy);
    check(old.duration->everValidExit->getValueDataAsEnum<ChataigneTimeTrigger::possibleActions>() == ChataigneTimeTrigger::action_triggerFalse,
        "legacy successful exit action migrates");
    check(old.duration->neverValidExit->getValueDataAsEnum<ChataigneTimeTrigger::possibleActions>() == ChataigneTimeTrigger::action_triggerTrue,
        "legacy invalid exit TRUE action migrates");
    legacyParameters.getArray()->removeLast();
    legacy.getDynamicObject()->setProperty("parameters", legacyParameters);
    ChataigneTimeTrigger oldPoint;
    oldPoint.loadJSONData(legacy, true);
    check(!oldPoint.duration && oldPoint.getJSONData().hasProperty("durationSettings"),
        "legacy hidden duration choices stay compact on point cues");
    check(oldPoint.getParameterByName("Evaluate conditions", true) == nullptr
        && oldPoint.getParameterByName("On exit if ever valid", true) == nullptr,
        "generic create-missing loading does not recreate hidden duration parameters on points");
    {
        Sequence source;
        auto* layer = static_cast<ChataigneTriggerLayer*>(source.layerManager->addItem(new ChataigneTriggerLayer(&source), var(), false));
        Array<TimeTrigger*> cues;
        for (int i = 0; i < 32; ++i)
        {
            auto* cue = new ChataigneTimeTrigger("Saved cue " + String(i));
            cue->time->setValue(1);
            cues.add(cue);
        }
        layer->ttm->addItems(cues, var(), false);
        Sequence copy;
        auto* loaded = static_cast<ChataigneTriggerLayer*>(copy.layerManager->addItem(new ChataigneTriggerLayer(&copy), layer->getJSONData(), false));
        bool stable = loaded->ttm->items.size() == 32;
        for (int i = 0; i < loaded->ttm->items.size(); ++i)
            stable = stable && loaded->ttm->items[i]->niceName == "Saved cue " + String(i);
        check(stable, "bulk loading preserves persisted order for many equal-time cues");
    }
}

static void mutationAndScale()
{
    {
        Fixture f(false);
        f.valid(true);
        int incoming = 0;
        auto* block = new ChataigneTimeTrigger("Incoming block");
        block->time->setValue(1.5); block->length->setValue(6);
        f.layer->ttm->addItem(block, var(), false);
        block->csm->addItem(new CounterConsequence(incoming), var(), false);
        f.trigger->csm->addItem(new CounterConsequence(f.on, [&f] { f.seek(4); }), var(), false);
        f.move(3);
        check(block->duration->active && incoming == 1,
            "nested seek synchronizes destination blocks not yet entered by the interrupted span");
    }
    {
        Fixture f(false);
        f.valid(true);
        int skipped = 0;
        auto* later = new ChataigneTimeTrigger("Later disabled cue");
        later->time->setValue(2); f.layer->ttm->addItem(later, var(), false);
        later->csm->addItem(new CounterConsequence(skipped), var(), false);
        f.trigger->csm->addItem(new CounterConsequence(f.on, [&f] { f.layer->enabled->setValue(false); }), var(), false);
        f.move(3);
        check(skipped == 0, "consequences respect a layer disabled by an earlier cue in the span");
    }
    {
        Fixture f;
        f.valid(true); f.layer->enabled->setValue(false); f.seek(2);
        f.layer->triggerAllTrue->trigger();
        check(f.on == 0 && f.trigger->duration->active, "disabled layers reconcile activity without executing commands");
        f.layer->enabled->setValue(true); f.layer->triggerAllFalse->trigger();
        check(f.off == 1 && f.trigger->duration->active, "layer FALSE control runs without changing activity");
        f.trigger->enabled->setValue(false); f.layer->triggerAllTrue->trigger();
        check(f.on == 0, "manual layer commands respect disabled triggers");
    }
    {
        Fixture f(false);
        int skipped = 0;
        struct Listener : Sequence::SequenceListener
        {
            int staleCalls = 0;
            void sequenceTimeChanged(Sequence*, const Sequence::TimeChange& change) override
            { if (change.currentTime == 3) ++staleCalls; }
        } listener;
        f.sequence.addSequenceListener(&listener);
        f.trigger->csm->clear();
        f.valid(true);
        f.trigger->csm->addItem(new CounterConsequence(f.on, [&f] { f.seek(0); }), var(), false);
        auto* later = static_cast<ChataigneTimeTrigger*>(f.layer->ttm->addItem(nullptr, var(), false));
        later->time->setValue(2);
        later->csm->addItem(new CounterConsequence(skipped), var(), false);
        f.move(3);
        check(f.on == 1 && skipped == 0, "consequence seek cancels stale remaining timeline actions");
        check(listener.staleCalls == 0, "consequence seek cancels stale notifications to remaining sequence listeners");
        f.sequence.removeSequenceListener(&listener);
    }
    {
        Fixture f(false);
        f.valid(true);
        f.trigger->csm->addItem(new CounterConsequence(f.on, [&f] { f.layer->ttm->clear(); }), var(), false);
        f.move(2);
        check(f.layer->ttm->items.isEmpty(), "deleting trigger and consequences during dispatch is safe");
    }
    {
        Fixture f;
        f.mode(ChataigneTimeTrigger::always);
        f.trigger->duration->invalidHandling->setValueWithData(ChataigneTimeTrigger::immediately);
        f.trigger->csm->addItem(new CounterConsequence(f.on, [&f] { f.valid(false); }), var(), false);
        f.valid(true); f.move(1);
        f.move(1.25);
        check(f.off == 1 && !f.trigger->duration->valid && f.trigger->duration->everValid,
            "consequence validity changes are serialized without losing history");
    }
    {
        Fixture f(false);
        auto* zero = f.trigger;
        const auto start = Time::getMillisecondCounterHiRes();
        for (int i = 0; i < 2000; ++i)
        {
            auto* t = new ChataigneTimeTrigger("Point " + String(i));
            t->time->setValue(2 + i * 0.01);
            f.layer->ttm->addItem(t, var(), false);
            if (t->duration || t->getParameterByName("Evaluate conditions", true) != nullptr)
                throw std::runtime_error("point batch allocated duration support");
        }
        f.seek(29);
        check(f.layer->ttm->items.size() == 2001 && !zero->duration, "thousands of points remain duration-free");
        std::cout << "2001 point cues, build and seek: " << Time::getMillisecondCounterHiRes() - start << " ms\n";
    }
}

static void asynchronousTransport()
{
#if JUCE_WINDOWS
    Fixture f(false);
    struct Listener : Sequence::SequenceListener
    {
        std::vector<Sequence::TimeChange> changes;
        void sequenceTimeChanged(Sequence*, const Sequence::TimeChange& change) override { changes.push_back(change); }
    } listener;
    f.sequence.addSequenceListener(&listener);
    std::thread worker([&f]
    {
        f.sequence.setCurrentTime(1, true, false, Sequence::TimeChangeKind::Playback);
        f.sequence.setCurrentTime(2, true, false, Sequence::TimeChangeKind::Playback);
    });
    worker.join();
    const auto deadline = Time::getMillisecondCounterHiRes() + 2000;
    MSG message;
    while (listener.changes.size() < 2 && Time::getMillisecondCounterHiRes() < deadline)
    {
        if (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) { TranslateMessage(&message); DispatchMessageW(&message); }
        else Thread::sleep(1);
    }
    check(listener.changes.size() == 2 && listener.changes[0].previousTime == 0
        && listener.changes[0].currentTime == 1 && listener.changes[1].previousTime == 1
        && listener.changes[1].currentTime == 2, "worker playback retains immutable time spans across asynchronous delivery");
    check(f.off == 1, "queued playback crossings execute exactly once despite the latest parameter value");
    std::thread staleWorker([&f]
    {
        f.sequence.setCurrentTime(3, true, false, Sequence::TimeChangeKind::Playback);
        f.sequence.setCurrentTime(4, true, false, Sequence::TimeChangeKind::Playback);
    });
    staleWorker.join();
    f.seek(0);
    const int afterSeek = f.off;
    const auto flushDeadline = Time::getMillisecondCounterHiRes() + 100;
    while (Time::getMillisecondCounterHiRes() < flushDeadline)
    {
        if (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) { TranslateMessage(&message); DispatchMessageW(&message); }
        else Thread::sleep(1);
    }
    f.sequence.currentTime->setValue(0.5);
    check(listener.changes.back().previousTime == 0 && f.off == afterSeek,
        "seek cancels queued playback without rolling back the next direct-edit time span");
    f.sequence.removeSequenceListener(&listener);
#endif
}

static void projectClearDuringDispatch()
{
    auto* sequence = ChataigneSequenceManager::getInstance()->addItem(new Sequence(), var(), false);
    auto* layer = static_cast<ChataigneTriggerLayer*>(sequence->layerManager->addItem(new ChataigneTriggerLayer(sequence), var(), false));
    auto* trigger = static_cast<ChataigneTimeTrigger*>(layer->ttm->addItem(nullptr, var(), false));
    trigger->time->setValue(1);
    int calls = 0, skipped = 0;
    trigger->csm->addItem(new CounterConsequence(calls, [] { Engine::mainEngine->clear(); }), var(), false);
    trigger->csm->addItem(new CounterConsequence(skipped), var(), false);
    WeakReference<ControllableContainer> safeSequence(sequence);
    sequence->setCurrentTime(2, true, false, Sequence::TimeChangeKind::Playback);
    check(safeSequence == nullptr && calls == 1 && skipped == 0, "clearing the project during a consequence cancels remaining dispatch safely");
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    TriggerTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        std::cerr << "Duration behavior\n"; durationBehavior();
        std::cerr << "Point cues and seeks\n"; pointsAndSeeks();
        std::cerr << "Loading and conversion\n"; loadingAndConversion();
        std::cerr << "Mutation and scale\n"; mutationAndScale();
        std::cerr << "Asynchronous transport\n"; asynchronousTransport();
        std::cerr << "Project clear during dispatch\n"; projectClearDuringDispatch();
        std::cout << "Timeline trigger integration tests passed (" << checks << " checks)\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Timeline trigger integration failed: " << e.what() << "\n";
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
