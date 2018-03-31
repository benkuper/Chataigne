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
	WakeOnLanCommand(OSModule * _module, CommandContext context, var params);
	~WakeOnLanCommand();

	OSModule * osModule;

	StringParameter * macAddress;
	MACAddress mac;

	void generateMacFromString();

	void onContainerParameterChanged(Parameter * p) override;
	void trigger() override;

	static WakeOnLanCommand * create(ControllableContainer * module, CommandContext context, var params) { return new WakeOnLanCommand((OSModule *)module, context, params); }

};