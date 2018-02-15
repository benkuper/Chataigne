/*
  ==============================================================================

    ConsequenceManager.cpp
    Created: 28 Oct 2016 8:07:30pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConsequenceManager.h"
#include "Common/Command/ui/BaseCommandHandlerManagerEditor.h"


juce_ImplementSingleton(ConsequenceManager)

ConsequenceManager::ConsequenceManager(const String &name) :
	BaseManager<Consequence>(name),
	forceDisabled(false)
{
	selectItemWhenCreated = false;
	triggerAll = addTrigger("Trigger All", "Trigger all the consequences in the manager");
	triggerAll->hideInEditor = true;
}

ConsequenceManager::~ConsequenceManager()
{
}

void ConsequenceManager::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	for (auto &i : items) i->forceDisabled = value;
}

void ConsequenceManager::onContainerTriggerTriggered(Trigger * t)
{
	if (forceDisabled) return;
	if (t == triggerAll) for (auto &c : items) c->trigger->trigger();
}

void ConsequenceManager::addItemInternal(Consequence * c, var data)
{
	c->forceDisabled = forceDisabled;
	triggerAll->hideInEditor = items.size() == 0;
}

void ConsequenceManager::removeItemInternal(Consequence *)
{
	triggerAll->hideInEditor = items.size() == 0;
}

InspectableEditor * ConsequenceManager::getEditor(bool isRoot)
{
	return new BaseCommandHandlerManagerEditor<Consequence>(this, CommandContext::ACTION, isRoot);
}
