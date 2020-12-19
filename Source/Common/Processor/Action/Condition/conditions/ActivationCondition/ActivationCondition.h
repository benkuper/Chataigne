/*
  ==============================================================================

    ActivationCondition.h
    Created: 30 May 2018 9:36:05am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../Condition.h"

class ActivationCondition :
	public Condition
{
public:
	ActivationCondition(var params);
	~ActivationCondition();

	enum Type { ON_ACTIVATE, ON_DEACTIVATE };
	Type type;

	void onContainerParameterChangedInternal(Parameter * p) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	String getTypeString() const override { return ActivationCondition::getTypeStringStatic(type); }
	static String getTypeStringStatic(Type type) { return type == ON_ACTIVATE?"On Activate":"On Deactivate"; }
};