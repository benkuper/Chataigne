/*
  ==============================================================================

    CustomEditor.h
    Created: 9 May 2016 6:42:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CUSTOMEDITOR_H_INCLUDED
#define CUSTOMEDITOR_H_INCLUDED

#include "InspectorEditor.h"



class CustomEditor : public InspectorEditor
{
public:
	CustomEditor(InspectableComponent * sourceComponent);
	virtual ~CustomEditor();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomEditor)
};



#endif  // CUSTOMEDITOR_H_INCLUDED
