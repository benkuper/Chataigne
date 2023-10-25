/*
  ==============================================================================

	StateManager.cpp
	Created: 28 Oct 2016 8:19:15pm
	Author:  bkupe

  ==============================================================================
*/

#include "StateMachine/StateMachineIncludes.h"

juce_ImplementSingleton(StateManager)

StateManager::StateManager() :
	BaseManager<State>("States"),
	stm(this)
{

	module.reset(new StateModule(this));

	itemDataType = "State";
	helpID = "StateMachine";

	addChildControllableContainer(&stm);
	stm.hideInEditor = true;
	stm.addBaseManagerListener(this);

	addChildControllableContainer(&commentManager);
	Engine::mainEngine->addEngineListener(this);

	scriptObject.getDynamicObject()->setMethod("addTransition", &StateManager::addTransitionFromScript);

	setHasGridOptions(true);
}

StateManager::~StateManager()
{
	if (Engine::mainEngine != nullptr) Engine::mainEngine->removeEngineListener(this);
}


void StateManager::clear()
{
	stm.clear();
	commentManager.clear();
	BaseManager::clear();
}

void StateManager::setStateActive(State* s)
{
	Array<State*> linkedStates = getLinkedStates(s);
	for (auto& ss : linkedStates)
	{
		ss->active->setValue(false);
	}
}

void StateManager::addItemInternal(State* s, var data)
{
	s->addStateListener(this);
	if (!Engine::mainEngine->isLoadingFile)
	{
		s->active->setValue(true);
	}
}

void StateManager::removeItemInternal(State* s)
{
	s->removeStateListener(this);

	Array<State*> avoid;
	avoid.add(s);
	Array<State*> linkedStates;
	for (auto& ls : linkedStates) checkStartActivationOverlap(ls, avoid);
}

Array<UndoableAction*> StateManager::getRemoveItemUndoableAction(State* item)
{
	Array<UndoableAction*> result;
	result.addArray(stm.getRemoveAllLinkedTransitionsAction(item));
	result.addArray(BaseManager::getRemoveItemUndoableAction(item));

	return result;
}

Array<UndoableAction*> StateManager::getRemoveItemsUndoableAction(Array<State*> itemsToRemove)
{
	Array<UndoableAction*> result;
	for (auto& i : itemsToRemove) result.addArray(stm.getRemoveAllLinkedTransitionsAction(i));
	result.addArray(BaseManager::getRemoveItemsUndoableAction(itemsToRemove));
	return result;
}

void StateManager::stateActivationChanged(State* s)
{
	if (s->active->boolValue())
	{
		setStateActive(s);
	}
}

void StateManager::stateStartActivationChanged(State* s)
{
	checkStartActivationOverlap(s);
}

void StateManager::checkStartActivationOverlap(State* s, Array<State*> statesToAvoid)
{
	if (s == nullptr) return;

	Array<State*> linkedStates = getLinkedStates(s, &statesToAvoid);
	linkedStates.add(s);
	Array<State*> forceActiveStates;
	for (auto& ss : linkedStates)
	{
		if (ss->loadActivationBehavior->getValueDataAsEnum<State::LoadBehavior>() == State::ACTIVE) forceActiveStates.add(ss);
		else ss->clearWarning();
	}

	if (forceActiveStates.size() > 1)
	{
		LOGWARNING("Multiple linked states are set to activate on load, it may lead to unexpected behaviors");
		for (auto& fs : forceActiveStates) fs->setWarningMessage("Concurrent Active on Load state");
	}
	else if (forceActiveStates.size() > 0) forceActiveStates[0]->clearWarning();
}

void StateManager::itemAdded(StateTransition* t)
{
	if (!Engine::mainEngine->isLoadingFile)
	{
		if (t->sourceState == nullptr || t->destState == nullptr)
		{
			LOGWARNING("StateTransition added with nullptr source or dest state");
			return;
		}

		if (t->sourceState->active->boolValue()) setStateActive(t->sourceState);
		else if (t->destState->active->boolValue()) setStateActive(t->destState);

		checkStartActivationOverlap(t->sourceState);
	}
}

void StateManager::itemsAdded(Array<StateTransition*> transitions)
{
	if (!Engine::mainEngine->isLoadingFile)
	{
		for (auto& t : transitions)
		{
			if (t->sourceState == nullptr || t->destState == nullptr)
			{
				LOGWARNING("StateTransition added with nullptr source or dest state");
				continue;
			}

			if (t->sourceState->active->boolValue()) setStateActive(t->sourceState);
			else if (t->destState->active->boolValue()) setStateActive(t->destState);

			checkStartActivationOverlap(t->sourceState);

		}

	}
}

