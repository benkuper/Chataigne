/*
  ==============================================================================

    BasicsGuide.cpp
    Created: 7 Nov 2018 2:57:23pm
    Author:  Ben

  ==============================================================================
*/

#include "BasicsGuide.h"
#include "Module/ui/ModuleManagerUI.h"
#include "Module/modules/audio/AudioModule.h"
#include "Module/modules/osc/resolume/ResolumeModule.h"

BasicsGuide::BasicsGuide() :
	BaseGuide("Getting Started"),
	audioModule(nullptr),
	resolumeModule(nullptr),
	state(nullptr),
	action(nullptr),
	condition(nullptr),
	consequence(nullptr),
	command(nullptr)
{
	numSteps = 7;
	ModuleManager::getInstance()->addAsyncManagerListener(this);

	mmui = dynamic_cast<ModuleManagerUI *>(ShapeShifterManager::getInstance()->getContentForName("Modules")->contentComponent);
	smui = dynamic_cast<StateMachineView *>(ShapeShifterManager::getInstance()->getContentForName("State Machine")->contentComponent);
	smui->addManagerUIListener(this);
	inspector = &dynamic_cast<InspectorUI *>(ShapeShifterManager::getInstance()->getContentForName("Inspector")->contentComponent)->inspector;
	inspector->addInspectorListener(this);
}

BasicsGuide::~BasicsGuide()
{
}

void BasicsGuide::clear()
{
	if (ModuleManager::getInstanceWithoutCreating() != nullptr) ModuleManager::getInstance()->removeAsyncManagerListener(this);
	
	if (smui != nullptr) smui->removeManagerUIListener(this);
	if (action != nullptr)
	{
		action->cdm.removeAsyncManagerListener(this);
		action->csmOn->removeAsyncManagerListener(this);
	}
	if (condition != nullptr) condition->removeAsyncConditionListener(this);
	if (consequence != nullptr) consequence->removeCommandHandlerListener(this);
	if (command != nullptr) command->removeCommandHandlerListener(this);
	if(inspector != nullptr) inspector->removeInspectorListener(this);

	if (cme != nullptr) cme->removeContainerEditorListener(this);
	if (csme != nullptr) csme->removeContainerEditorListener(this);
	if (condition != nullptr) condition->removeAsyncConditionListener(this);
}

void BasicsGuide::handleStep(int step)
{
	switch (step)
	{
	case AUDIO_MODULE:
	case RESOLUME_MODULE:
	{
		Rectangle<int> r = getLocalArea(mmui->addItemBT, mmui->addItemBT->getLocalBounds()).expanded(10);
		String s;
		if (step == 1) s = "Let's start by adding a module.\nClick on the + icon and then choose the Hardware > Sound Card module.";
		else if (step == 2) s = "Now, let's add another module, for instance let's try to control Resolume.\nChoose in the same menu the Software > Resolume module.";
		fc.setFocus(r, fc.CIRCLE, s);
	}
	break;

	case STATE:
	{
		Rectangle<int> r = getLocalArea(smui->addItemBT, smui->addItemBT->getLocalBounds()).expanded(10);
		fc.setFocus(r, fc.CIRCLE, "Now, let's add a State, in which will be able\nto add behaviors to create interaction between our 2 modules.");
	}
	break;


	case ACTION:
	{
		if (svui != nullptr)
		{
			Rectangle<int> r = getLocalArea(svui->pmui.addItemBT, svui->pmui.addItemBT->getLocalBounds()).expanded(10);
			fc.setFocus(r, fc.CIRCLE, "Ok we have a State. Now we want to create an Action that will compare an input value\nagainst another one.");
		}
	}
	break;

	case CONDITION:
	{
		Rectangle<int> r = getLocalArea(cme->addItemBT, cme->addItemBT->getLocalBounds()).expanded(10);
		fc.setFocus(r, fc.CIRCLE, "Good ! We can now edit our Action inside the Inspector.\nLet's create a condition \"From Input Value\"");
	}
	break;

	case CONDITION_SOURCE:
	{
		Rectangle<int> r = getLocalArea(ce->targetUI, ce->targetUI->getLocalBounds()).expanded(10);
		fc.setFocus(r, fc.RECTANGLE, "Great, now let's choose what value we want to compare.\nLet's choose the value \"Volume\" of the Sound Card");
	}
	break;

	case CONSEQUENCE:
	{
		Rectangle<int> r = getLocalArea(csme->addItemBT, csme->addItemBT->getLocalBounds()).expanded(10);
		fc.setFocus(r, fc.CIRCLE, "Almost done ! Now that our condition is set,\nlet's decide what consequence we want when this condition is validated.\nChoose whatever command from Resolume makes you happy !");

	}
	break;

	}
}


