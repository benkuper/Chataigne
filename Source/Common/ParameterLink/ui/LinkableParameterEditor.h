/*
  ==============================================================================

    LinkableParameterEditor.h
    Created: 21 Dec 2020 11:12:32pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../ParameterLink.h"

class LinkableParameterEditor :
    public InspectableEditor,
    public Button::Listener
{
public:
    LinkableParameterEditor(ParameterLink* pLink, bool showMappingOptions);
    ~LinkableParameterEditor();

    static Image linkOnImage;
    static Image linkOffImage;

    bool showMappingOptions;

    ParameterLink* link;
    std::unique_ptr<ParameterEditor> paramEditor;
    std::unique_ptr<ImageButton> linkBT;

    void resized() override;
    void buttonClicked(Button* b) override;
    void childBoundsChanged(Component* c) override;

};