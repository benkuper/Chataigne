/*
  ==============================================================================

    PJLinkModule.cpp
    Created: 5 Jan 2018 3:41:58pm
    Author:  Ben

  ==============================================================================
*/

#include "PJLinkModule.h"
#include "../../common/streaming/commands/SendStreamStringCommand.h"

PJLinkModule::PJLinkModule() :
	TCPModule(getDefaultTypeString(), 4352)
{
	alwaysShowValues = true;
	includeValuesInSave = true;

	setAutoAddAvailable(false);
	streamingType->setValueWithData(LINES);

	powerStatus = valuesCC.addEnumParameter("Power Status", "Status of the projector");
	powerStatus->addOption("Not set", -1)->addOption("Powering OFF", "0")->addOption("Powering ON", "1")->addOption("Cooling", "2")->addOption("Warming Up", "3")->addOption("Unavailable Time", "ERR3")->addOption("Projector / Display Failure", "ERR4");
	
	shutterVideoStatus = valuesCC.addBoolParameter("Shutter Video Status", "Is the projector's shutter on", false);
	shutterAudioStatus = valuesCC.addBoolParameter("Shutter Audio Status", "Is the projector's shutter on", false);

	defManager.add(getBasePJCommand("", "Power On", "%1POWR 1", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Power Off", "%1POWR 0", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Power Status Request", "%1POWR ?", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Shutter Video On", "%1AVMT 11", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Shutter Video Off", "%1AVMT 10", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Shutter Audio On", "%1AVMT 21", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Shutter Audio Off", "%1AVMT 20", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Shutter Video and Audio On", "%1AVMT 31", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Shutter Video and Audio Off", "%1AVMT 30", CommandContext::ACTION));
	defManager.add(getBasePJCommand("", "Shutter Status Request", "%1AVMT ?", CommandContext::ACTION));
}

CommandDefinition * PJLinkModule::getBasePJCommand(const String & menu, const String & commandName, const String & command, CommandContext context)
{
	return CommandDefinition::createDef(this, menu, commandName, &SendStreamStringCommand::create, context)->addParam("fixedValue", command)->addParam("forceCR", true)->addParam("forceNL", false);

}

void PJLinkModule::processDataLineInternal(const String & message)
{
	if (message.contains("%1POWR="))
	{
		String status = message.substring(7);
		if(status != "OK") powerStatus->setValueWithData(status);
	}else if (message.contains("%1AVMT="))
	{
		bool isVideo = message[7] == '1' || message[7] == '3';
		bool isAudio = message[7] == '2' || message[7] == '3';
		bool isOn = message[8] == '1';

		if (isVideo) shutterVideoStatus->setValue(isOn);
		if (isAudio) shutterAudioStatus->setValue(isOn);
	}
}
