/*
  ==============================================================================

    ConditionManagerEditor.h
    Created: 28 Oct 2016 8:07:44pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "ConditionEditor.h"
#include "../ConditionManager.h"

class ConditionManagerEditor :
	public GenericManagerEditor<Condition>,
	public ConditionManager::AsyncListener
{
public:
	ConditionManagerEditor(ConditionManager *_manager, bool isRoot);
	~ConditionManagerEditor();

	ConditionManager * conditionManager;

	void updateSequentialUI();

	void itemAddedAsync(Condition *) override;
	void itemRemovedAsync(Condition *) override;

	void resetAndBuild() override;

	void newMessage(const ConditionManager::ConditionManagerEvent& e) override;

};