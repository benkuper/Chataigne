/*
  ==============================================================================

	ManualCondition.h
	Created: 1 May 2023 2:59:24pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class ManualCondition :
	public Condition
{
public:
	ManualCondition(var params = var(), Multiplex* processor = nullptr);
	~ManualCondition();

	BoolParameter* active;

	void onContainerParameterChangedInternal(Parameter* p) override;

	DECLARE_TYPE("Manual");
};