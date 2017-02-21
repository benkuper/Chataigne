/*
  ==============================================================================

    StandardCondition.h
    Created: 21 Feb 2017 11:37:26am
    Author:  Ben

  ==============================================================================
*/

#ifndef STANDARDCONDITION_H_INCLUDED
#define STANDARDCONDITION_H_INCLUDED

#include "Condition.h"

class StandardCondition :
	public Condition
{
public:
	StandardCondition();
	~StandardCondition();

	TargetParameter * sourceTarget;
	WeakReference<Controllable> sourceControllable;
	ScopedPointer<BaseComparator> comparator;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void setSourceControllable(WeakReference<Controllable> c);
	void comparatorValidationChanged(BaseComparator *) override;

	void onContainerParameterChangedInternal(Parameter * p) override;

	InspectableEditor * getEditor(bool isRoot) override;


	String getTypeString() const { return "Standard"; }
	static StandardCondition * create() { return new StandardCondition(); }

};


#endif  // STANDARDCONDITION_H_INCLUDED
