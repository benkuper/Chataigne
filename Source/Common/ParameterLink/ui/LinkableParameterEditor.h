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
    public Button::Listener,
    public ParameterLink::AsyncListener
{
public:
    LinkableParameterEditor(Array<ParameterLink*> pLinks, bool showMappingOptions);
    ~LinkableParameterEditor();

    static Image linkImage;

    bool showMappingOptions;


    Array<ParameterLink*> links;
    ParameterLink* link;
    std::unique_ptr<ParameterEditor> paramEditor;
    std::unique_ptr<ImageButton> linkBT;

    Rectangle<int> btRect;

    void paint(Graphics& g) override;
    void resized() override;
    void buttonClicked(Button* b) override;
    void childBoundsChanged(Component* c) override;

    String getLinkLabel() const;

    template<class T>
    static Array<T*> getLinksAs(Array<ParameterLink*> links)
    {
        Array<T*> result;
        for (auto& l : links) result.add(l->parameter);
        return result;
    }

    void newMessage(const ParameterLink::ParameterLinkEvent& e) override;

};

class ParamLinkContainerEditor :
    public GenericControllableContainerEditor
{
public:
    ParamLinkContainerEditor(ParamLinkContainer* container, bool isRoot, bool buildAtCreation = true);
    ~ParamLinkContainerEditor();

    ParamLinkContainer* paramLinkContainer;
    bool showLinkEditor;
    bool showMappingOptions;

    virtual InspectableEditor* getEditorUIForControllable(Controllable* c) override;
};
