/*
  ==============================================================================

    GenericControllableContainerEditor.cpp
    Created: 9 May 2016 6:41:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "GenericControllableContainerEditor.h"
#include "InspectableComponent.h"
#include "ControllableUI.h"

//ControllableUIComparator CCInnerContainer::comparator;

GenericControllableContainerEditor::GenericControllableContainerEditor(WeakReference<Inspectable> inspectable, bool isRoot) :
	InspectableEditor(inspectable, isRoot),
	headerHeight(12),
	containerLabel("containerLabel", dynamic_cast<ControllableContainer *>(inspectable.get())->niceName),
container(dynamic_cast<ControllableContainer *>(inspectable.get()))
{
	container->addAsyncContainerListener(this);

	addAndMakeVisible(containerLabel);
	containerLabel.setFont(containerLabel.getFont().withHeight(10));
	containerLabel.setColour(containerLabel.backgroundColourId, BG_COLOR.brighter(.2f));
	containerLabel.setSize(containerLabel.getFont().getStringWidth(containerLabel.getText()) + 10, 14);
	containerLabel.setColour(containerLabel.textColourId, TEXTNAME_COLOR);
	containerLabel.setInterceptsMouseClicks(false, false);

	if (container->canHavePresets)
	{
		presetChooser = new PresetChooser(container);
		addAndMakeVisible(presetChooser);
	}

	resetAndBuild();
}

GenericControllableContainerEditor::~GenericControllableContainerEditor()
{
	if (!inspectable.wasObjectDeleted())
	{
		if(container != nullptr) container->removeAsyncContainerListener(this);
		clear();
	}
	
}

void GenericControllableContainerEditor::clear()
{
	//
}

void GenericControllableContainerEditor::resetAndBuild()
{
	//DBG("Reset and build " << container->niceName << "/ " << container->controllableContainers.size() << " / " << (int)(container->canInspectChildContainers));

	clear();
	if (container->hideInEditor) return;

	for (auto &c : container->controllables)
	{
		if (!c->hideInEditor) addControllableUI(c);
	}

	if (container->canInspectChildContainers)
	{
		for (auto &cc : container->controllableContainers)
		{
			//DBG("CC hide ? " << (int)(cc->hideInEditor));
			if(!cc->hideInEditor) addEditorUI(cc);
		}
	}
}


void GenericControllableContainerEditor::addEditorUI(ControllableContainer * cc, bool resize)
{
	InspectableEditor * ccui = cc->getEditor(false);
	childEditors.add(ccui);
	addAndMakeVisible(ccui);
	if (resize) resized();
}

void GenericControllableContainerEditor::removeEditorUI(ControllableContainer * cc, bool resize)
{
	InspectableEditor * ccui = getEditorForInspectable(cc);
	if (ccui == nullptr) return;

	removeChildComponent(ccui);
	childEditors.removeObject(ccui);
	if (resize) resized();
}

InspectableEditor * GenericControllableContainerEditor::getEditorForInspectable(Inspectable * i)
{
	for (auto &cui : childEditors)
	{
		if (cui->inspectable == i) return cui;
	}

	return nullptr;
}

void GenericControllableContainerEditor::addControllableUI(Controllable * c, bool resize)
{
	if (!c->isControllableExposed || c->hideInEditor) return;

	InspectableEditor * cui = c->getEditor(false);
	childEditors.add(cui);
	addAndMakeVisible(cui);
	if (resize) resized();
}

void GenericControllableContainerEditor::removeControllableUI(Controllable * c, bool resize)
{
	InspectableEditor * cui = getEditorForInspectable(c);
	if (cui == nullptr) return;

	removeChildComponent(cui);
	childEditors.removeObject(cui);
	if (resize) resized();

}

void GenericControllableContainerEditor::newMessage(const ContainerAsyncEvent & p)
{
	switch (p.type)
	{
	case ContainerAsyncEvent::ControllableAdded:
		if (p.targetControllable->parentContainer != container) return;
		if (p.targetControllable->hideInEditor) return;
		addControllableUI(p.targetControllable,true);
		break;

	case ContainerAsyncEvent::ControllableRemoved:
		removeControllableUI(p.targetControllable,true);
		break;

	case ContainerAsyncEvent::ControllableContainerAdded:
		if (p.targetContainer->parentContainer != container) return;

		if (container->canInspectChildContainers) addEditorUI(p.targetContainer,true);
		break;

	case ContainerAsyncEvent::ControllableContainerRemoved:
		removeEditorUI(p.targetContainer,true);
		break;

	case ContainerAsyncEvent::ChildStructureChanged:
		//nothing ?
		break;

	case ContainerAsyncEvent::ControllableContainerReordered:
		//resized();
		break;

	case ContainerAsyncEvent::ControllableFeedbackUpdate:
		controllableFeedbackUpdate(p.targetControllable);
		break;

    default:
    //not handled
    break;

	}
}

void GenericControllableContainerEditor::childBoundsChanged(Component *)
{
	resized();
}

void GenericControllableContainerEditor::paint(Graphics & g)
{
	g.setColour(BG_COLOR.brighter(.3f));
	g.drawRoundedRectangle(getLocalBounds().toFloat(),4,2);
}

void GenericControllableContainerEditor::resized()
{
	containerLabel.setBounds(getLocalBounds().removeFromTop(containerLabel.getHeight()).withSizeKeepingCentre(containerLabel.getWidth(), containerLabel.getHeight()));
	Rectangle<int> r = getLocalBounds().reduced(5).translated(0,headerHeight);

	if (container->canHavePresets)
	{
		presetChooser->setBounds(r.withHeight(16));
		r.translate(0, 18);
	}

	for (auto &cui : childEditors)
	{
		int th = cui->getHeight();
		cui->setBounds(r.withHeight(th));
		r.translate(0, th + 2);
	}

	setSize(getWidth(),jmax<int>(r.getY()+2,50));
}
