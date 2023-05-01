/*
  ==============================================================================

    ActivationCondition.h
    Created: 30 May 2018 9:36:05am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ActivationCondition :
	public Condition
{
public:
	ActivationCondition(var params = var(), Multiplex * processor = nullptr);
	~ActivationCondition();

	enum Type { ON_ACTIVATE, ON_DEACTIVATE };
	Type type;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	String getTypeString() const override { return ActivationCondition::getTypeStringStatic(type); }
	static String getTypeStringStatic(Type type) { return type == ON_ACTIVATE?"On Activate":"On Deactivate"; }
};