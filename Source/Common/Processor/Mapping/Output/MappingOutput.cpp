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

void MappingOutput::setOutParams(Array<WeakReference<Parameter>> mOutParams, int multiplexIndex)
{
	outParams.ensureStorageAllocated(multiplexIndex + 1);
	outParams.set(multiplexIndex, mOutParams);
	
	updateCommandOutParams();
}

void MappingOutput::updateCommandOutParams()
{
	if (command != nullptr)
	{
		command->setInputNamesFromParams(outParams[0]);

		if (outParams[0].size() > 0)
		{
			if(outParams[0][0] != nullptr) command->setMappingValueType(outParams[0][0]->type);
		}
	}
}

void MappingOutput::setCommand(CommandDefinition * cd)
{
	BaseCommandHandler::setCommand(cd);
	if (command != nullptr && command->context == CommandContext::MAPPING)
	{
		updateCommandOutParams();
	}
}

void MappingOutput::setValue(var value, int multiplexIndex)
{
	if (!enabled->boolValue()) return;
	if(command != nullptr) command->setValue(value, multiplexIndex);
}