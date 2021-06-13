/*
  ==============================================================================

    BaseCommandHandlerEditor.cpp
    Created: 19 Jan 2017 6:41:22pm
    Author:  Ben

  ==============================================================================
*/

#include "BaseCommandHandlerEditor.h"
#include "../CommandFactory.h"

BaseCommandHandlerEditor::BaseCommandHandlerEditor(BaseCommandHandler * _handler, bool isRoot) :
	BaseItemEditor(_handler, isRoot),
	handler(_handler),
	chooser(_handler->context, _handler->isMultiplexed())
{	
	chooser.addChooserListener(this);
	chooser.lockedModule = handler->lockedModule;

	addAndMakeVisible(&chooser);
	handler->addAsyncCommandHandlerListener(this);

	if (handler->trigger != nullptr)
	{
		triggerBT.reset(handler->trigger->createButtonUI());
		addAndMakeVisible(triggerBT.get());
	}

	setSize(10, 40);
	
	updateChooserLabel();

	resetAndBuild();
	resized();

}

BaseCommandHandlerEditor::~BaseCommandHandlerEditor()
{
	if(!inspectable.wasObjectDeleted()) handler->removeAsyncCommandHandlerListener(this);
}

void BaseCommandHandlerEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	if(triggerBT != nullptr) triggerBT->setBounds(r.removeFromRight(50).reduced(2));
	chooser.setBounds(r.removeFromRight(chooser.label.getFont().getStringWidth(chooser.label.getText()) + 50));
}

void BaseCommandHandlerEditor::updateChooserLabel()
{
	if (inspectable.wasObjectDeleted()) return;

	String t = "";
	bool ghostMode = false;
	if (handler->command != nullptr || handler->commandDefinition != nullptr)
	{
		t = handler->command->module->niceName + " > " + handler->commandDefinition->commandType;
	}
	else if (handler->ghostCommandName.isNotEmpty())
	{
		ghostMode = true;
		t = "### " + handler->ghostModuleName + " > " + handler->ghostCommandName;
	}

	chooser.setLabel(t, ghostMode);
	chooser.repaint();
}

void BaseCommandHandlerEditor::definitionChosen(CommandDefinition * d)
{
	handler->setCommand(d);
}

void BaseCommandHandlerEditor::newMessage(const BaseCommandHandler::CommandHandlerEvent & e)
{
	if (e.type == BaseCommandHandler::CommandHandlerEvent::COMMAND_CHANGED)
	{
		updateChooserLabel();
		resized();
	}
}
