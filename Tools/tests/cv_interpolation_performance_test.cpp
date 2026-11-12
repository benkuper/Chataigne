#include "MainIncludes.h"
#include "CustomVariables/CustomVariablesIncludes.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>
#undef main
#include <windows.h>

class CVPerformanceApplication : public OrganicApplication
{
public:
    CVPerformanceApplication() : OrganicApplication("CV performance tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

static void pumpMessages(int milliseconds)
{
    const auto end = Time::getMillisecondCounterHiRes() + milliseconds;
    do
    {
        MSG message;
        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        { TranslateMessage(&message); DispatchMessage(&message); }
        Thread::sleep(1);
    } while (Time::getMillisecondCounterHiRes() < end);
}

struct ValueListener : ParameterListener, Parameter::AsyncListener
{
    int values = 0, displays = 0;
    int previous = 0;
    var displayedValue;
    void parameterValueChangedWithValue(Parameter*, const var& value) override
    {
        check(MessageManager::getInstance()->isThisTheMessageThread(), "functional callbacks stay on the message thread");
        check((int)value == previous + 1, "functional callbacks preserve captured values and ordering");
        previous = (int)value;
        ++values;
    }
    void newMessage(const Parameter::ParameterEvent& event) override { ++displays; displayedValue = event.value; }
};

static void notificationBurst()
{
    FloatParameter parameter("Animated", "", 0);
    ValueListener listener, ordinary;
    parameter.addParameterListener(&listener);
    parameter.addAsyncCoalescedParameterListener(&listener);
    parameter.addAsyncParameterListener(&ordinary);
    std::thread worker([&] { for (int i = 1; i <= 500; ++i) parameter.setValue(i); });
    worker.join();
    int valuesBeforeInput = -1;
    MessageManager::callAsync([&] { valuesBeforeInput = listener.values; });
    const auto deadline = Time::getMillisecondCounterHiRes() + 5000;
    while (listener.values < 500 && Time::getMillisecondCounterHiRes() < deadline) pumpMessages(5);
    pumpMessages(20);
    std::cout << "500 queued values: functional=" << listener.values << ", display=" << listener.displays << std::endl;
    check(listener.values == 500, "every functional value must be preserved");
    check(ordinary.displays == 500, "ordinary async listeners still receive every value");
    check(valuesBeforeInput > 0 && valuesBeforeInput <= 128, "input messages run before the notification backlog drains");
    check(listener.displays > 0 && listener.displays < 50, "queued value burst coalesces display updates between batches");
    check((int)listener.displayedValue == 500, "display reaches the final value");

    listener.values = listener.displays = 0;
    listener.previous = 0;
    for (int i = 1; i <= 500; ++i) parameter.setValue(i);
    check(listener.values == 500 && listener.displays == 0, "message-thread values are immediate but display updates are deferred");
    pumpMessages(20);
    check(listener.displays == 1, "message-thread value burst coalesces too");
    parameter.removeParameterListener(&listener);
    parameter.removeAsyncParameterListener(&listener);
    parameter.removeAsyncParameterListener(&ordinary);

    // Destroying a parameter must cancel both deferred displays and queued values.
    auto temporary = std::make_unique<FloatParameter>("Temporary", "", 0);
    temporary->addAsyncCoalescedParameterListener(&listener);
    temporary->setValue(1);
    std::thread producer([&] { temporary->setValue(2); });
    producer.join();
    temporary.reset();
    listener.displays = 0;
    pumpMessages(20);
    check(listener.displays == 0, "destroyed parameter has no deferred callbacks");
}

static void interpolationCompatibility()
{
    FloatParameter number("Float", "", 7.123456789);
    IntParameter integer("Int", "", 21);
    ColorParameter color("Color", "", Colour::fromFloatRGBA(.2f, .4f, .8f, .7f));
    Point2DParameter point("2D", ""); point.setPoint(.7f, -2.3f);
    Point3DParameter point3("3D", ""); point3.setValue(var(Array<var>{ .1, -.3, .9 }));
    for (auto* parameter : Array<Parameter*>{ &number, &integer, &color, &point, &point3 })
    {
        const var target = parameter->value.clone();
        var start = -13.456789;
        if (target.isArray())
        {
            start = var();
            for (int i = 0; i < target.size(); ++i) start.append(.05f * (i + 1));
        }
        for (float weight : { -.2f, 0.f, .17f, .5f, .99f, 1.f, 1.2f })
        {
            CVGroup::InterpolationValue snapshot { parameter, parameter->type, start, target, ParameterPreset::INTERPOLATE };
            const auto expected = weight == 0 ? start : weight == 1 ? target : parameter->getLerpValueTo(start, 1 - weight);
            check(JSON::toString(snapshot.getValueAt(weight)) == JSON::toString(expected), "captured interpolation matches the parameter implementation");
        }
    }

    CVGroup group;
    EnumParameter choice("Choice", ""); choice.addOption("First", 11)->addOption("Second", 22);
    group.addItemFromParameter(&choice, false);
    auto* parameter = static_cast<EnumParameter*>(group.values.items[0]->controllable);
    auto* preset = group.pm->addItem(nullptr, var(), false);
    auto* value = preset->values.getParameterPresetForSource(parameter);
    value->parameter->setValue("Second");
    value->interpolationMode->setValueWithData(ParameterPreset::CHANGE_AT_END);
    group.goToPreset(preset, .04f, &group.defaultInterpolation);
    value->parameter->setValue("First"); // In-flight destination is immutable.
    check(group.waitForThreadToExit(2000), "captured preset completes");
    check(parameter->getValueKey() == "Second", "enum snapshot stores its key and ignores later preset edits");
    pumpMessages(40);
    std::cout << "Interpolation compatibility and snapshot lifetime passed" << std::endl;
}

static void multipleProducers()
{
    constexpr int parameterCount = 64, frameCount = 64;
    OwnedArray<FloatParameter> parameters;
    OwnedArray<ValueListener> listeners;
    for (int i = 0; i < parameterCount; ++i)
    {
        auto* parameter = parameters.add(new FloatParameter("Animated " + String(i), "", 0));
        auto* listener = listeners.add(new ValueListener());
        parameter->addParameterListener(listener);
        parameter->addAsyncCoalescedParameterListener(listener);
    }
    std::vector<std::thread> producers;
    for (int thread = 0; thread < 4; ++thread)
        producers.emplace_back([&, thread]
        {
            for (int frame = 1; frame <= frameCount; ++frame)
                for (int i = thread; i < parameterCount; i += 4) parameters[i]->setValue(frame);
        });
    for (auto& producer : producers) producer.join();

    auto delivered = [&]
    {
        int count = 0;
        for (auto* listener : listeners) count += listener->values;
        return count;
    };
    int valuesBeforeInput = -1;
    MessageManager::callAsync([&] { valuesBeforeInput = delivered(); });
    struct Heartbeat : Timer
    {
        int ticks = 0;
        void timerCallback() override { ++ticks; }
    } heartbeat;
    heartbeat.startTimer(5);
    const auto deadline = Time::getMillisecondCounterHiRes() + 5000;
    while (delivered() < parameterCount * frameCount && Time::getMillisecondCounterHiRes() < deadline)
        pumpMessages(5);
    heartbeat.stopTimer();
    check(delivered() == parameterCount * frameCount, "all values from four producer threads arrive");
    check(valuesBeforeInput > 0 && valuesBeforeInput <= 128, "input interleaves with multi-parameter delivery");
    check(heartbeat.ticks > 1, "UI timers continue while worker values drain");
    pumpMessages(20);
    for (int i = 0; i < parameterCount; ++i)
    {
        check(listeners[i]->previous == frameCount && (int)listeners[i]->displayedValue == frameCount, "each parameter and display reaches its final value");
        parameters[i]->removeParameterListener(listeners[i]);
        parameters[i]->removeAsyncParameterListener(listeners[i]);
    }
    std::cout << "Four producers: " << delivered() << " ordered values, input after " << valuesBeforeInput
        << ", UI timer ticks=" << heartbeat.ticks << std::endl;
}

static void projectPresets(const File& file)
{
    if (!file.existsAsFile()) return;
    const auto data = JSON::parse(file);
    check(data.isObject(), "project JSON parses");
    auto* groups = CVGroupManager::getInstance();
    // Load only CV data: no devices, scripts, network outputs or performance cues.
    groups->loadJSONData(data.getProperty("customVariables", var()));
    pumpMessages(100);
    for (int pass = 0; pass < 2; ++pass)
    {
        double total = 0, maximum = 0;
        int count = 0;
        for (auto* group : groups->items)
        {
            if (group->pm->items.isEmpty()) continue;
            auto* preset = group->pm->items[jmin(pass, group->pm->items.size() - 1)];
            const auto start = Time::getMillisecondCounterHiRes();
            group->goToPreset(preset, 1, &group->defaultInterpolation);
            const auto elapsed = Time::getMillisecondCounterHiRes() - start;
            total += elapsed; maximum = jmax(maximum, elapsed); ++count;
        }
        std::cout << "Preset pass " << pass << ": " << count << " groups, total=" << total << "ms, slowest=" << maximum << "ms" << std::endl;
        pumpMessages(1200);
        for (auto* group : groups->items) group->stopInterpolation();
    }
}

int main(int argc, char** argv)
{
    ScopedJuceInitialiser_GUI gui;
    CVPerformanceApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        notificationBurst();
        interpolationCompatibility();
        multipleProducers();
        if (argc > 1) projectPresets(File(argv[1]));
        std::cout << "CV performance tests passed" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
