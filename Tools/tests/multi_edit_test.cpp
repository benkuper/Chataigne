#include "MainIncludes.h"
#include <iostream>
#include <stdexcept>
#undef main

class MultiEditTestApplication : public OrganicApplication
{
public:
    MultiEditTestApplication() : OrganicApplication("Multi-edit tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

static void select(Array<Inspectable*> items)
{
    InspectableSelectionManager::mainSelectionManager->selectInspectables(items);
    UndoMaster::getInstance()->clearUndoHistory();
}

static void undo()
{
    check(UndoMaster::getInstance()->undo(), "operation is undoable");
    check(!UndoMaster::getInstance()->canUndo(), "operation uses one undo transaction");
}

static void redo() { check(UndoMaster::getInstance()->redo(), "operation can be redone"); }

static FloatParameter* editableFloat(ControllableContainer& parent, const String& name, double value)
{
    auto* parameter = parent.addFloatParameter(name, "", value, -100, 100);
    parameter->userCanChangeName = true;
    parameter->isCustomizableByUser = true;
    parameter->isRemovableByUser = true;
    parameter->canBeDisabledByUser = true;
    parameter->userCanSetReadOnly = true;
    parameter->saveValueOnly = false;
    return parameter;
}

static void testMetadata()
{
    ControllableContainer first("First"), second("Second"), missing("Missing");
    ControllableContainer firstChild("Settings"), secondChild("Settings");
    first.addChildControllableContainer(&firstChild);
    second.addChildControllableContainer(&secondChild);
    auto* a = editableFloat(firstChild, "Amount", 80);
    auto* b = editableFloat(secondChild, "Amount", -40);
    auto* untouched = editableFloat(missing, "Elsewhere", 20);
    select({ &first, &second, &missing });
    check(a->getRelatedSelectedParameters().size() == 2, "nested same paths match, missing paths are skipped");
    {
        std::unique_ptr<ControllableEditor> editor(static_cast<ControllableEditor*>(a->getEditor(false)));
        check(editor->isMultiEditing(), "nested control editor knows all matching targets");
        editor->label.setText("Renamed", dontSendNotification);
        editor->labelTextChanged(&editor->label);
        check(a->niceName == "Renamed" && b->niceName == "Renamed", "label renames matching controls");
        undo();
        check(a->niceName == "Amount" && b->niceName == "Amount", "undo restores names and paths");
        redo();
        check(a->niceName == "Renamed" && b->niceName == "Renamed", "redo renames both");
    }

    select({ &first, &second, &missing });
    a->setUndoableRangeForSelected(-10, 10);
    check(a->doubleValue() == 10 && b->doubleValue() == -10, "new range clamps each value");
    check((double)b->minimumValue == -10 && untouched->doubleValue() == 20, "range applies only to matching paths");
    undo();
    check(a->doubleValue() == 80 && b->doubleValue() == -40, "range undo restores distinct unclamped values");
    check((double)a->maximumValue == 100 && (double)b->maximumValue == 100, "range undo restores bounds");
    redo();
    check(a->doubleValue() == 10 && b->doubleValue() == -10, "range redo clamps again");

    select({ &first, &second });
    a->clearUndoableRangeForSelected();
    check(!a->hasRange() && !b->hasRange(), "clear range applies to both");
    undo();
    check(a->hasRange() && b->hasRange(), "clear range undo restores both");

    select({ &first, &second });
    a->setUndoableAttributeForSelected("enabled", false);
    check(!a->enabled && !b->enabled, "enabled metadata applies to both");
    undo();
    check(a->enabled && b->enabled, "enabled metadata undoes together");

    select({ &first, &second });
    a->setUndoableAttributeForSelected("readOnly", true);
    check(a->isControllableFeedbackOnly && b->isControllableFeedbackOnly, "read-only metadata applies to both");
    undo();
    check(!a->isControllableFeedbackOnly && !b->isControllableFeedbackOnly, "read-only metadata undoes together");

    select({ &first, &second });
    a->setUndoableAttributeForSelected("alwaysNotify", true);
    check(a->alwaysNotify && b->alwaysNotify, "alwaysNotify metadata applies to both");
    undo();
    check(!a->alwaysNotify && !b->alwaysNotify, "alwaysNotify metadata undoes together");

    firstChild.nameCanBeChangedByUser = secondChild.nameCanBeChangedByUser = true;
    select({ &first, &second });
    firstChild.setUndoableNiceNameForSelected("Shared settings");
    check(firstChild.niceName == "Shared settings" && secondChild.niceName == "Shared settings", "nested containers rename together");
    undo();
    check(firstChild.niceName == "Settings" && secondChild.niceName == "Settings", "container rename undo restores both");

    select({ a, b, untouched });
    b->userCanChangeName = false;
    a->setUndoableNiceNameForSelected("Direct");
    check(a->niceName == "Direct" && b->niceName == "Renamed" && untouched->niceName == "Direct", "direct compatible selection respects rename permission");
    undo();
    InspectableSelectionManager::mainSelectionManager->clearSelection();
}

static void testRanges()
{
    Point2DParameter a("Point", ""), b("Other point", "");
    a.isCustomizableByUser = b.isCustomizableByUser = true;
    a.setBounds(-100, -100, 100, 100);
    b.setBounds(-100, -100, 100, 100);
    a.setPoint(50, -50);
    b.setPoint(-30, 30);
    select({ &a, &b });
    {
        std::unique_ptr<ParameterUI> ui(static_cast<ParameterUI*>(a.createDefaultUI()));
        ui->handleMenuSelectedID(-71);
        check(a.x == 1 && a.y == -1 && b.x == -1 && b.y == 1, "vector preset applies to every selected point");
        undo();
        check(a.x == 50 && a.y == -50 && b.x == -30 && b.y == 30, "vector range undo restores original coordinates");
    }
    b.isCustomizableByUser = false;
    select({ &a, &b });
    a.setUndoableRangeForSelected(var(Array<var>{ 0, 0 }), var(Array<var>{ 1, 1 }));
    check(a.x == 1 && b.x == -30, "range changes respect customization permission");
    undo();
    InspectableSelectionManager::mainSelectionManager->clearSelection();
}

static void testManagers()
{
    BaseItem first("First"), second("Second"), missing("Missing");
    BaseManager<BaseItem> a("Children"), b("Children"), wrongPath("Different");
    first.addChildControllableContainer(&a);
    second.addChildControllableContainer(&b);
    missing.addChildControllableContainer(&wrongPath);
    select({ &first, &second, &missing });
    check(a.getRelatedSelectedContainers().size() == 2, "nested managers match by path");
    {
        GenericManagerEditor<BaseItem> editor(&a, false);
        check(editor.isMultiEditing(), "manager inspector knows its matching managers");
        editor.handleMenuSelectedID(1101);
        check(a.items.size() == 1 && b.items.size() == 1 && wrongPath.items.isEmpty(), "add menu adds to matching managers");
        check(first.isSelected && second.isSelected, "group add preserves selected parents");
        undo();
        check(a.items.isEmpty() && b.items.isEmpty(), "group add undoes together");
        redo();
        check(a.items.size() == 1 && b.items.size() == 1, "group add redoes together");
        undo();
        redo();
        check(a.items.size() == 1 && b.items.size() == 1, "group add survives repeated undo and redo");
    }
    a.items[0]->askConfirmationBeforeRemove = false;
    select({ &first, &second, &missing });
    {
        BaseItemEditor editor(a.items[0], false);
        editor.buttonClicked(editor.removeBT.get());
        check(a.items.isEmpty() && b.items.isEmpty(), "delete button removes matching nested items");
        undo();
        check(a.items.size() == 1 && b.items.size() == 1, "group deletion restores both items");
        redo();
        check(a.items.isEmpty() && b.items.isEmpty(), "group deletion redo removes both restored items");
    }

    auto* one = a.addItem(new BaseItem("One"), var(), false);
    auto* two = a.addItem(new BaseItem("Two"), var(), false);
    a.addItem(new BaseItem("Three"), var(), false);
    select({ one, two });
    one->removeForSelected();
    check(a.items.size() == 1 && a.items[0]->niceName == "Three", "direct selection deletes together within a manager");
    undo();
    check(a.items.size() == 3 && a.items[0]->niceName == "One" && a.items[1]->niceName == "Two", "delete undo restores original order");
    redo();
    check(a.items.size() == 1 && a.items[0]->niceName == "Three", "same-manager deletion redo works");
    undo();
    const String previousClipboard = SystemClipboard::getTextFromClipboard();
    struct ClipboardRestore
    {
        String text;
        ~ClipboardRestore() { SystemClipboard::copyTextToClipboard(text); }
    } restore { previousClipboard };
    SystemClipboard::copyTextToClipboard(JSON::toString(a.items[0]->getJSONData()));
    select({ &first, &second });
    a.addItemsFromClipboardForSelected();
    check(a.items.size() == 4 && b.items.size() == 1, "paste adds to matching managers");
    undo();
    check(a.items.size() == 3 && b.items.isEmpty(), "paste undoes together");
    redo();
    check(a.items.size() == 4 && b.items.size() == 1, "paste redoes together");

    select({ &first, &second });
    a.addItem(new BaseItem("Only first"), var(), false);
    check(a.items.size() == 5 && b.items.size() == 1, "programmatic adds remain local");
    InspectableSelectionManager::mainSelectionManager->clearSelection();
}

static void testControllables()
{
    ControllableContainer first("First"), second("Second");
    first.userCanAddControllables = second.userCanAddControllables = true;
    auto* control = new FloatParameter("Added", "", 5, 0, 10);
    control->isCustomizableByUser = control->isRemovableByUser = control->userCanChangeName = true;
    control->saveValueOnly = false;
    select({ &first, &second });
    first.addUndoableControllableForSelected(control);
    check(first.controllables.size() == 1 && second.controllables.size() == 1, "controls add together");
    undo();
    check(first.controllables.isEmpty() && second.controllables.isEmpty(), "controls undo together");
    redo();
    check(first.controllables.size() == 1 && second.controllables.size() == 1, "control add redo recreates and attaches both");
    check(((Parameter*)second.controllables[0])->doubleValue() == 5, "control add redo retains data");
    check(second.controllables[0]->userCanChangeName, "cloned and redone controls remain renameable");

    auto* a = editableFloat(first, "One", 1);
    auto* b = editableFloat(first, "Two", 2);
    auto* c = editableFloat(first, "Three", 3);
    select({ a, b });
    a->removeForSelected();
    check(first.controllables.size() == 2 && first.controllables[1] == c, "selected control deletion removes both");
    undo();
    check(first.controllables.size() == 4 && first.controllables[1]->niceName == "One"
        && first.controllables[2]->niceName == "Two", "control deletion restores original order");
    redo();
    check(first.controllables.size() == 2, "selected control deletion redoes together");
    InspectableSelectionManager::mainSelectionManager->clearSelection();
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    MultiEditTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        testMetadata();
        testRanges();
        testManagers();
        testControllables();
        std::cout << "Multi-edit regression tests passed\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << "Multi-edit regression failed: " << error.what() << "\n";
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
