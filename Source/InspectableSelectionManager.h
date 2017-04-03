/*
  ==============================================================================

    InspectableSelectionManager.h
    Created: 6 Feb 2017 12:40:22pm
    Author:  Ben

  ==============================================================================
*/

#ifndef INSPECTABLESELECTIONMANAGER_H_INCLUDED
#define INSPECTABLESELECTIONMANAGER_H_INCLUDED

#include "Inspectable.h"

class InspectableSelectionManager :
	public Inspectable::InspectableListener
{
public:
	juce_DeclareSingleton(InspectableSelectionManager, true);
	InspectableSelectionManager();
	~InspectableSelectionManager();

	bool enabled;
	Array<Inspectable *> currentInspectables;
	
	void setEnabled(bool value);

	void selectInspectables(Array<Inspectable *> inspectables, bool clearSelection = true, bool notify = true);
	void selectInspectable(WeakReference<Inspectable> inspectable, bool clearSelection = true, bool notify = true);
	void deselectInspectable(WeakReference<Inspectable> inspectable, bool notify = true);

	void clearSelection(bool notify = true);

	bool isEmpty();

	//From InspectableListener
	void inspectableDestroyed(Inspectable * inspectable);

	class  Listener
	{
	public:
		virtual ~Listener() {}
		virtual void inspectablesSelectionChanged() {};
	};

	ListenerList<Listener> listeners;
	void addSelectionListener(Listener* newListener) { listeners.add(newListener); }
	void removeSelectionListener(Listener* listener) { listeners.remove(listener); }
};



#endif  // INSPECTABLESELECTIONMANAGER_H_INCLUDED
