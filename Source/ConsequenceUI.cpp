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

	updateChooserLabel();
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

	/*
	if (commandUI != nullptr)
	{
		commandUI->setBounds(r.withHeight(commandUI->getHeight()));
		if (commandUI->getBottom() != getHeight()) setSize(getWidth(), commandUI->getBottom());
	}
	*/
}

void ConsequenceUI::updateChooserLabel()
{
	String text;
	if (item->command != nullptr)
		text = item->command->container->niceName + ":" + item->commandDefinition->commandType;

	chooser.setLabel(text);
}

void ConsequenceUI::definitionChosen(CommandDefinition * d)
{
	item->setCommand(d);
}

void ConsequenceUI::consequenceCommandChanged(Consequence *)
{
	updateChooserLabel();
}
