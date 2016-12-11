/*
  ==============================================================================

    Automation.h
    Created: 11 Dec 2016 1:21:37pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATION_H_INCLUDED
#define AUTOMATION_H_INCLUDED

#include "AutomationKey.h"
#include "BaseManager.h"

class Automation :
	public BaseManager<AutomationKey>
{
public:
	Automation();
	virtual ~Automation();

	float valueMax;
	float positionMax;


	AutomationKey * addItem(const float position, const float value);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Automation)
};



#endif  // AUTOMATION_H_INCLUDED
