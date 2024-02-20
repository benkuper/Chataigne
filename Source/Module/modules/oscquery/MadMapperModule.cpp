#include "MadMapperModule.h"
/*
  ==============================================================================

	MadMapperModule.cpp
	Created: 28 Feb 2019 10:33:28pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

MadMapperModule::MadMapperModule() :
	GenericOSCQueryModule("MadMapper", 8010)
{
	oscReceivePort = sendCC->addIntParameter("OSC Feedback Port", "OSC Receive Port", 8011, 0, 65535, false);
	oscReceivePort->canBeDisabledByUser = true;
}

MadMapperModule::~MadMapperModule()
{
}

void MadMapperModule::setupReceiver()
{
	receiver.disconnect();
	if (!enabled->boolValue()) return;
	if (!oscReceivePort->enabled) return;

	bool result = receiver.connect(oscReceivePort->intValue());
	if (result)	NLOG(niceName, "Now listening OSC feedback messages on " << oscReceivePort->intValue());
	else NLOGWARNING(niceName, "Failed to listen OSC feedback messages on " << oscReceivePort->intValue());
}

void MadMapperModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	GenericOSCQueryModule::onControllableFeedbackUpdateInternal(cc, c);
	if (c == oscReceivePort) setupReceiver();
}

void MadMapperModule::onControllableStateChanged(Controllable* c)
{
	if (c == oscReceivePort) setupReceiver();
}