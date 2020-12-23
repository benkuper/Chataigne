/*
  ==============================================================================

    MappingOutput.cpp
    Created: 28 Oct 2016 8:12:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingOutput.h"
#include "Common/Command/BaseCommand.h"
 

MappingOutput::MappingOutput(IteratorProcessor * iterator) :
	BaseCommandHandler("MappingOutput",CommandContext::MAPPING, nullptr, iterator)
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

void MappingOutput::setValue(var value, int iterationIndex)
{
	if (!enabled->boolValue()) return;
	if(command != nullptr) command->setValue(value, iterationIndex);
}