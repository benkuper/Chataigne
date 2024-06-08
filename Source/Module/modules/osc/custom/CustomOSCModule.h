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
	
	EnumParameter* colorMode;
	EnumParameter* boolMode;
	Trigger* clearValues;

	HashMap<String, WeakReference<Controllable>, DefaultHashFunctions, CriticalSection> controllableAddressMap;
	bool hierarchyStructureSwitch;

	OSCHelpers::ColorMode getColorMode() override;
	OSCHelpers::BoolMode getBoolMode() override;

	void processMessageInternal(const OSCMessage &msg) override;

	Array<WeakReference<Controllable>> getMatchingControllables(const OSCAddressPattern& address);
	void updateControllableAddressMap();

	void childStructureChanged(ControllableContainer * cc) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static void showMenuAndCreateValue(ControllableContainer * container);

	void afterLoadJSONDataInternal() override;

	static CustomOSCModule * create() { return new CustomOSCModule(); }
	virtual String getDefaultTypeString() const override { return "OSC"; }

	virtual void setupModuleFromJSONData(var data) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCModule)
};
