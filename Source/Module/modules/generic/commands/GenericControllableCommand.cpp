/*
  ==============================================================================

	GenericControllableCommand.cpp
	Created: 28 Apr 2017 5:51:16pm
	Author:  Ben

  ==============================================================================
*/

#include "GenericControllableCommand.h"


GenericControllableCommand::GenericControllableCommand(ChataigneGenericModule* _module, CommandContext context, var params, IteratorProcessor* iterator) :
	BaseCommand(_module, context, params, iterator),
	value(nullptr)
{
	target = addTargetParameter("Target", "Target to set the value");

	target->targetType = TargetParameter::CONTROLLABLE;
	action = (Action)(int)params.getProperty("action", SET_VALUE);

	if (action == TRIGGER) target->typesFilter.add(Trigger::getTypeStringStatic());
	else if (action == SET_VALUE) target->excludeTypesFilter.add(Trigger::getTypeStringStatic());
}

GenericControllableCommand::~GenericControllableCommand()
{
}

void GenericControllableCommand::setValueParameter(Parameter* p)
{
	if (!value.wasObjectDeleted() && value != nullptr)
	{
		ghostValueData = value->getJSONData();
		removeControllable(value.get());
	}

	Parameter* tp = dynamic_cast<Parameter*>(target->target.get());
	if (tp == nullptr) return;

	value = p;

	if (value != nullptr)
	{
		linkParamToMappingIndex(value, 0);
		addParameter(p);
		if (!ghostValueData.isVoid()) value->loadJSONData(ghostValueData);
		ghostValueData = var();
	}
}


void GenericControllableCommand::triggerInternal(int iterationIndex)
{
	BaseCommand::triggerInternal(iterationIndex);

	if (target->target == nullptr) return;

	if (action == SET_VALUE)
	{
		if (value == nullptr) return;
		Parameter* p = static_cast<Parameter*>(target->target.get());
		p->setValue(getLinkedValue(value, iterationIndex));
	}
	else if (action == TRIGGER)
	{
		Trigger* t = static_cast<Trigger*>(target->target.get());
		t->trigger();
	}



}

void GenericControllableCommand::onContainerParameterChanged(Parameter* p)
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
					Controllable* c = ControllableFactory::createParameterFrom(target->target);
					if (c == nullptr)
					{
						DBG("Should not be null here");
						jassertfalse;
					}

					c->setNiceName("Value");
					Parameter* tp = dynamic_cast<Parameter*>(c);
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
	}
	else BaseCommand::loadJSONDataInternal(data);
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

BaseCommand* GenericControllableCommand::create(ControllableContainer* module, CommandContext context, var params, IteratorProcessor* iterator)
{
	return new GenericControllableCommand((ChataigneGenericModule*)module, context, params, iterator);
}
