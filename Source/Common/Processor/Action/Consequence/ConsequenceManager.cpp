/*
  ==============================================================================

	ConsequenceManager.cpp
	Created: 28 Oct 2016 8:07:30pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Command/ui/BaseCommandHandlerManagerEditor.h"

ConsequenceManager::ConsequenceManager(const String& name, Multiplex* multiplex) :
    BaseManager<Consequence>(name),
    MultiplexTarget(multiplex),
	forceDisabled(false)
{
	canBeDisabled = false;
	canBeCopiedAndPasted = true;

	selectItemWhenCreated = false;


	delay = addFloatParameter("Delay", "Delay the triggering of the commands", 0, 0);
	delay->defaultUI = FloatParameter::TIME;
	delay->hideInEditor = true;
	stagger = addFloatParameter("Stagger", "If multiple consequences are there, delay between each consequence trigger", 0, 0);
	stagger->defaultUI = FloatParameter::TIME;
	stagger->hideInEditor = true;
}

ConsequenceManager::~ConsequenceManager()
{
	
}

Consequence* ConsequenceManager::createItem()
{
	return new Consequence(multiplex);
}

void ConsequenceManager::triggerAll(int multiplexIndex)
{
	if (items.size() > 0)
	{
		if (delay->floatValue() == 0 && stagger->floatValue() == 0)
		{
			for (auto& c : items)
			{
				c->triggerCommand(multiplexIndex);
			}
		}
		else
		{
			staggerLaunchers.add(new StaggerLauncher(this, multiplexIndex));
		}
	}
}

void ConsequenceManager::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	for (auto& i : items) i->forceDisabled = value;
}

void ConsequenceManager::onContainerTriggerTriggered(Trigger* t)
{
	if (forceDisabled) return;
	//for manual trigger eventually
	BaseManager::onContainerTriggerTriggered(t);
}

void ConsequenceManager::addItemInternal(Consequence* c, var data)
{
	c->forceDisabled = forceDisabled;
	//triggerAll->hideInEditor = items.size() == 0;
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2;
}

void ConsequenceManager::removeItemInternal(Consequence*)
{
	//triggerAll->hideInEditor = items.size() == 0;
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2;
}

void ConsequenceManager::launcherFinished(StaggerLauncher* launcher)
{
	staggerLaunchers.removeObject(launcher);
}

InspectableEditor* ConsequenceManager::getEditor(bool isRoot)
{
	return new BaseCommandHandlerManagerEditor<Consequence>(this, CommandContext::ACTION, isRoot, isMultiplexed());
}

ConsequenceManager::StaggerLauncher::StaggerLauncher(ConsequenceManager* csm, int multiplexIndex) :
	Thread("Stagger Launcher"),
	csm(csm),
	multiplexIndex(multiplexIndex)
{
	startThread();
}

ConsequenceManager::StaggerLauncher::~StaggerLauncher()
{
	stopThread(100);
}

void ConsequenceManager::StaggerLauncher::run()
{
	timeAtRun = Time::getMillisecondCounter();
	triggerIndex = 0;

	uint32 d = (uint32)(csm->delay->floatValue() * 1000);
	uint32 s = (uint32)(csm->stagger->floatValue() * 1000);

	uint32 curTime = timeAtRun;

	while (triggerIndex < csm->items.size())
	{
		uint32 nextTriggerTime = timeAtRun + d + s * triggerIndex;
		while (nextTriggerTime > curTime)
		{
			if (threadShouldExit()) return;
			wait(jmin<uint32>(nextTriggerTime - curTime, 20));
			curTime = Time::getMillisecondCounter();
			nextTriggerTime = timeAtRun + d + s * triggerIndex;
		}

		if (triggerIndex >= csm->items.size()) return;
		csm->items[triggerIndex]->triggerCommand(multiplexIndex);
		triggerIndex++;
	}
}
