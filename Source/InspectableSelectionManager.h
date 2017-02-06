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
	WeakReference<Inspectable> currentInspectable;
	
	void setEnabled(bool value);

	void setCurrentInspectable(WeakReference<Inspectable> inspectable);
	void clearSelection();

	//From InspectableListener
	void inspectableDestroyed(Inspectable * inspectable);

	class  Listener
	{
	public:
		virtual ~Listener() {}
		virtual void currentInspectableSelectionChanged(Inspectable * /*oldInspectable*/, Inspectable * /*newInspectable*/) {};
	};

	ListenerList<Listener> listeners;
	void addSelectionListener(Listener* newListener) { listeners.add(newListener); }
	void removeSelectionListener(Listener* listener) { listeners.remove(listener); }
};



#endif  // INSPECTABLESELECTIONMANAGER_H_INCLUDED
