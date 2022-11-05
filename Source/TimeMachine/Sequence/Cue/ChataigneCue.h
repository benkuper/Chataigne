/*
  ==============================================================================

	ChataigneCue.h
	Created: 5 Nov 2022 6:47:36pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once


class ChataigneCue :
	public TimeCue,
	public ConditionManagerListener
{
public:
	ChataigneCue(float time = 0, TimeCueManager* m = nullptr);
	~ChataigneCue();

	std::unique_ptr<ConditionManager> cdm;
	var ghostCDMData;

	virtual bool isCurrentlyActive() override; // can be overriden by children 

	virtual void onContainerParameterChangedInternal(Parameter* p) override;

	virtual void conditionManagerValidationChanged(ConditionManager*, int multiplexIndex, bool dispatchOnChangeOnly) override;
};