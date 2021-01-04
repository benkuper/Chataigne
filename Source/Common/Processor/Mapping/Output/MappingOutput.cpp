/*
  ==============================================================================

    MappingOutput.cpp
    Created: 28 Oct 2016 8:12:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingOutput.h"
#include "Common/Command/BaseCommand.h"
 

MappingOutput::MappingOutput(Multiplex * multiplex) :
	BaseCommandHandler("MappingOutput",CommandContext::MAPPING, nullptr, multiplex)
{
	isSelectable = false;
}

MappingOutput::~MappingOutput()
{
}

void MappingOutput::setOutputType(Controllable::Type type)
{
	outputType = type;
	if (command != nullptr && command->context == CommandContext::MAPPING) command->setMappingValueType(outputType);
}

void MappingOutput::setCommand(CommandDefinition * cd)
{
	BaseCommandHandler::setCommand(cd);
	if (command != nullptr && command->context == CommandContext::MAPPING) command->setMappingValueType(outputType);
}

void MappingOutput::setValue(var value, int multiplexIndex)
{
	if (!enabled->boolValue()) return;
	if(command != nullptr) command->setValue(value, multiplexIndex);
}