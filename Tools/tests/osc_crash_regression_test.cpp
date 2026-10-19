#include "MainIncludes.h"
#include "Module/ModuleIncludes.h"
#include <iostream>
#include <stdexcept>
#include <windows.h>
#undef main

class OSCCrashTestApplication : public OrganicApplication
{
public:
    OSCCrashTestApplication() : OrganicApplication("OSC crash regression tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

static void testInvalidScriptReceivers()
{
    using Callback = var (*)(const var::NativeFunctionArgs&);
    const Callback callbacks[] = { OSCModule::sendOSCFromScript, OSCModule::sendOSCToFromScript,
        OSCModule::matchOSCAddrFromScript, OSCModule::registerOSCCallbackFromScript };
    var arguments[] = { "/test", "/test", 9000 };
    for (const auto& receiver : { var(), var(new DynamicObject()) })
        for (auto callback : callbacks)
            check(callback(var::NativeFunctionArgs(receiver, arguments, 3)).isVoid(),
                "OSC callbacks reject a missing module receiver");

    // Scripts may retain an OSC object's methods after the module is deleted.
    var retained;
    {
        auto module = std::make_unique<OSCModule>("Test OSC", 0, 0, false, true);
        retained = module->getScriptObject();
    }
    for (auto callback : callbacks)
        check(callback(var::NativeFunctionArgs(retained, arguments, 3)).isVoid(),
            "OSC callbacks reject a deleted module receiver");

    OSCModule module("Test OSC", 0, 0, false, true);
    const auto receiver = module.getScriptObject();
    var matchArguments[] = { "/test/1", "/test/*" };
    check((bool)OSCModule::matchOSCAddrFromScript(var::NativeFunctionArgs(receiver, matchArguments, 2)),
        "OSC match still works with a live module receiver");
    OSCModule::sendOSCFromScript(var::NativeFunctionArgs(receiver, arguments, 1));
    JavascriptEngine javascript;
    javascript.registerNativeObject("osc", receiver.getDynamicObject());
    check(javascript.execute("var send = osc.send; send('/test');").wasOk(),
        "a detached send method does not crash the JavaScript engine");
}

static void testHierarchyConversion()
{
    CustomOSCModule module;
    module.autoAdd->setValue(false);
    auto* first = module.valuesCC.addFloatParameter("/group/level", "", 0.75f);
    auto* invalid = module.valuesCC.addStringParameter("Invalid address", "", "keep me");
    WeakReference<Controllable> firstReference(first), invalidReference(invalid);

    module.useHierarchy->setValue(true);
    check(!module.useHierarchy->boolValue(), "invalid conversion restores the previous hierarchy mode");
    check(firstReference != nullptr && invalidReference != nullptr,
        "invalid conversion preserves all original values");
    check(first->floatValue() == 0.75f && invalid->stringValue() == "keep me",
        "invalid conversion leaves parameter contents intact");
    check(!module.hierarchyStructureSwitch && !module.autoAdd->boolValue(),
        "invalid conversion restores conversion and auto-add state");

    module.valuesCC.removeControllable(invalid);
    for (bool feedback : { false, true })
    {
        module.autoFeedback->setValue(feedback);
        module.useHierarchy->setValue(true);
        check(module.useHierarchy->boolValue(), "valid hierarchy conversion succeeds");
        auto* nested = module.valuesCC.getControllableForAddress("/group/level");
        check(nested != nullptr && static_cast<Parameter*>(nested)->floatValue() == 0.75f,
            "hierarchy conversion preserves the nested address and value");
        check(nested->niceName == "level", "hierarchy conversion creates nested containers");
        check(module.getMatchingControllables(OSCAddressPattern("/group/level")).size() == 1,
            "hierarchy conversion rebuilds the OSC address map");
        module.useHierarchy->setValue(false);
        auto* flat = module.valuesCC.getControllableByName("/group/level", true);
        check(flat != nullptr && static_cast<Parameter*>(flat)->floatValue() == 0.75f,
            "flattening preserves the full address and value");
        check(!module.hierarchyStructureSwitch && !module.autoAdd->boolValue(),
            "successful conversion restores conversion and auto-add state");
    }
    module.clearValues->trigger();
    check(module.valuesCC.getAllControllables(true).isEmpty(),
        "clear values works with auto feedback enabled");
    check(module.getMatchingControllables(OSCAddressPattern("/group/level")).isEmpty(),
        "clear values removes OSC address mappings");
}

int main(int argc, char** argv)
{
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    ScopedJuceInitialiser_GUI gui;
    OSCCrashTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        const String selection = argc > 1 ? argv[1] : "all";
        if (selection == "all" || selection == "script") testInvalidScriptReceivers();
        if (selection == "all" || selection == "hierarchy") testHierarchyConversion();
        std::cout << "OSC crash regression tests passed\n";
    }
    catch (const OSCFormatError& error)
    {
        std::cerr << "Uncaught OSC format error: " << error.description << '\n';
        app.engine.reset();
        return 1;
    }
    catch (const std::exception& error)
    {
        std::cerr << "OSC crash regression failed: " << error.what() << '\n';
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
