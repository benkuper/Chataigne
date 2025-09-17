/*
  ==============================================================================

    BasicsGuide.cpp
    Created: 7 Nov 2018 2:57:23pm
    Author:  Ben

  ==============================================================================
*/

BasicsGuide::BasicsGuide() :
	BaseGuide("Getting Started"),
	keyboardModule(nullptr),
	audioModule(nullptr),
	state(nullptr),
	condition(nullptr),
	conditionReference(nullptr),
	audioFile(nullptr),
	cme(nullptr),
	csme(nullptr)
{
	numSteps = STEPS_MAX-1;
}

BasicsGuide::~BasicsGuide()
{
}

void BasicsGuide::initInternal()
{

	if (RootModuleManager::getInstanceWithoutCreating() != nullptr) RootModuleManager::getInstance()->addAsyncManagerListener(this);

	mmui = dynamic_cast<ModuleManagerUI*>(ShapeShifterManager::getInstance()->getContentForName("Modules")->contentComponent);
	smui = dynamic_cast<StateMachineView*>(ShapeShifterManager::getInstance()->getContentForName("State Machine")->contentComponent);
	if (smui != nullptr) smui->addManagerUIListener(this);

	InspectorUI* inspectorUI = dynamic_cast<InspectorUI*>(ShapeShifterManager::getInstance()->getContentForName("Inspector")->contentComponent);

	if (inspectorUI != nullptr)
	{
		inspector = inspectorUI->inspector.get();
		inspector->addInspectorListener(this);
	}
}

void BasicsGuide::clear()
{
	if (RootModuleManager::getInstanceWithoutCreating() != nullptr) RootModuleManager::getInstance()->removeAsyncManagerListener(this);
	
	if (smui != nullptr) smui->removeManagerUIListener(this);
	if (condition != nullptr) condition->removeAsyncConditionListener(this);
	if (conditionReference != nullptr) conditionReference->removeAsyncParameterListener(this);
	if (audioFile != nullptr) audioFile->removeAsyncParameterListener(this);
	if (inspector != nullptr) inspector->removeInspectorListener(this);

	if (cme != nullptr) cme->removeContainerEditorListener(this);
	if (csme != nullptr) csme->removeContainerEditorListener(this);
}

