/*
  ==============================================================================

    Processor.h
    Created: 15 Oct 2017 1:24:47pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ProcessorUI;

class Processor :
	public BaseItem
{
public:
	enum ProcessorType { ACTION, MAPPING };
	
	Processor(const String &name = "Processor", bool canBeDisabled = true);
	virtual ~Processor();

	ProcessorType type;
	
	bool forceDisabled;

	virtual void setForceDisabled(bool value, bool force = false);


	class ProcessorEvent {
	public:
		enum Type { FORCE_DISABLED_CHANGED };
		ProcessorEvent(Type type, Processor * p) : type(type), processor(p) {}
		Type type;
		Processor * processor;
	};
	QueuedNotifier<ProcessorEvent> processorAsyncNotifier;
	typedef QueuedNotifier<ProcessorEvent>::Listener AsyncListener;


	void addAsyncProcessorListener(AsyncListener* newListener) { processorAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedProcessorListener(AsyncListener* newListener) { processorAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncProcessorListener(AsyncListener* listener) { processorAsyncNotifier.removeListener(listener); }

	//UI
	virtual ProcessorUI * getUI();
};