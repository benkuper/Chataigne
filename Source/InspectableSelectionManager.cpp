/*
  ==============================================================================

    InspectableSelectionManager.cpp
    Created: 6 Feb 2017 12:40:22pm
    Author:  Ben

  ==============================================================================
*/

#include "InspectableSelectionManager.h"

juce_ImplementSingleton(InspectableSelectionManager)

InspectableSelectionManager::InspectableSelectionManager()
{
}

InspectableSelectionManager::~InspectableSelectionManager()
{
	clearSelection();
}

void InspectableSelectionManager::setEnabled(bool value)
{
	enabled = value;
	if (!enabled) clearSelection();
}

void InspectableSelectionManager::setCurrentInspectable(WeakReference<Inspectable> inspectable)
{
	if (!enabled) return;

	if (inspectable == currentInspectable)
	{
		return;
	}

	Inspectable * oldInspectable = currentInspectable;

	if (currentInspectable != nullptr)
	{
		if (!currentInspectable.wasObjectDeleted())
		{
			currentInspectable->removeInspectableListener(this);
			currentInspectable->setSelected(false);
		}
	}

	currentInspectable = inspectable;

	if (currentInspectable.get() != nullptr)
	{
		currentInspectable->setSelected(true);
		currentInspectable->addInspectableListener(this);
	}

	listeners.call(&Listener::currentInspectableSelectionChanged, oldInspectable, currentInspectable);
}

void InspectableSelectionManager::clearSelection()
{
	setCurrentInspectable(nullptr);
}

void InspectableSelectionManager::inspectableDestroyed(Inspectable * i)
{
	if (currentInspectable == i) setCurrentInspectable(nullptr);
}
