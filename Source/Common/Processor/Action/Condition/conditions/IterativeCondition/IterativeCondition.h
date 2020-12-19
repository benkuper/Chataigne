/*
  ==============================================================================

    IterativeStandardCondition.h
    Created: 19 Dec 2020 12:05:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Condition.h"
#include "Common/Processor/Iterator/List/IteratorList.h"

class IteratorProcessor;

class IterativeCondition :
	public Condition
{
public:
	IterativeCondition(var params = var());
	~IterativeCondition();

	IteratorProcessor * iterator;

	TargetParameter* sourceTarget;
	BaseIteratorList * sourceList;
	std::unique_ptr<BaseComparator> comparator;
	var loadingComparatorData; //for dynamically created parameter, allows to reload comparator data after these are created

	Array<bool> validStates;

	void setupIterator(IteratorProcessor* iterator);

	void setValidState(int index, bool force);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void setSourceList(BaseIteratorList* list);
	void comparatorValidationChanged(BaseComparator*, int interationIndex) override;

	void forceCheck() override;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	class IterativeConditionListener
	{
	public:
		virtual ~IterativeConditionListener() {}
		virtual void iterativeConditionValidationChanged(IterativeCondition * c, const IterativeContext& context) {}
	};

	ListenerList<IterativeConditionListener> iterativeConditionListeners;
	void addIterativeConditionListener(IterativeConditionListener* newListener) { iterativeConditionListeners.add(newListener); }
	void removeIterativeConditionListener(IterativeConditionListener* listener) { iterativeConditionListeners.remove(listener); }


	InspectableEditor* getEditor(bool isRoot) override;
	String getTypeString() const override { return IterativeCondition::getTypeStringStatic(); }
	static String getTypeStringStatic() { return "From Iterator List"; }
};