/*
  ==============================================================================

	Processor.cpp
	Created: 15 Oct 2017 1:24:47pm
	Author:  Ben

  ==============================================================================
*/

#include "Processor.h"
#include "ProcessorUI.h"

Processor::Processor(const String & name, bool canBeDisabled) :
	BaseItem(name, canBeDisabled),
	forceDisabled(false)
{
}

Processor::~Processor()
{

}

void Processor::setForceDisabled(bool value)
{
	if (forceDisabled == value) return;
	forceDisabled = value;

	//todo disabled conditions for better performance (no computation) when disabled
}

ProcessorUI * Processor::getUI()
{
	return new ProcessorUI(this);
}

