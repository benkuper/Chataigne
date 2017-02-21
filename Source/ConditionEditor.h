/*
  ==============================================================================

    ConditionEditor.h
    Created: 21 Feb 2017 11:40:21am
    Author:  Ben

  ==============================================================================
*/

#ifndef CONDITIONEDITOR_H_INCLUDED
#define CONDITIONEDITOR_H_INCLUDED


#include "BaseItemEditor.h"
#include "Condition.h"

class ConditionEditor :
	public BaseItemEditor,
	public Condition::ConditionListener,
	public Condition::AsyncListener
{
public:
	ConditionEditor(Condition *, bool isRoot);
	virtual ~ConditionEditor();

	Condition * condition;


	virtual void paintOverChildren(Graphics &g) override;
	virtual void updateUI() {}

	void conditionSourceChanged(Condition *) override;

	//async
	void newMessage(const Condition::ValidationAsyncEvent &e) override;

	void childBoundsChanged(Component *) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionEditor)
};





#endif  // CONDITIONEDITOR_H_INCLUDED
