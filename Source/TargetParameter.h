/*
  ==============================================================================

    TargetParameter.h
    Created: 2 Nov 2016 5:00:04pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef TARGETPARAMETER_H_INCLUDED
#define TARGETPARAMETER_H_INCLUDED

#include "StringParameter.h"
#include "ControllableContainer.h"

class TargetParameterUI;

class TargetParameter : 
	public StringParameter,
	public ControllableContainerListener,
	public Controllable::Listener
{
public:
	
	TargetParameter(const String &niceName, const String &description, const String &initialValue,  WeakReference<ControllableContainer> rootReference = nullptr, bool enabled = true);
	~TargetParameter();

	enum TargetType {CONTAINER, CONTROLLABLE};
	TargetType targetType;

	bool useGhosting;
	String ghostValue;

	WeakReference<ControllableContainer> rootContainer;
	
	WeakReference<Controllable> target;
	WeakReference<ControllableContainer> targetContainer;
	
	void setGhostValue(const String &ghostVal);

	void setValueFromTarget(Controllable *);
	void setValueFromTarget(ControllableContainer *);

	void setValueInternal(var &) override;

	void setTarget(WeakReference<Controllable>);
	void setTarget(WeakReference<ControllableContainer>);

	void childStructureChanged(ControllableContainer *) override;
	void controllableRemoved(Controllable *) override;
	void controllableContainerRemoved(ControllableContainer *) override;

	TargetParameterUI * createTargetUI(TargetParameter * target = nullptr);
	ControllableUI* createDefaultUI(Controllable * targetControllable = nullptr) override;


	static StringParameter * create() { return new StringParameter("New TargetParameter", "", ""); }
	virtual String getTypeString() const override { return "Target"; }
};



#endif  // TARGETPARAMETER_H_INCLUDED
