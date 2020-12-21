/*
  ==============================================================================

    ConditionEditor.h
    Created: 21 Feb 2017 11:40:21am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../Condition.h"

class ConditionEditor :
	public BaseItemEditor,
	public Condition::AsyncListener
{
public:
	ConditionEditor(Condition *, bool isRoot);
	virtual ~ConditionEditor();

	Condition * condition;


	virtual void paintOverChildren(Graphics &g) override;
	virtual void updateUI() {}

	virtual void conditionSourceChangedAsync(Condition *);

	//async
	void newMessage(const Condition::ConditionEvent &e) override;

	void childBoundsChanged(Component *) override;
};