/*
  ==============================================================================

    StandardCondition.h
    Created: 21 Feb 2017 11:37:26am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../Condition.h"

class StandardCondition :
	public Condition
{
public:
	StandardCondition(var params = var());
	~StandardCondition();

	TargetParameter * sourceTarget;
	Array<WeakReference<Controllable>> sourceControllables;
	std::unique_ptr<BaseComparator> comparator;

	var loadingComparatorData; //for dynamically created parameter, allows to reload comparator data after these are created

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void setSourceControllables(Array<WeakReference<Controllable>> newSources);
	void comparatorValidationChanged(BaseComparator *, int iterationIndex) override;

	void forceCheck() override;

	void onContainerParameterChangedInternal(Parameter * p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	InspectableEditor * getEditor(bool isRoot) override;

	String getTypeString() const override { return StandardCondition::getTypeStringStatic(); }
	static String getTypeStringStatic() { return "From Input Value"; }
};

