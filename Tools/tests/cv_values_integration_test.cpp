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

class ConstantAudioSource : public PositionableAudioSource
{
public:
    explicit ConstantAudioSource(float level) : level(level) {}
    float level; int64 position = 0;
    void prepareToPlay(int, double) override {}
    void releaseResources() override {}
    void getNextAudioBlock(const AudioSourceChannelInfo& info) override
    {
        for (int ch = 0; ch < info.buffer->getNumChannels(); ++ch)
            FloatVectorOperations::fill(info.buffer->getWritePointer(ch, info.startSample), level, info.numSamples);
        position += info.numSamples;
    }
    void setNextReadPosition(int64 p) override { position = p; }
    int64 getNextReadPosition() const override { return position; }
    int64 getTotalLength() const override { return 48000 * 30; }
    bool isLooping() const override { return false; }
};

static void testAudioBlocks(ChataigneSequence* sequence)
{
    ConstantAudioSource first(.2f), second(.6f);
    AudioProcessorGraph graph;
    graph.setPlayConfigDetails(0, 2, 48000, 256); graph.prepareToPlay(48000, 256);
    AudioLayer audio(sequence, var()); audio.setAudioProcessorGraph(&graph);
    auto add = [&](double start, ConstantAudioSource& source)
    {
        auto* clip = new AudioLayerClip(); clip->time->setValue(start); clip->coreLength->setValue(10);
        clip->filePath->setValue("memory", true); clip->clipDuration = 30; clip->numChannels = 1;
        audio.clipManager.addItem(clip, var(), false);
        clip->transportSource.setSource(&source, 0, nullptr, 48000, 2);
        return clip;
    };
    auto* a = add(0, first); auto* b = add(8, second);
    check(audio.clipManager.blocksCanOverlap, "audio supports overlapping blocks");
    check(a->getEffectiveFades().out == 2 && b->getEffectiveFades().in == 2, "automatic overlap durations");
    sequence->isPlaying->setValue(true, true);
    auto seekAudio = [&](double time)
    {
        sequence->currentTime->setValue(time, true); sequence->hiResAudioTime = time;
        const ScopedValueSetter<bool> seeking(sequence->isSeeking, true);
        audio.sequenceCurrentTimeChanged(sequence, 0, false);
    };
    AudioBuffer<float> buffer(2, 256); MidiBuffer midi;
    auto sample = [&]()
    {
        audio.currentProcessor->processBlock(buffer, midi); // finish the short discontinuity ramp
        audio.currentProcessor->processBlock(buffer, midi);
        check(std::abs(buffer.getSample(0, 128) - buffer.getSample(1, 128)) < .0001f, "mixed mono routing");
        return buffer.getSample(0, 128);
    };
    seekAudio(9);
    check(a->isActive->boolValue() && b->isActive->boolValue(), "both audio clips active in overlap");
    check(std::abs(sample() - .4f) < .005f, "audio renders both crossfade contributors");
    b->fadeIn->setEnabled(true); b->fadeIn->setValue(0); seekAudio(9);
    check(std::abs(sample() - .7f) < .005f, "manual zero fade disables incoming auto fade");
    a->fadeOut->setEnabled(true); a->fadeOut->setValue(0); seekAudio(9);
    check(std::abs(sample() - .8f) < .005f, "both manual zero edges mix at full gain");
    b->fadeIn->setEnabled(false); a->fadeOut->setEnabled(false); seekAudio(4);
    check(a->isActive->boolValue() && !b->isActive->boolValue(), "audio active flags update on seek");
    check(std::abs(sample() - .2f) < .005f, "single audio clip unchanged");
    AudioLayerClipUI ui(b); ui.setSize(500, 120); ui.handleContextMenuResult(1002);
    check(ui.automationUI != nullptr, "audio automation embedded");
    ui.setViewRange(1, 6);
    check(ui.automationUI->keysUI.viewPosRange == Point<float>(1, 6), "audio automation follows cropped zoom");
    check(std::abs(ui.xForLocalTime(2) - 100) < .001, "fade positions use cropped coordinates");
    ui.setViewRange(3, 6);
    check(!ui.fadeInHandle.isVisible(), "offscreen fade handle is hidden");
    ui.handleContextMenuResult(1001);
    check(!ui.automationUI, "hide inline editor");
    ui.setViewRange(1, 6);
    auto fadeEvent = [&](float x, int modifiers, bool dragged)
    {
        return MouseEvent(Desktop::getInstance().getMainMouseSource(), { x, 5 }, ModifierKeys(modifiers),
            1, 0, 0, 0, 0, &ui.fadeInHandle, &ui.fadeInHandle, Time::getCurrentTime(),
            { 5, 5 }, Time::getCurrentTime(), 1, dragged);
    };
    ui.mouseDown(fadeEvent(5, ModifierKeys::leftButtonModifier, false));
    ui.mouseDrag(fadeEvent(105, ModifierKeys::leftButtonModifier, true));
    ui.mouseUp(fadeEvent(5, 0, true));
    check(b->fadeIn->enabled && std::abs(b->fadeIn->doubleValue() - 3) < .01, "fade handle edits local duration and enables override");
    check(UndoMaster::getInstance()->undo() && !b->fadeIn->enabled, "fade gesture undo restores automatic mode");
    AudioLayerClip oldFade;
    oldFade.fadeIn->setValue(.5);
    auto legacyAudio = oldFade.getJSONData().clone();
    for (auto p : *legacyAudio.getProperty("parameters", var()).getArray())
        if (p.getProperty("controlAddress", "").toString() == "/fadeIn") p.getDynamicObject()->removeProperty("enabled");
    AudioLayerClip migratedAudio; migratedAudio.loadJSONData(legacyAudio);
    check(migratedAudio.fadeIn->enabled && migratedAudio.fadeIn->doubleValue() == .5, "legacy audio fade remains explicit");
    audio.enabled->setValue(false);
    check(!a->transportSource.isPlaying() && !b->transportSource.isPlaying(), "disable stops all audio transports");
    check(sample() == 0, "disabled audio is silent");
    sequence->isPlaying->setValue(false, true);
    audio.setAudioProcessorGraph(nullptr);
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
        custom->enabled->setValue(true); custom->value->setValue(200);
        layer->rebuildSnapshot(); seek(3, 200);
        block->blockFadeIn->setEnabled(true); block->blockFadeIn->setValue(2);
        layer->rebuildSnapshot(); seek(3, 100);
        block->blockFadeIn->setValue(0); layer->rebuildSnapshot(); seek(2, 200);
        block->blockFadeIn->setEnabled(false); layer->rebuildSnapshot(); seek(2, 200);
        auto legacyValueData = custom->getJSONData().clone();
        for (auto p : *legacyValueData.getProperty("parameters", var()).getArray())
            if (p.getProperty("controlAddress", "").toString() == "/enabled") p.getDynamicObject()->setProperty("controlAddress", "/override");
        CVValuesValue migratedValue;
        migratedValue.loadJSONData(legacyValueData, true);
        check(migratedValue.enabled->boolValue() && migratedValue.getControllableForAddress("/override", false) == nullptr,
            "legacy override migrates into enabling container without a second toggle");
        custom->animated->setValue(true);
        custom->automation->clear();
        custom->automation->addKey(0, 0, false)->easingType->setValueWithData(Easing::LINEAR);
        custom->automation->addKey(4, 400, false)->easingType->setValueWithData(Easing::LINEAR);
        layer->rebuildSnapshot(); seek(3, 100); seek(5, 300); seek(3, 100);
        custom->enabled->setValue(false); custom->unsetBehavior->setValueWithData(1);
        layer->rebuildSnapshot(); number->setValue(777); seek(3, 777);
        custom->enabled->setValue(true);
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
            v->enabled->setValue(true); v->value->setValue(value); return v;
        };
        auto* av = overrideFor(a, 0, 100);
        auto* bv = overrideFor(b, 0, 800);
        av->animated->setValue(true); av->automation->clear();
        av->automation->addKey(0, 0)->easingType->setValueWithData(Easing::LINEAR);
        av->automation->addKey(4, 400)->easingType->setValueWithData(Easing::LINEAR);
        edit->handleAsyncUpdate(); seek(5.5, 575);
        bv->enabled->setValue(false); edit->handleAsyncUpdate(); seek(5.5, 350); seek(7, 400);
        bv->unsetBehavior->setValueWithData(1); edit->handleAsyncUpdate(); seek(5.5, 350);
        number->setValue(777); seek(7, 777);
        bv->unsetBehavior->setValueWithData(0); check(b->setTiming(8, 4), "valid move out of overlap");
        edit->handleAsyncUpdate(); seek(9, 0);
        edit->interpolationMode->setValueWithData(2); edit->handleAsyncUpdate(); seek(9, 400);
        edit->interpolationMode->setValueWithData(1); edit->handleAsyncUpdate(); seek(7, 400); seek(9, 400);
        edit->interpolationMode->setValueWithData(0); check(b->setTiming(5, 4), "valid overlap restored");
        av->animated->setValue(false); av->value->setValue(100); bv->enabled->setValue(true); bv->value->setValue(500);
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
        bv = b->groups.items[0]->values.items[0]; bv->enabled->setValue(false); bv->unsetBehavior->setValueWithData(1); edit->handleAsyncUpdate();
        seek(7, 200); // Lower layer abstains; upper layer's second block inherits its endpoint.
        sequence->layerManager->setItemIndex(edit, 0, false); seek(7, 200);
        sequence->layerManager->setItemIndex(edit, sequence->layerManager->items.size() - 1, false);
        bv->enabled->setValue(true); edit->handleAsyncUpdate(); seek(7, 500);
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
        Viewport valuesViewport;
        valuesViewport.setViewedComponent(b->createValuesEditor(), true);
        Component* valuesEditor = &valuesViewport;
        valuesEditor->setSize(700, 650);
        valuesViewport.getViewedComponent()->setSize(680, valuesViewport.getViewedComponent()->getHeight());
        auto* viewport = &valuesViewport;
        auto* rowValue = b->groups.items[0]->values.items[0];
        rowValue->enabled->setValue(true); rowValue->animated->setValue(false);
        std::unique_ptr<InspectableEditor> rowEditor(rowValue->getEditor(false)); rowEditor->setSize(600, 28);
        ParameterEditor* typedEditor = nullptr;
        for (auto* child : rowEditor->getChildren()) if (auto* p = dynamic_cast<ParameterEditor*>(child)) typedEditor = p;
        check(typedEditor && typedEditor->ui->isInteractable() && typedEditor->isEnabled(), "compact typed value is editable");
        check(rowEditor->getHeight() <= 30, "one-line custom value row");
        auto* slider = dynamic_cast<FloatSliderUI*>(typedEditor->ui.get());
        check(slider != nullptr, "custom number uses typed slider");
        const auto beforeValue = rowValue->value->value;
        slider->setParamNormalizedValue(.65f);
        check(rowValue->value->value != beforeValue, "inspector slider changes authored custom value");
        rowValue->value->setValue(beforeValue);
        std::unique_ptr<SequenceLayerTimeline> timelineUI(edit->getTimelineUI());
        sequence->viewStartTime->setValue(0); sequence->viewEndTime->setValue(15);
        timelineUI->setSize(1100, 100); timelineUI->updateContent();
        LayerBlockManagerUI* managerUI = nullptr;
        for (auto* child : timelineUI->getChildren()) if (auto* ui = dynamic_cast<LayerBlockManagerUI*>(child)) managerUI = ui;
        check(managerUI && managerUI->itemsUI.size() == 2, "block timeline UI");
        PopupMenu menu; managerUI->itemsUI[0]->addContextMenuItems(menu); check(menu.getNumItems() > 0, "animation context menu");
        auto* firstUI = managerUI->itemsUI[0];
        firstUI->handleContextMenuResult(2000);
        check(firstUI->automationUI != nullptr, "CV number automation embedded in block");
        sequence->viewStartTime->setValue(3); sequence->viewEndTime->setValue(5);
        timelineUI->setSize(1100, 120); timelineUI->updateContent();
        check(firstUI->automationUI->keysUI.viewPosRange == Point<float>(1, 3), "cropped automation view uses local time");
        check(firstUI->automationUI->keysUI.getBounds() == firstUI->automationUI->getLocalBounds(), "automation child bounds update in the same frame as zoom");
        firstUI->handleContextMenuResult(2002);
        check(firstUI->gradientUI != nullptr && !firstUI->automationUI, "CV color gradient embedded in block");
        check(firstUI->gradientUI->viewStartPos == 1 && firstUI->gradientUI->viewEndPos == 3, "cropped gradient uses local time");
        firstUI->handleContextMenuResult(1002);
        sequence->viewStartTime->setValue(0); sequence->viewEndTime->setValue(15);
        timelineUI->updateContent();
        if (argc > 1)
        {
            sequence->setCurrentTime(5.5, true, true); sequence->evaluateCVValues();
            firstUI->handleContextMenuResult(2000);
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
            sequence->viewStartTime->setValue(3); sequence->viewEndTime->setValue(5); timelineUI->updateContent();
            render(*timelineUI, "cv-values-cropped-automation.png");
            firstUI->handleContextMenuResult(2002);
            pumpMessages(100);
            render(*timelineUI, "cv-values-cropped-gradient.png");
            firstUI->handleContextMenuResult(1002);
            sequence->viewStartTime->setValue(0); sequence->viewEndTime->setValue(15); timelineUI->updateContent();
            valuesEditor->removeFromDesktop(); timelineUI->removeFromDesktop();
        }
        valuesViewport.setViewedComponent(nullptr, true);
        rowEditor.reset();
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
        testAudioBlocks(sequence);
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
