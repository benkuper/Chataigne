#include "MainIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"
#include "Module/ModuleIncludes.h"
#include <limits>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>
#undef main
#include <windows.h>

class MappingFrameTestApplication : public OrganicApplication
{
public:
    MappingFrameTestApplication() : OrganicApplication("Mapping frame tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition)
    {
        std::cerr << "Failed: " << label << std::endl;
        throw std::runtime_error(label);
    }
}

class TestMapping : public Mapping
{
public:
    ~TestMapping() override
    {
        // Test commands bypass CommandDefinition and must be detached before
        // Mapping destroys the output parameters referenced by its handlers.
        for (auto* output : om.items) output->command.reset();
    }
};

static void pumpMessages(int milliseconds = 100)
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

class RecordingCommand : public BaseCommand
{
public:
    RecordingCommand() : BaseCommand(nullptr, CommandContext::MAPPING, var()) {}
    std::vector<int> frames;
    IntParameter argument { "Frame", "", 0 };
    void setValueInternal(var value, int) override
    {
        // Reproduce the former integer cropping path to check the workbook's
        // duplicate-frame pattern independently of the OSC rounding fix.
        OSCMessage message("/example");
        OSCHelpers::addArgumentsForParameter(message, &argument, OSCHelpers::Int,
            OSCHelpers::ColorRGBA, argument.getCroppedValue(value[0]));
        frames.push_back(message[0].getInt32());
    }
};

class RecordingOSC : public IOSCSenderModule
{
public:
    std::vector<int> frames;
    void sendOSC(const OSCMessage& message) override
    {
        check(message.size() == 1 && message[0].isInt32(), "OSC argument stays int32");
        frames.push_back(message[0].getInt32());
    }
};

static OSCCommand* makeOSCCommand(RecordingOSC& sender, bool custom)
{
    if (custom)
    {
        auto* command = new CustomOSCCommand(&sender, CommandContext::MAPPING, var(new DynamicObject()));
        auto* argument = command->customValuesManager->createItemFromType(Controllable::INT);
        command->customValuesManager->addItem(argument, var(), false);
        argument->paramLink->setLinkType(ParameterLink::MAPPING_INPUT);
        return command;
    }
    auto* command = new OSCCommand(&sender, CommandContext::MAPPING, var(new DynamicObject()));
    auto* argument = command->argumentsContainer.addIntParameter("Frame", "", 0);
    command->argumentsContainer.linkParamToMappingIndex(argument, 0);
    return command;
}

static MathFilter* addMath(Mapping& mapping, MathFilter::Operation operation, double value = 0)
{
    auto* filter = new MathFilter(var(), nullptr);
    mapping.fm.addItem(filter, var(), false);
    filter->operation->setValueWithData(operation);
    if (filter->operationValue != nullptr) filter->operationValue->setValue(value);
    return filter;
}

static RecordingCommand* record(Mapping& mapping)
{
    auto* output = mapping.om.addItem(nullptr, var(), false);
    auto* command = new RecordingCommand();
    output->command.reset(command);
    return command;
}

static void checkFrames(const std::vector<int>& frames, const char* label)
{
    if (frames.size() != 500)
    {
        std::cerr << label << ": got " << frames.size() << " packets, expected 500\n";
        throw std::runtime_error(label);
    }
    for (int i = 0; i < 500; ++i)
        if (frames[i] != i + 1)
        {
            std::cerr << label << ": packet " << i + 1 << " was " << frames[i] << "\n";
            throw std::runtime_error(label);
        }
}

static void integerConversion()
{
    FloatParameter time("Time", "", 0, 0, 20);
    TestMapping mapping;
    mapping.lockInputTo({ &time });
    mapping.sendOnOutputChangeOnly->setValue(false);
    addMath(mapping, MathFilter::MULTIPLY, 25);
    auto* output = record(mapping);
    for (int frame = 1; frame <= 500; ++frame) time.setValue(frame / 25.0);
    check(output->frames.size() == 500, "unrounded test produces 500 packets");
    int incorrect = 0;
    for (int frame = 1; frame <= 500; ++frame)
        if (output->frames[frame - 1] != frame) ++incorrect;
    check(incorrect == 30, "unrounded conversion reproduces the workbook's 30 incorrect frame numbers");
    int duplicates = 0;
    for (int i = 1; i < 500; ++i) if (output->frames[i] == output->frames[i - 1]) ++duplicates;
    check(duplicates == 22, "unrounded conversion reproduces the workbook's 22 duplicate steps");
    check(output->frames[52] == 52 && output->frames[53] == 54,
        "frame 53 truncates to 52 before the next value jumps to 54");
    addMath(mapping, MathFilter::ROUND);
    time.setValue(0);
    output->frames.clear();
    for (int frame = 1; frame <= 500; ++frame) time.setValue(frame / 25.0);
    checkFrames(output->frames, "explicit rounding restores every integer frame");
    std::cout << "Float/integer conversion: workbook pattern reproduced; Round yields 1..500\n";
}

static void queuedInput(bool sendInput, bool sendOutput, bool withFilters)
{
    FloatParameter time("Time", "", 0, 0, withFilters ? 20 : 500);
    TestMapping mapping;
    mapping.lockInputTo({ &time });
    mapping.sendOnInputChangeOnly->setValue(sendInput);
    mapping.sendOnOutputChangeOnly->setValue(sendOutput);
    if (withFilters)
    {
        addMath(mapping, MathFilter::MULTIPLY, 25);
        addMath(mapping, MathFilter::ROUND);
    }
    auto* output = record(mapping);
    pumpMessages();
    output->frames.clear();
    // Intentionally keep the message thread blocked until every source tick
    // has queued. Each callback must still process its own captured value.
    std::thread producer([&]
    {
        for (int frame = 1; frame <= 500; ++frame)
            time.setValue(withFilters ? frame / 25.0 : frame);
    });
    producer.join();
    check(output->frames.empty(), "mapping callbacks remain on the message thread");
    pumpMessages();
    checkFrames(output->frames, "queued mapping input preserves every captured frame");
}

static void queuedFilterReference()
{
    FloatParameter time("Time", "", 0, 0, 20);
    FloatParameter input("Input", "", 0);
    TestMapping mapping;
    mapping.lockInputTo({ &input });
    mapping.sendOnOutputChangeOnly->setValue(true);
    auto* offset = addMath(mapping, MathFilter::OFFSET);
    offset->operationValue->setReferenceParameter(&time);
    addMath(mapping, MathFilter::MULTIPLY, 25);
    addMath(mapping, MathFilter::ROUND);
    auto* output = record(mapping);
    pumpMessages();
    output->frames.clear();
    std::thread producer([&]
    {
        for (int frame = 1; frame <= 500; ++frame) time.setValue(frame / 25.0);
    });
    producer.join();
    pumpMessages();
    checkFrames(output->frames, "queued filter reference preserves every captured frame");
}

static void oscRounding(bool custom, bool queued)
{
    RecordingOSC sender;
    FloatParameter time("Time", "", 0, 0, 20);
    TestMapping mapping;
    mapping.lockInputTo({ &time });
    mapping.sendOnOutputChangeOnly->setValue(true);
    addMath(mapping, MathFilter::MULTIPLY, 25);
    auto* output = mapping.om.addItem(nullptr, var(), false);
    output->command.reset(makeOSCCommand(sender, custom));
    pumpMessages();
    sender.frames.clear();
    auto produce = [&]
    {
        for (int frame = 1; frame <= 500; ++frame) time.setValue(frame / 25.0);
    };
    if (queued)
    {
        std::thread producer(produce);
        producer.join();
        check(sender.frames.empty(), "OSC delivery stays on the message thread");
        pumpMessages();
    }
    else produce();
    checkFrames(sender.frames, "OSC rounds all 500 frame values without a Round filter");
}

static void oscIntegerBoundaries(bool custom)
{
    RecordingOSC sender;
    std::unique_ptr<OSCCommand> command(makeOSCCommand(sender, custom));
    const double values[] = { 52.9999961853, 54.0000038147, 0.49, 0.5, -0.49, -0.5,
        1.5, -1.5, 2147483647.0, 2147483648.0, -2147483649.0,
        std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::quiet_NaN() };
    const int expected[] = { 53, 54, 0, 1, 0, -1, 2, -2, INT_MAX, INT_MAX, INT_MIN,
        INT_MAX, INT_MIN, 0 };
    for (int i = 0; i < numElementsInArray(values); ++i)
    {
        var input;
        input.append(values[i]);
        command->setValue(input, 0);
        check(sender.frames.back() == expected[i], "OSC rounding and integer saturation");
    }
    IntParameter integer("Integer", "", 0, -10, 10);
    ParameterLink link(&integer);
    link.setLinkType(ParameterLink::MAPPING_INPUT);
    var input;
    input.append(1.9);
    link.updateMappingInputValue(input, 0);
    check((int)link.getLinkedValue(0) == 1, "non-OSC links retain their conversion");
    check((int)link.getLinkedValue(0, true) == 2, "OSC rounds before integer cropping");
    input.getArray()->set(0, 20.0);
    link.updateMappingInputValue(input, 0);
    check((int)link.getLinkedValue(0, true) == 10, "OSC respects the argument range");
    FloatParameter floating("Float", "", 0);
    ParameterLink floatLink(&floating);
    floatLink.setLinkType(ParameterLink::MAPPING_INPUT);
    input.getArray()->set(0, 1.25);
    floatLink.updateMappingInputValue(input, 0);
    check((double)floatLink.getLinkedValue(0, true) == 1.25, "OSC float arguments retain fractions");
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    MappingFrameTestApplication app;
    app.engine.reset(new ChataigneEngine());
    struct TestTheme : LookAndFeelOO
    {
        ~TestTheme() override { LookAndFeel::setDefaultLookAndFeel(nullptr); }
    } theme;
    LookAndFeel::setDefaultLookAndFeel(&theme);
    try
    {
        integerConversion();
        for (bool sendInput : { false, true })
            for (bool sendOutput : { false, true })
                for (bool withFilters : { false, true })
                    queuedInput(sendInput, sendOutput, withFilters);
        queuedFilterReference();
        for (bool custom : { false, true })
        {
            oscRounding(custom, false);
            oscRounding(custom, true);
            oscIntegerBoundaries(custom);
        }
        std::cout << "Mapping frame consistency tests passed (OSC rounding, integer limits, all send flags, queued inputs and filter references)\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Mapping frame consistency regression failed: " << e.what() << "\n";
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
