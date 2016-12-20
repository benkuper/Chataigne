/*
  ==============================================================================

    ConsequenceUI.cpp
    Created: 28 Oct 2016 8:08:09pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConsequenceUI.h"
#include "CommandFactory.h"

ConsequenceUI::ConsequenceUI(Consequence * consequence) :
	BaseItemUI<Consequence>(consequence)
{
	autoSelectWithChildRespect = false;
	setSize(10, 40);
	removeMouseListener(this);
	item->addConsequenceListener(this);
	chooser.addChooserListener(this);
	addAndMakeVisible(&chooser);

	updateCommandUI();
}

ConsequenceUI::~ConsequenceUI()
{
	item->removeConsequenceListener(this);

}

void ConsequenceUI::resized()
{
	BaseItemUI::resized();
	Rectangle<int> r = getLocalBounds().reduced(2);
	r.removeFromTop(headerHeight + headerGap);

	Rectangle<int> sr = r.withHeight(headerHeight);
	chooser.setBounds(sr);

	r.translate(0, headerHeight);
	
	if (commandEditor != nullptr)
	{
		commandEditor->setBounds(r.withHeight(commandEditor->getHeight()));
		if (commandEditor->getBottom() != getHeight()) setSize(getWidth(), commandEditor->getBottom());
	}
	
}

void ConsequenceUI::updateChooserLabel()
{
	String text;
	if (item->command != nullptr)
		text = item->command->container->niceName + ":" + item->commandDefinition->commandType;

	chooser.setLabel(text);
}

void ConsequenceUI::updateCommandUI()
{
	if (commandEditor != nullptr) removeChildComponent(commandEditor);
	if (item->command != nullptr)
	{
		commandEditor = item->command->getEditor(false);
		addAndMakeVisible(commandEditor);
	}
	else
	{
		commandEditor = nullptr;
	}
	updateChooserLabel();

	resized();
}

void ConsequenceUI::definitionChosen(CommandDefinition * d)
{
	item->setCommand(d);
}

void ConsequenceUI::consequenceCommandChanged(Consequence *)
{
	updateCommandUI();
}
