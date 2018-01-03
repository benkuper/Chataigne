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
	StandardCondition(var params);
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

	String getTypeString() const override { return "From Input Value"; }
	static StandardCondition * create(var params) { return new StandardCondition(params); }

};


#endif  // STANDARDCONDITION_H_INCLUDED
