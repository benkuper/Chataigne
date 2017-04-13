/*
  ==============================================================================

    ConditionManager.h
    Created: 28 Oct 2016 8:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONDITIONMANAGER_H_INCLUDED
#define CONDITIONMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "Condition.h"

class ConditionManager :
	public BaseManager<Condition>,
	public Condition::ConditionListener
{
public:
	juce_DeclareSingleton(ConditionManager, true)

	ConditionManager(bool operatorOnSide = false);
	~ConditionManager();
	
	BoolParameter * isValid;

	enum ConditionOperator { AND, OR };
	EnumParameter * conditionOperator;
	bool operatorOnSide;

	void addItemFromData(var data,bool fromUndoableAction = false) override;

	void addItemInternal(Condition *, var data) override;
	void removeItemInternal(Condition *) override;

	void checkAllConditions();

	bool areAllConditionsValid();
	bool isAtLeastOneConditionValid();

	int getNumEnabledConditions();
	int getNumValidConditions();

	bool getIsValid(bool emptyIsValid);

	void conditionValidationChanged(Condition *) override;

	InspectableEditor * getEditor(bool isRoot) override;

	class ConditionManagerListener
	{
	public:
		virtual ~ConditionManagerListener() {}
		virtual void conditionManagerValidationChanged(ConditionManager *) {}
	};


	ListenerList<ConditionManagerListener> conditionManagerListeners;
	void addConditionManagerListener(ConditionManagerListener* newListener) { conditionManagerListeners.add(newListener); }
	void removeConditionManagerListener(ConditionManagerListener* listener) { conditionManagerListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionManager)

};


#endif  // CONDITIONMANAGER_H_INCLUDED