void BasicsGuide::handleStep(int step)
{
	if (mmui == nullptr || smui == nullptr || inspector == nullptr) return;

	switch (step)
	{
	case INTRO:
		fc.setFocus(Rectangle<int>(), GuideFocusComponent::NONE, 
			"Welcome to Chataigne !\nIn this guide, we will explore some core concepts of Chataigne and learn how to trigger an action, based on some user input.", 
			"Let's go !");
		break;
	
	case OBSERVE_MODULES:
	{
		Rectangle<int> r = getLocalArea(mmui, mmui->getLocalBounds());

		fc.setFocus(r, fc.RECTANGLE, 
			"This is the Modules panel.\nModules are used to connect to other devices or pieces of software.", 
			"Got it");
	}
	break;

	case ADD_KBD_MOD:
	case ADD_AUDIO_MOD:
	{
		Rectangle<int> r = getLocalArea(mmui->addItemBT.get(), mmui->addItemBT->getLocalBounds()).expanded(10);
		String s;
		if (step == ADD_KBD_MOD) s = "Let's add a module.\nClick on the + icon and then choose the Hardware > Keyboard module.";
		else if (step == ADD_AUDIO_MOD) s = "Now, let's add another, more complex, module.\nAdd a Sound Card module from the Hardware submenu.";
		fc.setFocus(r, fc.CIRCLE, s);
	}
	break;

	case OBSERVE_INSPECTOR:
	case OBSERVE_INSPECTOR_2:
	{
		Rectangle<int> r = getLocalArea(inspector, inspector->getLocalBounds());
		String s;

		if (step == OBSERVE_INSPECTOR) s = "This is the Inspector panel.\nYou can find here all of the data related to the selected item.\nTake some time to check how the Values of the Keyboard module react when you press some keys.";
		else if (step == OBSERVE_INSPECTOR_2) s = "As you can see, there is quite a lot more to see in the Inspector.\nThis module has extra parameters and some values just as the keyboard.\nIf you scroll down to the Command tester tab, you can also see that it exposes some commands that we will use later.";

		fc.setFocus(r, fc.RECTANGLE, s, "Next step please !");
	}
	break;

	case ADD_STATE:
	{
		Rectangle<int> r = getLocalArea(smui->addItemBT.get(), smui->addItemBT->getLocalBounds()).expanded(10);
		fc.setFocus(r, fc.CIRCLE, "Now, we are ready to connect our two modules together !\nFor this, we need a State, which is a collection of relationships between values, modules and so on.\nLet's add a State to our project !");
	}
	break;


	case ADD_ACTION:
	{
		if (svui != nullptr)
		{
			Rectangle<int> r = getLocalArea(svui->pmui->addItemBT.get(), svui->pmui->addItemBT->getLocalBounds()).expanded(10);
			fc.setFocus(r, fc.CIRCLE, "Ok, we have an empty State.\nStates can contain Actions, that are triggered when certain conditions are met, or Mappings, that create a continuous relationship between values.\nLet's add a new Action inside this State.");
		}
	}
	break;

	case ADD_CONDITION:
	{
		Rectangle<int> r = getLocalArea(cme->addItemBT.get(), cme->addItemBT->getLocalBounds()).expanded(10);
		fc.setFocus(r, fc.CIRCLE, "If you have a look at the Inspector, you can see that an Action has one or more Conditions, that can trigger Consequences when they are met.\nLet's create a condition \"From Input Value\"");
	}
	break;

	case SELECT_CONDITION_SOURCE:
	{
		Rectangle<int> r = getLocalArea(ce->targetUI.get(), ce->targetUI->getLocalBounds()).expanded(10);
		fc.setFocus(r, fc.RECTANGLE, "Great, now let's choose what value we want to work with.\nLet's choose the value \"Last Key\" from the Keyboard module.\nAlternatively, you can use the \"Learn\" button then press a key.");
	}
	break;

	case EDIT_CONDITION:
	{
		for (int i = 0; i < cme->manager->items.size(); ++i)
		{
			ce = dynamic_cast<StandardConditionEditor*>(cme->getEditorForInspectable(cme->manager->items[i]));
			if (ce != nullptr && condition == nullptr) break;
		}
		if (ce->comparatorUI != nullptr)
		{
			Rectangle<int> r = getLocalArea(ce->comparatorUI.get(), ce->comparatorUI->getLocalBounds()).expanded(10);
			fc.setFocus(r, fc.RECTANGLE, "When should this action trigger ? Choose the \"=\" condition in the menu, then type a key in the field next to it, for example \"P\".");
		}
	}
	break;

	case OBSERVE_CONDITION:
	{
		if (cme != nullptr)
		{
			Rectangle<int> r = getLocalArea(cme, cme->getLocalBounds());
			fc.setFocus(r, fc.RECTANGLE, "Pressing the \"P\" key on your keyboard will now validate the condition. You can see that the background turns green whenever you hit the key.", "Yay! Let's do something useful now");
		}
	}
	break;

	case ADD_CONSEQUENCE:
	{
		Rectangle<int> r = getLocalArea(csme->addItemBT.get(), csme->addItemBT->getLocalBounds()).expanded(10);
		fc.setFocus(r, fc.CIRCLE, "Almost done ! Now that our condition is set,\nlet's decide what consequence we want when this condition is validated.\nChoose the \"Play audio file\" command from the Sound Card module.");
	}
	break;

	case EDIT_CONSEQUENCE:
	{
		Rectangle<int> r = getLocalArea(cse, cse->getLocalBounds()).expanded(10);
		fc.setFocus(r, fc.RECTANGLE, "Now, select an audio file to play from your computer.");
	}
	break;

	case END:
		fc.setFocus(Rectangle<int>(), GuideFocusComponent::NONE, 
			"Well done ! You can now play an audio file by pressing a key on your keyboard.\nIf you want to keep exploring Chataigne, you can try to modify the project to:\n - launch another sound when you press another key\n - use keyboard modifiers to launch a sound when you press Ctrl+p\n\nIf you want more, you can also check out the online tutorials and documentation from the \"Help\" menu.", 
			"OK !");
		break;

	default: 
		break;

	}
}

// Events


