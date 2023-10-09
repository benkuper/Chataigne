/*
  ==============================================================================

	CustomOSCCommand.h
	Created: 3 Nov 2016 12:41:23pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class CustomOSCCommand :
	public OSCCommand
{
public:
	CustomOSCCommand(IOSCSenderModule* module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~CustomOSCCommand();

	var lastValue;

	std::unique_ptr<CustomValuesCommandArgumentManager> wildcardsContainer;

	bool addressHasWildcards;
	Array<String> wildcardsMap;

	void triggerInternal(int multiplexIndex) override;

	void updateWildcardsMap(const String& address);

	virtual void onContainerParameterChanged(Parameter* p) override;

	void itemAdded(CustomValuesCommandArgument* i) override;
	void itemsAdded(Array<CustomValuesCommandArgument*> items) override;

	void updateMappingInputValue(var value, int multiplexIndex) override;
	void setInputNamesFromParams(Array<WeakReference<Parameter>> outParams) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static CustomOSCCommand* create(ControllableContainer* cc, CommandContext context, var params, Multiplex* multiplex) { return new CustomOSCCommand(dynamic_cast<IOSCSenderModule*>(cc), context, params, multiplex); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCCommand)
};
