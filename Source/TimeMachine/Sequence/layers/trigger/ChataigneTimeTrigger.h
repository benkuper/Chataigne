/*
  ==============================================================================

    ChataigneTimeTrigger.h
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

class ConsequenceManager;

class ChataigneTimeTrigger :
	public ConditionManagerListener,
	public TimeTrigger
{
public:
	ChataigneTimeTrigger(StringRef name = "Trigger");
	virtual ~ChataigneTimeTrigger();

	enum evaluateSetting
    {
        onlyOnce       = 0,
        onlyEnterExit  = 1,
        always         = 2,
    };

	enum possibleActions
    {
        action_nothing       = 0,
        action_deactivate    = 1,
        action_triggerTrue   = 2,
        action_triggerFalse  = 3,
		action_triggerBoth   = 4
    };

	EnumParameter * triggerCondition;
	EnumParameter * onValidExit;
	EnumParameter * onInvalidExit;
	EnumParameter * onRewind;

	bool alreadyTriggered;

	std::unique_ptr<ConsequenceManager> csm;
	std::unique_ptr<ConsequenceManager> untcsm;

	std::unique_ptr<ConditionManager> cdm;

	void updateTriggerParams();

	virtual void onContainerParameterChangedInternal(Parameter* p) override;

	virtual void conditionManagerValidationChanged(ConditionManager*, int multiplexIndex, bool dispatchOnChangeOnly) override;

	virtual void triggerInternal() override;
	virtual void unTriggerInternal() override;
	virtual void exitedInternal(bool force) override;

	virtual var getJSONData(bool includeNonOverriden = false) override;
	virtual void loadJSONDataInternal(var data) override;
};