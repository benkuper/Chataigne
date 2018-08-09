/*
  ==============================================================================

    CustomOSCModule.h
    Created: 29 Oct 2016 7:07:54pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CUSTOMOSCMODULE_H_INCLUDED
#define CUSTOMOSCMODULE_H_INCLUDED

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

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static CustomOSCModule * create() { return new CustomOSCModule(); }
	virtual String getDefaultTypeString() const override { return "OSC"; }

	virtual void setupModuleFromJSONData(var data) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCModule)
};

#endif  // CUSTOMOSCMODULE_H_INCLUDED
