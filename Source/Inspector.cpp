/*
  ==============================================================================

    Inspector.cpp
    Created: 9 May 2016 6:41:38pm
    Author:  bkupe

  ==============================================================================
*/

#include "Inspector.h"

Inspector::Inspector(bool isMain) :
isMainInspector(isMain),
currentInspectable(nullptr),
	currentEditor(nullptr)
{
	
	if (InspectableSelectionManager::getInstanceWithoutCreating() != nullptr) InspectableSelectionManager::getInstance()->addSelectionListener(this);

	vp.setScrollBarsShown(true, false);
	vp.setScrollOnDragEnabled(false);
	vp.setScrollBarThickness(10);
	addAndMakeVisible(vp);

	resized();
}


Inspector::~Inspector()
{
	if (InspectableSelectionManager::getInstanceWithoutCreating() != nullptr) InspectableSelectionManager::getInstance()->removeSelectionListener(this); 
	clear();
}

void Inspector::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(3);
	vp.setBounds(r);
	r.removeFromRight(10);

	if (currentEditor != nullptr)
	{
		if (!currentEditor->fitToContent) r.setHeight(currentEditor->getHeight());
		currentEditor->setSize(r.getWidth(), r.getHeight());
	}
}

void Inspector::setCurrentInspectable(WeakReference<Inspectable> inspectable)
{
	if (!isEnabled()) return;

	if (inspectable == currentInspectable)
	{
		return;
	}

	if (currentInspectable != nullptr)
	{
		if (!currentInspectable.wasObjectDeleted())
		{
			currentInspectable->removeInspectableListener(this); 
			currentInspectable->setSelected(false);
		}

		if (currentEditor != nullptr)
		{
			vp.setViewedComponent(nullptr);
			currentEditor = nullptr;
		}
	}
	currentInspectable = inspectable;

	if (currentInspectable.get() != nullptr)
	{
		currentInspectable->setSelected(true);
		currentInspectable->addInspectableListener(this);
		currentEditor = currentInspectable->getEditor(true);
	}

	vp.setViewedComponent(currentEditor, false);
	resized();

	listeners.call(&InspectorListener::currentInspectableChanged, this);
}


void Inspector::clear()
{
	setCurrentInspectable(nullptr);
}

void Inspector::inspectableDestroyed(Inspectable * i)
{
	if (currentInspectable == i) setCurrentInspectable(nullptr);
}

void Inspector::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::getInstance()->isEmpty())
	{
		setCurrentInspectable(nullptr);
		return;
	}

	Inspectable * newI = InspectableSelectionManager::getInstance()->currentInspectables[0];
	if (!newI->showInspectorOnSelect) return;
	setCurrentInspectable(newI);
}

InspectorUI::InspectorUI(bool isMainInspector) :
	ShapeShifterContentComponent("Inspector"),
	inspector(isMainInspector)
{
	addAndMakeVisible(&inspector);
}

InspectorUI::~InspectorUI()
{
}

void InspectorUI::resized()
{
	inspector.setBounds(getLocalBounds());
}
