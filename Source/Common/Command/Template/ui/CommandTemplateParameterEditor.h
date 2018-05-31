/*
  ==============================================================================

    CommandTemplateParameterEditor.h
    Created: 31 May 2018 11:29:46am
    Author:  Ben

  ==============================================================================
*/

#pragma once


#include "../CommandTemplateParameter.h"

class CommandTemplateParameterEditor :
	public InspectableEditor
{
public:
	CommandTemplateParameterEditor(CommandTemplateParameter * p, bool isRoot);
	~CommandTemplateParameterEditor();

	CommandTemplateParameter * parameter;
	ScopedPointer<InspectableEditor> parameterUI;
	ScopedPointer<BoolToggleUI> editableUI;

	void resized() override;
};
