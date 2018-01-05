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
	editorIsCollapsed = true;
}

Processor::~Processor()
{

}

void Processor::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
}

ProcessorUI * Processor::getUI()
{
	return new ProcessorUI(this);
}

