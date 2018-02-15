/*
  ==============================================================================

    MappingOutput.cpp
    Created: 28 Oct 2016 8:12:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingOutput.h"
#include "Common/Command/BaseCommand.h"
 

MappingOutput::MappingOutput() :
	BaseCommandHandler("MappingOutput",CommandContext::MAPPING)
{
	isSelectable = false;
}

MappingOutput::~MappingOutput()
{
}

void MappingOutput::setValue(var value)
{
	if (!enabled->boolValue()) return;
	if(command != nullptr) command->setValue(value);
}