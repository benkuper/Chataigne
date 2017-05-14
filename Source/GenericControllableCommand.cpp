/*
  ==============================================================================

	GenericControllableCommand.cpp
	Created: 28 Apr 2017 5:51:16pm
	Author:  Ben

  ==============================================================================
*/

#include "GenericControllableCommand.h"


GenericControllableCommand::GenericControllableCommand(ChataigneGenericModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	value(nullptr)
{
	target = addTargetParameter("Target", "Target to set the value");

	target->targetType = TargetParameter::CONTROLLABLE;
	action = (Action)(int)params.getProperty("action", SET_VALUE);
	target->showTriggers = action == TRIGGER;
	target->showParameters = action == SET_VALUE;
}

GenericControllableCommand::~GenericControllableCommand()
{
}

void GenericControllableCommand::setValueParameter(Parameter * p)
{
	if (value != nullptr)
	{
		removeControllable(value);
		clearTargetMappingParameters();
	}

	Parameter * tp = dynamic_cast<Parameter *>(target->target.get());
	if (tp == nullptr) return;

	value = p;

	if (value != nullptr)
	{
		
		value->setRange(tp->minimumValue, tp->maximumValue);
		setTargetMappingParameterAt(value, 0);
		addParameter(p);
	}
}


void GenericControllableCommand::trigger()
{

	if (target->target == nullptr) return;

	if (action == SET_VALUE)
	{
		if (value == nullptr) return;
		Parameter * p = static_cast<Parameter *>(target->target.get());
		p->setValue(value->getValue());
	} else if (action == TRIGGER)
	{
		Trigger * t = static_cast<Trigger *>(target->target.get());
		t->trigger();
	}
	

	
}

void GenericControllableCommand::onContainerParameterChanged(Parameter * p)
{
	if (p == target)
	{
		if (action == SET_VALUE)
		{
			if (target->target == nullptr) setValueParameter(nullptr);
			else
			{
				if (target->target->type == Controllable::TRIGGER) setValueParameter(nullptr);
				else
				{
					Controllable * c = ControllableFactory::createControllable(target->target->getTypeString());
					Parameter * tp = dynamic_cast<Parameter *>(c);
					setValueParameter(tp);
				}

			}
		}
		
	}
}

void GenericControllableCommand::loadJSONDataInternal(var data)
{
	if (Engine::mainEngine->isLoadingFile)
	{
		//DBG("Engine is loading, waiting after load");
		Engine::mainEngine->addEngineListener(this);
		dataToLoad = data;
	} else BaseCommand::loadJSONDataInternal(data);
}

void GenericControllableCommand::endLoadFile()
{
	//reset data we want to reload
	target->setValue("", true);
	//DBG("Engine after load, load command data");

	loadJSONData(dataToLoad);
	dataToLoad = var();

	Engine::mainEngine->removeEngineListener(this);
}

BaseCommand * GenericControllableCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	return new GenericControllableCommand((ChataigneGenericModule *)module, context, params);
}