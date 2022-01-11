/*
  ==============================================================================

	DMXCommand.cpp
	Created: 6 Apr 2017 10:22:35pm
	Author:  Ben

  ==============================================================================
*/

DMXCommand::DMXCommand(DMXModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	dmxModule(_module),
	byteOrder(nullptr),
	channel(nullptr),
	channel2(nullptr),
	value(nullptr),
	colorParam(nullptr),
    remapTarget(0),
	remap01To255(nullptr)
{

	dmxAction = (DMXAction)(int)params.getProperty("action", 0);

	if (dmxAction == SET_VALUE_16BIT)
	{
		byteOrder = addEnumParameter("Byte Order", "Byte ordering, most devices use MSB");
		byteOrder->addOption("MSB", DMXByteOrder::MSB)->addOption("LSB", DMXByteOrder::LSB);
	}


	switch (dmxAction)
	{

	case SET_VALUE:
	case SET_VALUE_16BIT:
		channel = addIntParameter("Channel", "DMX Channel", 1, 1, 512);
		value = addIntParameter("Value", "DMX Value", 0, 0, dmxAction == SET_VALUE_16BIT ? 65535 : 255);
		linkParamToMappingIndex(value, 0);
		break;

	case SET_RANGE:
		channel = addIntParameter("Start Channel", "First DMX Channel", 1, 1, 512);
		channel2 = addIntParameter("End Channel", "Last DMX Channel (inclusive)", 4, 1, 512);
		value = addIntParameter("Value", "DMX Value", 0, 0, 255);
		linkParamToMappingIndex(value, 0);
		break;

	case COLOR:
		channel = addIntParameter("Start Channel", "DMX Channel", 1, 1, 512);
		colorParam = new ColorParameter("Color", "DMX Color");
		addParameter(colorParam);
		linkParamToMappingIndex(colorParam, 0);
		break;

	case BLACK_OUT:
		break;

	case SET_ALL:
		value = addIntParameter("Value", "DMX Value", 0, 0, dmxAction == SET_VALUE_16BIT ? 65535 : 255);
		linkParamToMappingIndex(value, 0);
		break;

	case SET_CUSTOM:
	{
		channel = addIntParameter("Start Channel", "First DMX Channel", 1, 1, 512);
		setUseCustomValues(true);
		customValuesManager->allowedTypes.add(Controllable::INT);
		customValuesManager->addBaseManagerListener(this);
	}
	break;
	}


	if (context == MAPPING && (dmxAction == SET_VALUE || dmxAction == SET_VALUE_16BIT || dmxAction == SET_RANGE || dmxAction == SET_CUSTOM))
	{
		remapTarget = dmxAction == SET_VALUE_16BIT ? 65535 : 255;
		remap01To255 = addBoolParameter("Remap to 0-" + String(remapTarget), "If checked, this will automatically remap values from 0-1 to 0-" + String(remapTarget), false);
	}
}

DMXCommand::~DMXCommand()
{

}

void DMXCommand::setValue(var val, int multiplexIndex)
{
	//DBG("Value val " << (int)val.isArray() << " / " << val.size()) ;

	float mapFactor = (remap01To255 != nullptr && (int)getLinkedValue(remap01To255, multiplexIndex) == 1) ? remapTarget : 1;
	var newVal;

	if (val.isArray())
	{
		for (int i = 0; i < val.size(); ++i)
		{
			//DBG("Value remap to " << i << " / " << (float)val[i]);
			newVal.append(((float)val[i]) * mapFactor);
		}
	}
	else
	{
		newVal = ((float)val) * mapFactor;
	}

	if (newVal.isArray())
	{
		//DBG("Val is array ");
		//for(int i=0;i<newVal.size();++i) DBG("new val [" << i << "]/ " << (float)newVal[i]);
	}
	BaseCommand::setValue(newVal, multiplexIndex);
}

void DMXCommand::triggerInternal(int multiplexIndex)
{
	BaseCommand::triggerInternal(multiplexIndex);

	switch (dmxAction) 
	{

	case SET_VALUE:
		dmxModule->sendDMXValue(getLinkedValue(channel, multiplexIndex), getLinkedValue(value, multiplexIndex));
		break;

	case SET_VALUE_16BIT:
	{
		int val = getLinkedValue(value, multiplexIndex);
		int v1 = val & 0xFF;
		int v2 = val >> 8 & 0xFF;
		bool msb = byteOrder->getValueDataAsEnum<DMXByteOrder>() == DMXByteOrder::MSB;
		
		int dmxV1 = msb ? v2 : v1;
		int dmxV2 = msb ? v1 : v2;

		Array<int> values(dmxV1, dmxV2);
		dmxModule->sendDMXValues(getLinkedValue(channel, multiplexIndex), values);
	}
	break;

	case SET_RANGE:
	case SET_ALL:
	{
		Array<int> values;
		int chVal = (int)getLinkedValue(channel, multiplexIndex);
		int numValues = dmxAction == SET_ALL ? 512 : jmax((int)getLinkedValue(channel2, multiplexIndex) - chVal + 1, 0);
		int startChannel = dmxAction == SET_ALL ? 1 : chVal;
		values.resize(numValues);
		values.fill((int)getLinkedValue(value, multiplexIndex));
		dmxModule->sendDMXValues(startChannel, values);
	}
	break;

	case SET_CUSTOM:
	{
		Array<int> values;
		for (auto& i : customValuesManager->items)
		{
			values.add(i->getLinkedValue(multiplexIndex));
		}
		dmxModule->sendDMXValues(getLinkedValue(channel, multiplexIndex), values);
	}
	break;


	case COLOR:
	{
		var val = getLinkedValue(colorParam, multiplexIndex);
		Array<int> values;
		for (int i = 0; i < 3; ++i) values.add((int)((float)val[i] * 255));
		dmxModule->sendDMXValues(getLinkedValue(channel, multiplexIndex), values);
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

void DMXCommand::itemAdded(CustomValuesCommandArgument* a)
{
	BaseCommand::itemAdded(a);
	a->param->setRange(0, 255);
}
