#include "MainIncludes.h"
#include "Module/ModuleIncludes.h"
#include <iostream>
#include <stdexcept>
#undef main

class MIDINoteTemplateTestApplication : public OrganicApplication
{
public:
    MIDINoteTemplateTestApplication() : OrganicApplication("MIDI note template tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

static var templateParams(const String& commandType)
{
    var params(new DynamicObject());
    params.getDynamicObject()->setProperty("commandType", commandType);
    return params;
}

static CommandTemplateParameter* templateParameter(CommandTemplate& ct, const String& name)
{
    auto* parameter = dynamic_cast<CommandTemplateParameter*>(ct.paramsContainer.getControllableContainerByName(name, true));
    check(parameter != nullptr, ("template parameter exists: " + name).toRawUTF8());
    return parameter;
}

static void checkVisibility(CommandTemplate& ct, bool pitchMode)
{
    check(templateParameter(ct, "Note")->hideInEditor == pitchMode, "Note visibility follows mode");
    check(templateParameter(ct, "Octave")->hideInEditor == pitchMode, "Octave visibility follows mode");
    check(templateParameter(ct, "Pitch")->hideInEditor != pitchMode, "Pitch visibility follows mode");
}

static std::unique_ptr<MIDINoteAndCCCommand> createCommand(CommandTemplate& ct, MIDIModule& module, CommandContext context)
{
    auto* command = dynamic_cast<MIDINoteAndCCCommand*>(ct.createCommand(&module, context, var()));
    check(command != nullptr, "template creates a MIDI note command");
    return std::unique_ptr<MIDINoteAndCCCommand>(command);
}

static void exerciseTemplate(MIDIModule& module, const String& commandType, CommandContext context)
{
    CommandTemplate ct(&module, templateParams(commandType));
    auto* mode = templateParameter(ct, "Note Mode");
    auto* pitch = templateParameter(ct, "Pitch");
    auto* note = templateParameter(ct, "Note");
    auto* octave = templateParameter(ct, "Octave");
    checkVisibility(ct, false);
    // Set both representations so switching must inherit their distinct defaults.
    pitch->parameter->setValue(73);
    note->parameter->setValue("D");
    octave->parameter->setValue(3);
    note->editable->setValue(false);
    octave->editable->setValue(false);

    auto command = createCommand(ct, module, context);
    check(command->noteEnum->getValueKey() == "D" && command->octave->intValue() == 3, "Note defaults are inherited");
    const int controlCount = command->controllables.size();
    auto* originalNote = command->noteEnum;
    command->updateParametersFromTemplate();
    check(command->noteEnum == originalNote && command->controllables.size() == controlCount, "reapplying the template is idempotent");

    mode->parameter->setValue("Pitch");
    checkVisibility(ct, true);
    check(command->noteEnum == nullptr && command->octave == nullptr && command->number != nullptr, "linked command switches to Pitch");
    check(command->number->intValue() == 73 && !command->number->isOverriden, "new Pitch inherits the template without a false override");
    check(command->controllables.size() == controlCount - 1, "mode switch removes exactly the obsolete fields");
    pitch->parameter->setValue(74);
    check(command->number->intValue() == 74, "editable Pitch tracks template changes until overridden");
    command->number->setValue(88);
    pitch->parameter->setValue(75);
    check(command->number->intValue() == 88 && (int)command->number->defaultValue == 75, "editable Pitch preserves a user override");
    command->number->resetValue();
    check(command->number->intValue() == 75 && !command->number->isOverriden, "Pitch resets to the latest template default");

    pitch->editable->setValue(false);
    check(command->number->isControllableFeedbackOnly, "Pitch inherits non-editable state");
    if (context == MAPPING)
        check(!command->getLinkedParam(command->number)->isLinkable, "locked Pitch cannot be linked in mappings");
    pitch->parameter->setValue(76);
    check(command->number->intValue() == 76, "locked Pitch follows the template");
    auto pitchCommand = createCommand(ct, module, context);
    check(pitchCommand->number->intValue() == 76 && pitchCommand->number->isControllableFeedbackOnly, "new Pitch command inherits value and editability");

    // Visibility is derived from the mode even if saved metadata disagrees.
    var saved = JSON::parse(JSON::toString(ct.getJSONData()));
    for (auto& item : *saved.getProperty("params", var()).getArray())
        item.getDynamicObject()->setProperty("hideInEditor", item.getProperty("niceName", "").toString() == "Pitch");
    CommandTemplate loaded(&module, saved);
    loaded.loadJSONData(saved);
    checkVisibility(loaded, true);
    auto loadedCommand = createCommand(loaded, module, context);
    check(loadedCommand->number->intValue() == 76 && loadedCommand->number->isControllableFeedbackOnly, "saved Pitch template restores values and editability");

    mode->parameter->setValue("Note");
    checkVisibility(ct, false);
    check(command->number == nullptr && command->noteEnum->getValueKey() == "D" && command->octave->intValue() == 3, "switching back restores Note and Octave template values");
    check(command->noteEnum->isControllableFeedbackOnly && command->octave->isControllableFeedbackOnly, "recreated Note and Octave inherit editability");
    check(command->controllables.size() == controlCount, "switching back restores the original field count");

    var savedNote = JSON::parse(JSON::toString(ct.getJSONData()));
    CommandTemplate loadedNote(&module, savedNote);
    loadedNote.loadJSONData(savedNote);
    checkVisibility(loadedNote, false);
    check(templateParameter(loadedNote, "Pitch")->parameter->intValue() == 76, "inactive Pitch value survives saving in Note mode");

    // A locally overridden mode can use a different representation to the template.
    mode->editable->setValue(true);
    command->noteMode->setValue("Pitch");
    check(command->number->intValue() == 76 && command->number->isControllableFeedbackOnly, "local mode override still inherits the matching Pitch settings");
    note->parameter->setValue("E");
    check(command->number->intValue() == 76, "inactive Note changes leave Pitch alone");
    command->noteMode->resetValue();
    check(command->noteEnum->getValueKey() == "E", "resetting local mode recreates the template representation");
}

static void exerciseLegacyTemplate(MIDIModule& module)
{
    CommandTemplate original(&module, templateParams("Note On"));
    templateParameter(original, "Note")->parameter->setValue("G");
    templateParameter(original, "Octave")->parameter->setValue(4);
    var saved = JSON::parse(JSON::toString(original.getJSONData()));
    auto* parameters = saved.getDynamicObject()->getProperty("params").getArray();
    for (int i = parameters->size(); --i >= 0;)
        if ((*parameters)[i].getProperty("niceName", "").toString() == "Pitch") parameters->remove(i);
    CommandTemplate loaded(&module, saved);
    loaded.loadJSONData(saved);
    checkVisibility(loaded, false);
    auto command = createCommand(loaded, module, ACTION);
    check(command->noteEnum->getValueKey() == "G" && command->octave->intValue() == 4, "legacy Note templates retain their settings");
    templateParameter(loaded, "Note Mode")->parameter->setValue("Pitch");
    check(command->number != nullptr && !command->number->isOverriden, "legacy template can adopt Pitch mode");

    // Rebuilding a template must replace its callback and owned parameters together.
    const int count = original.templateParams.size();
    original.generateParametersFromDefinition(original.sourceDef);
    check(original.templateParams.size() == count, "regenerating a template does not accumulate parameters");
    templateParameter(original, "Note Mode")->parameter->setValue("Pitch");
    checkVisibility(original, true);
}

static void exerciseOtherCommands(MIDIModule& module)
{
    for (const auto& name : StringArray("Control Change", "Program Change", "Pitch Wheel", "Channel Pressure"))
    {
        CommandTemplate ct(&module, templateParams(name));
        check(ct.paramsContainer.getControllableContainerByName("Pitch", true) == nullptr, "non-note templates gain no Pitch field");
        auto command = createCommand(ct, module, ACTION);
        check(command->noteMode == nullptr, "non-note commands gain no Note Mode");
    }
    var params(new DynamicObject());
    params.getDynamicObject()->setProperty("type", MIDINoteAndCCCommand::NOTE_ON);
    MIDINoteAndCCCommand standalone(&module, ACTION, params);
    standalone.noteEnum->setValue("F");
    standalone.octave->setValue(2);
    const int expectedPitch = standalone.getPitchFromNote(0);
    standalone.noteMode->setValue("Pitch");
    check(standalone.number->intValue() == expectedPitch, "standalone commands preserve note-to-pitch conversion");
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    MIDINoteTemplateTestApplication app;
    app.engine.reset(new ChataigneEngine());
    int failures = 0;
    {
        MIDIModule module;
        auto run = [&](const String& label, auto test)
        {
            try { test(); std::cout << "PASS: " << label << std::endl; }
            catch (const std::exception& error)
            {
                ++failures;
                std::cerr << "FAIL: " << label << ": " << error.what() << std::endl;
            }
        };
        for (const auto& name : StringArray("Note On", "Note Off", "Full Note", "After Touch"))
        {
            run(name + " action", [&] { exerciseTemplate(module, name, ACTION); });
            run(name + " mapping", [&] { exerciseTemplate(module, name, MAPPING); });
        }
        run("legacy template", [&] { exerciseLegacyTemplate(module); });
        run("other MIDI commands", [&] { exerciseOtherCommands(module); });
    }
    app.engine.reset();
    return failures == 0 ? 0 : 1;
}
