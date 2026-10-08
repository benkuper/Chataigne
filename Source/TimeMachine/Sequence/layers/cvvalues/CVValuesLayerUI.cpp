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

class CVValuesEditorWindow : public Component
{
public:
    CVValuesEditorWindow(CVValuesBlock* block)
    {
        viewport.setViewedComponent(block->getEditor(true), true);
        addAndMakeVisible(viewport); setSize(700, 650);
    }
    Viewport viewport;
    void paint(Graphics& g) override { g.fillAll(BG_COLOR); }
    void resized() override
    {
        viewport.setBounds(getLocalBounds());
        if (auto* editor = viewport.getViewedComponent()) editor->setSize(getWidth() - viewport.getScrollBarThickness(), editor->getHeight());
    }
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
        auto bounds = getLocalBounds().toFloat();
        const auto length = block->coreLength->floatValue();
        const auto visible = viewEnd - viewStart;
        if (visible > 0)
        {
            auto shade = [&](float start, float end, Colour color)
            {
                const float x1 = jlimit(0.0f, bounds.getWidth(), (start - viewStart) / visible * bounds.getWidth());
                const float x2 = jlimit(0.0f, bounds.getWidth(), (end - viewStart) / visible * bounds.getWidth());
                g.setColour(color); g.fillRect(x1, 0.0f, jmax(0.0f, x2 - x1), bounds.getHeight());
            };
            auto data = std::atomic_load(&block->layer->snapshot);
            if (data)
            {
                const auto& timeline = data->timeline;
                for (size_t i = 0; i < timeline.blocks.size(); ++i)
                {
                    const auto& b = timeline.blocks[i];
                    if (b.start != block->time->doubleValue()) continue;
                    const auto* previous = i > 0 ? &timeline.blocks[i - 1] : nullptr;
                    const auto* next = i + 1 < timeline.blocks.size() ? &timeline.blocks[i + 1] : nullptr;
                    const bool incomingOverlap = previous && previous->end() > b.start;
                    const bool outgoingOverlap = next && next->start < b.end();
                    if (incomingOverlap) shade(0, static_cast<float>(previous->end() - b.start), Colours::cyan.withAlpha(.15f));
                    else shade(0, static_cast<float>(timeline.edgeFades(i).first), Colours::white.withAlpha(.08f));
                    if (outgoingOverlap) shade(static_cast<float>(next->start - b.start), length, Colours::cyan.withAlpha(.15f));
                    else shade(length - static_cast<float>(timeline.edgeFades(i).second), length, Colours::white.withAlpha(.08f));
                    break;
                }
            }
        }
        String label = block->blockType->getValueKey();
        if (block->blockType->getValueDataAsEnum<int>() == 0)
            for (auto* group : block->groups.items)
                if (auto* preset = group->preset->getTargetContainer()) label += " | " + preset->niceName;
        if (!block->valid) label = "Invalid overlap | " + label;
        g.setColour(block->valid ? TEXT_COLOR : Colours::orange);
        g.drawFittedText(label, getLocalBounds().reduced(8, 3), Justification::centredLeft, 2);
    }

    void addContextMenuItems(PopupMenu& menu) override { menu.addItem(1001, "Edit Values / Animations..."); }
    void handleContextMenuResult(int result) override
    {
        if (result != 1001) return;
        block->syncTargets();
        DialogWindow::LaunchOptions options;
        options.dialogTitle = block->niceName + " - Values / Animations";
        options.dialogBackgroundColour = BG_COLOR;
        options.content.setOwned(block->createValuesEditor());
        options.escapeKeyTriggersCloseButton = true;
        options.useNativeTitleBar = true;
        options.resizable = true;
        options.launchAsync();
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
        b->acceptedStart = time;
        b->syncTargets();
        m->addItem(b.release());
    }
    void mouseDoubleClick(const MouseEvent& e) override
    {
        if (!miniMode && !e.mods.isCommandDown() && !e.mods.isShiftDown()) addAt(timeline->getTimeForX(getMouseXYRelative().x), 0);
    }
    void addItemFromMenu(bool fromButton, Point<int> position) override
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
    void resized() override { blocks.setBounds(getLocalBounds()); updateNeedlePosition(); }
    void updateContent() override { blocks.updateContent(); }
    void updateMiniModeUI() override { blocks.setMiniMode(item->miniMode->boolValue()); }
    void addSelectableComponentsAndInspectables(Array<Component*>& components, Array<Inspectable*>& inspectables) override
    { blocks.addSelectableComponentsAndInspectables(components, inspectables); }
};
}

SequenceLayerTimeline* CVValuesLayer::getTimelineUI() { return new CVValuesLayerTimeline(this); }
Component* CVValuesBlock::createValuesEditor() { syncTargets(); return new CVValuesEditorWindow(this); }
InspectableEditor* CVValuesTarget::getEditorInternal(bool isRoot, Array<Inspectable*>) { return new CVValuesTargetEditor(this, isRoot); }
