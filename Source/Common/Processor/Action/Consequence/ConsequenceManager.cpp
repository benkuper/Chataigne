/*
  ==============================================================================

	ConsequenceManager.cpp
	Created: 28 Oct 2016 8:07:30pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

juce_ImplementSingleton(ConsequenceStaggerLauncher)

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

	killDelaysOnTrigger = addBoolParameter("Cancel delays on Trigger", "If checked, this will ensure that when triggering, all previous delayed consequences are cancelled", false, false);

	staggerProgression = addFloatParameter("Stagger Progression", "If stagger is used, this animates the progression of the stagger", 0, 0, 1);
	staggerProgression->hideInEditor = true;
	staggerProgression->setControllableFeedbackOnly(true);
}

ConsequenceManager::~ConsequenceManager()
{
	cancelDelayedConsequences(-1);
}

void ConsequenceManager::triggerAll(int multiplexIndex)
{
	if (killDelaysOnTrigger->boolValue()) cancelDelayedConsequences(multiplexIndex);

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
			ConsequenceStaggerLauncher::getInstance()->addLaunch(this, multiplexIndex);
		}
	}
}

void ConsequenceManager::cancelDelayedConsequences(int multiplexIndex)
{
	ConsequenceStaggerLauncher::getInstance()->removeLaunchesFor(this, multiplexIndex);
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

void ConsequenceManager::updateKillDelayTrigger()
{
	bool showKill = items.size() > 1 && (delay->floatValue() > 0 || stagger->floatValue() > 0);
	killDelaysOnTrigger->setEnabled(showKill);

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
		updateKillDelayTrigger();
	}
}

void ConsequenceManager::addItemInternal(BaseItem* bi, var data)
{
	if (Consequence* c = dynamic_cast<Consequence*>(bi)) c->forceDisabled = forceDisabled;
	else if (ConsequenceGroup* g = dynamic_cast<ConsequenceGroup*>(bi)) g->csm.setForceDisabled(forceDisabled);

	//triggerAll->hideInEditor = items.size() == 0;
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2;
	if (!isClearing) csmNotifier.addMessage(new ConsequenceManagerEvent(ConsequenceManagerEvent::STAGGER_CHANGED, this));
	updateKillDelayTrigger();
}

void ConsequenceManager::addItemsInternal(Array<BaseItem*> items, var data)
{
	for (auto& bi : items)
	{
		if (Consequence* c = dynamic_cast<Consequence*>(bi)) c->forceDisabled = forceDisabled;
		else if (ConsequenceGroup* g = dynamic_cast<ConsequenceGroup*>(bi)) g->csm.setForceDisabled(forceDisabled);
	}

	//triggerAll->hideInEditor = items.size() == 0;
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2;
	if (!isClearing) csmNotifier.addMessage(new ConsequenceManagerEvent(ConsequenceManagerEvent::STAGGER_CHANGED, this));
	updateKillDelayTrigger();
}

void ConsequenceManager::removeItemInternal(BaseItem*)
{
	//triggerAll->hideInEditor = items.size() == 0;
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2;
	if (!isClearing) csmNotifier.addMessage(new ConsequenceManagerEvent(ConsequenceManagerEvent::STAGGER_CHANGED, this));
	updateKillDelayTrigger();
}

void ConsequenceManager::removeItemsInternal(Array<BaseItem*>)
{
	delay->hideInEditor = items.size() == 0;
	stagger->hideInEditor = items.size() < 2;
	if (!isClearing) csmNotifier.addMessage(new ConsequenceManagerEvent(ConsequenceManagerEvent::STAGGER_CHANGED, this));
	updateKillDelayTrigger();
}

void ConsequenceManager::launcherTriggered(int multiplexIndex, int triggerIndex)
{
	staggerProgression->setValue((triggerIndex + 1) * 1.0f / items.size());
}


InspectableEditor* ConsequenceManager::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ConsequenceManagerEditor(this, CommandContext::ACTION, isRoot, isMultiplexed());
}

ConsequenceStaggerLauncher::ConsequenceStaggerLauncher() :
	Thread("Stagger Launcher")
{
	startThread();
}

ConsequenceStaggerLauncher::~ConsequenceStaggerLauncher()
{
	stopThread(1000);
}

void ConsequenceStaggerLauncher::run()
{
	Array<Launch*> toRemove;

	while (!threadShouldExit())
	{
		{
			GenericScopedLock lock(launches.getLock());
			for (auto& l : launches)
			{
				processLaunch(l);
				if (l->isFinished()) toRemove.add(l);
			}

			for (auto& l : toRemove) launches.removeObject(l);

			if (launches.isEmpty()) break;
		}

		wait(10);
	}

}

void ConsequenceStaggerLauncher::processLaunch(Launch* l)
{

	ConsequenceManager* csm = l->manager;
	uint32 d = (uint32)(csm->delay->floatValue() * 1000);
	uint32 s = (uint32)(csm->stagger->floatValue() * 1000);

	//check if last item
	if (l->triggerIndex >= csm->items.size()) return;


	//get relative trigger index, not taking into account disabled items
	int relativeIndex = 0;
	for (int i = 0; i < l->triggerIndex; i++) if (csm->items[i]->enabled->boolValue()) relativeIndex++;

	//Check if should trigger
	uint32 curTime = Time::getMillisecondCounter();
	uint32 triggerTime = l->startTime + d + s * relativeIndex;

	if (curTime < triggerTime) return;

	//Get first enabled item starting at index
	{
		GenericScopedLock lock(csm->items.getLock());
		BaseItem* bi = csm->items[l->triggerIndex];
		while (bi->enabled->boolValue() == false)
		{
			l->triggerIndex++;
			if (l->triggerIndex >= csm->items.size()) return;
			bi = csm->items[l->triggerIndex];
		}


		if (Consequence* c = dynamic_cast<Consequence*>(bi)) c->triggerCommand(l->multiplexIndex);
		else if (ConsequenceGroup* g = dynamic_cast<ConsequenceGroup*>(bi)) if (g->enabled->boolValue()) g->csm.triggerAll(l->multiplexIndex);
	}

	csm->launcherTriggered(l->multiplexIndex, l->triggerIndex);
	l->triggerIndex++;
}

void ConsequenceStaggerLauncher::addLaunch(ConsequenceManager* csm, int multiplexIndex)
{
	launches.add(new Launch(csm, multiplexIndex));
	if (!isThreadRunning()) startThread();
	else notify();
}

void ConsequenceStaggerLauncher::removeLaunchesFor(ConsequenceManager* manager, int multiplexIndex)
{
	GenericScopedLock lock(launches.getLock());
	Array<Launch*> toRemove;
	for (auto& l : launches)
	{
		if (l->manager == manager && (multiplexIndex == -1 || l->multiplexIndex == multiplexIndex)) toRemove.add(l);
	}

	for (auto& l : toRemove) launches.removeObject(l);
}

void ConsequenceManager::multiplexPreviewIndexChanged()
{
	csmNotifier.addMessage(new ConsequenceManagerEvent(ConsequenceManagerEvent::MULTIPLEX_PREVIEW_CHANGED, this));
}

bool ConsequenceStaggerLauncher::Launch::isFinished() {
	return triggerIndex >= manager->items.size();
}
