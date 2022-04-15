/*
  ==============================================================================

	MultiplexIndexCondition.cpp
	Created: 15 Apr 2022 1:37:27pm
	Author:  bkupe

  ==============================================================================
*/

MultiplexIndexCondition::MultiplexIndexCondition(var params, Multiplex* multiplex) :
	Condition(getTypeString(), params, multiplex),
	index("Index", "", 0, 0),
	comp(&index, multiplex)
{
	saveAndLoadRecursiveData = true;

	startAt0 = addBoolParameter("Start at 0", "If checked, #1 will be 0, #2 will be 1, etc.", "");
	addChildControllableContainer(&comp);
	checkIsValid();
}

MultiplexIndexCondition::~MultiplexIndexCondition()
{
	
}

void MultiplexIndexCondition::onContainerParameterChangedInternal(Parameter* p)
{
	Condition::onContainerParameterChangedInternal(p);
	if(p == startAt0) checkIsValid();
}

void MultiplexIndexCondition::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Condition::onControllableFeedbackUpdateInternal(cc, c);
	if (cc == &comp) checkIsValid();
}

void MultiplexIndexCondition::checkIsValid(int multiplexIndex)
{
	if (multiplexIndex == -1)
	{
		for (int i = 0; i < getMultiplexCount(); i++) checkIsValid(i);
		return;
	}

	index.setValue(startAt0->boolValue() ? multiplexIndex : (multiplexIndex + 1));
	setValid(multiplexIndex, comp.compare(&index, multiplexIndex));
}

void MultiplexIndexCondition::afterLoadJSONDataInternal()
{
	Condition::afterLoadJSONDataInternal();
	checkIsValid();
}

InspectableEditor* MultiplexIndexCondition::getEditorInternal(bool isRoot, Array<Inspectable *> inspectables)
{
	if (inspectables.isEmpty()) inspectables.add(this);
	return new MultiplexIndexConditionEditor(this, isRoot);
}
