/*
  ==============================================================================

    WakeOnLanCommand.h
    Created: 31 Mar 2018 11:06:18am
    Author:  Ben

  ==============================================================================
*/


#pragma once
#include "Common/Command/BaseCommand.h"
#include "../OSModule.h"

class WakeOnLanCommand :
	public BaseCommand
{
public:
	WakeOnLanCommand(OSModule * _module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	~WakeOnLanCommand();

	OSModule * osModule;

	Trigger * autoSet;
	StringParameter * macAddress;
	MACAddress mac;

	void generateMacFromString();

	void onContainerParameterChanged(Parameter * p) override;
	void triggerInternal(int iterationIndex) override;

	InspectableEditor * getEditor(bool isRoot) override;

	static WakeOnLanCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new WakeOnLanCommand((OSModule *)module, context, params, iterator); }

};
