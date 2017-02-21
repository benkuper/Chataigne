/*
  ==============================================================================

    ConditionGroup.h
    Created: 21 Feb 2017 11:27:53am
    Author:  Ben

  ==============================================================================
*/

#ifndef CONDITIONGROUP_H_INCLUDED
#define CONDITIONGROUP_H_INCLUDED

#include "Condition.h"
#include "ConditionManager.h"

class ConditionGroup :
	public Condition
{
public:
	ConditionGroup();
	~ConditionGroup();

	ScopedPointer<ConditionManager> manager;


	String getTypeString() const { return "Group"; }
	static ConditionGroup * create() { return new ConditionGroup(); }
};



#endif  // CONDITIONGROUP_H_INCLUDED
