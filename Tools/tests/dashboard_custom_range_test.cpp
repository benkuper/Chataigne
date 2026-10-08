#include "MainIncludes.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <windows.h>
#undef main

class DashboardRangeTestApplication : public OrganicApplication
{
public:
    DashboardRangeTestApplication() : OrganicApplication("Dashboard range tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

static void pump()
{
    auto deadline = GetTickCount64() + 30;
    do
    {
        MSG message;
        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        MsgWaitForMultipleObjects(0, nullptr, FALSE, 1, QS_ALLINPUT);
    } while (GetTickCount64() < deadline);
}

static var values(std::initializer_list<double> coordinates)
{
    var result;
    for (double coordinate : coordinates) result.append(coordinate);
    return result;
}

static void enableRange(DashboardParameterItem& item, float low, float high)
{
    item.customRange->setPoint(low, high);
    item.useCustomRange->setValue(true);
}

static ParameterUI* getParameterUI(DashboardParameterItemUI& ui)
{
    auto* result = dynamic_cast<ParameterUI*>(ui.itemUI.get());
    check(result != nullptr, "dashboard has a parameter UI");
    return result;
}

static void testFloat()
{
    FloatParameter source("Float", "", 75, -100, 100);
    DashboardParameterItem item(&source), other(&source);
    check(!item.hasCustomRange(), "custom range is disabled by default");
    check(item.customRange->x == -100 && item.customRange->y == 100, "initial bounds follow source");
    enableRange(item, -10, 10);
    enableRange(other, -20, 20);
    DashboardParameterItemUI ui(&item), otherUI(&other);
    auto* slider = dynamic_cast<FloatSliderUI*>(ui.itemUI.get());
    check(slider != nullptr, "float default dashboard uses slider");
    check(source.doubleValue() == 75, "creating UI does not clamp source value");
    check(slider->getParamNormalizedValue() == 1, "feedback outside range stays at slider edge");
    check((double)getParameterUI(otherUI)->getUIMinimumValue() == -20, "items have independent bounds");
    check((double)item.getServerData()["minVal"] == -10, "web minimum uses custom bounds");
    check((double)item.getServerData()["maxVal"] == 10, "web maximum uses custom bounds");
    slider->initValue = 75;
    slider->setParamNormalizedValue(0.25f);
    check(source.doubleValue() == -5, "slider maps custom bounds");
    UndoMaster::getInstance()->clearUndoHistory();
    slider->setParamNormalizedValueUndoable(1, 0.25f);
    check(UndoMaster::getInstance()->undo(), "slider interaction can be undone");
    check(source.doubleValue() == 75, "undo restores original out-of-range value");
    slider->setParamNormalizedValue(2);
    check(source.doubleValue() == 10, "drag is clamped to custom maximum");
    source.setRange(-200, 200);
    pump();
    check((double)slider->getUIMinimumValue() == -10, "source range edits retain override");
    check((double)source.minimumValue == -200, "dashboard does not change source bounds");

    item.useCustomRange->setValue(false);
    pump();
    check(!getParameterUI(ui)->useCustomRange, "disabling restores source range");
    check((double)item.getServerData()["minVal"] == -200, "web falls back to source bounds");
    item.useCustomRange->setValue(true);
    pump();
    check((double)getParameterUI(ui)->getUIMinimumValue() == -10, "re-enabling retains saved bounds");

    item.style->setValueWithData(2);
    pump();
    auto* label = dynamic_cast<FloatParameterLabelUI*>(ui.itemUI.get());
    check(label != nullptr, "text style is available");
    label->valueLabel.setText("50", dontSendNotification);
    label->updateValueFromLabel();
    check(source.doubleValue() == 10, "typed values respect custom bounds");
    auto editor = std::unique_ptr<ParameterUI::ValueEditCalloutComponent>(
        static_cast<ParameterUI::ValueEditCalloutComponent*>(label->getEditValueComponent()));
    editor->labels[0]->setText("-50", dontSendNotification);
    editor->labelTextChanged(editor->labels[0]);
    check(source.doubleValue() == -10, "value callout respects custom bounds");

    item.customRange->setPoint(8, -8);
    pump();
    check((double)item.getRangeBound(false) == -8 && (double)item.getRangeBound(true) == 8,
        "reversed bounds are ordered safely");
    item.customRange->setPoint(4, 4);
    pump();
    check((double)getParameterUI(ui)->getUINormalizedValue() == 0, "equal bounds have finite normalization");
    check((double)getParameterUI(ui)->cropUIValue(100) == 4, "equal bounds clamp to fixed value");
}

static void testInt()
{
    IntParameter source("Int", "", 75, -100, 100);
    DashboardParameterItem item(&source);
    enableRange(item, 2, 8);
    DashboardParameterItemUI ui(&item);
    auto* stepper = dynamic_cast<IntStepperUI*>(ui.itemUI.get());
    check(stepper != nullptr, "integer default dashboard uses stepper");
    pump();
    check(source.intValue() == 75, "stepper range update does not write clipped feedback");
    check(stepper->slider->getMinimum() == 2 && stepper->slider->getMaximum() == 8,
        "stepper applies custom range");
    stepper->slider->setValue(5, sendNotificationSync);
    check(source.intValue() == 5, "stepper writes integer value");
    item.style->setValueWithData(0);
    pump();
    auto* slider = dynamic_cast<FloatSliderUI*>(ui.itemUI.get());
    check(slider != nullptr, "integer slider style supports custom range");
    slider->setParamNormalizedValue(0.5f);
    check(source.intValue() == 5, "integer slider maps custom range");
    check(item.getServerData()["minVal"].isInt(), "integer server bounds retain type");
    item.customRange->setPoint(4, 4);
    item.style->setValueWithData(-1);
    pump();
    check(std::isfinite((double)getParameterUI(ui)->getUINormalizedValue()), "fixed integer bounds are safe");
}

static void testPoints()
{
    Point2DParameter source2("Point 2D", "");
    source2.setBounds(-100, -200, 100, 200);
    source2.setPoint(75, 150);
    DashboardParameterItem item2(&source2);
    enableRange(item2, -10, 10);
    item2.customRangeY->setPoint(-20, 20);
    DashboardParameterItemUI ui2(&item2);
    auto* sliders2 = dynamic_cast<DoubleSliderUI*>(ui2.itemUI.get());
    check(sliders2 != nullptr, "2D dashboard has axis sliders");
    check((double)sliders2->xSlider->getUIMinimumValue() == -10
        && (double)sliders2->ySlider->getUIMinimumValue() == -20, "2D bounds reach each axis");
    auto* xSlider = dynamic_cast<FloatSliderUI*>(sliders2->xSlider.get());
    check(xSlider != nullptr, "ranged X axis is a slider");
    xSlider->setParamNormalizedValue(0.25f);
    pump();
    check(source2.x == -5 && source2.y == 150, "editing X preserves other axis feedback outside custom range");
    check((double)source2.minimumValue[0] == -100 && (double)source2.minimumValue[1] == -200,
        "2D source bounds are unchanged");

    item2.style->setValueWithData(12);
    pump();
    auto* canvas = dynamic_cast<P2DUI*>(ui2.itemUI.get());
    check(canvas != nullptr, "2D canvas style supports override");
    auto mapped = canvas->getUIValueFromNormalized(values({ 0.25, 0.75 }));
    check((double)mapped[0] == -5 && (double)mapped[1] == 10, "canvas maps per-axis custom ranges");
    item2.customRangeY->setPoint(3, 3);
    pump();
    canvas->setSize(200, 200);
    check(std::isfinite(canvas->canvasRect.getWidth()) && std::isfinite(canvas->canvasRect.getHeight()),
        "canvas geometry handles fixed axis bounds");

    Point3DParameter source3("Point 3D", "");
    source3.setBounds(-100, -200, -300, 100, 200, 300);
    source3.setVector(75, 150, 250);
    DashboardParameterItem item3(&source3);
    enableRange(item3, -10, 10);
    item3.customRangeY->setPoint(-20, 20);
    item3.customRangeZ->setPoint(-30, 30);
    DashboardParameterItemUI ui3(&item3);
    auto* sliders3 = dynamic_cast<TripleSliderUI*>(ui3.itemUI.get());
    check(sliders3 != nullptr, "3D dashboard has axis sliders");
    auto* zSlider = dynamic_cast<FloatSliderUI*>(sliders3->zSlider.get());
    check(zSlider != nullptr, "ranged Z axis is a slider");
    zSlider->setParamNormalizedValue(0.25f);
    pump();
    check(source3.x == 75 && source3.y == 150 && source3.z == -15,
        "3D axis edit maps custom range and preserves other coordinates");
    auto data3 = item3.getServerData();
    check(data3["minVal"].size() == 3 && (double)data3["minVal"][2] == -30
        && (double)data3["maxVal"][1] == 20, "web exports all 3D custom bounds");
    item3.useCustomRange->setValue(false);
    pump();
    sliders3 = dynamic_cast<TripleSliderUI*>(ui3.itemUI.get());
    check(sliders3 != nullptr, "3D dashboard restores axis sliders");
    check(!sliders3->zSlider->useCustomRange, "3D disabling clears child overrides");
}

static void testPersistenceAndTargets()
{
    FloatParameter source("Saved", "", 0.5, 0, 1);
    DashboardParameterItem item(&source);
    item.useCustomRange->setValue(true);
    auto saved = JSON::parse(JSON::toString(item.getJSONData()));
    source.setRange(-100, 100);
    DashboardParameterItem restored(&source);
    restored.loadJSONData(saved);
    check(restored.hasCustomRange() && (double)restored.getRangeBound(false) == 0
        && (double)restored.getRangeBound(true) == 1, "save/load preserves initialized range after source bounds change");
    restored.setInspectable(nullptr);
    check(!restored.hasCustomRange(), "missing target is safe");
    restored.setInspectable(&source);
    check(restored.hasCustomRange() && (double)restored.getRangeBound(true) == 1,
        "retarget restores custom bounds");
    BoolParameter boolean("Bool", "", false);
    DashboardParameterItem booleanItem(&boolean);
    check(booleanItem.useCustomRange->hideInEditor && !booleanItem.hasCustomRange(),
        "unsupported parameters do not expose custom bounds");
    FloatParameter unbounded("Unbounded", "", 0);
    DashboardParameterItem unboundedItem(&unbounded);
    unboundedItem.useCustomRange->setValue(true);
    DashboardParameterItemUI unboundedUI(&unboundedItem);
    check(dynamic_cast<FloatSliderUI*>(unboundedUI.itemUI.get()) != nullptr,
        "custom range enables default slider for unbounded float");
    check((double)unboundedItem.getRangeBound(false) == 0 && (double)unboundedItem.getRangeBound(true) == 1,
        "unbounded source starts with usable custom bounds");
    Point3DParameter unboundedPoint("Unbounded Point", "");
    DashboardParameterItem unboundedPointItem(&unboundedPoint);
    unboundedPointItem.useCustomRange->setValue(true);
    auto minimum = unboundedPointItem.getRangeBound(false);
    auto maximum = unboundedPointItem.getRangeBound(true);
    for (int axis = 0; axis < 3; ++axis)
        check((double)minimum[axis] == 0 && (double)maximum[axis] == 1,
            "unbounded vector starts with usable per-axis bounds");
    FloatParameter lowerBounded("Lower Bounded", "", 5, 5);
    DashboardParameterItem lowerBoundedItem(&lowerBounded);
    lowerBoundedItem.useCustomRange->setValue(true);
    check((double)lowerBoundedItem.getRangeBound(false) == 5 && (double)lowerBoundedItem.getRangeBound(true) == 6,
        "one-sided source starts with a non-empty custom range");
    item.clearItem();
    restored.clearItem();
}

int main()
{
    std::cerr << "Initializing dashboard range tests\n";
    ScopedJuceInitialiser_GUI gui;
    DashboardRangeTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        std::cerr << "Float ranges\n";
        testFloat();
        std::cerr << "Integer ranges\n";
        testInt();
        std::cerr << "Point ranges\n";
        testPoints();
        std::cerr << "Persistence and targets\n";
        testPersistenceAndTargets();
        std::cout << "Dashboard custom range regression tests passed\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Dashboard custom range regression failed: " << e.what() << "\n";
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
