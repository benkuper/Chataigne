/*
  ==============================================================================

    CustomOSCModule.h
    Created: 29 Oct 2016 7:07:54pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../OSCModule.h"

class CustomOSCModule :
	public OSCModule
{
public:
	CustomOSCModule();
	~CustomOSCModule() {}

	BoolParameter * autoAdd;
	BoolParameter * splitArgs;
	BoolParameter * autoFeedback;

	void processMessageInternal(const OSCMessage &msg) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static void showMenuAndCreateValue(ControllableContainer * container);

	static CustomOSCModule * create() { return new CustomOSCModule(); }
	virtual String getDefaultTypeString() const override { return "OSC"; }

	virtual void setupModuleFromJSONData(var data) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCModule)
};
