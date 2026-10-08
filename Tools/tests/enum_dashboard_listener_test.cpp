#include "MainIncludes.h"
#include <iostream>
#include <stdexcept>
#undef main

class EnumDashboardTestApplication : public OrganicApplication
{
public:
    EnumDashboardTestApplication() : OrganicApplication("Enum dashboard tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

static void editOptions(EnumParameter& parameter)
{
    // Exercise the same clear/rebuild path as validating a Key/Value label.
    EnumOptionManager editor(&parameter);
    editor.optionsUI[0]->valueLabel.setText("/updated/path", dontSendNotification);
    editor.labelTextChanged(&editor.optionsUI[0]->valueLabel);
    check(parameter.enumValues[0]->value.toString() == "/updated/path", "Key/Value edit");
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    EnumDashboardTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        EnumParameter first("First", ""), second("Second", "");
        first.addOption("A", "/first");
        second.addOption("B", "/second");
        {
            DashboardEnumParameterItem item(&first);
            check(first.enumListeners.contains(&item), "constructor attaches listener");
            item.clearItem();
            check(!first.enumListeners.contains(&item), "clearItem detaches Enum listener before deletion");
        }
        check(first.enumListeners.size() == 0, "removed item leaves no stale listener");
        editOptions(first);

        {
            DashboardEnumParameterItem item(&first);
            item.setInspectable(&second);
            check(!first.enumListeners.contains(&item), "retarget detaches old Enum");
            check(second.enumListeners.contains(&item), "retarget attaches new Enum");
            item.setInspectable(&second);
            check(second.enumListeners.size() == 1, "same target does not duplicate listener");
            editOptions(first);
            editOptions(second);
            // Dashboard managers call clearItem() before destroying an item.
            item.clearItem();
        }
        check(first.enumListeners.size() == 0 && second.enumListeners.size() == 0,
            "removal after retarget detaches current Enum");
        editOptions(second);

        {
            DashboardEnumParameterItem item;
            item.setInspectable(&first);
            check(first.enumListeners.contains(&item), "loaded dashboard attaches listener");
            item.setInspectable(nullptr);
            check(!first.enumListeners.contains(&item), "null target detaches listener");
            item.setInspectable(&first);
            check(first.enumListeners.contains(&item), "restored target reattaches listener");
            item.clearItem();
        }

        {
            auto source = std::make_unique<EnumParameter>("Temporary", "");
            DashboardEnumParameterItem item(source.get());
            source.reset();
            check(item.parameter == nullptr, "deleted target clears weak reference");
            item.clearItem();
        }

        std::cout << "Enum dashboard listener regression tests passed\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Enum dashboard listener regression failed: " << e.what() << "\n";
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
