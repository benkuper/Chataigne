/*
  ==============================================================================

    DMXModule.cpp
    Created: 6 Apr 2017 10:22:10pm
    Author:  Ben

  ==============================================================================
*/

#include "DMXModule.h"
#include "DMXCommand.h"
//#include "DMXModuleEditor.h"

DMXModule::DMXModule() :
	Thread("dmxWrite"),
	SerialModule("DMX")
{
	defManager.add(CommandDefinition::createDef(this, "", "Send value", &DMXCommand::create));
	
	memset(dmxData, 0, 513 * sizeof(uint8));
}

DMXModule::~DMXModule()
{
	stopThread(100);
}

void DMXModule::sendDMXValue(int channel, int value)
{
	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Send DMX : " + String(channel) + " > " + String(value));
		
	}
	dmxData[channel+1] = (uint8)value;
}

void DMXModule::setCurrentPort(SerialDevice * device)
{
	SerialModule::setCurrentPort(device);

	if (port != nullptr)
	{
		DBG("Init DMX config");
		port->port->setBaudrate(250000);
		port->port->setBytesize(serial::eightbits);
		port->port->setStopbits(serial::stopbits_two);
		port->port->setParity(serial::parity_none);
		port->port->setFlowcontrol(serial::flowcontrol_none);
		port->port->setRTS(false);
		port->port->flush();
		startThread();

	}
}

/*
InspectableEditor * DMXModule::getEditor(bool isRoot)
{
	return Module::getEditor(this);// new SerialModuleEditor(this, isRoot);
}
*/

void DMXModule::run()
{
	while (!threadShouldExit())
	{

		if (port != nullptr && port->port->isOpen())
		{
			DBG("send " << (int)dmxData[0] << " / " << (int)dmxData[1] << " / " << (int)dmxData[2]);
			
			port->port->setBreak(true);
			port->port->setBreak(false);
			port->port->write(dmxData, 513);
		}
		sleep(23);
	}
	
}