// Events


void BasicsGuide::newMessage(const ModuleManager::ManagerEvent & e)
{
	if((currentStep == AUDIO_MODULE || currentStep == RESOLUME_MODULE) && e.type == ModuleManager::ManagerEvent::ITEM_ADDED)
	{
		if (currentStep == AUDIO_MODULE)
		{
			AudioModule * a = dynamic_cast<AudioModule *>(e.getItem());
			if (a != nullptr)
			{
				audioModule = a;
				nextStep();
			}
		} else if (currentStep == RESOLUME_MODULE)
		{
			ResolumeModule * r = dynamic_cast<ResolumeModule *>(e.getItem());
			if (r != nullptr)
			{
				resolumeModule = r;
				nextStep();
			}
		}
	}
}


void BasicsGuide::newMessage(const ConditionManager::ManagerEvent & e)
{
	/*
	if (currentStep == CONDITION && e.type == ConditionManager::ManagerEvent::ITEM_ADDED)
	{
		StandardCondition * a = dynamic_cast<StandardCondition *>(e.getItem());
		if (a != nullptr)
		{
			a->addAsyncConditionListener(this);
			nextStep();
		}
	}
	*/
}

void BasicsGuide::newMessage(const StandardCondition::ConditionEvent & e)
{
	if (currentStep == CONDITION_SOURCE && e.type == StandardCondition::ConditionEvent::SOURCE_CHANGED)
	{
		StandardCondition * c = dynamic_cast<StandardCondition *>(e.condition);
		if (c != nullptr)
		{
			if (c->sourceTarget->target != nullptr && c->sourceTarget->target == audioModule->volume) nextStep();
		}
	}
}

void BasicsGuide::newMessage(const ConsequenceManager::ManagerEvent & e)
{
	/*
	if (currentStep == CONSEQUENCE && e.type == ConditionManager::ManagerEvent::ITEM_ADDED)
	{
		Consequence * c = dynamic_cast<Consequence *>(e.getItem());
		if (c != nullptr)
		{
			command = c;
			command->addCommandHandlerListener(this);
			nextStep();
		}
	}
	*/
}



void BasicsGuide::commandChanged(BaseCommandHandler * bch)
{
	nextStep();
}

void BasicsGuide::currentInspectableChanged(Inspector *)
{
	if (currentStep == ACTION)
	{
		Action * a = dynamic_cast<Action *>(inspector->currentInspectable.get());
		if (a != nullptr)
		{
			GenericControllableContainerEditor * cce = dynamic_cast<GenericControllableContainerEditor *>(inspector->currentEditor.get());
			if (cce != nullptr)
			{
				cme = dynamic_cast<GenericManagerEditor<Condition> *>(cce->getEditorForInspectable(&a->cdm));
				csme = dynamic_cast<BaseCommandHandlerManagerEditor<Consequence> *>(cce->getEditorForInspectable(a->csmOn));
				cme->addContainerEditorListener(this);
				csme->addContainerEditorListener(this);
			}

			nextStep();
		}
	}
}

void BasicsGuide::itemUIAdded(StateViewUI * _svui)
{
	if (currentStep == STATE)
	{
		svui = _svui;
		nextStep();
	}
}

void BasicsGuide::containerRebuilt(GenericControllableContainerEditor * editor)
{
	if (editor == cme && currentStep == CONDITION)
	{
		for (int i = 0; i < cme->manager->items.size(); i++)
		{
			ce = dynamic_cast<StandardConditionEditor *>(cme->getEditorForInspectable(cme->manager->items[i]));
			if (ce != nullptr && condition == nullptr)
			{
				condition = dynamic_cast<StandardCondition *>(ce->condition);
				condition->addAsyncConditionListener(this);
				auto nsFunc = std::bind(&BasicsGuide::nextStep, this);
				Timer::callAfterDelay(500, nsFunc);
				break;
			}
		}
	} else if (editor == csme && currentStep == CONSEQUENCE)
	{
		nextStep();
	}
}
