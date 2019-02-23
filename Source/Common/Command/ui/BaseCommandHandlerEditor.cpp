/*
  ==============================================================================

    BaseCommandHandlerEditor.cpp
    Created: 19 Jan 2017 6:41:22pm
    Author:  Ben

  ==============================================================================
*/

#include "BaseCommandHandlerEditor.h"


#include "BaseCommandHandlerEditor.h"
#include "../CommandFactory.h"

BaseCommandHandlerEditor::BaseCommandHandlerEditor(BaseCommandHandler * _handler, bool isRoot) :
	BaseItemEditor(_handler, isRoot),
	handler(_handler),
	chooser(_handler->context)
{	
	chooser.addChooserListener(this);
	chooser.lockedModule = handler->lockedModule;

	addAndMakeVisible(&chooser);
	handler->addAsyncCommandHandlerListener(this);

	triggerBT = handler->trigger->createButtonUI();
	addAndMakeVisible(triggerBT);
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
	triggerBT->setBounds(r.removeFromRight(60).reduced(2));
	chooser.setBounds(r.removeFromRight(200));
}

void BaseCommandHandlerEditor::updateChooserLabel()
{
	if (inspectable.wasObjectDeleted()) return;
	if (handler->command == nullptr || handler->commandDefinition == nullptr) return;

	chooser.setLabel(handler->command->module->niceName + ":" + handler->commandDefinition->commandType);
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
