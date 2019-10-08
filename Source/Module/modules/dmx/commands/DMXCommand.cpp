/*
  ==============================================================================

	DMXCommand.cpp
	Created: 6 Apr 2017 10:22:35pm
	Author:  Ben

  ==============================================================================
*/

#include "DMXCommand.h"

DMXCommand::DMXCommand(DMXModule* _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	dmxModule(_module),
	byteOrder(nullptr),
	channel(nullptr),
	channel2(nullptr),
	value(nullptr),
	colorParam(nullptr),
	remap01To255(nullptr)
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
		channel = addIntParameter("Channel", "DMX Channel", 1, 1, 512);
		value = addIntParameter("Value", "DMX Value", 0, 0, dmxAction == SET_VALUE_16BIT ? 65535 : 255);
		addTargetMappingParameterAt(value, 0);
		break;

	case SET_RANGE:
		channel = addIntParameter("Start Channel", "First DMX Channel", 1, 1, 512);
		channel2 = addIntParameter("End Channel", "Last DMX Channel (inclusive)", 4, 1, 512);
		value = addIntParameter("Value", "DMX Value", 0, 0, 255);
		addTargetMappingParameterAt(value, 0);
		break;

	case COLOR:
		channel = addIntParameter("Start Channel", "DMX Channel", 1, 1, 512);
		colorParam = new ColorParameter("Color", "DMX Color");
		addParameter(colorParam);
		addTargetMappingParameterAt(colorParam, 0);
		break;

	case BLACK_OUT:
		break;

	case SET_ALL:
		value = addIntParameter("Value", "DMX Value", 0, 0, dmxAction == SET_VALUE_16BIT ? 65535 : 255);
		addTargetMappingParameterAt(value, 0); 
		break;

	case SET_CUSTOM:
	{
		channel = addIntParameter("Start Channel", "First DMX Channel", 1, 1, 512);
		customValuesManager.reset(new CustomValuesCommandArgumentManager(context == MAPPING));
		customValuesManager->allowedTypes.add(Controllable::INT); 
		addChildControllableContainer(customValuesManager.get());
		customValuesManager->addArgumentManagerListener(this);
		customValuesManager->addBaseManagerListener(this);
	}
	break;
	}


	if (context == MAPPING && (dmxAction == SET_VALUE || dmxAction == SET_VALUE_16BIT || dmxAction == SET_RANGE || dmxAction == SET_CUSTOM))
	{
		remap01To255 = addBoolParameter("Remap to 0-255", "If checked, this will automatically remap values from 0-1 to 0-255", false);
	}
}

DMXCommand::~DMXCommand()
{

}

void DMXCommand::setValue(var val)
{
	DBG("Value val " << (int)val.isArray() << " / " << val.size()) ;

	float mapFactor = (remap01To255 != nullptr && remap01To255->boolValue()) ? 255 : 1;
	var newVal;

	if (val.isArray())
	{
		for (int i = 0; i < val.size(); i++)
		{
			DBG("Value remap to " << i << " / " << (float)val[i]);
			newVal.append(((float)val[i]) * mapFactor);
		}
	}
	else
	{
		newVal = ((float)val) * mapFactor;
	}

	if (newVal.isArray())
	{
		DBG("Val is array ");
		for(int i=0;i<newVal.size();i++) DBG("new val [" << i << "]/ " << (float)newVal[i]);
	}
	BaseCommand::setValue(newVal);
}

void DMXCommand::triggerInternal()
{
	BaseCommand::triggerInternal();

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
	case SET_ALL:
	{
		Array<int> values;
		int numValues = dmxAction == SET_ALL ? 512 : channel2->intValue() - channel->intValue() + 1;
		values.resize(numValues);
		values.fill(value->intValue());
		dmxModule->sendDMXValues(0, values);
	}
	break;

	case SET_CUSTOM:
	{
		Array<int> values;
		for (auto& i : customValuesManager->items)
		{
			values.add(i->param->intValue());
		}
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

	case BLACK_OUT:
	{
		Array<int> values;
		values.resize(512);
		values.fill(0);
		dmxModule->sendDMXValues(1, values);
	}
	break;

	
	}
}

var DMXCommand::getJSONData()
{
	var data = BaseCommand::getJSONData();
	if(customValuesManager != nullptr) data.getDynamicObject()->setProperty("customValues", customValuesManager->getJSONData());
	return data;
}

void DMXCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);
	if(customValuesManager != nullptr) customValuesManager->loadJSONData(data.getProperty("customValues", var()), true);
}

void DMXCommand::useForMappingChanged(CustomValuesCommandArgument*)
{
	if (context != CommandContext::MAPPING) return;
	if (customValuesManager == nullptr) return;

	clearTargetMappingParameters();
	int index = 0;
	for (auto& item : customValuesManager->items)
	{
		if (item->useForMapping != nullptr && item->useForMapping->boolValue())
		{
			addTargetMappingParameterAt(item->param, index);
			index++;
		}
	}
}

void DMXCommand::itemAdded(CustomValuesCommandArgument* a)
{
	a->param->setRange(0, 255);
}