void StateManager::itemRemoved(StateTransition* s)
{
	if (!Engine::mainEngine->isClearing)
	{
		Array<State*> avoidStates;
		avoidStates.add(s->sourceState, s->destState);
		checkStartActivationOverlap(s->sourceState, avoidStates);
		checkStartActivationOverlap(s->destState, avoidStates);
	}
}

void StateManager::itemsRemoved(Array<StateTransition*> states)
{
	if (!Engine::mainEngine->isClearing)
	{
		for (auto& s : states)
		{
			Array<State*> avoidStates;
			avoidStates.add(s->sourceState, s->destState);
			checkStartActivationOverlap(s->sourceState, avoidStates);
			checkStartActivationOverlap(s->destState, avoidStates);
		}
	}
}


void StateManager::showMenuAndGetState(ControllableContainer* startFromCC, std::function<void(State*)> returnFunc)
{
	PopupMenu menu;
	StateManager* sm = StateManager::getInstance();
	for (int i = 0; i < sm->items.size(); ++i)
	{
		menu.addItem(1 + i, sm->items[i]->niceName);
	}

	menu.showMenuAsync(PopupMenu::Options(), [sm, returnFunc](int result)
		{
			if (result <= 0) return;
			returnFunc(sm->items[result - 1]);
		}
	);

}

void StateManager::showMenuAndGetAction(ControllableContainer* startFromCC, std::function<void(Action*)> returnFunc)
{
	PopupMenu menu;
	StateManager* sm = StateManager::getInstance();

	Array<Processor*> actions;

	if (ProcessorManager* pm = dynamic_cast<ProcessorManager*>(startFromCC)) menu = getProcessorMenuForManager(pm, Processor::ACTION, &actions);
	else
	{
		for (auto& s : sm->items)
		{
			PopupMenu sMenu = getProcessorMenuForManager(s->pm.get(), Processor::ACTION, &actions);
			menu.addSubMenu(s->niceName, sMenu);
		}
	}

	menu.showMenuAsync(PopupMenu::Options(), [actions, returnFunc](int result)
		{
			if (result <= 0) return;
			returnFunc((Action*)actions[result - 1]);
		}
	);
}

void StateManager::showMenuAndGetMapping(ControllableContainer* startFromCC, std::function<void(Mapping*)> returnFunc)
{
	PopupMenu menu;
	StateManager* sm = StateManager::getInstance();

	Array<Processor*> mappings;

	if (ProcessorManager* pm = dynamic_cast<ProcessorManager*>(startFromCC)) menu = getProcessorMenuForManager(pm, Processor::MAPPING, &mappings);
	else
	{
		for (auto& s : sm->items)
		{
			PopupMenu sMenu = getProcessorMenuForManager(s->pm.get(), Processor::MAPPING, &mappings);
			menu.addSubMenu(s->niceName, sMenu);
		}
	}

	menu.showMenuAsync(PopupMenu::Options(), [mappings, returnFunc](int result)
		{
			if (result <= 0) return;
			returnFunc((Mapping*)mappings[result - 1]);
		}
	);
}

void StateManager::showMenuAndGetConductor(ControllableContainer* startFromCC, std::function<void(Mapping*)> returnFunc)
{
	PopupMenu menu;
	StateManager* sm = StateManager::getInstance();

	Array<Processor*> conductors;

	if (ProcessorManager* pm = dynamic_cast<ProcessorManager*>(startFromCC)) menu = getProcessorMenuForManager(pm, Processor::CONDUCTOR, &conductors);
	else
	{
		for (auto& s : sm->items)
		{
			PopupMenu sMenu = getProcessorMenuForManager(s->pm.get(), Processor::CONDUCTOR, &conductors);
			menu.addSubMenu(s->niceName, sMenu);
		}
	}

	menu.showMenuAsync(PopupMenu::Options(), [conductors, returnFunc](int result)
		{
			if (result <= 0) return;
			returnFunc((Mapping*)conductors[result - 1]);
		}
	);
}

PopupMenu StateManager::getProcessorMenuForManager(ProcessorManager* manager, Processor::ProcessorType type, Array<Processor*>* arrayToFill)
{
	PopupMenu result;
	for (auto& p : manager->items)
	{
		if (p->type == type)
		{
			arrayToFill->add(p);
			result.addItem(arrayToFill->size(), p->niceName);
		}
		else if (p->type == Processor::MULTIPLEX)
		{
			PopupMenu mp = getProcessorMenuForManager(&((Multiplex*)p)->processorManager, type, arrayToFill);
			result.addSubMenu(p->niceName, mp);
		}
	}

	return result;
}

