/*
  ==============================================================================

    BaseCommandHandlerEditor.cpp
    Created: 19 Jan 2017 6:41:22pm
    Author:  Ben

  ==============================================================================
*/

#include "BaseCommandHandlerEditor.h"


#include "BaseCommandHandlerEditor.h"
#include "CommandFactory.h"

BaseCommandHandlerEditor::BaseCommandHandlerEditor(BaseCommandHandler * _handler, bool isRoot) :
	BaseItemEditor(_handler, isRoot),
	handler(_handler),
	chooser(_handler->context)
{
	
	chooser.addChooserListener(this);
	addAndMakeVisible(&chooser);

	handler->adCommandHandlerListener(this);

	updateCommandUI();
	setSize(10, 40);

}

BaseCommandHandlerEditor::~BaseCommandHandlerEditor()
{
	handler->removeCommandHandlerListener(this);
}


void BaseCommandHandlerEditor::resizedInternalContent(Rectangle<int>& r)
{
	Rectangle<int> sr = r.withHeight(headerHeight);
	chooser.setBounds(sr);

	r.translate(0, headerHeight);

	if (commandEditor != nullptr)
	{
		r.setHeight(commandEditor->getHeight());
		commandEditor->setBounds(r);
	}
}

void BaseCommandHandlerEditor::updateChooserLabel()
{
	String text;
	if (handler->command != nullptr)
		text = handler->command->container->niceName + ":" + handler->commandDefinition->commandType;

	chooser.setLabel(text);
}

void BaseCommandHandlerEditor::updateCommandUI()
{
	if (commandEditor != nullptr) removeChildComponent(commandEditor);
	if (handler->command != nullptr)
	{
		commandEditor = handler->command->getEditor(false);
		addAndMakeVisible(commandEditor);
	} else
	{
		commandEditor = nullptr;
	}
	updateChooserLabel();

	resized();
}

void BaseCommandHandlerEditor::definitionChosen(CommandDefinition * d)
{
	handler->setCommand(d);
}

void BaseCommandHandlerEditor::commandChanged(BaseCommandHandler *)
{
	updateCommandUI();

}
