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

class DMXCommand :
	public BaseCommand
{
public:
	enum DMXAction { SET_VALUE, SET_RANGE, COLOR, CLEAR_ALL, SET_VALUE_16BIT };

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
	void trigger() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params) { return new DMXCommand((DMXModule *)module, context, params); }

	
};



#endif  // DMXCOMMAND_H_INCLUDED