void StateManager::showMenuAndGetToggleCondition(ControllableContainer* startFromCC, std::function<void(StandardCondition*)> returnFunc)
{
	PopupMenu menu;
	StateManager* sm = StateManager::getInstance();

	Array<StandardCondition*> conditions;

	for (auto& s : sm->items)
	{
		PopupMenu sMenu;

		for (auto& p : s->pm->items)
		{
			if (p->type == Processor::ACTION)
			{
				Action* a = (Action*)p;
				PopupMenu aMenu = getToggleConditionMenuForConditionManager(a->cdm.get(), &conditions);
				sMenu.addSubMenu(a->niceName, aMenu);
			}
			else if (p->type == Processor::MULTIPLEX)
			{
				PopupMenu mpMenu;
				Multiplex* mp = (Multiplex*)p;
				for (auto& p : mp->processorManager.items)
				{
					if (p->type == Processor::ACTION)
					{
						Action* a = (Action*)p;
						PopupMenu aMenu = getToggleConditionMenuForConditionManager(a->cdm.get(), &conditions);
						mpMenu.addSubMenu(a->niceName, aMenu);
					}
				}
				sMenu.addSubMenu(mp->niceName, mpMenu);
			}
		}
		menu.addSubMenu(s->niceName, sMenu);
	}

	menu.showMenuAsync(PopupMenu::Options(), [conditions, returnFunc](int result)
		{
			if (result <= 0) return;
			returnFunc(conditions[result - 1]);
		}
	);
}

PopupMenu StateManager::getToggleConditionMenuForConditionManager(ConditionManager* cdm, Array<StandardCondition*>* arrayToFill)
{
	PopupMenu result;
	if (cdm == nullptr) return result;

	for (auto& c : cdm->items)
	{
		if (StandardCondition* sc = dynamic_cast<StandardCondition*>(c))
		{
			if (sc->toggleMode->boolValue())
			{
				arrayToFill->add(sc);
				result.addItem(arrayToFill->size(), sc->niceName);
			}
		}
		else if (ConditionGroup* gc = dynamic_cast<ConditionGroup*>(c))
		{
			PopupMenu gcMenu = getToggleConditionMenuForConditionManager(&gc->manager, arrayToFill);
			result.addSubMenu(gc->niceName, gcMenu);
		}
	}
	return result;
}


Array<State*> StateManager::getLinkedStates(State* s, Array<State*>* statesToAvoid)
{
	Array<State*> sAvoid;
	if (statesToAvoid == nullptr)
	{
		statesToAvoid = &sAvoid;
	}

	statesToAvoid->add(s);

	Array<State*> result;

	Array<State*> linkStates = stm.getAllStatesLinkedTo(s);
	for (auto& ss : linkStates)
	{
		if (statesToAvoid->contains(ss)) continue;
		result.add(ss);
		statesToAvoid->add(ss);
		Array<State*> linkedSS = getLinkedStates(ss, statesToAvoid);
		result.addArray(linkedSS);
	}

	return result;
}

var StateManager::addTransitionFromScript(const var::NativeFunctionArgs& a)
{
	if (a.numArguments != 2) return var();

	StateManager* sm = StateManager::getInstance();
	State* source = sm->getItemWithName(a.arguments[0].toString(), true);
	State* dest = sm->getItemWithName(a.arguments[1].toString(), true);

	if (source == nullptr || dest == nullptr) return var();

	if(StateTransition* t = sm->stm.getItemForSourceAndDest(source, dest)) return t->scriptObject;
	
	StateTransition* t = new StateTransition(source, dest);
	sm->stm.addItem(t);

	return t->scriptObject;
}


var StateManager::getJSONData()
{
	var data = BaseManager::getJSONData();

	var tData = stm.getJSONData();
	if (!tData.isVoid() && tData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(stm.shortName, tData);
	var cData = commentManager.getJSONData();
	if (!cData.isVoid() && cData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(commentManager.shortName, cData);
	return data;
}

void StateManager::loadJSONDataManagerInternal(var data)
{
	BaseManager::loadJSONDataManagerInternal(data);

	stm.loadJSONData(data.getProperty(stm.shortName, var()));
	commentManager.loadJSONData(data.getProperty(commentManager.shortName, var()));

	for (auto& s : items)
	{
		State::LoadBehavior b = s->loadActivationBehavior->getValueDataAsEnum<State::LoadBehavior>();
		if (b == State::ACTIVE)
		{
			s->active->setValue(true);
			setStateActive(s);
			checkStartActivationOverlap(s);
		}

	}
}

void StateManager::endLoadFile()
{
	for (auto& s : items) s->handleLoadActivation();
}
