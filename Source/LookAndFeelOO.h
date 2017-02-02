/*
  ==============================================================================

    LookAndFeelOO.h
    Created: 3 Mar 2016 10:21:45am
    Author:  bkupe

  ==============================================================================
*/

#ifndef LOOKANDFEELOO_H_INCLUDED
#define LOOKANDFEELOO_H_INCLUDED

#pragma warning( disable : 4505 )

#include "JuceHeader.h"//keep
#include "Style.h"//keep


namespace LookAndFeelHelpers {


    // initialise the standard set of colours..
    static const uint32 textButtonColour = NORMAL_COLOR.getARGB();
    static const uint32 textHighlightColour = TEXT_COLOR.getARGB();
    static const uint32 standardOutlineColour = 0xb2808080;

	static const uint32 standardColours[] =
	{
		TextButton::buttonColourId,                 textButtonColour,
		TextButton::buttonOnColourId,               HIGHLIGHT_COLOR.getARGB(),
		TextButton::textColourOnId,                 textHighlightColour,
		TextButton::textColourOffId,                TEXT_COLOR.getARGB(),

		ToggleButton::textColourId,                 TEXT_COLOR.getARGB(),

		TextEditor::backgroundColourId,             0xffffffff,
		TextEditor::textColourId,                   0xff000000,
		TextEditor::highlightColourId,              textHighlightColour,
		TextEditor::highlightedTextColourId,        0xff000000,
		TextEditor::outlineColourId,                0x00000000,
		TextEditor::focusedOutlineColourId,         textButtonColour,
		TextEditor::shadowColourId,                 0x38000000,

		CaretComponent::caretColourId,              0xff000000,

		Label::backgroundColourId,                  0x00000000,
		Label::textColourId,                        TEXT_COLOR.getARGB(),
		Label::outlineColourId,                     0x00000000,

		ScrollBar::backgroundColourId,              BG_COLOR.brighter(.2f).getARGB(),
		ScrollBar::thumbColourId,                   BG_COLOR.brighter(.6f).getARGB(),
		ScrollBar::trackColourId,                   Colours::red.getARGB(),

		TreeView::linesColourId,                    0x4c000000,
		TreeView::backgroundColourId,               0x00000000,
		TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
		TreeView::selectedItemBackgroundColourId,   0x00000000,
		TreeView::oddItemsColourId,					0,
		TreeView::evenItemsColourId,				0,

		PopupMenu::backgroundColourId,              BG_COLOR.getARGB(),
		PopupMenu::textColourId,                    TEXT_COLOR.getARGB(),
		PopupMenu::headerTextColourId,              0xff000000,
		PopupMenu::highlightedTextColourId,         BG_COLOR.getARGB(),
		PopupMenu::highlightedBackgroundColourId,   HIGHLIGHT_COLOR.getARGB(),

		ComboBox::buttonColourId,                   0xffCCCCCC,
		ComboBox::outlineColourId,                  standardOutlineColour,
		ComboBox::textColourId,                     0xff000000,
		ComboBox::backgroundColourId,               0xffCCCCCC,
		ComboBox::arrowColourId,                    0x99000000,

		PropertyComponent::backgroundColourId,      0x66ffffff,
		PropertyComponent::labelTextColourId,       0xff000000,

		TextPropertyComponent::backgroundColourId,  0xffffffff,
		TextPropertyComponent::textColourId,        0xff000000,
		TextPropertyComponent::outlineColourId,     standardOutlineColour,

		BooleanPropertyComponent::backgroundColourId, 0xffffffff,
		BooleanPropertyComponent::outlineColourId,  standardOutlineColour,

		ListBox::backgroundColourId,                0xffffffff,
		ListBox::outlineColourId,                   standardOutlineColour,
		ListBox::textColourId,                      0xff000000,

		Slider::backgroundColourId,                 0x00000000,
		Slider::thumbColourId,                      textButtonColour,
		Slider::trackColourId,                      0x7fffffff,
		Slider::rotarySliderFillColourId,           0x7f0000ff,
		Slider::rotarySliderOutlineColourId,        0x66000000,
		Slider::textBoxTextColourId,                TEXT_COLOR.getARGB(),
		Slider::textBoxBackgroundColourId,          BG_COLOR.darker(.1f).getARGB(),
		Slider::textBoxHighlightColourId,           textHighlightColour,
		Slider::textBoxOutlineColourId,             BG_COLOR.brighter(.1f).getARGB() ,

        ResizableWindow::backgroundColourId,        0xff777777,
        //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)

        AlertWindow::backgroundColourId,            0xffededed,
        AlertWindow::textColourId,                  0xff000000,
        AlertWindow::outlineColourId,               0xff666666,

        ProgressBar::backgroundColourId,            0xffeeeeee,
        ProgressBar::foregroundColourId,            0xffaaaaee,

        TooltipWindow::backgroundColourId,          0xffeeeebb,
        TooltipWindow::textColourId,                0xff000000,
        TooltipWindow::outlineColourId,             0x4c000000,

        TabbedComponent::backgroundColourId,        0x00000000,
        TabbedComponent::outlineColourId,           0xff777777,
        TabbedButtonBar::tabOutlineColourId,        0x80000000,
        TabbedButtonBar::frontOutlineColourId,      0x90000000,

        Toolbar::backgroundColourId,                0xfff6f8f9,
        Toolbar::separatorColourId,                 0x4c000000,
        Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
        Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
        Toolbar::labelTextColourId,                 0xff000000,
        Toolbar::editingModeOutlineColourId,        0xffff0000,

        DrawableButton::textColourId,               0xff000000,
        DrawableButton::textColourOnId,             0xff000000,
        DrawableButton::backgroundColourId,         0x00000000,
        DrawableButton::backgroundOnColourId,       0xaabbbbff,

        HyperlinkButton::textColourId,              0xcc1111ee,

        GroupComponent::outlineColourId,            0x66000000,
        GroupComponent::textColourId,               0xff000000,

        BubbleComponent::backgroundColourId,        0xeeeeeebb,
        BubbleComponent::outlineColourId,           0x77000000,

        DirectoryContentsDisplayComponent::highlightColourId,   textHighlightColour,
        DirectoryContentsDisplayComponent::textColourId,        0xff000000,

        0x1000440, /*LassoComponent::lassoFillColourId*/        0x66dddddd,
        0x1000441, /*LassoComponent::lassoOutlineColourId*/     0x99111111,

        0x1005000, /*MidiKeyboardComponent::whiteNoteColourId*/               0xffffffff,
        0x1005001, /*MidiKeyboardComponent::blackNoteColourId*/               0xff000000,
        0x1005002, /*MidiKeyboardComponent::keySeparatorLineColourId*/        0x66000000,
        0x1005003, /*MidiKeyboardComponent::mouseOverKeyOverlayColourId*/     0x80ffff00,
        0x1005004, /*MidiKeyboardComponent::keyDownOverlayColourId*/          0xffb6b600,
        0x1005005, /*MidiKeyboardComponent::textLabelColourId*/               0xff000000,
        0x1005006, /*MidiKeyboardComponent::upDownButtonBackgroundColourId*/  0xffd3d3d3,
        0x1005007, /*MidiKeyboardComponent::upDownButtonArrowColourId*/       0xff000000,
        0x1005008, /*MidiKeyboardComponent::shadowColourId*/                  0x4c000000,

        0x1004500, /*CodeEditorComponent::backgroundColourId*/                0xffffffff,
        0x1004502, /*CodeEditorComponent::highlightColourId*/                 textHighlightColour,
        0x1004503, /*CodeEditorComponent::defaultTextColourId*/               0xff000000,
        0x1004504, /*CodeEditorComponent::lineNumberBackgroundId*/            0x44999999,
        0x1004505, /*CodeEditorComponent::lineNumberTextId*/                  0x44000000,

        0x1007000, /*ColourSelector::backgroundColourId*/                     0xffe5e5e5,
        0x1007001, /*ColourSelector::labelTextColourId*/                      0xff000000,

        0x100ad00, /*KeyMappingEditorComponent::backgroundColourId*/          0x00000000,
        0x100ad01, /*KeyMappingEditorComponent::textColourId*/                0xff000000,

        FileSearchPathListComponent::backgroundColourId,        0xffffffff,

        FileChooserDialogBox::titleTextColourId,                0xff000000,
    };




