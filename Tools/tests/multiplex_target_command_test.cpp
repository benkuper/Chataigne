#include "MainIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"
#include "StateMachine/StateMachineIncludes.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
#undef main

class MultiplexTargetTestApplication : public OrganicApplication
{
public:
    MultiplexTargetTestApplication() : OrganicApplication("Multiplex target tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

static var commandParams()
{
    var params(new DynamicObject());
    params.getDynamicObject()->setProperty("action", GenericControllableCommand::SET_VALUE);
    return params;
}

static void finishLoading(Engine& engine)
{
    engine.engineListeners.call(&EngineListener::endLoadFile);
    engine.isLoadingFile = false;
    engine.engineListeners.call(&EngineListener::fileLoaded);
}

static void exerciseLiveLink(Engine& engine, CommandContext context)
{
    auto* text = engine.addStringParameter("Live text", "", "initial");
    auto* secondText = engine.addStringParameter("Second live text", "", "initial");
    {
        Multiplex multiplex;
        multiplex.count->setValue(2);
        auto* list = static_cast<TargetMultiplexList*>(multiplex.listManager.addItem(new TargetMultiplexList(), var(), false));
        auto* first = static_cast<TargetParameter*>(list->list[0]);
        auto* second = static_cast<TargetParameter*>(list->list[1]);
        first->setValueFromTarget(text);
        second->setValueFromTarget(secondText);
        GenericControllableCommand command(nullptr, context, commandParams(), &multiplex);
        command.getLinkedParam(command.target)->setLinkedList(list);
        check(command.value != nullptr && command.value->type == Controllable::STRING,
            "linking an existing list immediately creates the string value");
        check(command.componentOperator->getValueKey() == "All" && command.valueOperator->getValueKey() == "Equals",
            "linking a list populates the component and operator");
        command.getLinkedParam(command.value)->setLinkType(ParameterLink::NONE);
        command.value->setValue("linked");
        command.triggerInternal(0);
        command.triggerInternal(1);
        check(text->stringValue() == "linked" && secondText->stringValue() == "linked", "command updates each multiplex target");

        auto* empty = static_cast<TargetMultiplexList*>(multiplex.listManager.addItem(new TargetMultiplexList(), var(), false));
        command.getLinkedParam(command.target)->setLinkedList(empty);
        check(command.value == nullptr, "switching to an empty list clears the value editor");
        static_cast<TargetParameter*>(empty->list[0])->setValueFromTarget(text);
        check(command.value != nullptr && command.value->type == Controllable::STRING,
            "populating a linked empty list creates the value editor");
        auto* number = engine.addFloatParameter("Live number", "", 0);
        static_cast<TargetParameter*>(empty->list[0])->setValueFromTarget(number);
        check(command.value != nullptr && command.value->type == Controllable::FLOAT,
            "changing the first list target rebuilds the value type");
        command.getLinkedParam(command.value)->setLinkType(ParameterLink::NONE);
        command.value->setValue(0.25f);
        command.triggerInternal(0);
        check(std::abs(number->floatValue() - 0.25f) < 0.0001f, "changed target receives the command value");
        static_cast<TargetParameter*>(empty->list[0])->setValueFromTarget(text);
        engine.removeControllable(number);
    }
    engine.removeControllable(secondText);
    engine.removeControllable(text);
}

static void exerciseDeferredColor(Engine& engine, CommandContext context)
{
    const String name = context == ACTION ? "Deferred action color" : "Deferred mapping color";
    auto* color = engine.addColorParameter(name, "", Colours::black);
    const String component = color->getValuesNames()[1];
    var saved, savedList;
    {
        Multiplex multiplex;
        auto* list = static_cast<TargetMultiplexList*>(multiplex.listManager.addItem(new TargetMultiplexList(), var(), false));
        static_cast<TargetParameter*>(list->list[0])->setValueFromTarget(color);
        GenericControllableCommand source(nullptr, context, commandParams(), &multiplex);
        source.getLinkedParam(source.target)->setLinkedList(list);
        // Make the fixture independently of the live-link bug.
        source.updateComponentFromTarget();
        source.componentOperator->setValueWithKey(component);
        source.valueOperator->setValueWithData(GenericControllableCommand::ADD);
        source.getLinkedParam(source.value)->setLinkType(ParameterLink::NONE);
        source.value->setValue(0.2f);
        saved = source.getJSONData();
        savedList = list->getJSONData();
    }
    engine.removeControllable(color);
    engine.isLoadingFile = true;
    Multiplex multiplex;
    auto* list = static_cast<TargetMultiplexList*>(multiplex.listManager.addItem(new TargetMultiplexList(), savedList, false));
    GenericControllableCommand loaded(nullptr, context, commandParams(), &multiplex);
    loaded.loadJSONData(saved);
    color = engine.addColorParameter(name, "", Colours::black);
    finishLoading(engine);
    check(loaded.value != nullptr, "deferred target resolution creates the value editor");
    check(loaded.componentOperator->getValueKey() == component, "deferred target preserves the selected color component");
    check(loaded.valueOperator->getValueDataAsEnum<GenericControllableCommand::Operator>() == GenericControllableCommand::ADD,
        "deferred target preserves a non-default operator");
    check(loaded.getLinkedParam(loaded.value)->linkType == ParameterLink::NONE, "deferred recovery preserves an unlinked value");
    check(std::abs(loaded.value->floatValue() - 0.2f) < 0.0001f, "deferred target preserves the saved value");
    loaded.triggerInternal(0);
    check(std::abs(static_cast<float>(color->getValue()[1]) - 0.2f) < 0.0001f, "restored command changes the selected color component");
    loaded.valueOperator->setValueWithData(GenericControllableCommand::MULTIPLY);
    list->notifyItemUpdated(0);
    check(loaded.valueOperator->getValueDataAsEnum<GenericControllableCommand::Operator>() == GenericControllableCommand::MULTIPLY,
        "later list updates preserve the edited operator");
    engine.removeControllable(color);
}

static GenericControllableCommand* genericCommand(BaseItem* item)
{
    auto* consequence = dynamic_cast<Consequence*>(item);
    auto* command = consequence ? dynamic_cast<GenericControllableCommand*>(consequence->command.get()) : nullptr;
    check(command != nullptr, "demo consequence uses a generic command");
    return command;
}

static void exerciseDemo(Engine& engine, const var& data)
{
    // Use the real saved managers in Engine's order, without restoring UI layout.
    engine.isLoadingFile = true;
    StateManager::getInstance()->loadJSONData(data.getProperty("states", var()));
    DashboardManager::getInstance()->loadJSONData(data.getProperty("dashboardManager", var()));
    finishLoading(engine);
    auto* multiplex = dynamic_cast<Multiplex*>(engine.getControllableContainerForAddress("/states/state/processors/multiplex"));
    check(multiplex != nullptr, "demo multiplex loaded");
    auto* action = dynamic_cast<Action*>(multiplex->processorManager.items[0]);
    check(action != nullptr, "demo action loaded");
    auto* on = genericCommand(action->csmOn->items[0]);
    auto* off = genericCommand(action->csmOff->items[0]);
    auto* text = dynamic_cast<StringParameter*>(engine.getControllableForAddress("/dashboards/dashboard/dashboard/item/customText"));
    check(text != nullptr, "demo dashboard target loaded");
    check(on->value != nullptr && off->value != nullptr, "demo TRUE and FALSE consequences restore their values");
    check(on->valueOperator->getValueKey() == "Equals" && off->valueOperator->getValueKey() == "Equals",
        "demo operators restored");
    on->triggerInternal(0);
    check(text->stringValue() == "HIGH", "demo TRUE consequence writes HIGH");
    off->triggerInternal(0);
    check(text->stringValue() == "LOW", "demo FALSE consequence writes LOW");

    var roundTrip(new DynamicObject());
    roundTrip.getDynamicObject()->setProperty("states", StateManager::getInstance()->getJSONData());
    roundTrip.getDynamicObject()->setProperty("dashboardManager", DashboardManager::getInstance()->getJSONData());
    roundTrip = JSON::parse(JSON::toString(roundTrip));
    // Exercise a second save/load in the same version, without metadata migration.
    StateManager::getInstance()->clear();
    DashboardManager::getInstance()->clear();
    engine.isLoadingFile = true;
    StateManager::getInstance()->loadJSONData(roundTrip.getProperty("states", var()));
    DashboardManager::getInstance()->loadJSONData(roundTrip.getProperty("dashboardManager", var()));
    finishLoading(engine);
    multiplex = dynamic_cast<Multiplex*>(engine.getControllableContainerForAddress("/states/state/processors/multiplex"));
    action = dynamic_cast<Action*>(multiplex->processorManager.items[0]);
    on = genericCommand(action->csmOn->items[0]);
    off = genericCommand(action->csmOff->items[0]);
    text = dynamic_cast<StringParameter*>(engine.getControllableForAddress("/dashboards/dashboard/dashboard/item/customText"));
    check(on->value != nullptr && off->value != nullptr, "same-version reload restores both values");
    on->triggerInternal(0);
    check(text->stringValue() == "HIGH", "same-version reload writes HIGH");
    off->triggerInternal(0);
    check(text->stringValue() == "LOW", "same-version reload writes LOW");
}

int main(int argc, char** argv)
{
    ScopedJuceInitialiser_GUI gui;
    MultiplexTargetTestApplication app;
    app.engine.reset(new ChataigneEngine());
    int failures = 0;
    auto run = [&](const char* name, auto test)
    {
        try { test(); std::cout << "PASS: " << name << std::endl; }
        catch (const std::exception& e)
        {
            ++failures;
            std::cerr << "FAIL: " << name << ": " << e.what() << std::endl;
            app.engine->isLoadingFile = false;
        }
    };
    run("live action list", [&] { exerciseLiveLink(*app.engine, ACTION); });
    run("live mapping list", [&] { exerciseLiveLink(*app.engine, MAPPING); });
    run("deferred action color", [&] { exerciseDeferredColor(*app.engine, ACTION); });
    run("deferred mapping color", [&] { exerciseDeferredColor(*app.engine, MAPPING); });
    run("demo and same-version reload", [&]
    {
        check(argc == 2, "pass the demo fixture path");
        const var data = JSON::parse(File(String::fromUTF8(argv[1])));
        check(data.isObject(), "demo fixture is valid JSON");
        exerciseDemo(*app.engine, data);
    });
    app.engine.reset();
    return failures == 0 ? 0 : 1;
}
