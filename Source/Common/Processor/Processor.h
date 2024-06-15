/*
  ==============================================================================

    Processor.h
    Created: 15 Oct 2017 1:24:47pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ProcessorUI;

class Processor :
	public BaseItem
{
public:
	enum ProcessorType { ACTION, MAPPING, MULTIPLEX, CONDUCTOR };
	
	Processor(const String &name = "Processor", bool canBeDisabled = true, bool canHaveScripts = false);
	virtual ~Processor();

	ProcessorType type;
	
	bool forceDisabled;

	virtual  void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void setForceDisabled(bool value, bool force = false, bool fromActivation = false);
	virtual void updateDisables(bool force = false, bool fromActivation = false) {}

	DECLARE_ASYNC_EVENT(Processor, Processor, processor, { FORCE_DISABLED_CHANGED }, EVENT_ITEM_CHECK)

	virtual ProcessorUI * getUI();
};