#include "TimeMachine/TimeMachineIncludes.h"

namespace
{
class CVValuesTargetEditor : public BaseItemEditor, public WarningTarget::AsyncListener
{
public:
    CVValuesTargetEditor(CVValuesTarget* target, bool root) : BaseItemEditor({ target }, root)
    {
        warning.setColour(Label::textColourId, Colours::orange);
        warning.setJustificationType(Justification::centredLeft);
        addAndMakeVisible(warning);
        target->addAsyncWarningTargetListener(this);
        updateWarning();
    }
    ~CVValuesTargetEditor() override
    { if (!inspectable.wasObjectDeleted()) item->removeAsyncWarningTargetListener(this); }
    Label warning;
    void updateWarning()
    {
        if (inspectable.wasObjectDeleted()) return;
        warning.setText(item->getWarningMessage(), dontSendNotification);
        warning.setVisible(warning.getText().isNotEmpty()); resized();
    }
    void resizedInternalContent(juce::Rectangle<int>& r) override
    {
        BaseItemEditor::resizedInternalContent(r);
        if (warning.getText().isNotEmpty())
        { warning.setBounds(r.withHeight(42)); r.translate(0, 46); }
    }
    void newMessage(const WarningTarget::WarningTargetEvent&) override { updateWarning(); }
};

// A value uses its enabling container's toggle and the same typed editor as
// preset rows. Automation is edited in the timeline, leaving this row compact.
class CVValuesValueEditor : public InspectableEditor, public Parameter::AsyncListener
{
public:
    CVValuesValueEditor(CVValuesValue* v, bool root) : InspectableEditor(v, root), value(v)
    {
        enabledUI.reset(v->enabled->createToggle()); enabledUI->showLabel = false;
        valueUI.reset(v->value->getEditor(false));
        if (auto* editor = dynamic_cast<ControllableEditor*>(valueUI.get()))
        { editor->label.setText(v->niceName, dontSendNotification); editor->minLabelWidth = 100; }
        modeUI.reset(v->interpolation->createDefaultUI()); modeUI->showLabel = false;
        unsetUI.reset(v->unsetBehavior->createDefaultUI()); unsetUI->showLabel = false;
        animationUI.reset(v->animated->createButtonToggle());
        animationUI->customLabel = "Animate";
        animationUI->setTooltip("Edit this animation with the block's right-click menu");
        Component* controls[] = { enabledUI.get(), valueUI.get(), modeUI.get(), unsetUI.get(), animationUI.get() };
        for (auto* c : controls) addAndMakeVisible(c);
        v->enabled->addAsyncParameterListener(this); v->animated->addAsyncParameterListener(this);
        setSize(500, jmax(26, valueUI->getHeight())); update();
    }
    ~CVValuesValueEditor() override
    {
        if (inspectable.wasObjectDeleted()) return;
        value->enabled->removeAsyncParameterListener(this); value->animated->removeAsyncParameterListener(this);
    }
    CVValuesValue* value;
    std::unique_ptr<ControllableUI> enabledUI, modeUI, unsetUI, animationUI;
    std::unique_ptr<InspectableEditor> valueUI;
    void update()
    {
        const bool enabled = value->enabled->boolValue();
        valueUI->setEnabled(enabled && !value->animated->boolValue());
        modeUI->setVisible(enabled); unsetUI->setVisible(!enabled);
        animationUI->setVisible(enabled && (value->automation || value->gradient)); resized();
    }
    void resized() override
    {
        auto r = getLocalBounds().reduced(0, 2);
        enabledUI->setBounds(r.removeFromLeft(22)); r.removeFromLeft(4);
        auto right = r.removeFromRight(jmin(140, r.getWidth() / 3));
        modeUI->setBounds(right); unsetUI->setBounds(right); r.removeFromRight(6);
        if (animationUI->isVisible()) { animationUI->setBounds(r.removeFromRight(65)); r.removeFromRight(6); }
        valueUI->setBounds(r);
    }
    void newMessage(const Parameter::ParameterEvent&) override { if (!inspectable.wasObjectDeleted()) update(); }
};

class CVValuesBlockUI : public LayerBlockUI
{
public:
    CVValuesBlockUI(CVValuesBlock* block) : LayerBlockUI(block), block(block) {}
    CVValuesBlock* block;
    var stateBeforeDrag;
    void mouseDown(const MouseEvent& e) override
    {
        stateBeforeDrag = block->layer->blocks.getJSONData();
        LayerBlockUI::mouseDown(e);
    }

    void paint(Graphics& g) override
    {
        LayerBlockUI::paint(g);
        String label = block->niceName + " | " + block->blockType->getValueKey();
        if (block->blockType->getValueDataAsEnum<int>() == 0)
            for (auto* group : block->groups.items)
                if (auto* preset = group->preset->getTargetContainer()) label += " | " + preset->niceName;
        if (!block->valid) label = "Invalid overlap | " + label;
        g.setColour(block->valid ? TEXT_COLOR : Colours::orange);
        g.drawFittedText(label, getLocalBounds().reduced(14, 3).withHeight(18), Justification::centredLeft, 1);
    }

