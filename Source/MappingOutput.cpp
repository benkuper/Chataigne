/*
  ==============================================================================

    MappingOutput.cpp
    Created: 28 Oct 2016 8:12:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingOutput.h"
#include "BaseCommand.h"
#include "Engine.h"

MappingOutput::MappingOutput() :
	BaseCommandHandler("MappingOutput")
{
	isSelectable = false;
}

MappingOutput::~MappingOutput()
{
}

void MappingOutput::setValue(var value)
{
	if(command != nullptr) command->setValue(value);
}