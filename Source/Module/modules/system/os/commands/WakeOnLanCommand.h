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
	WakeOnLanCommand(OSModule * _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~WakeOnLanCommand();

	OSModule * osModule;

	Trigger * autoSet;
	StringParameter * macAddress;
	MACAddress mac;

	void generateMacFromString();

	void onContainerParameterChanged(Parameter * p) override;
	void triggerInternal(int multiplexIndex) override;

	InspectableEditor * getEditor(bool isRoot) override;

	static WakeOnLanCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new WakeOnLanCommand((OSModule *)module, context, params, multiplex); }

};
