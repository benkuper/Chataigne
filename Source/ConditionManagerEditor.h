/*
  ==============================================================================

    ConditionManagerEditor.h
    Created: 28 Oct 2016 8:07:44pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ConditionManagerEditor_H_INCLUDED
#define ConditionManagerEditor_H_INCLUDED


#include "ConditionEditor.h"
#include "ConditionManager.h"

class ConditionManagerEditor :
	public GenericManagerEditor<Condition>
{
public:
	ConditionManagerEditor(ConditionManager *_manager, bool isRoot);
	~ConditionManagerEditor();

	ConditionManager * conditionManager;

	void showMenuAndAddItem(bool isFromAddButton) override;


	void controllableFeedbackUpdate(Controllable * c) override;
	

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionManagerEditor)
};





#endif  // ConditionManagerEditor_H_INCLUDED
