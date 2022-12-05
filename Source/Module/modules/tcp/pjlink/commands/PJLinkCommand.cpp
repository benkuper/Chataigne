/*
  ==============================================================================

	PJLinkCommand.cpp
	Created: 1 Nov 2022 12:23:25pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

PJLinkCommand::PJLinkCommand(PJLinkModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	pjlinkModule(_module),
	op(nullptr),
	message(nullptr)
{

	autoLoadPreviousCommandData = true;

	action = (ActionType)(int)params.getProperty("action", CUSTOM);
	String cmdMessage = params.getProperty("message", "");

	projectorID = addIntParameter("Projector ID", "The projector id to send to, leave disable to send to all projectors.", 1, 1, 100, false);
	projectorID->canBeDisabledByUser = true;

	if (action != CUSTOM)
	{
		op = addEnumParameter("Action", "Action to do for this command");
		if (action == POWER)
		{
			prefix = "%1POWR ";
			op->addOption("Power On", "1")->addOption("Power Off", "0");
		}
		else if (action == SHUTTER)
		{
			prefix = "%1AVMT ";
			op->addOption("Video On", "10")->addOption("Video Off", "11")->addOption("Audio On", "20")->addOption("Audio Off", "21")->addOption("Video & Audio On", "30")->addOption("Video & Audio Off", "31");
		}

		op->addOption("Status Request", "?");
	}
	else
	{
		message = addStringParameter("Message", "The data to send", cmdMessage);
		//message->setControllableFeedbackOnly(true);
	}


}

PJLinkCommand::~PJLinkCommand()
{
}


void PJLinkCommand::triggerInternal(int multiplexIndex)
{
	int id = projectorID->enabled ? (int)getLinkedValue(projectorID, multiplexIndex) : -1;

	String msg = "";

	if (action == CUSTOM) msg = getLinkedValue(message, multiplexIndex).toString();
	else msg = prefix + getLinkedValue(op, multiplexIndex).toString();
	pjlinkModule->sendMessageToClient(msg, id);
}