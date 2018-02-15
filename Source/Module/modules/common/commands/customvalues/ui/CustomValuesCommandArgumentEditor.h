/*
  ==============================================================================

    CustomValuesCommandArgumentEditor.h
    Created: 22 Feb 2017 8:53:11am
    Author:  Ben

  ==============================================================================
*/

#ifndef CustomValuesCommandARGUMENTEDITOR_H_INCLUDED
#define CustomValuesCommandARGUMENTEDITOR_H_INCLUDED


#include "../CustomValuesCommandArgument.h"

class CustomValuesCommandArgumentEditor :
	public BaseItemEditor
{
public:
	CustomValuesCommandArgumentEditor(CustomValuesCommandArgument * a, bool isRoot);
	~CustomValuesCommandArgumentEditor();

	CustomValuesCommandArgument * arg;

	ScopedPointer<ParameterEditor> paramUI;
	ScopedPointer<BoolToggleUI> useInMappingUI;

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
	void childBoundsChanged(Component *) override;
};



#endif  // CustomValuesCommandARGUMENTEDITOR_H_INCLUDED
