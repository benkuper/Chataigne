/*
  ==============================================================================

	DMXCommand.cpp
	Created: 6 Apr 2017 10:22:35pm
	Author:  Ben

  ==============================================================================
*/

#include "DMXCommand.h"

DMXCommand::DMXCommand(DMXModule * _module, CommandContext context, var params) :
	BaseCommand(_module,context,params),
	dmxModule(_module)
{
	
	dmxAction = (DMXAction)(int)params.getProperty("action", 0);

	if (dmxAction == SET_VALUE_16BIT)
	{
		byteOrder = addEnumParameter("Byte Order", "Byte ordering, most devices use MSB");
		byteOrder->addOption("MSB", DMXModule::MSB)->addOption("LSB", DMXModule::LSB);
	}

	switch (dmxAction)
	{
	
	case SET_VALUE:
	case SET_VALUE_16BIT:
		channel = addIntParameter("Channel", "DMX Channel", 1,1,512);
		value = addIntParameter("Value", "DMX Value", 0, 0, dmxAction == SET_VALUE_16BIT?65535:255);
		setTargetMappingParameterAt(value,0);
		break;

	case SET_RANGE:
		channel = addIntParameter("Start Channel", "First DMX Channel", 1,1,512);
		channel2 = addIntParameter("End Channel", "Last DMX Channel (inclusive)", 4, 1,512);
		value = addIntParameter("Value", "DMX Value", 0, 0, 255);
		setTargetMappingParameterAt(value,0);
		break;

	case COLOR:
		channel = addIntParameter("Start Channel", "DMX Channel", 1, 1,512);
		colorParam = new ColorParameter("Color", "DMX Color");
		addParameter(colorParam);
		setTargetMappingParameterAt(colorParam, 0);
		break;

	case CLEAR_ALL:
		break;
	}

	
}

DMXCommand::~DMXCommand()
{

}

void DMXCommand::trigger()
{
	BaseCommand::trigger();

	switch (dmxAction) 
	{

	case SET_VALUE:
		dmxModule->sendDMXValue(channel->intValue(), value->intValue());
		break;

	case SET_VALUE_16BIT:
	{
		int v1 = value->intValue() & 0xFF;
		int v2 = value->intValue() >> 8 & 0xFF;
		bool msb = byteOrder->getValueDataAsEnum<DMXModule::DMXByteOrder>() == DMXModule::MSB;
		
		int dmxV1 = msb ? v2 : v1;
		int dmxV2 = msb ? v1 : v2;

		Array<int> values(dmxV1, dmxV2);
		dmxModule->sendDMXValues(channel->intValue(), values);
	}
	break;

	case SET_RANGE:
	{
		Array<int> values;
		int numValues = channel2->intValue() - channel->intValue();
		values.resize(numValues);
		values.fill(value->intValue());
		dmxModule->sendDMXValues(channel->intValue(), values);
	}
	break;

	case COLOR:
	{
		Array<int> values;
		for (int i = 0; i < 3; i++) values.add((int)((float)colorParam->value[i] * 255));
		dmxModule->sendDMXValues(channel->intValue(), values);
	}
	break;

	case CLEAR_ALL:
	{
		Array<int> values;
		values.resize(512);
		values.fill(0);
		dmxModule->sendDMXValues(channel->intValue(), values);
	}
	break;
	}
	
}
