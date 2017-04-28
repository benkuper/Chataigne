/*
  ==============================================================================

	GenericSetParameterCommand.cpp
	Created: 28 Apr 2017 5:51:16pm
	Author:  Ben

  ==============================================================================
*/

#include "GenericSetParameterCommand.h"
#include "ControllableFactory.h"
#include "GenericSetParameterCommandEditor.h"

GenericSetParameterCommand::GenericSetParameterCommand(ChataigneGenericModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	value(nullptr)
{
	target = addTargetParameter("Target", "Target to set the value");
	
	target->targetType = TargetParameter::CONTROLLABLE;
	target->showTriggers = false;
}

GenericSetParameterCommand::~GenericSetParameterCommand()
{
}

void GenericSetParameterCommand::setValueParameter(Parameter * p)
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


void GenericSetParameterCommand::trigger()
{
	if (value == nullptr) return;
	if (target->target == nullptr) return;

	Parameter * p = static_cast<Parameter *>(target->target.get());
	p->setValue(value->value);
}

void GenericSetParameterCommand::onContainerParameterChanged(Parameter * p)
{
	if (p == target)
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

void GenericSetParameterCommand::loadJSONDataInternal(var data)
{
	if (Engine::getInstance()->isLoadingFile)
	{
		//DBG("Engine is loading, waiting after load");
		Engine::getInstance()->addEngineListener(this);
		dataToLoad = data;
	} else BaseCommand::loadJSONDataInternal(data);
}

void GenericSetParameterCommand::endLoadFile()
{
	//reset data we want to reload
	target->setValue("", true);
	//DBG("Engine after load, load command data");

	loadJSONData(dataToLoad);
	dataToLoad = var();

	Engine::getInstance()->removeEngineListener(this);
}

BaseCommand * GenericSetParameterCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	return new GenericSetParameterCommand((ChataigneGenericModule *)module, context, params);
}

/*
InspectableEditor * GenericSetParameterCommand::getEditor(bool isRoot)
{
	return new GenericSetParameterCommandEditor(this,isRoot);
}
*/
