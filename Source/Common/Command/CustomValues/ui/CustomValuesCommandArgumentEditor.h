/*
  ==============================================================================

    CustomValuesCommandArgumentEditor.h
    Created: 22 Feb 2017 8:53:11am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../CustomValuesCommandArgument.h"
#include "Common/ParameterLink/ui/LinkableParameterEditor.h"

class CustomValuesCommandArgumentEditor :
	public BaseItemEditor
{
public:
	CustomValuesCommandArgumentEditor(CustomValuesCommandArgument * a, bool isRoot);
	~CustomValuesCommandArgumentEditor();

	CustomValuesCommandArgument * arg;

	std::unique_ptr<InspectableEditor> paramUI;
	std::unique_ptr<BoolToggleUI> editableUI;

	void resizedInternalHeader(Rectangle<int> &r) override;
	void resizedInternalContent(Rectangle<int> &r) override;
	void childBoundsChanged(Component *) override;
};