void BasicsGuide::newMessage(const ModuleManager::ManagerEvent & e)
{
	if((currentStep == ADD_KBD_MOD || currentStep == ADD_AUDIO_MOD) && e.type == ModuleManager::ManagerEvent::ITEM_ADDED)
	{
		if (currentStep == ADD_KBD_MOD)
		{
			KeyboardModule * k = dynamic_cast<KeyboardModule *>(e.getItem());
			if (k != nullptr)
			{
				keyboardModule = k;
				nextStep();
			}
		}
		else if (currentStep == ADD_AUDIO_MOD)
		{
			AudioModule * a = dynamic_cast<AudioModule *>(e.getItem());
			if (a != nullptr)
			{
				audioModule = a;
				nextStep();
			}
		} 
	}
}

void BasicsGuide::newMessage(const StandardCondition::ConditionEvent & e)
{
	if (currentStep == SELECT_CONDITION_SOURCE && e.type == StandardCondition::ConditionEvent::SOURCE_CHANGED)
	{
		StandardCondition * c = dynamic_cast<StandardCondition *>(e.condition);
		if (c != nullptr)
		{
			if (c->sourceTarget->target != nullptr && c->sourceTarget->target == keyboardModule->lastKey)
			{
				Parameter * p = dynamic_cast<Parameter *>(c->comparator->reference);
				if (p != nullptr && conditionReference == nullptr)
				{
					conditionReference = p;
					conditionReference->addAsyncParameterListener(this);
					auto nsFunc = std::bind(&BasicsGuide::nextStep, this);
					Timer::callAfterDelay(500, nsFunc);
				}
			} 
		}
	}
}

void BasicsGuide::newMessage(const Parameter::ParameterEvent &e)
{
	if (e.type == Parameter::ParameterEvent::VALUE_CHANGED)
	{
		if (currentStep == EDIT_CONDITION && e.parameter == conditionReference && e.value.toString() == "P") nextStep();
		if (currentStep == EDIT_CONSEQUENCE && e.parameter == audioFile && !e.value.toString().isEmpty()) nextStep();
	}
}

void BasicsGuide::currentInspectableChanged(Inspector *)
{
	if (currentStep == ADD_ACTION)
	{
		Inspectable* i = inspector->currentInspectables.size() > 0 ? inspector->currentInspectables[0] : nullptr;
		if (Action* a = dynamic_cast<Action*>(i))
		{
			GenericControllableContainerEditor * cce = dynamic_cast<GenericControllableContainerEditor *>(inspector->currentEditor.get());
			if (cce != nullptr)
			{
				if (cme != nullptr) cme->removeContainerEditorListener(this);
				if(csme != nullptr) csme->removeContainerEditorListener(this);

				if (a->cdm != nullptr) cme = dynamic_cast<GenericManagerEditor<Condition> *>(cce->getEditorForInspectable(a->cdm.get()));
				else cme = nullptr;

				csme = dynamic_cast<ConsequenceManagerEditor *>(cce->getEditorForInspectable(a->csmOn.get()));
				if(cme != nullptr) cme->addContainerEditorListener(this);
				csme->addContainerEditorListener(this);
			}

			nextStep();
		}
	}
}

void BasicsGuide::BaseItemUIAdded(StateViewUI * _svui)
{
	if (currentStep == ADD_STATE)
	{
		svui = _svui;
		auto nsFunc = std::bind(&BasicsGuide::nextStep, this);
		Timer::callAfterDelay(500, nsFunc);
	}
}

void BasicsGuide::containerRebuilt(GenericControllableContainerEditor * editor)
{
	if (editor == cme && currentStep == ADD_CONDITION)
	{
		for (int i = 0; i < cme->manager->items.size(); ++i)
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
	} else if (editor == csme && currentStep == ADD_CONSEQUENCE)
	{
		for (int i = 0; i < csme->manager->items.size(); ++i)
		{
			cse = dynamic_cast<BaseCommandHandlerEditor *>(csme->getEditorForInspectable(csme->manager->items[i]));
			if (cse != nullptr && audioFile == nullptr)
			{
 				PlayAudioFileCommand * cmd = dynamic_cast<PlayAudioFileCommand *>(cse->handler->command.get());
				if (cmd != nullptr)
				{
					audioFile = cmd->audioFile;
					audioFile->addAsyncParameterListener(this);
					auto nsFunc = std::bind(&BasicsGuide::nextStep, this);
					Timer::callAfterDelay(500, nsFunc);
					break;
				}
			}
		}
	}
}
