#include "MainIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include <iostream>
#include <stdexcept>
#include <random>
#undef main // SDL's public headers rename main for applications using SDL startup.
#include <windows.h>

// JUCE disables nested modal loops in this build. Pump this test process's
// message queue so real sequence clocks and component paint timers execute.
static void pumpMessages(int milliseconds)
{
    const double end = Time::getMillisecondCounterHiRes() + milliseconds;
    do
    {
        MSG message;
        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        { TranslateMessage(&message); DispatchMessage(&message); }
        Thread::sleep(1);
    } while (Time::getMillisecondCounterHiRes() < end);
}

class CVValuesTestApplication : public OrganicApplication
{
public:
    CVValuesTestApplication() : OrganicApplication("CV Values tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

int main(int argc, char** argv)
{
    ScopedJuceInitialiser_GUI gui;
    CVValuesTestApplication app;
    app.engine.reset(new ChataigneEngine());
    struct TestTheme : LookAndFeelOO { ~TestTheme() override { LookAndFeel::setDefaultLookAndFeel(nullptr); } } theme;
    LookAndFeel::setDefaultLookAndFeel(&theme);
    try
    {
        auto* group = CVGroupManager::getInstance()->addItem(new CVGroup("CV Values Test"), var(), false);
        FloatParameter original("Number", "", 0, -1000, 1000);
        IntParameter integer("Integer", "", 0, -1000, 1000);
        ColorParameter color("Color", "", Colours::black);
        Point2DParameter point("Position", "");
        Point3DParameter point3("Position 3D", "");
        BoolParameter boolean("Boolean", "", false);
        StringParameter text("Text", "", "base");
        EnumParameter choice("Choice", "");
        choice.addOption("Base", 0)->addOption("Other", 1);
        for (auto* p : Array<Parameter*>{ &original, &integer, &color, &point, &boolean, &text, &choice, &point3 }) group->addItemFromParameter(p, false);
        auto* number = static_cast<Parameter*>(group->values.items[0]->controllable);
        auto* base = group->pm->addItem(new CVPreset(group), var(), false);
        base->setNiceName("Base");
        auto* preset = group->pm->addItem(new CVPreset(group), var(), false);
        preset->setNiceName("Bright");
        preset->values.getParameterPresetForSource(number)->parameter->setValue(100);
        auto* sequence = static_cast<ChataigneSequence*>(ChataigneSequenceManager::getInstance()->addItem(nullptr, var(), false));
        sequence->setNiceName("CV Values Test");
        auto* layer = static_cast<CVValuesLayer*>(sequence->layerManager->addItem(sequence->layerManager->factory.create("CV Values"), var(), false));
        auto* row = layer->targets.addItem(nullptr, var(), false);
        row->group->setValueFromTarget(group); row->basePreset->setValueFromTarget(base);
        layer->fadeIn->setValue(0); layer->fadeOut->setValue(0);
        auto* block = new CVValuesBlock(layer);
        block->time->setValue(2); block->coreLength->setValue(4);
        layer->blocks.addItem(block, var(), false);
        layer->rebuildSnapshot();
        check(block->groups.items.size() == 1, "group synchronization");
        auto* entry = block->groups.items[0];
        entry->preset->setValueFromTarget(preset);
        layer->rebuildSnapshot();
        int seekIndex = 0;
        auto seek = [&](double t, double expected)
        {
            ++seekIndex;
            sequence->setCurrentTime(t, true, true); sequence->evaluateCVValues();
            if (std::abs(number->doubleValue() - expected) >= 1.0e-5)
                std::cerr << "seek=" << seekIndex << " t=" << t << " expected=" << expected << " actual=" << number->doubleValue()
                    << " samples=" << layer->evaluate(t).size() << " warning=" << layer->getWarningMessage() << "\n";
            check(std::abs(number->doubleValue() - expected) < 1.0e-5, "sequence evaluation");
        };
        seek(0, 0); seek(3, 100); seek(6, 0); seek(4, 100);
        preset->values.getParameterPresetForSource(number)->parameter->setValue(120);
        seek(4, 120); // A direct seek consumes an edit even before async delivery.
        block->blockType->setValueWithData(1);
        layer->rebuildSnapshot();
        check(entry->values.items.size() == 8, "typed custom overrides");
        auto* custom = entry->values.items[0];
        custom->overrideValue->setValue(true); custom->value->setValue(200);
        layer->rebuildSnapshot(); seek(3, 200);
        custom->animated->setValue(true);
        custom->automation->clear();
        custom->automation->addKey(0, 0, false)->easingType->setValueWithData(Easing::LINEAR);
        custom->automation->addKey(4, 400, false)->easingType->setValueWithData(Easing::LINEAR);
        layer->rebuildSnapshot(); seek(3, 100); seek(5, 300); seek(3, 100);
        custom->overrideValue->setValue(false); custom->unsetBehavior->setValueWithData(1);
        layer->rebuildSnapshot(); number->setValue(777); seek(3, 777);
        custom->overrideValue->setValue(true);
        layer->rebuildSnapshot();

        const var saved = layer->getJSONData();
        const String json = JSON::toString(saved);
        check(json.contains("valueType") && json.contains("animation"), "serialized override and curve");
        layer->loadJSONData(JSON::parse(json)); layer->rebuildSnapshot();
        check(layer->targets.items.size() == 1 && layer->blocks.items.size() == 1, "layer round trip");
        seek(3, 100);
        block = static_cast<CVValuesBlock*>(layer->blocks.items[0]);
        entry = block->groups.items[0]; custom = entry->values.items[0];
        check(entry->values.items[6]->value->type == Controllable::ENUM, "enum restored");
        check(static_cast<EnumParameter*>(entry->values.items[6]->value)->enumValues.size() == 2, "enum options restored");
        block->setCoreLength(8, true); layer->rebuildSnapshot(); seek(6, 200);
        block->setCoreLength(4, false); layer->rebuildSnapshot(); seek(4, 100);

        // Invalid direct edits are reverted; imported invalid blocks are retained.
        auto* second = new CVValuesBlock(layer);
        second->time->setValue(5); second->coreLength->setValue(4);
        layer->blocks.addItem(second, var(), false); layer->rebuildSnapshot();
        second->time->setValue(2); check(second->time->doubleValue() == 5, "identical-start edit rejected");
        second->coreLength->setValue(.5); check(second->coreLength->doubleValue() == 4, "containment edit rejected");

        auto* lower = static_cast<CVValuesLayer*>(sequence->layerManager->addItem(new CVValuesLayer(sequence, var()), var(), false));
        auto* lowerRow = lower->targets.addItem(nullptr, var(), false);
        lowerRow->group->setValueFromTarget(group); lowerRow->basePreset->setValueFromTarget(preset);
        lower->rebuildSnapshot(); seek(3, 120);
        lower->enabled->setValue(false); seek(3, 50);
        sequence->layerManager->setItemIndex(lower, 0, false);
        lower->enabled->setValue(true); seek(3, 50);
        lower->enabled->setValue(false);
        group->controlMode->setValueWithData(CVGroup::WEIGHTS);
        number->setValue(555); sequence->evaluateCVValues();
        check(number->doubleValue() == 555, "non-Free group skipped");
        group->controlMode->setValueWithData(CVGroup::FREE); layer->rebuildSnapshot();

        // Exercise the host layer's sparse channels, edits, and priority without
        // explicit snapshot rebuilds after every edit (the real async edit path).
        layer->enabled->setValue(false); lower->enabled->setValue(false);
        auto* edit = static_cast<CVValuesLayer*>(sequence->layerManager->addItem(new CVValuesLayer(sequence, var()), var(), false));
        auto* editRow = edit->targets.addItem(nullptr, var(), false);
        editRow->group->setValueFromTarget(group); editRow->basePreset->setValueFromTarget(base);
        edit->fadeIn->setValue(0); edit->fadeOut->setValue(0);
        auto add = [&](double start, double length)
        {
            auto* b = new CVValuesBlock(edit); b->time->setValue(start); b->coreLength->setValue(length);
            b->blockType->setValueWithData(1); edit->blocks.addItem(b, var(), false); edit->handleAsyncUpdate(); return b;
        };
        auto* a = add(2, 4); auto* b = add(5, 4);
        auto overrideFor = [](CVValuesBlock* b, int index, var value)
        {
            auto* v = b->groups.items[0]->values.items[index];
            v->overrideValue->setValue(true); v->value->setValue(value); return v;
        };
        auto* av = overrideFor(a, 0, 100);
        auto* bv = overrideFor(b, 0, 800);
        av->animated->setValue(true); av->automation->clear();
        av->automation->addKey(0, 0)->easingType->setValueWithData(Easing::LINEAR);
        av->automation->addKey(4, 400)->easingType->setValueWithData(Easing::LINEAR);
        edit->handleAsyncUpdate(); seek(5.5, 575);
        bv->overrideValue->setValue(false); edit->handleAsyncUpdate(); seek(5.5, 350); seek(7, 400);
        bv->unsetBehavior->setValueWithData(1); edit->handleAsyncUpdate(); seek(5.5, 350);
        number->setValue(777); seek(7, 777);
        bv->unsetBehavior->setValueWithData(0); check(b->setTiming(8, 4), "valid move out of overlap");
        edit->handleAsyncUpdate(); seek(9, 0);
        edit->interpolationMode->setValueWithData(2); edit->handleAsyncUpdate(); seek(9, 400);
        edit->interpolationMode->setValueWithData(1); edit->handleAsyncUpdate(); seek(7, 400); seek(9, 400);
        edit->interpolationMode->setValueWithData(0); check(b->setTiming(5, 4), "valid overlap restored");
        av->animated->setValue(false); av->value->setValue(100); bv->overrideValue->setValue(true); bv->value->setValue(500);
        overrideFor(b, 1, 101); overrideFor(b, 2, var(Array<var>{ 1., 1., 1., 1. }));
        overrideFor(b, 3, var(Array<var>{ 10., 20. })); overrideFor(b, 4, true); overrideFor(b, 5, "other");
        overrideFor(b, 6, "Other"); overrideFor(b, 7, var(Array<var>{ 10., 20., 30. }));
        edit->handleAsyncUpdate(); seek(5.5, 300);
        auto actual = [&](int index) { return static_cast<Parameter*>(group->values.items[index]->controllable); };
        check(actual(1)->intValue() == 50, "integer interpolation follows parameter truncation");
        check(std::abs((double)actual(2)->value[0] - .5) < .01, "color interpolation");
        check((double)actual(3)->value[0] == 5 && (double)actual(3)->value[1] == 10, "2D interpolation");
        check(actual(4)->boolValue(), "boolean custom value");
        check(actual(5)->stringValue() == "other", "string custom value");
        check(static_cast<EnumParameter*>(actual(6))->getValueKey() == "Other", "enum custom value");
        check((double)actual(7)->value[2] == 15, "3D interpolation");
        auto* colorOverride = b->groups.items[0]->values.items[2];
        colorOverride->animated->setValue(true); colorOverride->gradient->clear();
        colorOverride->gradient->addColorAt(0, Colours::red); colorOverride->gradient->addColorAt(4, Colours::blue);
        auto* intOverride = b->groups.items[0]->values.items[1];
        intOverride->animated->setValue(true); intOverride->automation->clear();
        intOverride->automation->addKey(0, 0)->easingType->setValueWithData(Easing::LINEAR);
        intOverride->automation->addKey(4, 400)->easingType->setValueWithData(Easing::LINEAR);
        edit->handleAsyncUpdate(); seek(7, 500);
        check(actual(1)->intValue() == 200, "integer animation sampled locally");
        check(std::abs((double)actual(2)->value[0] - .5) < .01 && std::abs((double)actual(2)->value[2] - .5) < .01, "gradient sampled locally");
        check(!b->groups.items[0]->values.items[3]->automation && !b->groups.items[0]->values.items[3]->gradient, "vectors are static only");

        // Noise must be time-addressable too, rather than consuming an RNG.
        av->animated->setValue(true); av->automation->items[0]->easingType->setValueWithData(Easing::NOISE);
        edit->handleAsyncUpdate(); const double noiseValue = edit->evaluate(3.375)[0].value;
        edit->evaluate(4.875); check((double)edit->evaluate(3.375)[0].value == noiseValue, "deterministic noise");
        const var noiseData = edit->getJSONData();
        edit->loadJSONData(noiseData); edit->handleAsyncUpdate();
        check((double)edit->evaluate(3.375)[0].value == noiseValue, "noise survives reload");
        a = static_cast<CVValuesBlock*>(edit->blocks.items[0]); b = static_cast<CVValuesBlock*>(edit->blocks.items[1]);
        av = a->groups.items[0]->values.items[0]; bv = b->groups.items[0]->values.items[0];
        av->automation->items[0]->easingType->setValueWithData(Easing::LINEAR); edit->handleAsyncUpdate();

        // Playback ticks, reverse ticks, shuffled seeks, jumps, and repeated loops
        // agree at identical double-precision timestamps through Sequence itself.
        std::vector<double> visits;
        for (int i = 0; i <= 140; ++i) visits.push_back(i / 10.0);
        auto verifyVisits = [&](bool seekMode)
        {
            for (double time : visits)
            {
                const double expected = edit->evaluate(time)[0].value;
                sequence->setCurrentTime(time, true, seekMode);
                check(std::abs(number->doubleValue() - expected) < 1.e-6, "seek/playback invariance");
            }
        };
        verifyVisits(false); std::reverse(visits.begin(), visits.end()); verifyVisits(false);
        std::mt19937 random(42); std::shuffle(visits.begin(), visits.end(), random); verifyVisits(true);
        for (int loop = 0; loop < 3; ++loop) { std::sort(visits.begin(), visits.end()); verifyVisits(true); }

        // Validate the final batch, so moving adjacent/overlapping selected blocks
        // is not rejected because of an invalid intermediate one-block edit.
        const var beforeMove = edit->blocks.getJSONData();
        check(edit->blocks.applyTimings({ {a, 12, 4}, {b, 15, 4} }), "batch move accepts final layout");
        const var afterMove = edit->blocks.getJSONData();
        UndoMaster::getInstance()->performAction("Move test blocks", edit->blocks.stateAction(beforeMove, afterMove, true));
        check(UndoMaster::getInstance()->undo(), "move undo");
        check(edit->blocks.items[0]->time->doubleValue() == 2, "move undo timing");
        check(UndoMaster::getInstance()->redo(), "move redo");
        check(edit->blocks.items[0]->time->doubleValue() == 12, "move redo timing");
        edit->blocks.loadJSONData(beforeMove); edit->handleAsyncUpdate();

        struct ClipboardGuard
        {
            String previous = SystemClipboard::getTextFromClipboard();
            ~ClipboardGuard() { SystemClipboard::copyTextToClipboard(previous); }
        } clipboardGuard;
        var clipboard(new DynamicObject());
        clipboard.getDynamicObject()->setProperty("itemType", edit->blocks.itemDataType);
        clipboard.getDynamicObject()->setProperty("items", var(Array<var>{ edit->blocks.items[0]->getJSONData() }));
        SystemClipboard::copyTextToClipboard(JSON::toString(clipboard));
        sequence->setCurrentTime(2, true, true);
        check(edit->blocks.addItemsFromClipboard(false).isEmpty(), "invalid paste rejected atomically");
        sequence->setCurrentTime(20, true, true);
        check(edit->blocks.addItemsFromClipboard(false).size() == 1, "valid paste accepted");
        check(edit->blocks.items.getLast()->time->doubleValue() == 20, "paste anchors at seeker");
        check(UndoMaster::getInstance()->undo(), "paste undo");
        check(edit->blocks.items.size() == 2, "paste undo structure");
        check(UndoMaster::getInstance()->redo(), "paste redo");
        check(edit->blocks.items.getLast()->time->doubleValue() == 20, "paste redo preserves offset");
        check(UndoMaster::getInstance()->undo(), "paste cleanup undo");
        auto* copySource = static_cast<CVValuesBlock*>(edit->blocks.items[0]);
        copySource->copy(); sequence->setCurrentTime(20, true, true);
        check(edit->blocks.addItemsFromClipboard(false).size() == 1, "single block clipboard accepted");
        check(static_cast<CVValuesBlock*>(edit->blocks.items.getLast())->groups.items[0]->values.items[0]->automation->items.size() == 2, "single copy retains animation");
        check(UndoMaster::getInstance()->undo(), "single paste undo");
        auto* duplicateSource = edit->blocks.items[1];
        edit->blocks.askForDuplicateItem(duplicateSource);
        check(edit->blocks.items.size() == 3 && edit->blocks.items.getLast()->time->doubleValue() == 9, "duplicate retains adjacent timing");
        edit->blocks.askForDuplicateItem(duplicateSource);
        check(edit->blocks.items.size() == 3, "invalid duplicate rejected without moving blocks");
        check(UndoMaster::getInstance()->undo(), "duplicate undo");

        // Live preset exclusions and broken links have a per-channel write mask.
        b = static_cast<CVValuesBlock*>(edit->blocks.items[1]);
        auto* referenced = group->pm->addItem(new CVPreset(group), var(), false);
        referenced->setNiceName("Referenced");
        auto* pp = referenced->values.getParameterPresetForSource(number); pp->parameter->setValue(900);
        b->blockType->setValueWithData(0); b->groups.items[0]->preset->setValueFromTarget(referenced);
        pp->interpolationMode->setValueWithData(ParameterPreset::NONE); edit->handleAsyncUpdate();
        number->setValue(777); seek(7, 777); seek(5.5, 350);
        pp->interpolationMode->setValueWithData(ParameterPreset::CHANGE_AT_END); edit->handleAsyncUpdate(); seek(5.5, 350); seek(6, 900);
        pp->interpolationMode->setValueWithData(ParameterPreset::CHANGE_AT_START); edit->handleAsyncUpdate(); seek(5, 900);
        referenced->setNiceName("Renamed"); edit->handleAsyncUpdate(); seek(7, 900);
        check(b->groups.items[0]->preset->getTargetContainer() == referenced, "preset rename tracks reference");
        group->pm->removeItem(referenced, false); edit->handleAsyncUpdate();
        number->setValue(777); seek(7, 777);
        check(b->groups.items[0]->getWarningMessage().isNotEmpty(), "broken preset remains visible with warning");
        b->blockType->setValueWithData(1); edit->handleAsyncUpdate();

        // Multiple groups, independent base validity, and lower-layer no-write.
        auto* group2 = CVGroupManager::getInstance()->addItem(new CVGroup("Second Target"), var(), false);
        FloatParameter secondNumber("Number", "", 21); group2->addItemFromParameter(&secondNumber, false);
        auto* base2 = group2->pm->addItem(new CVPreset(group2), var(), false);
        auto* row2 = edit->targets.addItem(nullptr, var(), false);
        row2->group->setValueFromTarget(group2); row2->basePreset->setValueFromTarget(base2); edit->handleAsyncUpdate();
        seek(7, 500);
        check(static_cast<Parameter*>(group2->values.items[0]->controllable)->doubleValue() == 21, "multiple targets evaluate independently");
        row2->basePreset->setValueFromTarget(static_cast<ControllableContainer*>(nullptr)); edit->handleAsyncUpdate();
        static_cast<Parameter*>(group2->values.items[0]->controllable)->setValue(99); seek(7, 500);
        check(static_cast<Parameter*>(group2->values.items[0]->controllable)->doubleValue() == 99 && row2->getWarningMessage().isNotEmpty(), "missing base skips group with warning");
        edit->targets.removeItem(row2, false); edit->handleAsyncUpdate();
        CVGroupManager::getInstance()->removeItem(group2, false);
        layer->enabled->setValue(true); layer->handleAsyncUpdate();
        bv = b->groups.items[0]->values.items[0]; bv->overrideValue->setValue(false); bv->unsetBehavior->setValueWithData(1); edit->handleAsyncUpdate();
        seek(7, 200); // Lower layer abstains; upper layer's second block inherits its endpoint.
        sequence->layerManager->setItemIndex(edit, 0, false); seek(7, 200);
        sequence->layerManager->setItemIndex(edit, sequence->layerManager->items.size() - 1, false);
        bv->overrideValue->setValue(true); edit->handleAsyncUpdate(); seek(7, 500);
        layer->enabled->setValue(false);

        // Taking timeline control cancels the group's existing timed preset fade.
        group->goToPreset(preset, 10, nullptr);
        sequence->evaluateCVValues(); check(!group->isThreadRunning(), "timeline stops CV preset transition");
        a = static_cast<CVValuesBlock*>(edit->blocks.items[0]); b = static_cast<CVValuesBlock*>(edit->blocks.items[1]);
        const var beforeStretch = edit->blocks.getJSONData();
        check(edit->blocks.applyTimings({ {a, 2, 8}, {b, 9, 8} }, true), "batch stretch");
        UndoMaster::getInstance()->performAction("Stretch test blocks", edit->blocks.stateAction(beforeStretch, edit->blocks.getJSONData(), true));
        check(UndoMaster::getInstance()->undo(), "stretch undo");
        a = static_cast<CVValuesBlock*>(edit->blocks.items[0]); b = static_cast<CVValuesBlock*>(edit->blocks.items[1]);
        check(a->groups.items[0]->values.items[0]->automation->items[1]->position->doubleValue() == 4, "undo restores animation keys");
        UndoMaster::getInstance()->performActions("Insert test span", edit->getInsertTimespanInternal(0, 10));
        check(edit->blocks.items[0]->time->doubleValue() == 12 && edit->blocks.items[1]->time->doubleValue() == 15, "span insertion atomic");
        check(UndoMaster::getInstance()->undo(), "span undo");
        UndoMaster::getInstance()->performActions("Remove test span", edit->getRemoveTimespanInternal(0, 1));
        check(edit->blocks.items[0]->time->doubleValue() == 1 && edit->blocks.items[1]->time->doubleValue() == 4, "span removal atomic");
        check(UndoMaster::getInstance()->undo(), "span removal undo");
        a = static_cast<CVValuesBlock*>(edit->blocks.items[0]); b = static_cast<CVValuesBlock*>(edit->blocks.items[1]);
        check(!edit->blocks.addItem(new CVValuesBlock(edit), var(), false), "invalid creation rejected");
        b->enabled->setValue(false); edit->handleAsyncUpdate(); b->time->setValue(2);
        b->enabled->setValue(true); check(!b->enabled->boolValue(), "invalid enable rejected");
        b->time->setValue(5); b->enabled->setValue(true); edit->handleAsyncUpdate();

        // Imported conflicts stay in the document and can be repaired explicitly.
        var imported = edit->blocks.getJSONData();
        auto* list = imported.getProperty("items", var()).getArray(); list->add((*list)[0].clone());
        edit->blocks.loadJSONData(imported); edit->handleAsyncUpdate();
        check(edit->blocks.items.size() == 3 && std::atomic_load(&edit->snapshot)->timeline.blocks.size() == 2, "invalid imports retained and excluded");
        edit->blocks.loadJSONData(beforeMove); edit->handleAsyncUpdate();

        // Missing base values use parameter defaults; added/removed variables and
        // changes to metadata refresh wrappers and invalidate weak targets safely.
        auto* added = new FloatParameter("Added", "", 17, -100, 100); added->setValue(99);
        var addedData(new DynamicObject()); addedData.getDynamicObject()->setProperty("type", group->values.getTypeForControllableType("Float"));
        group->values.addItem(new GenericControllableItem(added, addedData), var(), false);
        edit->handleAsyncUpdate(); check(edit->blocks.items[0]->getJSONData().isObject(), "variable addition snapshot");
        sequence->setCurrentTime(0, true, true);
        check(static_cast<Parameter*>(group->values.items.getLast()->controllable)->doubleValue() == 17, "new base value default");
        auto* addedParameter = static_cast<Parameter*>(group->values.items.getLast()->controllable);
        auto* addedPresetValue = base->values.getParameterPresetForSource(addedParameter);
        check(addedPresetValue->parameter->doubleValue() == 99 && !addedPresetValue->hasTimelineValue, "ordinary preset auto-fill preserved");
        var baseData = base->getJSONData(); base->loadJSONData(baseData); edit->handleAsyncUpdate();
        sequence->setCurrentTime(0, true, true);
        check(addedParameter->doubleValue() == 17, "unwritten base default survives reload");
        addedParameter->setValue(70);
        UndoMaster::getInstance()->clearUndoHistory(); base->values.syncValues(true);
        sequence->setCurrentTime(0, true, true); sequence->evaluateCVValues();
        check(addedParameter->doubleValue() == 70, "preset update authors new value");
        check(UndoMaster::getInstance()->undo(), "preset update undo"); sequence->setCurrentTime(0, true, true); sequence->evaluateCVValues();
        check(addedParameter->doubleValue() == 17, "preset update undo restores default intent");
        check(UndoMaster::getInstance()->redo(), "preset update redo"); sequence->setCurrentTime(0, true, true); sequence->evaluateCVValues();
        check(addedParameter->doubleValue() == 70, "preset update redo restores authored intent");
        var missingBase = base->getJSONData();
        missingBase.getProperty("values", var()).getProperty("containers", var()).getDynamicObject()->removeProperty(addedParameter->shortName);
        base->loadJSONData(missingBase); edit->handleAsyncUpdate(); sequence->setCurrentTime(0, true, true);
        check(addedParameter->doubleValue() == 17, "missing serialized base value uses default");
        group->values.removeItem(group->values.items.getLast(), false); edit->handleAsyncUpdate();
        number->setRange(-2000, 2000); edit->handleAsyncUpdate();
        check(static_cast<CVValuesBlock*>(edit->blocks.items[0])->groups.items[0]->values.items[0]->value->maximumValue == var(2000.), "live range metadata");
        static_cast<EnumParameter*>(actual(6))->addOption("Third", 2); edit->handleAsyncUpdate();
        check(static_cast<EnumParameter*>(static_cast<CVValuesBlock*>(edit->blocks.items[0])->groups.items[0]->values.items[6]->value)->enumValues.size() == 3, "live enum options");

        // Run the actual sequence clock in both directions and through a loop.
        sequence->fps->setValue(20); sequence->playSpeed->setValue(1);
        sequence->setCurrentTime(3, true, true); sequence->playTrigger->trigger();
        pumpMessages(160); sequence->pauseTrigger->trigger();
        check(sequence->currentTime->doubleValue() > 3, "forward playback clock advances");
        check(std::abs(number->doubleValue() - (double)edit->evaluate(sequence->currentTime->doubleValue())[0].value) < 1.e-6, "forward playback equals direct evaluation");
        sequence->playSpeed->setValue(-1); sequence->setCurrentTime(4, true, true); sequence->playTrigger->trigger();
        pumpMessages(160); sequence->pauseTrigger->trigger();
        check(sequence->currentTime->doubleValue() < 4, "reverse playback clock advances");
        check(std::abs(number->doubleValue() - (double)edit->evaluate(sequence->currentTime->doubleValue())[0].value) < 1.e-6, "reverse playback equals direct evaluation");
        const double duration = sequence->totalTime->doubleValue();
        sequence->totalTime->setValue(6); sequence->loopParam->setValue(true); sequence->playSpeed->setValue(1);
        sequence->setCurrentTime(5.95, true, true); sequence->playTrigger->trigger();
        pumpMessages(160); sequence->pauseTrigger->trigger();
        check(sequence->currentTime->doubleValue() < 1, "actual playback loops");
        check(std::abs(number->doubleValue() - (double)edit->evaluate(sequence->currentTime->doubleValue())[0].value) < 1.e-6, "loop playback equals direct evaluation");
        sequence->loopParam->setValue(false); sequence->totalTime->setValue(duration);

        // Render the actual editor components, including their interactive
        // automation and gradient editors, for visual inspection.
        a = static_cast<CVValuesBlock*>(edit->blocks.items[0]); b = static_cast<CVValuesBlock*>(edit->blocks.items[1]);
        av = a->groups.items[0]->values.items[0]; av->animated->setValue(true);
        edit->handleAsyncUpdate();
        std::unique_ptr<Component> valuesEditor(b->createValuesEditor()); valuesEditor->setSize(700, 650);
        check(valuesEditor->getNumChildComponents() == 1 && dynamic_cast<Viewport*>(valuesEditor->getChildComponent(0)), "scrollable block editor");
        auto* viewport = static_cast<Viewport*>(valuesEditor->getChildComponent(0));
        int automationEditors = 0, gradientEditors = 0;
        std::function<void(Component&)> inspectEditor = [&](Component& c)
        {
            if (dynamic_cast<AutomationEditor*>(&c)) ++automationEditors;
            if (dynamic_cast<GradientColorManagerEditor*>(&c)) ++gradientEditors;
            for (auto* child : c.getChildren()) inspectEditor(*child);
        };
        inspectEditor(*valuesEditor);
        check(automationEditors > 0 && gradientEditors > 0, "typed animation editing UI");
        std::unique_ptr<SequenceLayerTimeline> timelineUI(edit->getTimelineUI());
        sequence->viewStartTime->setValue(0); sequence->viewEndTime->setValue(15);
        timelineUI->setSize(1100, 100); timelineUI->updateContent();
        LayerBlockManagerUI* managerUI = nullptr;
        for (auto* child : timelineUI->getChildren()) if (auto* ui = dynamic_cast<LayerBlockManagerUI*>(child)) managerUI = ui;
        check(managerUI && managerUI->itemsUI.size() == 2, "block timeline UI");
        PopupMenu menu; managerUI->itemsUI[0]->addContextMenuItems(menu); check(menu.getNumItems() > 0, "animation context menu");
        if (argc > 1)
        {
            valuesEditor->addToDesktop(ComponentPeer::windowIsTemporary); valuesEditor->setVisible(true);
            timelineUI->addToDesktop(ComponentPeer::windowIsTemporary); timelineUI->setVisible(true);
            pumpMessages(200);
            File folder = File(argv[1]).getParentDirectory();
            auto render = [&](Component& c, const String& name)
            {
                Image image = c.createComponentSnapshot(c.getLocalBounds());
                FileOutputStream stream(folder.getChildFile(name)); PNGImageFormat format;
                stream.setPosition(0); stream.truncate();
                check(format.writeImageToStream(image, stream), "editor preview render");
            };
            render(*valuesEditor, "cv-values-editor-top.png");
            viewport->setViewPosition(0, 500); render(*valuesEditor, "cv-values-editor-animations.png");
            render(*timelineUI, "cv-values-timeline.png");
            valuesEditor->removeFromDesktop(); timelineUI->removeFromDesktop();
        }
        valuesEditor.reset();
        // Exercise the timeline's actual resize gesture and shared snapping
        // callbacks; undo must restore both timing and authored animation keys.
        auto* blockUI = managerUI->itemsUI[0];
        auto event = [&](float x, int modifiers, bool dragged)
        {
            return MouseEvent(Desktop::getInstance().getMainMouseSource(), {x, 20}, ModifierKeys(modifiers),
                1, 0, 0, 0, 0, &blockUI->coreGrabber, &blockUI->coreGrabber, Time::getCurrentTime(),
                {0, 20}, Time::getCurrentTime(), 1, dragged);
        };
        sequence->autoSnap->setValue(true);
        blockUI->mouseDown(event(0, ModifierKeys::leftButtonModifier, false));
        managerUI->snapTimes = { 6.5f };
        blockUI->mouseDrag(event(75, ModifierKeys::leftButtonModifier, true));
        blockUI->mouseUp(event(75, 0, true));
        check(edit->blocks.items[0]->coreLength->doubleValue() == 4.5, "UI edge resize snaps");
        check(static_cast<CVValuesBlock*>(edit->blocks.items[0])->groups.items[0]->values.items[0]->automation->items[1]->position->doubleValue() == 4, "UI trim preserves local keys");
        check(UndoMaster::getInstance()->undo(), "UI resize undo"); pumpMessages(40);
        check(edit->blocks.items[0]->coreLength->doubleValue() == 4, "UI resize undo timing");
        blockUI = managerUI->itemsUI[0]; sequence->autoSnap->setValue(false);
        blockUI->mouseDown(event(0, ModifierKeys::leftButtonModifier | ModifierKeys::shiftModifier, false));
        blockUI->mouseDrag(event(75, ModifierKeys::leftButtonModifier | ModifierKeys::shiftModifier, true));
        blockUI->mouseUp(event(75, 0, true));
        check(static_cast<CVValuesBlock*>(edit->blocks.items[0])->groups.items[0]->values.items[0]->automation->items[1]->position->doubleValue() > 4, "UI stretch scales local keys");
        check(UndoMaster::getInstance()->undo(), "UI stretch undo"); pumpMessages(40);
        check(static_cast<CVValuesBlock*>(edit->blocks.items[0])->groups.items[0]->values.items[0]->automation->items[1]->position->doubleValue() == 4, "UI stretch undo keys");
        timelineUI.reset();
        sequence->layerManager->removeItem(edit, false); layer->enabled->setValue(true); layer->handleAsyncUpdate();

        // Completed project loading rebuilds all layer snapshots before writing.
        const var sequenceData = sequence->getJSONData(); sequence->enabled->setValue(false);
        app.engine->isLoadingFile = true;
        auto* reloaded = static_cast<ChataigneSequence*>(ChataigneSequenceManager::getInstance()->addItem(nullptr, var(), false));
        reloaded->loadJSONData(sequenceData); reloaded->setCurrentTime(3, true, true);
        number->setValue(999); reloaded->evaluateCVValues(); check(number->doubleValue() == 999, "loading suppresses writes");
        app.engine->isLoadingFile = false; reloaded->fileLoaded();
        check(std::abs(number->doubleValue() - 50) < 1.e-6, "completed load refreshes priority and curves");
        ChataigneSequenceManager::getInstance()->removeItem(reloaded, false); sequence->enabled->setValue(true);
        if (argc > 1)
        {
            // Optional project fixture for opening the actual timeline editor.
            layer->blocks.removeItem(second, false);
            sequence->layerManager->removeItem(lower, false);
            layer->fadeIn->setValue(1); layer->fadeOut->setValue(1);
            File(argv[1]).replaceWithText(JSON::toString(app.engine->getJSONData(), true));
        }
        std::cout << "CV Values integration passed\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "CV Values integration failed: " << e.what() << "\n";
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
