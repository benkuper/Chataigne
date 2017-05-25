/*
  ==============================================================================

    CustomOSCModuleEditor.h
    Created: 27 Dec 2016 1:54:47pm
    Author:  Ben

  ==============================================================================
*/

#ifndef CUSTOMOSCMODULEEDITOR_H_INCLUDED
#define CUSTOMOSCMODULEEDITOR_H_INCLUDED

#include "OSCModuleBaseEditor.h"
#include "CustomOSCModule.h"

class CustomOSCModuleEditor :
	public OSCModuleBaseEditor
{
public:
	CustomOSCModuleEditor(CustomOSCModule * cModule, bool isRoot);
	~CustomOSCModuleEditor();

	CustomOSCModule * cModule;

	ScopedPointer<BoolToggleUI> autoAddUI;
	ScopedPointer<BoolToggleUI> autoRangeUI;
	ScopedPointer<BoolToggleUI> splitArgsUI;
	ScopedPointer<InspectableEditor> valuesEditor;

	void resizedOSCInternalContent(Rectangle<int> &r) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCModuleEditor)
};




#endif  // CUSTOMOSCMODULEEDITOR_H_INCLUDED
