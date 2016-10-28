/*
  ==============================================================================

    ControllableEditor.cpp
    Created: 26 Mar 2016 10:13:46pm
    Author:  Martin Hermant

  ==============================================================================
*/

#include "ControllableContainerEditor.h"


#include "ControllableUI.h"
#include "ControllableContainer.h"

ControllableContainerEditor::ControllableContainerEditor(ControllableContainer * cc, Component* _embeddedComp):
owner(cc),
embeddedComp(_embeddedComp)
{

    if(embeddedComp){
        addAndMakeVisible(embeddedComp);
        //setBounds(embeddedComp->getBounds());
	}
	else
	{
		buildFromContainer(cc);
	}

    cc->addControllableContainerListener(this);
}

ControllableContainerEditor::~ControllableContainerEditor(){
    if(owner.get())owner->removeControllableContainerListener(this);
}


void ControllableContainerEditor::addControlUI(ControllableUI * c){
    controllableUIs.add(c);
    addAndMakeVisible(c);
}
void ControllableContainerEditor::removeControlUI(ControllableUI * c){
    removeChildComponent(c);
    controllableUIs.removeObject(c);

}


void ControllableContainerEditor::childBoundsChanged(Component * ){
	layoutChildrens();
}

void ControllableContainerEditor::childrenChanged(){layoutChildrens();};


void ControllableContainerEditor::layoutChildrens(){
    int y = 0;
    int pad = 3;
    for(int i = 0 ; i < getNumChildComponents() ; i ++){
        Component * ch= getChildComponent(i);
        ch->setTopLeftPosition(0, y);
        y+=ch->getHeight()+pad;

    }
    setSize(getWidth(),y);
}

void ControllableContainerEditor::resized(){
	Rectangle<int> r = getLocalBounds().reduced(5,0);

    if(embeddedComp){
		embeddedComp->setBounds(r);
	}

    for(int i = 0 ;i < getNumChildComponents() ; i++){
        getChildComponent(i)->setSize(getWidth(), getChildComponent(i)->getHeight());
    }
}


void ControllableContainerEditor::buildFromContainer(ControllableContainer * cc, bool recursive){
    for(auto & c : cc->controllables)
	{
		if (!c->isControllableExposed || c->isControllableFeedbackOnly) continue;
        ControllableUI *cUI= new NamedControllableUI(c->createDefaultUI(),100);
        addControlUI(cUI);
    }

	if(recursive)
	{
		for(auto & c:cc->controllableContainers){
			ControllableContainerEditor * cEd = new ControllableContainerEditor(c,nullptr);
			addAndMakeVisible(cEd);
			editors.add(cEd);
		}
	}
}

void ControllableContainerEditor::controllableAdded(Controllable * c) {
    ControllableUI *cUI= new NamedControllableUI(c->createDefaultUI(),100);
    addControlUI(cUI);

};
void ControllableContainerEditor::controllableRemoved(Controllable * c) {
    for(auto & cc:controllableUIs){
        if(cc->controllable==c){
            removeChildComponent(cc);
        }
    }

};
void ControllableContainerEditor::controllableContainerAdded(ControllableContainer * c) {
    ControllableContainerEditor * cEd = new ControllableContainerEditor(c,nullptr);
    addAndMakeVisible(cEd);
    editors.add(cEd);

};
void ControllableContainerEditor::controllableContainerRemoved(ControllableContainer * c) {
    for(auto & cc:editors){
        if(cc->owner==c){
            removeChildComponent(cc);
        }
    }
};
