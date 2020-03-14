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
	Thread("consequences"),
	forceDisabled(false)
{
	canBeDisabled = false;

	selectItemWhenCreated = false;
	triggerAll = addTrigger("Trigger All", "Trigger all the consequences in the manager");
	triggerAll->hideInEditor = true;

	delay = addFloatParameter("Delay", "Delay the triggering of the commands", 0, 0);
	delay->defaultUI = FloatParameter::TIME;
	delay->hideInEditor = true;
	stagger = addFloatParameter("Stagger", "If multiple consequences are there, delay between each consequence trigger", 0, 0);
	stagger->defaultUI = FloatParameter::TIME;
	stagger->hideInEditor = true;
}

ConsequenceManager::~ConsequenceManager()
{
	signalThreadShouldExit();
	waitForThreadToExit(1000);
}

void ConsequenceManager::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	triggerAll->setEnabled(!forceDisabled);
	for (auto &i : items) i->forceDisabled = value;
}

void ConsequenceManager::onContainerTriggerTriggered(Trigger * t)
{
	if (forceDisabled) return;
	if (t == triggerAll)
	{
		if (items.size() > 0)
		{
			if (delay->floatValue() == 0 && stagger->floatValue() == 0) for (auto &c : items) c->trigger->trigger();
			else
			{
				timeAtRun = Time::getMillisecondCounter();
				triggerIndex = 0;
				startThread();
			}
		}
	}
}

void ConsequenceManager::addItemInternal(Consequence * c, var data)
{
	c->forceDisabled = forceDisabled;
	triggerAll->hideInEditor = items.size() == 0;
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2; 
}

void ConsequenceManager::removeItemInternal(Consequence *)
{
	triggerAll->hideInEditor = items.size() == 0;
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2;
}

void ConsequenceManager::run()
{
	timeAtRun = Time::getMillisecondCounter();
	triggerIndex = 0;

	uint32 d = (uint32)(delay->floatValue() * 1000);
	uint32 s = (uint32)(stagger->floatValue() * 1000);

	uint32 curTime = timeAtRun; 

	while(triggerIndex < items.size())
	{
		uint32 nextTriggerTime = timeAtRun + d + s * triggerIndex;
		while (nextTriggerTime > curTime)
		{
			if (threadShouldExit()) return;
			sleep(jmin<uint32>(nextTriggerTime-curTime, 20));
			curTime = Time::getMillisecondCounter();
			nextTriggerTime = timeAtRun + d + s * triggerIndex;
		}

		if (triggerIndex >= items.size()) return; 
		items[triggerIndex]->trigger->trigger();
		triggerIndex++;
	}
}

InspectableEditor * ConsequenceManager::getEditor(bool isRoot)
{
	return new BaseCommandHandlerManagerEditor<Consequence>(this, CommandContext::ACTION, isRoot);
}
