/*
  ==============================================================================

	PosiStageNetCommand.cpp
	Created: 17 May 2022 12:04:47am
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

PosiStageNetCommand::PosiStageNetCommand(PosiStageNetModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	psnModule(_module)
{
	action = (Action)(int)params.getProperty("action", SET_POSITION); 

	slot = addIntParameter("Slot", "", 0, 0, 127);
	position = addPoint3DParameter("Position", "Position to send");
	linkParamToMappingIndex(position, 0);
}

PosiStageNetCommand::~PosiStageNetCommand()
{
}


void PosiStageNetCommand::triggerInternal(int multiplexIndex)
{
	Vector3D<float> p;
	var v = getLinkedValue(position, multiplexIndex);
	if (v.size() >= 3) p = Vector3D<float>(v[0], v[1], v[2]);

	if (action == SET_POSITION)
	{
		psnModule->setPositionAt(getLinkedValue(slot, multiplexIndex), p);
	}
}