    Array<WeakReference<ControllableContainer>> menuValues;
    void addContextMenuItems(PopupMenu& menu) override
    {
        menu.addItem(1001, "Edit Values");
        menu.addItem(1002, "Hide animation editor", automationUI || gradientUI);
        menuValues.clear();
        if (block->blockType->getValueDataAsEnum<int>() != 1) return;
        for (auto* group : block->groups.items)
        {
            if (group->hideInEditor) continue;
            PopupMenu variables;
            for (auto* v : group->values.items)
            {
                if (v->hideInEditor || (!v->automation && !v->gradient)) continue;
                menuValues.add(v);
                variables.addItem(2000 + menuValues.size() - 1, "Animate " + v->niceName, true, editedValue == v);
            }
            if (variables.getNumItems() > 0) menu.addSubMenu(group->niceName, variables);
        }
    }
    WeakReference<ControllableContainer> editedValue;
    void handleContextMenuResult(int result) override
    {
        if (result == 1001) { block->syncTargets(); block->selectThis(); return; }
        if (result == 1002) { editedValue = nullptr; setInlineEditor(nullptr); return; }
        const int index = result - 2000;
        if (!isPositiveAndBelow(index, menuValues.size())) return;
        auto* value = dynamic_cast<CVValuesValue*>(menuValues[index].get());
        if (!value) return;
        Array<UndoableAction*> actions;
        if (!value->enabled->boolValue()) actions.add(value->enabled->setUndoableValue(false, true, true));
        if (!value->animated->boolValue()) actions.add(value->animated->setUndoableValue(false, true, true));
        if (!actions.isEmpty()) UndoMaster::getInstance()->performActions("Animate CV value", actions);
        editedValue = value; setInlineEditor(value->automation.get(), value->gradient.get());
    }
    void mouseEnter(const MouseEvent& e) override { LayerBlockUI::mouseEnter(e); loopGrabber.setVisible(false); }
    void mouseUp(const MouseEvent& e) override
    {
        auto& manager = block->layer->blocks;
        var after = manager.getJSONData();
        if (!stateBeforeDrag.isVoid() && JSON::toString(stateBeforeDrag) != JSON::toString(after))
            UndoMaster::getInstance()->performAction("Edit CV Values blocks", manager.stateAction(stateBeforeDrag, after, true));
        stateBeforeDrag = var();
        isDragging = false;
        grabber.setVisible(isMouseOverOrDragging()); coreGrabber.setVisible(isMouseOverOrDragging());
        loopGrabber.setVisible(false);
        blockUIListeners.call(&BlockUIListener::blockUINeedsReorder);
        BaseItemMinimalUI<LayerBlock>::mouseUp(e);
    }
    void resizedBlockInternal() override { loopGrabber.setVisible(false); }
    void handlePaintTimerInternal() override
    {
        auto* value = dynamic_cast<CVValuesValue*>(editedValue.get());
        if ((automationUI || gradientUI) && (!value || value->hideInEditor || !value->enabled->boolValue()
            || !value->animated->boolValue() || block->blockType->getValueDataAsEnum<int>() != 1))
        { editedValue = nullptr; setInlineEditor(nullptr); }
        LayerBlockUI::handlePaintTimerInternal();
    }
};

class CVValuesBlockManagerUI : public LayerBlockManagerUI
{
public:
    CVValuesBlockManagerUI(SequenceLayerTimeline* timeline, CVValuesBlockManager* manager) : LayerBlockManagerUI(timeline, manager)
    { addExistingItems(); }
    LayerBlockUI* createUIForItem(LayerBlock* b) override
    {
        auto* ui = new CVValuesBlockUI(static_cast<CVValuesBlock*>(b));
        ui->setInterceptsMouseClicks(!miniMode, !miniMode);
        return ui;
    }
    void blockUIDragged(LayerBlockUI* ui, const MouseEvent& e) override
    {
        if (miniMode) return;
        auto* m = static_cast<CVValuesBlockManager*>(manager);
        double delta = timeline->getTimeForX(e.getOffsetFromDragStart().x, false);
        if (e.mods.isShiftDown() || timeline->item->sequence->autoSnap->boolValue())
        {
            const double start = ui->item->movePositionReference.x + delta, length = ui->item->coreLength->doubleValue();
            double closest = 1, snapped = delta;
            for (auto t : snapTimes)
            {
                if (std::abs(start - t) < closest) { closest = std::abs(start - t); snapped = t - ui->item->movePositionReference.x; }
                if (std::abs(start + length - t) < closest) { closest = std::abs(start + length - t); snapped = t - length - ui->item->movePositionReference.x; }
            }
            delta = snapped;
        }
        std::vector<CVValuesBlockManager::Timing> timings;
        for (auto* b : manager->items) if (b == ui->item || (b->isSelected && !b->isUILocked->boolValue()))
            timings.push_back({ static_cast<CVValuesBlock*>(b), b->movePositionReference.x + delta, b->coreLength->doubleValue() });
        m->applyTimings(timings);
    }
    void blockUIStartDragged(LayerBlockUI* ui, const MouseEvent& e) override
    {
        if (miniMode) return;
        double start = ui->item->movePositionReference.x + timeline->getTimeForX(e.getOffsetFromDragStart().x, false);
        if (timeline->item->sequence->autoSnap->boolValue())
        {
            const double snapped = timeline->item->sequence->getClosestSnapTimeFor(snapTimes, static_cast<float>(start));
            if (std::abs(snapped - start) < 1) start = snapped;
        }
        const double length = ui->coreLengthAtMouseDown + ui->item->movePositionReference.x - start;
        static_cast<CVValuesBlock*>(ui->item)->setTiming(start, length, e.mods.isShiftDown());
    }
    void blockUICoreDragged(LayerBlockUI* ui, const MouseEvent& e) override
    {
        if (miniMode) return;
        double length = ui->coreLengthAtMouseDown + timeline->getTimeForX(e.getOffsetFromDragStart().x, false);
        if (timeline->item->sequence->autoSnap->boolValue())
        {
            const double end = ui->item->time->doubleValue() + length;
            const double snapped = timeline->item->sequence->getClosestSnapTimeFor(snapTimes, static_cast<float>(end));
            if (std::abs(snapped - end) < 1) length = snapped - ui->item->time->doubleValue();
        }
        static_cast<CVValuesBlock*>(ui->item)->setTiming(ui->item->time->doubleValue(), length, e.mods.isShiftDown());
    }
    void blockUILoopDragged(LayerBlockUI*, const MouseEvent&) override {}
    void addAt(float time, int type)
    {
        auto* m = static_cast<CVValuesBlockManager*>(manager);
        std::unique_ptr<CVValuesBlock> b(new CVValuesBlock(m->cvLayer));
        if (!m->accepts(b.get(), time, b->coreLength->doubleValue()))
        {
            m->cvLayer->setWarningMessage("Cannot add this block here: only ordinary two-block crossfades are supported.", "add", false);
            return;
        }
        m->cvLayer->clearWarning("add");
        b->time->setValue(time); b->blockType->setValueWithData(type);
        b->setNiceName(b->blockType->getValueKey());
        b->acceptedStart = time;
        b->syncTargets();
        m->addItem(b.release());
    }
    void mouseDoubleClick(const MouseEvent& e) override
    {
        if (!miniMode && !e.mods.isCommandDown() && !e.mods.isShiftDown()) addAt(timeline->getTimeForX(getMouseXYRelative().x), 0);
    }
    void showMenuAndAddItem(bool fromButton, Point<int> position) override
    {
        if (miniMode || fromButton) return;
        PopupMenu menu;
        menu.addItem(1, "Add Preset Block"); menu.addItem(2, "Add Custom Values Block");
        const float time = timeline->getTimeForX(position.x);
        Component::SafePointer<CVValuesBlockManagerUI> safe(this);
        menu.showMenuAsync(PopupMenu::Options(), [safe, time](int result) { if (safe && result > 0) safe->addAt(time, result - 1); });
    }
};

class CVValuesLayerTimeline : public SequenceLayerTimeline
{
public:
    CVValuesLayerTimeline(CVValuesLayer* layer) : SequenceLayerTimeline(layer), blocks(this, &layer->blocks)
    {
        addAndMakeVisible(blocks);
        needle.toFront(false);
        updateMiniModeUI();
    }
    CVValuesBlockManagerUI blocks;
    void newMessage(const ContainerAsyncEvent& e) override
    {
        SequenceLayerTimeline::newMessage(e);
        if (e.targetControllable != item->sequence->currentTime) blocks.updateContent();
    }
    void resized() override { blocks.setBounds(getLocalBounds()); updateNeedlePosition(); }
    void updateContent() override { blocks.updateContent(); }
    void updateMiniModeUI() override { blocks.setMiniMode(item->miniMode->boolValue()); }
    void addSelectableComponentsAndInspectables(Array<Component*>& components, Array<Inspectable*>& inspectables) override
    { blocks.addSelectableComponentsAndInspectables(components, inspectables); }
};
}

SequenceLayerTimeline* CVValuesLayer::getTimelineUI() { return new CVValuesLayerTimeline(this); }
Component* CVValuesBlock::createValuesEditor() { syncTargets(); return getEditor(true); }
InspectableEditor* CVValuesValue::getEditorInternal(bool isRoot, Array<Inspectable*>) { return new CVValuesValueEditor(this, isRoot); }
InspectableEditor* CVValuesTarget::getEditorInternal(bool isRoot, Array<Inspectable*>) { return new CVValuesTargetEditor(this, isRoot); }
