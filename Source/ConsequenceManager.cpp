/*
  ==============================================================================

    ConsequenceManager.cpp
    Created: 28 Oct 2016 8:07:30pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConsequenceManager.h"
#include "ConsequenceManagerEditor.h"


juce_ImplementSingleton(ConsequenceManager)

ConsequenceManager::ConsequenceManager() :
	BaseManager<Consequence>("Consequences")
{
	selectItemWhenCreated = false;
	triggerAll = addTrigger("Trigger All", "Trigger all the consequences in the manager");
}

ConsequenceManager::~ConsequenceManager()
{
}

void ConsequenceManager::onContainerTriggerTriggered(Trigger * t)
{
	if (t == triggerAll)
	{
		for (auto &c : items) c->trigger->trigger();
	}
}

InspectableEditor * ConsequenceManager::getEditor(bool isRoot)
{
	return new ConsequenceManagerEditor(this, isRoot);
}
