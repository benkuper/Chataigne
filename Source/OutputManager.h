/*
  ==============================================================================

    OutputManager.h
    Created: 28 Oct 2016 8:02:49pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef OUTPUTMANAGER_H_INCLUDED
#define OUTPUTMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "Output.h"
#include "CommandFactory.h"

class OutputManager :
	public BaseManager<Output>
{
public:
	juce_DeclareSingleton(OutputManager, false)

	OutputManager();
	~OutputManager();

	virtual void addItemFromData(var data) override;


	PopupMenu getAllOutputsCommandMenu();
	CommandDefinition * getCommandDefinitionForItemID(int id);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputManager)
};

#endif  // OUTPUTMANAGER_H_INCLUDED
