/*
  ==============================================================================

    CommandTemplateParameterEditor.cpp
    Created: 31 May 2018 11:29:46am
    Author:  Ben

  ==============================================================================
*/

#include "CommandTemplateParameterEditor.h"


CommandTemplateParameterEditor::CommandTemplateParameterEditor(CommandTemplateParameter * p, bool isRoot) :
	InspectableEditor(p, isRoot),
	parameter(p)
{
	parameterUI = parameter->parameter->getEditor(false);
	editableUI = parameter->editable->createToggle();
	addAndMakeVisible(parameterUI);
	addAndMakeVisible(editableUI);

	setSize(100, 16);
}

CommandTemplateParameterEditor::~CommandTemplateParameterEditor()
{
}

void CommandTemplateParameterEditor::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(1);
	editableUI->setBounds(r.removeFromRight(60));
	r.removeFromRight(10);
	parameterUI->setBounds(r);
}
