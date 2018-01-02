/*
  ==============================================================================

    CustomOSCCommandArgumentEditor.h
    Created: 22 Feb 2017 8:53:11am
    Author:  Ben

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMANDARGUMENTEDITOR_H_INCLUDED
#define CUSTOMOSCCOMMANDARGUMENTEDITOR_H_INCLUDED


#include "CustomOSCCommandArgument.h"



class CustomOSCCommandArgumentEditor :
	public BaseItemEditor
{
public:
	CustomOSCCommandArgumentEditor(CustomOSCCommandArgument * a, bool isRoot);
	~CustomOSCCommandArgumentEditor();

	CustomOSCCommandArgument * arg;

	ScopedPointer<ParameterEditor> paramUI;
	ScopedPointer<BoolToggleUI> useInMappingUI;

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
	void childBoundsChanged(Component *) override;
};



#endif  // CUSTOMOSCCOMMANDARGUMENTEDITOR_H_INCLUDED
