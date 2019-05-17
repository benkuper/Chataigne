/*
  ==============================================================================

    DMXCommand.h
    Created: 6 Apr 2017 10:22:35pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXCOMMAND_H_INCLUDED
#define DMXCOMMAND_H_INCLUDED

#include "Common/Command/BaseCommand.h"
#include "../DMXModule.h"
#include "Module/modules/common/commands/customvalues/CustomValuesCommandArgumentManager.h"

class DMXCommand :
	public BaseCommand,
	public CustomValuesCommandArgumentManager::ManagerListener,
	public CustomValuesCommandArgumentManager::BaseManager::Listener
{
public:
	enum DMXAction { SET_VALUE, SET_RANGE, COLOR, SET_VALUE_16BIT, BLACK_OUT, SET_ALL, SET_CUSTOM };

	DMXCommand(DMXModule * _module, CommandContext context, var params);
	~DMXCommand();

	DMXAction dmxAction;
	DMXModule * dmxModule;

	EnumParameter * byteOrder;

	IntParameter * channel;
	IntParameter * channel2;
	IntParameter * value;
	ColorParameter * colorParam;

	BoolParameter * remap01To255;

	void setValue(var value) override;
	void triggerInternal() override;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
	void useForMappingChanged(CustomValuesCommandArgument* a) override;

	void itemAdded(CustomValuesCommandArgument* a) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { return new DMXCommand((DMXModule *)module, context, params); }

	
};



#endif  // DMXCOMMAND_H_INCLUDED
