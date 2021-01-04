/*
  ==============================================================================

    GenericOSCQueryCommand.cpp
    Created: 28 Feb 2019 10:33:50pm
    Author:  bkupe

  ==============================================================================
*/

#include "GenericOSCQueryCommand.h"

GenericOSCQueryCommand::GenericOSCQueryCommand(GenericOSCQueryModule * oscQuerymodule, CommandContext context, var params, Multiplex * multiplex) :
	BaseCommand(oscQueryModule, context, params),
	oscQueryModule(oscQueryModule),
	valueParam(nullptr)
{
	target = addTargetParameter("Target", "The target parameter to set", &oscQueryModule->valuesCC);
	target->targetType = TargetParameter::CONTROLLABLE;
}

GenericOSCQueryCommand::~GenericOSCQueryCommand()
{
}

void GenericOSCQueryCommand::setupParamFromTarget()
{
	if (valueParam != nullptr && !valueParam.wasObjectDeleted())
	{
		if (valueGhostData.isVoid()) valueGhostData = valueParam->value;
		removeControllable(valueParam.get());
	}

	valueParam = ControllableFactory::createParameterFrom(target->target, false, true);
	if (valueParam != nullptr)
	{
		if (!valueGhostData.isVoid()) valueParam->setValue(valueGhostData);
		addParameter(valueParam);
		linkParamToMappingIndex(valueParam, 0);
		valueGhostData = var(); //reset ghostValue
	}
}

void GenericOSCQueryCommand::onContainerParameterChanged(Parameter * p)
{
	BaseCommand::onContainerParameterChanged(p);
	if (p == target)
	{
		setupParamFromTarget();
	}
}

var GenericOSCQueryCommand::getJSONData()
{
	var data = BaseCommand::getJSONData();
	if (valueParam != nullptr) data.getDynamicObject()->setProperty("ghostValue", valueParam->value);
	return data;
}

void GenericOSCQueryCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);
	valueGhostData = data.getProperty("ghostValue", var());
}

void GenericOSCQueryCommand::triggerInternal(int multiplexIndex)
{
	if (target->target.wasObjectDeleted() || target->target == nullptr) return;
	if(valueParam != nullptr) ((Parameter *)target->target.get())->setValue(valueParam->getValue());
	else if (target->target->type == Controllable::TRIGGER) ((Trigger *)target->target.get())->trigger();
}
