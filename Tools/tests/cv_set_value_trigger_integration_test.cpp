#include "MainIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include <iostream>
#include <stdexcept>
#undef main

class CVCommandTestApplication : public OrganicApplication
{
public:
    CVCommandTestApplication() : OrganicApplication("CV command tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

static GenericControllableCommand* getCommand(BaseItem* item)
{
    auto* consequence = dynamic_cast<Consequence*>(item);
    auto* command = consequence ? dynamic_cast<GenericControllableCommand*>(consequence->command.get()) : nullptr;
    check(command != nullptr, "Set Value consequence has a generic controllable command");
    return command;
}

static void exerciseDuplicateAndMove()
{
    check(Parameter::ValueInterpolator::Manager::getInstanceWithoutCreating() == nullptr,
        "reproduce before any interpolation manager exists");
    auto* groups = CVGroupManager::getInstance();
    auto* group = groups->addItem(new CVGroup("Bool command test"), var(), false);
    BoolParameter original("Boolean", "", false);
    group->addItemFromParameter(&original, false);
    auto* sourceBoolean = static_cast<Parameter*>(group->values.items[0]->controllable);
    auto* boolean = groups->module->getValueCCForGroup(group)->getParameterForSource(sourceBoolean);
    check(boolean != nullptr, "CV module exposes the Boolean target");
    Sequence sequence;
    auto* layer = static_cast<ChataigneTriggerLayer*>(sequence.layerManager->addItem(
        new ChataigneTriggerLayer(&sequence), var(), false));
    auto* trigger = static_cast<ChataigneTimeTrigger*>(layer->ttm->addItem(nullptr, var(), false));
    trigger->time->setValue(1);
    trigger->length->setValue(2);
    // This regression exercises explicit TRUE-on-entry / FALSE-on-exit commands.
    trigger->duration->everValidExit->setValueWithData(ChataigneTimeTrigger::action_triggerFalse);
    auto* on = static_cast<Consequence*>(trigger->csm->addItem(new Consequence(), var(), false));
    on->setCommand(groups->module->getCommandDefinitionFor("", "Set Value"));
    auto* onCommand = getCommand(on);
    onCommand->target->setValueFromTarget(boolean);
    check(onCommand->value != nullptr, "Boolean command value created");
    onCommand->value->setValue(true);

    trigger->csm->askForDuplicateItem(on);
    check(trigger->csm->items.size() == 2, "consequence duplicated");
    auto* duplicate = trigger->csm->items[1];
    auto* oldCommand = getCommand(duplicate);
    ParameterListener* oldListener = oldCommand;
    WeakReference<ControllableContainer> oldCommandRef = oldCommand;
    check(boolean->parameterListeners.contains(oldListener), "duplicate listens to Boolean target");
    check(trigger->untcsm->handleMoveFromRemoteControl(duplicate, true), "undoable move to FALSE consequences");
    check(oldCommandRef.wasObjectDeleted(), "move destroys the previous command");
    const bool staleListener = boolean->parameterListeners.contains(oldListener);
    // Clean up the dangling pointer on a failing build so the test reports the
    // regression deterministically instead of crashing during engine teardown.
    if (staleListener) boolean->removeParameterListener(oldListener);
    check(!staleListener, "destroyed Set Value command must remove its target listener without an interpolation manager");
    check(trigger->csm->items.size() == 1 && trigger->untcsm->items.size() == 1,
        "move leaves one TRUE and one FALSE consequence");
    auto* offCommand = getCommand(trigger->untcsm->items[0]);
    check(offCommand->value != nullptr && offCommand->value->boolValue(), "duplicate preserves TRUE value");
    offCommand->value->setValue(false);

    // The first mutation must also be safe when made manually, before any timeline entry.
    boolean->setValue(true);
    trigger->untcsm->triggerAll();
    check(!boolean->boolValue() && !sourceBoolean->boolValue(), "first manual Boolean change and FALSE preview are safe");

    for (int i = 0; i < 3; ++i)
    {
        layer->ttm->executeTriggersTimespan(0, 1.5f, true);
        check(boolean->boolValue(), "entering long trigger sets Boolean TRUE");
        check(sourceBoolean->boolValue(), "TRUE propagates to the CV group");
        layer->ttm->executeTriggersTimespan(1.5f, 3.5f, true);
        check(!boolean->boolValue(), "leaving long trigger sets Boolean FALSE");
        check(!sourceBoolean->boolValue(), "FALSE propagates to the CV group");
    }

    auto* undo = UndoMaster::getInstance();
    check(undo->undo(), "undo consequence move");
    check(trigger->csm->items.size() == 2 && trigger->untcsm->items.isEmpty(), "undo restores TRUE consequences");
    check(undo->redo(), "redo consequence move");
    getCommand(trigger->untcsm->items[0])->value->setValue(false);
    layer->ttm->executeTriggersTimespan(0, 1.5f, true);
    check(boolean->boolValue(), "TRUE consequence works after undo and redo");
    layer->ttm->executeTriggersTimespan(1.5f, 3.5f, true);
    check(!boolean->boolValue(), "FALSE consequence works after undo and redo");
    undo->clearUndoHistory();
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    CVCommandTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        exerciseDuplicateAndMove();
        std::cout << "CV Set Value trigger integration tests passed (duplicate, move, enter/exit, undo/redo)\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "CV Set Value trigger integration failed: " << e.what() << "\n";
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
