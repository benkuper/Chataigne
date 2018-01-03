/*
  ==============================================================================

    ProcessorManager.h
    Created: 15 Oct 2017 1:24:52pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#ifndef PROCESSORMANAGER_H_INCLUDED
#define PROCESSORMANAGER_H_INCLUDED

#include "Processor.h"

class ProcessorManager :
	public BaseManager<Processor>
{
public:
	ProcessorManager(const String &name);
	~ProcessorManager();

	bool forceDisabled;
	void setForceDisabled(bool value);
	void addItemInternal(Processor * item, var data) override;

	Factory<Processor> factory;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorManager)

};

#endif  // PROCESSORMANAGER_H_INCLUDED