    static Colour createBaseColour(Colour buttonColour,
        bool hasKeyboardFocus,
        bool isMouseOverButton,
        bool isButtonDown) noexcept
    {
        const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
        const Colour baseColour(buttonColour.withMultipliedSaturation(sat));

        if (isButtonDown)      return baseColour.contrasting(0.2f);
        if (isMouseOverButton) return baseColour.contrasting(0.1f);

        return baseColour;
    }

    static TextLayout layoutTooltipText(const String& text, Colour colour) noexcept
    {
        const float tooltipFontSize = 13.0f;
        const int maxToolTipWidth = 400;

        AttributedString s;
        s.setJustification(Justification::centred);
        s.append(text, Font(tooltipFontSize, Font::bold), colour);

        TextLayout tl;
        tl.createLayoutWithBalancedLineLengths(s, (float)maxToolTipWidth);
        return tl;
    }


};


class LookAndFeelOO:
	public LookAndFeel{
public:
//  juce_DeclareSingleton(LookAndFeelOO, true);
    LookAndFeelOO();
    ~LookAndFeelOO();
    //==============================================================================
    void drawButtonBackground (Graphics&, Button&, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;
    Font getTextButtonFont (TextButton&, int buttonHeight) override;

    void drawButtonText (Graphics&, TextButton&, bool isMouseOverButton, bool isButtonDown) override;
    int getTextButtonWidthToFitText (TextButton&, int buttonHeight) override;

    void drawToggleButton (Graphics&, ToggleButton&, bool isMouseOverButton, bool isButtonDown) override;

    void changeToggleButtonWidthToFitText (ToggleButton&) override;

    void drawTickBox (Graphics&, Component&,
                      float x, float y, float w, float h,
                      bool ticked, bool isEnabled, bool isMouseOverButton, bool isButtonDown) override;

    void drawDrawableButton (Graphics&, DrawableButton&, bool isMouseOverButton, bool isButtonDown) override;

    //==============================================================================
    AlertWindow* createAlertWindow (const String& title, const String& message,
                                    const String& button1,
                                    const String& button2,
                                    const String& button3,
                                    AlertWindow::AlertIconType iconType,
                                    int numButtons, Component* associatedComponent) override;

    void drawAlertBox (Graphics&, AlertWindow&, const Rectangle<int>& textArea, TextLayout&) override;
    int getAlertBoxWindowFlags() override;

	Array<int> getWidthsForTextButtons(AlertWindow &, const Array<TextButton *> & buttons) override
	{
		Array<int> w;
		for (int i =0;i<buttons.size();i++)
		{
			w.add(buttons.getUnchecked(i)->getWidth());
		}

		return w;
	}

    int getAlertWindowButtonHeight() override;

    /** Override this function to supply a custom font for the alert window title.
     This default implementation will use a boldened and slightly larger version
     of the alert window message font.

     @see getAlertWindowMessageFont.
     */
    Font getAlertWindowTitleFont() override;

    /** Override this function to supply a custom font for the alert window message.
     This default implementation will use the default font with height set to 15.0f.

     @see getAlertWindowTitleFont
     */
    Font getAlertWindowMessageFont() override;

    Font getAlertWindowFont() override;

    //==============================================================================
    void drawProgressBar (Graphics&, ProgressBar&, int width, int height, double progress, const String& textToShow) override;
    void drawSpinningWaitAnimation (Graphics&, const Colour& colour, int x, int y, int w, int h) override;

    //==============================================================================
    bool areScrollbarButtonsVisible() override;
    void drawScrollbarButton (Graphics&, ScrollBar&, int width, int height, int buttonDirection,
                              bool isScrollbarVertical, bool isMouseOverButton, bool isButtonDown) override;

    void drawScrollbar (Graphics&, ScrollBar&, int x, int y, int width, int height,
                        bool isScrollbarVertical, int thumbStartPosition, int thumbSize,
                        bool isMouseOver, bool isMouseDown) override;

    ImageEffectFilter* getScrollbarEffect() override;
    int getMinimumScrollbarThumbSize (ScrollBar&) override;
    int getDefaultScrollbarWidth() override;
    int getScrollbarButtonSize (ScrollBar&) override;

    //==============================================================================
    Path getTickShape (float height) override;
    Path getCrossShape (float height) override;

    //==============================================================================
    void drawTreeviewPlusMinusBox (Graphics&, const Rectangle<float>& area,
                                   Colour backgroundColour, bool isOpen, bool isMouseOver) override;
    bool areLinesDrawnForTreeView (TreeView&) override;
    int getTreeViewIndentSize (TreeView&) override;

    //==============================================================================
    void fillTextEditorBackground (Graphics&, int width, int height, TextEditor&) override;
    void drawTextEditorOutline (Graphics&, int width, int height, TextEditor&) override;
    CaretComponent* createCaretComponent (Component* keyFocusOwner) override;

    //==============================================================================
    const Drawable* getDefaultFolderImage() override;
    const Drawable* getDefaultDocumentFileImage() override;

    AttributedString createFileChooserHeaderText (const String& title, const String& instructions) override;

    void drawFileBrowserRow (Graphics&, int width, int height,
                             const String& filename, Image* icon,
                             const String& fileSizeDescription, const String& fileTimeDescription,
                             bool isDirectory, bool isItemSelected, int itemIndex,
                             DirectoryContentsDisplayComponent&) override;

    Button* createFileBrowserGoUpButton() override;

    void layoutFileBrowserComponent (FileBrowserComponent&,
                                     DirectoryContentsDisplayComponent*,
                                     FilePreviewComponent*,
                                     ComboBox* currentPathBox,
                                     TextEditor* filenameBox,
                                     Button* goUpButton) override;

    //==============================================================================
    void drawBubble (Graphics&, BubbleComponent&, const Point<float>& tip, const Rectangle<float>& body) override;

    void drawLasso (Graphics&, Component&) override;

    //==============================================================================

	void preparePopupMenuWindow (Component& newWindow) override ;
    void drawPopupMenuBackground (Graphics&, int width, int height) override;

    void drawPopupMenuItem (Graphics&, const Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu,
                            const String& text, const String& shortcutKeyText,
                            const Drawable* icon, const Colour* textColour) override;

    void drawPopupMenuSectionHeader (Graphics&, const Rectangle<int>& area,
                                     const String& sectionName) override;

    Font getPopupMenuFont() override;

    void drawPopupMenuUpDownArrow (Graphics&, int width, int height, bool isScrollUpArrow) override;

    void getIdealPopupMenuItemSize (const String& text, bool isSeparator, int standardMenuItemHeight,
                                    int& idealWidth, int& idealHeight) override;
    int getMenuWindowFlags() override;

    void drawMenuBarBackground (Graphics&, int width, int height, bool isMouseOverBar, MenuBarComponent&) override;
    int getMenuBarItemWidth (MenuBarComponent&, int itemIndex, const String& itemText) override;
    Font getMenuBarFont (MenuBarComponent&, int itemIndex, const String& itemText) override;
    int getDefaultMenuBarHeight() override;

    void drawMenuBarItem (Graphics&, int width, int height,
                          int itemIndex, const String& itemText,
                          bool isMouseOverItem, bool isMenuOpen, bool isMouseOverBar,
                          MenuBarComponent&) override;

	Component* getParentComponentForMenuOptions (const PopupMenu::Options& options) override;
    //==============================================================================
    void drawComboBox (Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox&) override;
    Font getComboBoxFont (ComboBox&) override;
    Label* createComboBoxTextBox (ComboBox&) override;
    void positionComboBoxText (ComboBox&, Label&) override;

    //==============================================================================
    void drawLabel (Graphics&, Label&) override;
    Font getLabelFont (Label&) override;

    //==============================================================================
    void drawLinearSlider (Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle, Slider&) override;

    void drawLinearSliderBackground (Graphics&, int x, int y, int width, int height,
                                     float sliderPos, float minSliderPos, float maxSliderPos,
                                     const Slider::SliderStyle, Slider&) override;

    void drawLinearSliderThumb (Graphics&, int x, int y, int width, int height,
                                float sliderPos, float minSliderPos, float maxSliderPos,
                                const Slider::SliderStyle, Slider&) override;

    void drawRotarySlider (Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                           Slider&) override;

    int getSliderThumbRadius (Slider&) override;
    Button* createSliderButton (Slider&, bool isIncrement) override;
    Label* createSliderTextBox (Slider&) override;
    ImageEffectFilter* getSliderEffect (Slider&) override;
    Font getSliderPopupFont (Slider&) override;
    int getSliderPopupPlacement (Slider&) override;
    Slider::SliderLayout getSliderLayout (Slider&) override;

    //==============================================================================
    Rectangle<int> getTooltipBounds (const String& tipText, Point<int> screenPos, Rectangle<int> parentArea) override;
    void drawTooltip (Graphics&, const String& text, int width, int height) override;

    //==============================================================================
    Button* createFilenameComponentBrowseButton (const String& text) override;
    void layoutFilenameComponent (FilenameComponent&, ComboBox* filenameBox, Button* browseButton) override;

    //==============================================================================
    void drawConcertinaPanelHeader (Graphics&, const Rectangle<int>& area,
                                    bool isMouseOver, bool isMouseDown,
                                    ConcertinaPanel&, Component& panel) override;

    //==============================================================================
    void drawCornerResizer (Graphics&, int w, int h, bool isMouseOver, bool isMouseDragging) override;
    void drawResizableFrame (Graphics&, int w, int h, const BorderSize<int>&) override;

    //==============================================================================
    void fillResizableWindowBackground (Graphics&, int w, int h, const BorderSize<int>&, ResizableWindow&) override;
    void drawResizableWindowBorder (Graphics&, int w, int h, const BorderSize<int>& border, ResizableWindow&) override;

    //==============================================================================
    void drawDocumentWindowTitleBar (DocumentWindow&, Graphics&, int w, int h,
                                     int titleSpaceX, int titleSpaceW,
                                     const Image* icon, bool drawTitleTextOnLeft) override;

    Button* createDocumentWindowButton (int buttonType) override;

    void positionDocumentWindowButtons (DocumentWindow&,
                                        int titleBarX, int titleBarY, int titleBarW, int titleBarH,
                                        Button* minimiseButton,
                                        Button* maximiseButton,
                                        Button* closeButton,
                                        bool positionTitleBarButtonsOnLeft) override;

    //==============================================================================
    DropShadower* createDropShadowerForComponent (Component*) override;

    //==============================================================================
    void drawStretchableLayoutResizerBar (Graphics&, int w, int h, bool isVerticalBar,
                                          bool isMouseOver, bool isMouseDragging) override;

    //==============================================================================
    void drawGroupComponentOutline (Graphics&, int w, int h, const String& text,
                                    const Justification&, GroupComponent&) override;

    //==============================================================================
    int getTabButtonSpaceAroundImage() override;
    int getTabButtonOverlap (int tabDepth) override;
    int getTabButtonBestWidth (TabBarButton&, int tabDepth) override;
    Rectangle<int> getTabButtonExtraComponentBounds (const TabBarButton&, Rectangle<int>& textArea, Component& extraComp) override;

    void drawTabButton (TabBarButton&, Graphics&, bool isMouseOver, bool isMouseDown) override;
    void drawTabButtonText (TabBarButton&, Graphics&, bool isMouseOver, bool isMouseDown) override;
    void drawTabbedButtonBarBackground (TabbedButtonBar&, Graphics&) override;
    void drawTabAreaBehindFrontButton (TabbedButtonBar&, Graphics&, int w, int h) override;

    void createTabButtonShape (TabBarButton&, Path&,  bool isMouseOver, bool isMouseDown) override;
    void fillTabButtonShape (TabBarButton&, Graphics&, const Path&, bool isMouseOver, bool isMouseDown) override;

    Button* createTabBarExtrasButton() override;

    //==============================================================================
    void drawImageButton (Graphics&, Image*,
                          int imageX, int imageY, int imageW, int imageH,
                          const Colour& overlayColour, float imageOpacity, ImageButton&) override;

    //==============================================================================
    void drawTableHeaderBackground (Graphics&, TableHeaderComponent&) override;

    void drawTableHeaderColumn (Graphics&, const String& columnName, int columnId,
                                int width, int height, bool isMouseOver, bool isMouseDown,
                                int columnFlags) override;

    //==============================================================================
    void paintToolbarBackground (Graphics&, int width, int height, Toolbar&) override;

    Button* createToolbarMissingItemsButton (Toolbar&) override;

    void paintToolbarButtonBackground (Graphics&, int width, int height,
                                       bool isMouseOver, bool isMouseDown,
                                       ToolbarItemComponent&) override;

    void paintToolbarButtonLabel (Graphics&, int x, int y, int width, int height,
                                  const String& text, ToolbarItemComponent&) override;

    //==============================================================================
    void drawPropertyPanelSectionHeader (Graphics&, const String& name, bool isOpen, int width, int height) override;
    void drawPropertyComponentBackground (Graphics&, int width, int height, PropertyComponent&) override;
    void drawPropertyComponentLabel (Graphics&, int width, int height, PropertyComponent&) override;
    Rectangle<int> getPropertyComponentContentPosition (PropertyComponent&) override;

    //==============================================================================
    void drawCallOutBoxBackground (CallOutBox&, Graphics&, const Path& path, Image& cachedImage) override;
    int getCallOutBoxBorderSize (const CallOutBox&) override;

    //==============================================================================
    void drawLevelMeter (Graphics&, int width, int height, float level) override;

    void drawKeymapChangeButton (Graphics&, int width, int height, Button&, const String& keyDescription) override;

    //==============================================================================
    /** Draws a 3D raised (or indented) bevel using two colours.

     The bevel is drawn inside the given rectangle, and greater bevel thicknesses
     extend inwards.

     The top-left colour is used for the top- and left-hand edges of the
     bevel; the bottom-right colour is used for the bottom- and right-hand
     edges.

     If useGradient is true, then the bevel fades out to make it look more curved
     and less angular. If sharpEdgeOnOutside is true, the outside of the bevel is
     sharp, and it fades towards the centre; if sharpEdgeOnOutside is false, then
     the centre edges are sharp and it fades towards the outside.
     */
    static void drawBevel (Graphics&,
                           int x, int y, int width, int height,
                           int bevelThickness,
                           const Colour& topLeftColour = Colours::white,
                           const Colour& bottomRightColour = Colours::black,
                           bool useGradient = true,
                           bool sharpEdgeOnOutside = true);

    /** Utility function to draw a shiny, glassy circle (for round LED-type buttons). */
    static void drawGlassSphere (Graphics&, float x, float y, float diameter,
                                 const Colour&, float outlineThickness) noexcept;

    static void drawGlassPointer (Graphics&, float x, float y, float diameter,
                                  const Colour&, float outlineThickness, int direction) noexcept;

    /** Utility function to draw a shiny, glassy oblong (for text buttons). */
    static void drawGlassLozenge (Graphics&,
                                  float x, float y, float width, float height,
                                  const Colour&, float outlineThickness, float cornerSize,
                                  bool flatOnLeft, bool flatOnRight, bool flatOnTop, bool flatOnBottom) noexcept;

private:
    //==============================================================================
    ScopedPointer<Drawable> folderImage, documentImage;

    void drawShinyButtonShape (Graphics&,
                               float x, float y, float w, float h, float maxCornerSize,
                               const Colour&, float strokeWidth,
                               bool flatOnLeft, bool flatOnRight, bool flatOnTop, bool flatOnBottom) noexcept;

    class GlassWindowButton;
    class SliderLabelComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LookAndFeelOO)

};

#endif  // LOOKANDFEELOO_H_INCLUDED
