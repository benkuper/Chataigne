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
	channel = addIntParameter("Channel", "DMX Channel", 1, 0, 512);
	value = addIntParameter("Value", "DMX Value", 0, 0, 255);
	targetMappingParameter = value;
}

DMXCommand::~DMXCommand()
{

}

void DMXCommand::trigger()
{
	dmxModule->sendDMXValue(channel->intValue(), value->intValue());
}
