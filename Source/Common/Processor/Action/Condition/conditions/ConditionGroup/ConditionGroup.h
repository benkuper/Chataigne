/*
  ==============================================================================

    ConditionGroup.h
    Created: 21 Feb 2017 11:27:53am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../Condition.h"
#include "../../ConditionManager.h"

class ConditionGroup :
	public Condition,
	public ConditionManager::ConditionManagerListener
{
public:
	ConditionGroup(var params = var(), Multiplex* multiplex = nullptr);
	~ConditionGroup();

	ConditionManager manager;

	void conditionManagerValidationChanged(ConditionManager *, int multiplexIndex) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	String getTypeString() const override { return ConditionGroup::getTypeStringStatic(); }
	static String getTypeStringStatic() { return "Group"; }};

