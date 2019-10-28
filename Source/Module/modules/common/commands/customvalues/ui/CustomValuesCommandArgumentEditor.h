/*
  ==============================================================================

    CustomValuesCommandArgumentEditor.h
    Created: 22 Feb 2017 8:53:11am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../CustomValuesCommandArgument.h"

class CustomValuesCommandArgumentEditor :
	public BaseItemEditor
{
public:
	CustomValuesCommandArgumentEditor(CustomValuesCommandArgument * a, bool isRoot);
	~CustomValuesCommandArgumentEditor();

	CustomValuesCommandArgument * arg;

	std::unique_ptr<ParameterEditor> paramUI;
	std::unique_ptr<BoolToggleUI> useInMappingUI;
	std::unique_ptr<BoolToggleUI> editableUI;

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
	void resizedInternalContent(Rectangle<int> &r) override;
	void childBoundsChanged(Component *) override;
};
