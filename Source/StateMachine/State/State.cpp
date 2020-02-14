/*
  ==============================================================================

    State.cpp
    Created: 28 Oct 2016 8:19:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "State.h"

State::State() :
	BaseItem(getTypeString(),true),
	pm("Processors")
{ 
	itemDataType = "State";

	active = addBoolParameter("Active", "If active, the state's actions and mappings will be effective, otherwise this state won't do anything.", false);
	loadActivationBehavior = addEnumParameter("On Load Behavior", "This is defining how the state is initializing when loading");
	loadActivationBehavior->addOption("Restore last state", KEEP)->addOption("Activate", ACTIVE)->addOption("Deactivate", NONACTIVE);
	checkTransitionsOnActivate = addBoolParameter("Check transitions on activate", "If checked, this will automatically check for already valid conditions on activate.\n \
		Otherwise, already valid transition will need to be unvalidated and then validated again to be activated.", true);

	addChildControllableContainer(&pm);

	pm.setForceDisabled(true);

	//set default value
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


void State::onContainerParameterChangedInternal(Parameter *p)
{
	if (p == active || p == enabled)
	{		
		if (!Engine::mainEngine->isLoadingFile)
		{
			if (p == enabled)
			{
				stateListeners.call(&StateListener::stateActivationChanged, this);
				pm.setForceDisabled(!active->boolValue() || !enabled->boolValue());

				if (enabled->boolValue() && active->boolValue())
					pm.processAllMappings();
			}
			else if (enabled->boolValue())
			{

				if (active->boolValue())
				{
					pm.setForceDisabled(!active->boolValue() || !enabled->boolValue());
					stateListeners.call(&StateListener::stateActivationChanged, this);

					pm.checkAllActivateActions();
					pm.processAllMappings();

					if (checkTransitionsOnActivate->boolValue())
					{
						for (auto& t : outTransitions)
						{
							if (t->cdm.isValid->boolValue())
							{
								t->triggerOn->trigger();
								break;
							}
						}
					}


				}
				else
				{
					pm.checkAllDeactivateActions();
					stateListeners.call(&StateListener::stateActivationChanged, this);
					pm.setForceDisabled(!active->boolValue() || !enabled->boolValue());
				}

			}
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
	var pData = pm.getJSONData();
	if(!pData.isVoid() && pData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(pm.shortName, pData);
	return data;
}

void State::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	pm.loadJSONData(data.getProperty(pm.shortName, var()));

	LoadBehavior b = loadActivationBehavior->getValueDataAsEnum<LoadBehavior>();
	if (b != KEEP) active->setValue(b == ACTIVE);
}

bool State::paste()
{
	Array<Processor *> p = pm.addItemsFromClipboard(false);
	if (p.isEmpty()) return BaseItem::paste();
	return true;
}

void State::selectAll(bool addToSelection)
{
	deselectThis(pm.items.size() == 0);
	pm.askForSelectAllItems(addToSelection);
}
