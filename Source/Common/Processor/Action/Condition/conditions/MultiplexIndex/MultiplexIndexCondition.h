/*
  ==============================================================================

    MultiplexIndexCondition.h
    Created: 15 Apr 2022 1:37:27pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MultiplexIndexCondition :
	public Condition
{
public:
	MultiplexIndexCondition(var params = var(), Multiplex* multiplex = nullptr);
	~MultiplexIndexCondition();

	BoolParameter* startAt0;
	IntParameter index;
	NumberComparator comp;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void checkIsValid(int multiplexIndex = -1);

	void afterLoadJSONDataInternal() override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	DECLARE_TYPE("Multiplex Index");
};