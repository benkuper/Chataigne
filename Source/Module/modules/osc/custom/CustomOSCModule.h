/*
  ==============================================================================

    CustomOSCModule.h
    Created: 29 Oct 2016 7:07:54pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class CustomOSCModule :
	public OSCModule
{
public:
	CustomOSCModule();
	~CustomOSCModule() {}

	BoolParameter * autoAdd;
	BoolParameter * splitArgs;
	BoolParameter * useHierarchy;
	BoolParameter * autoFeedback;
	
	enum ColorMode { ColorRGBA, Float3, Float4 };
	EnumParameter* colorMode;


	void processMessageInternal(const OSCMessage &msg) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static void showMenuAndCreateValue(ControllableContainer * container);

	void addColorArgumentToMessage(OSCMessage& m, const Colour& c);

	static CustomOSCModule * create() { return new CustomOSCModule(); }
	virtual String getDefaultTypeString() const override { return "OSC"; }

	virtual void setupModuleFromJSONData(var data) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCModule)
};
