/*
  ==============================================================================

    CustomOSCCommandArgumentEditor.h
    Created: 22 Feb 2017 8:53:11am
    Author:  Ben

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMANDARGUMENTEDITOR_H_INCLUDED
#define CUSTOMOSCCOMMANDARGUMENTEDITOR_H_INCLUDED

#include "BaseItemEditor.h"
#include "CustomOSCCommandArgument.h"
#include "ControllableEditor.h"


class CustomOSCCommandArgumentEditor :
	public BaseItemEditor
{
public:
	CustomOSCCommandArgumentEditor(CustomOSCCommandArgument * a, bool isRoot);
	~CustomOSCCommandArgumentEditor();

	CustomOSCCommandArgument * arg;

	ScopedPointer<ControllableEditor> paramUI;
	ScopedPointer<BoolToggleUI> useInMappingUI;

	void resizedInternalHeader(Rectangle<int> &r) override;
};



#endif  // CUSTOMOSCCOMMANDARGUMENTEDITOR_H_INCLUDED
