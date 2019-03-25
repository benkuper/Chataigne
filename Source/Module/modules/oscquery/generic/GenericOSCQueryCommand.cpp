/*
  ==============================================================================

    GenericOSCQueryCommand.cpp
    Created: 28 Feb 2019 10:33:50pm
    Author:  bkupe

  ==============================================================================
*/

#include "GenericOSCQueryCommand.h"

GenericOSCQueryCommand::GenericOSCQueryCommand(GenericOSCQueryModule * oscQueryModule, CommandContext context, var params) :
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
		removeControllable(valueParam.get());
		removeTargetMappingParameter(valueParam);
	}

	valueParam = ControllableFactory::createParameterFrom(target->target, false, true);
	if (valueParam != nullptr)
	{
		addParameter(valueParam);
		addTargetMappingParameterAt(valueParam, 0);
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

void GenericOSCQueryCommand::triggerInternal()
{
	if (target->target.wasObjectDeleted() || target->target == nullptr) return;
	if(valueParam != nullptr) ((Parameter *)target->target.get())->setValue(valueParam->getValue());
	else if (target->target->type == Controllable::TRIGGER) ((Trigger *)target->target.get())->trigger();
}
