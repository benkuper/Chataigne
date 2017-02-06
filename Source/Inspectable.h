/*
  ==============================================================================

    Inspectable.h
    Created: 30 Oct 2016 9:02:24am
    Author:  bkupe

  ==============================================================================
*/

#ifndef INSPECTABLE_H_INCLUDED
#define INSPECTABLE_H_INCLUDED

#include "JuceHeader.h"

class InspectableEditor;
class Inspector;

class Inspectable
{
public:
	Inspectable(const String &_inspectableType = "none");
	virtual ~Inspectable();

	Inspector * targetInspector;
	const String inspectableType;

	bool isSelected;
	bool isSelectable;
	bool showInspectorOnSelect;

	virtual void selectThis();
	virtual void setSelected(bool value);

	virtual void setSelectedInternal(bool value); //to be overriden

	virtual InspectableEditor * getEditor(bool /*isRoot*/) { jassert(false);  return nullptr; } //to override !

	//Listener
	class  InspectableListener
	{
	public:
		/** Destructor. */
		virtual ~InspectableListener() {}
		virtual void inspectableSelectionChanged(Inspectable *) {};
		virtual void inspectableDestroyed(Inspectable *) {};
	};

	ListenerList<InspectableListener> listeners;
	void addInspectableListener(InspectableListener* newListener) { listeners.add(newListener); }
	void removeInspectableListener(InspectableListener* listener) { listeners.remove(listener); }


	WeakReference<Inspectable>::Master masterReference;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Inspectable)
};



#endif  // INSPECTABLE_H_INCLUDED
