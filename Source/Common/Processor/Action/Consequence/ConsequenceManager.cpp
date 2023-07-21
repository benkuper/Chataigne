/*
  ==============================================================================

	ConsequenceManager.cpp
	Created: 28 Oct 2016 8:07:30pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

ConsequenceManager::ConsequenceManager(const String& name, Multiplex* multiplex) :
	BaseManager<BaseItem>(name),
	MultiplexTarget(multiplex),
	killDelaysOnTrigger(nullptr),
	forceDisabled(false),
	csmNotifier(5)
{
	canBeDisabled = false;
	canBeCopiedAndPasted = true;

	selectItemWhenCreated = false;

	factory.defs.add(MultiplexTargetDefinition<BaseItem>::createDef<Consequence>("", "Consequence", multiplex));
	factory.defs.add(MultiplexTargetDefinition<BaseItem>::createDef<ConsequenceGroup>("", "Group", multiplex));

	managerFactory = &factory;

	triggerPreview = addTrigger("Trigger", "Trigger all the consequences. Triggers at the previewed index if multiplexed");
	triggerPreview->hideInEditor = true;

	delay = addFloatParameter("Delay", "Delay the triggering of the commands", 0, 0);
	delay->defaultUI = FloatParameter::TIME;
	delay->hideInEditor = true;
	stagger = addFloatParameter("Stagger", "If multiple consequences are there, delay between each consequence trigger", 0, 0);
	stagger->defaultUI = FloatParameter::TIME;
	stagger->hideInEditor = true;

	staggerProgression = addFloatParameter("Stagger Progression", "If stagger is used, this animates the progression of the stagger", 0, 0, 1);
	staggerProgression->hideInEditor = true;
	staggerProgression->setControllableFeedbackOnly(true);
}

ConsequenceManager::~ConsequenceManager()
{

}




void ConsequenceManager::triggerAll(int multiplexIndex)
{
	if (killDelaysOnTrigger != nullptr && killDelaysOnTrigger->boolValue()) cancelDelayedConsequences();

	if (items.size() > 0)
	{
		if (delay->floatValue() == 0 && stagger->floatValue() == 0)
		{
			for (auto& bi : items)
			{
				if (Consequence* c = dynamic_cast<Consequence*>(bi)) c->triggerCommand(multiplexIndex);
				else if (ConsequenceGroup* g = dynamic_cast<ConsequenceGroup*>(bi))  if (g->enabled->boolValue()) g->csm.triggerAll(multiplexIndex);
			}
		}
		else
		{
			staggerLaunchers.add(new StaggerLauncher(this, multiplexIndex));
		}
	}
}

void ConsequenceManager::cancelDelayedConsequences()
{
	for (auto& launcher : staggerLaunchers) launcher->stopThread(100);
	staggerLaunchers.clear();
}

void ConsequenceManager::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	for (auto& i : items)
	{
		if (Consequence* c = dynamic_cast<Consequence*>(i)) c->forceDisabled = value;
		else if (ConsequenceGroup* g = dynamic_cast<ConsequenceGroup*>(i)) g->csm.setForceDisabled(value);
	}
}

void ConsequenceManager::onContainerTriggerTriggered(Trigger* t)
{
	if (forceDisabled) return;

	//for manual trigger eventually
	if (t == triggerPreview) triggerAll(getPreviewIndex());

	BaseManager::onContainerTriggerTriggered(t);
}

void ConsequenceManager::onContainerParameterChanged(Parameter* p)
{
	BaseManager::onContainerParameterChanged(p);
	if (p == delay || p == stagger)
	{
		bool showKill = delay->floatValue() > 0 || stagger->floatValue() > 0;
		if (showKill)
		{
			if (killDelaysOnTrigger == nullptr) killDelaysOnTrigger = addBoolParameter("Cancel delays on Trigger", "If checked, this will ensure that when triggering, all previous delayed consequences are cancelled", false);
		}
		else
		{
			if (killDelaysOnTrigger)
			{
				removeControllable(killDelaysOnTrigger);
				killDelaysOnTrigger = nullptr;
			}
		}
	}
}

void ConsequenceManager::addItemInternal(BaseItem* bi, var data)
{
	if (Consequence* c = dynamic_cast<Consequence*>(bi)) c->forceDisabled = forceDisabled;
	else if (ConsequenceGroup* g = dynamic_cast<ConsequenceGroup*>(bi)) g->csm.setForceDisabled(forceDisabled);

	//triggerAll->hideInEditor = items.size() == 0;
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2;
}

void ConsequenceManager::removeItemInternal(BaseItem*)
{
	//triggerAll->hideInEditor = items.size() == 0;
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2;
}

void ConsequenceManager::launcherTriggered(StaggerLauncher* launcher)
{
	if (staggerLaunchers.size() == 0 || items.size() == 0) return;
	if (launcher != staggerLaunchers.getLast()) return;
	staggerProgression->setValue((launcher->triggerIndex + 1) * 1.0f / items.size());
}

void ConsequenceManager::launcherFinished(StaggerLauncher* launcher)
{
	MessageManager::getInstance()->callAsync([=]()
		{
			launcher->stopThread(100);
			staggerLaunchers.removeObject(launcher);
		}
	);
}

InspectableEditor* ConsequenceManager::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ConsequenceManagerEditor(this, CommandContext::ACTION, isRoot, isMultiplexed());
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
	if (isThreadRunning())
	{
		MessageManager::callAsync([this] {
			stopThread(100);
			});
	}
}

void ConsequenceManager::StaggerLauncher::run()
{
	timeAtRun = Time::getMillisecondCounter();
	triggerIndex = 0;

	uint32 d = (uint32)(csm->delay->floatValue() * 1000);
	uint32 s = (uint32)(csm->stagger->floatValue() * 1000);

	uint32 curTime = timeAtRun;

	while (!threadShouldExit() && triggerIndex < csm->items.size())
	{
		uint32 nextTriggerTime = timeAtRun + d + s * triggerIndex;
		while (nextTriggerTime > curTime)
		{
			if (threadShouldExit()) break;
			wait(jmin<uint32>(nextTriggerTime - curTime, 20));
			curTime = Time::getMillisecondCounter();
			nextTriggerTime = timeAtRun + d + s * triggerIndex;
		}

		if (triggerIndex >= csm->items.size()) break;
		BaseItem* bi = csm->items[triggerIndex];

		while (bi != nullptr && !bi->enabled->boolValue())
		{
			triggerIndex++;
			if (triggerIndex >= csm->items.size()) break;
			bi = csm->items[triggerIndex];
		}


		if (threadShouldExit() || bi == nullptr) break;

		if (Consequence* c = dynamic_cast<Consequence*>(bi)) c->triggerCommand(multiplexIndex);
		else if (ConsequenceGroup* g = dynamic_cast<ConsequenceGroup*>(bi)) if (g->enabled->boolValue()) g->csm.triggerAll(multiplexIndex);

		csm->launcherTriggered(this);
		triggerIndex++;
	}

	if (!threadShouldExit()) csm->launcherFinished(this);
}

void ConsequenceManager::multiplexPreviewIndexChanged()
{
	csmNotifier.addMessage(new ConsequenceManagerEvent(ConsequenceManagerEvent::MULTIPLEX_PREVIEW_CHANGED, this));
}