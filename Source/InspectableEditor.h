/*
  ==============================================================================

    InspectableEditor.h
    Created: 9 May 2016 6:42:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INSPECTABLEEDITOR_H_INCLUDED
#define INSPECTABLEEDITOR_H_INCLUDED

#include "JuceHeader.h"
class Inspectable;

class InspectableEditor : 
	public Component
{
public:
	InspectableEditor(WeakReference<Inspectable> inspectable);
	virtual ~InspectableEditor();

	bool fitToContent; 

	WeakReference<Inspectable> inspectable;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectableEditor)
};



#endif  // INSPECTABLEEDITOR_H_INCLUDED
