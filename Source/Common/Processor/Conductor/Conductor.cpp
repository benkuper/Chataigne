/*
  ==============================================================================

	Conductor.cpp
	Created: 5 Oct 2021 9:35:28pm
	Author:  bkupe

  ==============================================================================
*/

Conductor::Conductor(var params, Multiplex* multiplex) :
	Action(params, multiplex),
	processorManager("Processors", multiplex)
{
	saveAndLoadRecursiveData = true;
	type = CONDUCTOR;

	setNiceName("Conductor");
	cueIndex = addIntParameter("Cue Index", "The Index of the action that will be triggered when", 1, 1);
	lastCueName = addStringParameter("Last Cue", "The name of the last triggered Cue.", "");
	lastCueName->setControllableFeedbackOnly(true);
	nextCueName = addStringParameter("Next Cue", "The name of the cue that will be trigger if triggering the conductor", "");
	nextCueName->setControllableFeedbackOnly(true);
	loop = addBoolParameter("Loop", "If checked, when finishing the cue list, this will set the cue index to 0 again", false);

	processorManager.factory.defs.clear();
	processorManager.factory.defs.add(MultiplexTargetDefinition<Processor>::createDef<ConductorCue>("", "Cue", multiplex));
	processorManager.hideInEditor = true;
	processorManager.addBaseManagerListener(this);
	addChildControllableContainer(&processorManager);
}

Conductor::~Conductor()
{
}

void Conductor::onContainerParameterChangedInternal(Parameter* p)
{
	Action::onContainerParameterChangedInternal(p);
	if (p == cueIndex) updateCurrentCue();
}

void Conductor::updateDisables(bool force)
{
	Action::updateDisables(force);
	processorManager.setForceDisabled(!enabled->boolValue() || forceDisabled);
}

void Conductor::itemAdded(Processor* p)
{
	if (!isCurrentlyLoadingData && !isClearing) cueIndex->setRange(1, jmax(processorManager.items.size() + 1, 2));
	updateIndices();
}

void Conductor::itemsAdded(Array<Processor*> pList)
{
	if (!isCurrentlyLoadingData && !isClearing) cueIndex->setRange(1, jmax(processorManager.items.size() + 1, 2));
	updateIndices();
}

void Conductor::itemRemoved(Processor* p)
{
	if (!isCurrentlyLoadingData && !isClearing) cueIndex->setRange(1, jmax(processorManager.items.size() + 1, 2));
	updateIndices();
}

void Conductor::itemsRemoved(Array<Processor*> p)
{
	if (!isCurrentlyLoadingData && !isClearing) cueIndex->setRange(1, jmax(processorManager.items.size() + 1, 2));
	updateIndices();
}

void Conductor::itemsReordered()
{
	updateIndices();
}

void Conductor::updateIndices()
{
	if (isCurrentlyLoadingData) return;

	for (int i = 0; i < processorManager.items.size(); i++)
	{
		((ConductorCue*)processorManager.items[i])->setIndex(i + 1);
	}

	updateCurrentCue();
}

void Conductor::triggerConsequences(bool triggerTrue, int multiplexIndex)
{
	if (!enabled->boolValue() || forceDisabled) return;
	Action::triggerConsequences(triggerTrue, multiplexIndex);

	if (triggerTrue)
	{
		int index = cueIndex->intValue() - 1;
		if (index < processorManager.items.size())
		{
			if (Action* a = dynamic_cast<Action*>(processorManager.items[index]))
			{
				if (a->enabled->boolValue())
				{
					a->triggerOn->trigger(); //loose multiplex to get preview but not using it for now
					lastCueName->setValue(a->niceName);
				}
			}

			cueIndex->setValue(getNextValidIndex() + 1);
		}
	}
}

int Conductor::getNextValidIndex()
{
	for (int i = cueIndex->intValue(); i < processorManager.items.size(); i++)
	{
		if (processorManager.items[i]->enabled->boolValue()) return i;
	}

	if (loop->boolValue()) return getFirstValidIndex();
	return processorManager.items.size();
}

int Conductor::getFirstValidIndex()
{
	for (int i = 0; i < processorManager.items.size(); i++)
	{
		if (processorManager.items[i]->enabled->boolValue()) return i;
	}

	return processorManager.items.size();
}

void Conductor::updateCurrentCue()
{
	int index = cueIndex->intValue() - 1;
	String nextName = "";
	for (int i = 0; i < processorManager.items.size(); i++)
	{
		ConductorCue* cc = (ConductorCue*)processorManager.items[i];
		bool isCurrent = i == index;
		cc->setIsCurrent(isCurrent);
		if (isCurrent) nextName = cc->niceName;
	}

	nextCueName->setValue(nextName);
}

void Conductor::afterLoadJSONDataInternal()
{
	Action::afterLoadJSONDataInternal();
	cueIndex->setRange(1, jmax(processorManager.items.size() + 1, 2));
	updateIndices();
}

ProcessorUI* Conductor::getUI()
{
	return new ConductorUI(this);
}
