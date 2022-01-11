/*
  ==============================================================================

    CommandTemplateParameterEditor.cpp
    Created: 31 May 2018 11:29:46am
    Author:  Ben

  ==============================================================================
*/

CommandTemplateParameterEditor::CommandTemplateParameterEditor(CommandTemplateParameter * p, bool isRoot) :
	InspectableEditor(p, isRoot),
	parameter(p)
{
	parameterUI.reset(parameter->parameter->getEditor(false));
	editableUI.reset(parameter->editable->createToggle());
	addAndMakeVisible(parameterUI.get());
	addAndMakeVisible(editableUI.get());

	setSize(200, parameterUI->getHeight());
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
