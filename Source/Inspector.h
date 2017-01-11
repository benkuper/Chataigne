/*
  ==============================================================================

    Inspector.h
    Created: 9 May 2016 6:41:38pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INSPECTOR_H_INCLUDED
#define INSPECTOR_H_INCLUDED

#include "ShapeShifterContent.h"
#include "InspectableEditor.h"

class Inspector :
	public Component,
	public Inspectable::InspectableListener
{
public:
	juce_DeclareSingleton(Inspector, false);
	Inspector();
	virtual ~Inspector();

	WeakReference<Inspectable> currentInspectable;
	Viewport vp;
	ScopedPointer<InspectableEditor> currentEditor;

	void resized() override;
	void setCurrentInspectable(WeakReference<Inspectable> inspectable);
	void clear();

	void inspectableDestroyed(Inspectable * inspectable);

	class  InspectorListener
	{
	public:
		virtual ~InspectorListener() {}
		virtual void currentInspectableChanged(Inspector *) {};
	};

	ListenerList<InspectorListener> listeners;
	void addInspectorListener(InspectorListener* newListener) { listeners.add(newListener); }
	void removeInspectorListener(InspectorListener* listener) { listeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Inspector)
};


class InspectorUI :
	public ShapeShifterContentComponent
{
public:
	InspectorUI() :
		ShapeShifterContentComponent("Inspector")
	{
		addAndMakeVisible(Inspector::getInstance());
	}

	~InspectorUI()
	{
		Inspector::deleteInstance();
	}

	void resized()
	{
		Inspector::getInstance()->setBounds(getLocalBounds());
	}
};

#endif  // INSPECTOR_H_INCLUDED
