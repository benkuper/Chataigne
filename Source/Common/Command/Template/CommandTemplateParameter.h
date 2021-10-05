/*
  ==============================================================================

	CommandTemplateParameter.h
	Created: 31 May 2018 11:27:54am
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class CommandTemplateParameter :
	public ControllableContainer
{
public:
	CommandTemplateParameter(Parameter* sourceParam);
	~CommandTemplateParameter();

	Parameter* parameter;
	BoolParameter* editable;

	InspectableEditor* getEditorInternal(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandTemplateParameter)
};