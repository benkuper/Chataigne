/*
  ==============================================================================

    OSCCommand.h
    Created: 3 Nov 2016 10:47:46am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class OSCModule;

class OSCCommand :
	public BaseCommand
{
public:
	OSCCommand(IOSCSenderModule * _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	virtual ~OSCCommand();

	IOSCSenderModule* oscModule;
	StringParameter * address;
	ParamLinkContainer argumentsContainer;

	String addressModel;
	bool rebuildAddressOnParamChanged;

	virtual void rebuildAddress();
	virtual String getTargetAddress(int multiplexIndex = 0);
	virtual String getTargetAddressInternal(const String& targetAddress, int multiplexIndex = 0) { return targetAddress; }

	void buildArgsAndParamsFromData(var data);

	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataInternal(var data) override;

	void controllableAdded(Controllable* c) override;

	void onContainerParameterChanged(Parameter * p) override;

	virtual void updateMappingInputValue(var value, int multiplexIndex) override;
	virtual void setInputNamesFromParams(Array<WeakReference<Parameter>> outParams) override;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand * create(ControllableContainer * cc, CommandContext context, var params, Multiplex * multiplex) { return new OSCCommand(dynamic_cast<IOSCSenderModule*>(cc), context, params, multiplex); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OSCCommand)
};