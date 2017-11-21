/*
  ==============================================================================

    AutomationKey.h
    Created: 11 Dec 2016 1:22:20pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATIONKEY_H_INCLUDED
#define AUTOMATIONKEY_H_INCLUDED

#include "Easing.h"

class AutomationKey :
	public BaseItem
{
public:
	AutomationKey();
	virtual ~AutomationKey();

	FloatParameter * position; //depends on parent automation
	FloatParameter * value; //0-1

	EnumParameter * easingType;

	ScopedPointer<Easing> easing;

	void setEasing(Easing::Type t);

	float getValue(AutomationKey * nextKey, const float &position);

	void setSelectionManager(InspectableSelectionManager * ism) override;

	void onContainerParameterChangedInternal(Parameter *) override;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AutomationKey)
};



#endif  // AUTOMATIONKEY_H_INCLUDED
