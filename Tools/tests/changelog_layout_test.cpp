#include "MainIncludes.h"
#include <iostream>
#include <stdexcept>
#undef main

class ChangelogTestApplication : public OrganicApplication
{
public:
    ChangelogTestApplication() : OrganicApplication("Changelog layout tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

static const String changelog =
    "Version 1.10.5b7:\n"
    "[OSC] Fixed the script callback crash reported by Romai when the OSC module receiver is missing or has been deleted\n"
    "[OSC] Fixed hierarchy conversion and Clear Values when Auto Feedback is enabled\n\n\n"
    "Version 1.10.5b6:\n"
    "[Timeline] Fixed direct and queued sequence seeks during playback, including audio/video synchronization\n";

static void checkLayout(UpdateDialogWindow& dialog)
{
    auto& editor = dialog.changelogLabel;
    check(editor.getText() == changelog, "changelog contents are preserved");
    check(editor.isReadOnly(), "changelog remains read-only");
    TextEditor reference;
    reference.setMultiLine(true);
    reference.setSize(editor.getWidth(), editor.getHeight());
    const auto font = Font(editor.getFont().getHeight());
    reference.setFont(Font(dialog.getLookAndFeel().getTypefaceForFont(font)).withHeight(font.getHeight()));
    reference.setText(changelog);
    for (int i = 0; i <= changelog.length(); ++i)
    {
        const auto actual = editor.getCaretRectangleForCharIndex(i);
        const auto expected = reference.getCaretRectangleForCharIndex(i);
        check(std::abs(actual.getX() - expected.getX()) <= 1
              && std::abs(actual.getY() - expected.getY()) <= 1,
              "word positions and wrapping match the active interface font");
    }
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    ChangelogTestApplication app;
    app.engine.reset(new ChataigneEngine());
    int failures = 0;
    {
        auto& laf = LookAndFeel::getDefaultLookAndFeel();
        laf.setDefaultSansSerifTypefaceName("Segoe UI");
        FloatParameter progression("Progression", "", 0, 0, 1);
        UpdateDialogWindow dialog("A new version is available", "1.10.5b7", changelog, &progression);
        const auto run = [&](const char* label, auto test)
        {
            try { test(); std::cout << "PASS: " << label << std::endl; }
            catch (const std::exception& error)
            {
                ++failures;
                std::cerr << "FAIL: " << label << ": " << error.what() << std::endl;
            }
        };
        run("initial layout", [&] { checkLayout(dialog); });
        dialog.changelogLabel.setHighlightedRegion({ 8, 16 });
        laf.setDefaultSansSerifTypefaceName("Arial");
        dialog.sendLookAndFeelChange();
        run("font switch with existing text", [&] { checkLayout(dialog); });
        run("selection survives font switch", [&]
        {
            check(dialog.changelogLabel.getHighlightedRegion() == Range<int>(8, 16), "selection is preserved");
        });
        Typeface::clearTypefaceCache();
        dialog.sendLookAndFeelChange();
        run("font renderer reload", [&] { checkLayout(dialog); });
        dialog.setSize(735, 600);
        run("resized wrapping", [&] { checkLayout(dialog); });
        laf.setDefaultSansSerifTypefaceName("Microsoft YaHei");
        dialog.sendLookAndFeelChange();
        run("second font switch", [&] { checkLayout(dialog); });
        auto snapshot = File::getSpecialLocation(File::currentExecutableFile).getSiblingFile("changelog_layout.png");
        FileOutputStream output(snapshot);
        output.setPosition(0);
        output.truncate();
        PNGImageFormat().writeImageToStream(dialog.createComponentSnapshot(dialog.getLocalBounds()), output);
    }
    app.engine.reset();
    return failures == 0 ? 0 : 1;
}
