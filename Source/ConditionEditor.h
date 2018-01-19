/*
  ==============================================================================

    ConditionEditor.h
    Created: 21 Feb 2017 11:40:21am
    Author:  Ben

  ==============================================================================
*/

#ifndef CONDITIONEDITOR_H_INCLUDED
#define CONDITIONEDITOR_H_INCLUDED

#include "Condition.h"

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

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionEditor)
};





#endif  // CONDITIONEDITOR_H_INCLUDED
