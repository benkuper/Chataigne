/*
  ==============================================================================

    ControllableEditor.h
    Created: 26 Mar 2016 10:13:46pm
    Author:  Martin Hermant

  ==============================================================================
*/
/*
#ifndef CONTROLLABLEEDITOR_H_INCLUDED
#define CONTROLLABLEEDITOR_H_INCLUDED



#include "ControllableContainer.h"
class ControllableUI;

//  base class for displaying an UI presenting all control available in ControllableContainer

class ControllableContainerEditor: public Component, public ControllableContainerListener
{
public:
    ControllableContainerEditor(ControllableContainer * ,Component*);

    virtual ~ControllableContainerEditor();

    WeakReference<ControllableContainer> owner;


protected:

    void buildFromContainer(ControllableContainer * , bool recursive = true);
    void addControlUI(ControllableUI * c);
    void removeControlUI(ControllableUI * c);

    void childrenChanged() override;
    void childBoundsChanged(Component *) override;

    virtual void controllableAdded(Controllable * ) override ;
    virtual void controllableRemoved(Controllable * ) override ;
    virtual void controllableContainerAdded(ControllableContainer *) override ;
    virtual void controllableContainerRemoved(ControllableContainer * ) override ;

    void resized() override;

    OwnedArray<ControllableUI> controllableUIs;
    OwnedArray<ControllableContainerEditor> editors;

    Component* embeddedComp;
    void layoutChildrens();

};



#endif  // CONTROLLABLEEDITOR_H_INCLUDED
*/