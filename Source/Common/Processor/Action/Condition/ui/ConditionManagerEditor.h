/*
  ==============================================================================

    ConditionManagerEditor.h
    Created: 28 Oct 2016 8:07:44pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConditionManagerEditor :
	public GenericManagerEditor<Condition>,
	public ConditionManagerAsyncListener
{
public:
	ConditionManagerEditor(ConditionManager *_manager, bool isRoot);
	~ConditionManagerEditor();

	ConditionManager * conditionManager;

	void updateSequentialUI();

	void itemAddedAsync(Condition*) override;
	void itemRemovedAsync(Condition *) override;

	void resetAndBuild() override;

	void newMessage(const ConditionManagerEvent& e) override;
};