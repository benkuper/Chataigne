/*
  ==============================================================================

    StateTransitionManager.cpp
    Created: 28 Oct 2016 8:20:59pm
    Author:  bkupe

  ==============================================================================
*/

StateTransitionManager::StateTransitionManager(StateManager * _sm) :
	BaseManager("Transitions"),
	sm(_sm)
{

}

StateTransitionManager::~StateTransitionManager()
{
}

StateTransition * StateTransitionManager::addItemFromData(var data, bool addToUndo)
{
	State * sourceState = sm->getItemWithName(data.getProperty("sourceState", ""));
	State * destState = sm->getItemWithName(data.getProperty("destState", ""));
	if (sourceState == nullptr || destState == nullptr) return nullptr;
	return addItem(sourceState, destState, data, addToUndo);
}

StateTransition * StateTransitionManager::addItem(State * source, State * dest, var data, bool addToUndo)
{
	if (getItemForSourceAndDest(source, dest) != nullptr) return nullptr;
	return BaseManager::addItem(new StateTransition(source, dest), data,addToUndo);
}

Array<State*> StateTransitionManager::getAllStatesLinkedTo(State * state)
{
	Array<State*> result;
	for (auto &t : state->inTransitions) result.add(t->sourceState);
	for (auto &t : state->outTransitions) result.add(t->destState);
	return result;
}

Array<UndoableAction *> StateTransitionManager::getRemoveAllLinkedTransitionsAction(State * linkedState)
{
	Array<StateTransition *> transitionsToRemove;
	transitionsToRemove.addArray(linkedState->inTransitions);
	transitionsToRemove.addArray(linkedState->outTransitions);
	return getRemoveItemsUndoableAction(transitionsToRemove);
}

StateTransition * StateTransitionManager::getItemForSourceAndDest(State * source, State * dest)
{
	for (auto &st : items)
	{
		if (st->sourceState == source && st->destState == dest) return st;
	}
	return nullptr;
}
