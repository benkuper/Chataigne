/*
  ==============================================================================

	State.cpp
	Created: 28 Oct 2016 8:19:10pm
	Author:  bkupe

  ==============================================================================
*/

#include "StateMachine/StateMachineIncludes.h"

State::State() :
	BaseItem(getTypeString(), true)
{

	itemDataType = "State";

	setHasCustomColor(true);
	itemColor->setDefaultValue(BG_COLOR.brighter(.1f));

	active = addBoolParameter("Active", "If active, the state's actions and mappings will be effective, otherwise this state won't do anything.", false);
	loadActivationBehavior = addEnumParameter("On Load Behavior", "This is defining how the state is initializing when loading");
	loadActivationBehavior->addOption("Restore last state", KEEP)->addOption("Activate", ACTIVE)->addOption("Deactivate", NONACTIVE);
	checkTransitionsOnActivate = addBoolParameter("Check transitions on activate", "If checked, this will automatically check for already valid conditions on activate.\n \
		Otherwise, already valid transition will need to be unvalidated and then validated again to be activated.", false);

	focusOnLastActionTriggered = addBoolParameter("Focus on Last Action", "If checked, this will automatically scroll to make the last triggered action visible", false);

	pm.reset(new ProcessorManager("Processors"));
	addChildControllableContainer(pm.get());

	//pm->setForceDisabled(true);

	pm->setForceDisabled(!active->boolValue() || !enabled->boolValue(), false, true);


	//set default value
	//viewUISize->setBounds(50, 50,1000,1000);
	viewUISize->setPoint(250, 200);
	viewUISize->defaultValue = viewUISize->value;
	viewUISize->resetValue();

	showWarningInUI = true;

	helpID = "State";

}

State::~State()
{
	masterReference.clear();
}


void State::handleLoadActivation()
{
	if (!active->boolValue() || !enabled->boolValue()) return;
	handleActiveChanged();
}

void State::handleActiveChanged()
{
	if (!enabled->boolValue()) return;

	if (active->boolValue())
	{
		pm->setForceDisabled(!active->boolValue() || !enabled->boolValue(), false, true);
		stateListeners.call(&StateListener::stateActivationChanged, this);

		pm->checkAllActivateActions();
		//pm->processAllMappings();

		for (auto& t : outTransitions)
		{
			t->forceCheck(false); //force setting the valid state even if listeners will trigger it after (avoir listener-order bugs)

			//check for onActivate in transitions
			for (auto& c : t->cdm->items)
			{
				ActivationCondition* ac = dynamic_cast<ActivationCondition*>(c);
				if (ac != nullptr)
				{
					bool valid = ac->type == ActivationCondition::Type::ON_ACTIVATE && !ac->forceDisabled;
					for (int i = 0; i < ac->getMultiplexCount(); i++) ac->setValid(i, valid, false);
				}
			}

			if (checkTransitionsOnActivate->boolValue())
			{
				if (t->cdm != nullptr && t->cdm->getIsValid())
				{
					t->triggerConsequences(true);
					break;
				}
			}
		}
	}
	else
	{
		for (auto& t : outTransitions)
		{
			for (auto& c : t->cdm->items)
			{
				ActivationCondition* ac = dynamic_cast<ActivationCondition*>(c);
				if (ac != nullptr)
				{
					for (int i = 0; i < ac->getMultiplexCount(); i++) ac->setValid(i, false, false);
				}
			}
		}

		pm->checkAllDeactivateActions();
		stateListeners.call(&StateListener::stateActivationChanged, this);
		pm->setForceDisabled(!active->boolValue() || !enabled->boolValue());
	}
}

void State::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == active || p == enabled)
	{
		if (!isCurrentlyLoadingData)
		{
			if (p == enabled)
			{
				stateListeners.call(&StateListener::stateActivationChanged, this);
				pm->setForceDisabled(!active->boolValue() || !enabled->boolValue());

				if (enabled->boolValue() && active->boolValue())
					pm->processAllMappings();
			}
			else
			{
				handleActiveChanged();
			}
		}
		else
		{
			pm->setForceDisabled(!active->boolValue() || !enabled->boolValue(), false, true);
		}
	}
	else if (p == loadActivationBehavior)
	{
		stateListeners.call(&StateListener::stateStartActivationChanged, this);
	}
}

var State::getJSONData()
{
	var data = BaseItem::getJSONData();
	var pData = pm->getJSONData();
	if (!pData.isVoid() && pData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(pm->shortName, pData);
	return data;
}

void State::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	pm->loadJSONData(data.getProperty(pm->shortName, var()));
}

void State::afterLoadJSONDataInternal()
{
	BaseItem::afterLoadJSONDataInternal();
	LoadBehavior b = loadActivationBehavior->getValueDataAsEnum<LoadBehavior>();
	if (b != KEEP) active->setValue(b == ACTIVE);
	else if (!Engine::mainEngine->isLoadingFile) active->setValue(active->boolValue(), false, true); //force reactivate if not already
}

bool State::paste()
{
	Array<Processor*> p = pm->addItemsFromClipboard(false);
	if (p.isEmpty()) return BaseItem::paste();
	return true;
}

void State::selectAll(bool addToSelection)
{
	deselectThis(pm->items.size() == 0);
	pm->askForSelectAllItems(addToSelection);
}
