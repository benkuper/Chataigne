/*
  ==============================================================================

	GenericControllableCommand.cpp
	Created: 28 Apr 2017 5:51:16pm
	Author:  Ben

  ==============================================================================
*/

GenericControllableCommand::GenericControllableCommand(ChataigneGenericModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
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

void GenericControllableCommand::updateValueFromTarget()
{
	Controllable* c = getLinkedTargetAs<Controllable>(target, 0); //check first item if multiplex and linked to a list

	if (c == nullptr) setValueParameter(nullptr);
	else
	{
		if (c->type == Controllable::TRIGGER) setValueParameter(nullptr);
		else
		{
			Controllable* tc = ControllableFactory::createParameterFrom(c);
			if (c == nullptr)
			{
				DBG("Should not be null here");
				jassertfalse;
			}

			tc->setNiceName("Value");
			Parameter* tp = dynamic_cast<Parameter*>(tc);
			setValueParameter(tp);
		}

	}
}

void GenericControllableCommand::setValueParameter(Parameter* p)
{
	if (!value.wasObjectDeleted() && value != nullptr)
	{
		ghostValueData = value->getJSONData();
		Parameter* tmpVal = value.get();
		value = nullptr; //force to be null here so when removeControllable triggers contentChanged, the triggerInternal will not use it

		removeControllable(tmpVal);
	}

	//Parameter* tp = dynamic_cast<Parameter*>(target->target.get());
	//if (tp == nullptr) return;

	value = p;

	if (value != nullptr)
	{
		addParameter(value);
		if (!ghostValueData.isVoid()) value->loadJSONData(ghostValueData);
		ghostValueData = var();

		linkParamToMappingIndex(value, 0);
	}
}


void GenericControllableCommand::triggerInternal(int multiplexIndex)
{
	BaseCommand::triggerInternal(multiplexIndex);

	Controllable* c = getLinkedTargetAs<Controllable>(target, multiplexIndex);
	if (c == nullptr) return;

	if (value == nullptr) return;

	if (action == SET_VALUE)
	{
		if (value == nullptr) return;

		if (c->type != Parameter::TRIGGER)
		{
			var v = getLinkedValue(value, multiplexIndex);
			if (EnumParameter * ep = dynamic_cast<EnumParameter *>(c))
			{
				if (v.isInt() || v.isDouble())
				{
					if ((int)v < ep->enumValues.size()) ep->setValueWithKey((ep->enumValues[v]->key));
				}
				else if (v.isString()) ep->setValueWithKey(v);
				else ep->setValueWithData(v);
			}
			else
			{
				((Parameter*)c)->setValue(v);
			}
		}
	}
	else if (action == TRIGGER)
	{
		if (c->type == Parameter::TRIGGER) ((Trigger*)c)->trigger();
	}
}

void GenericControllableCommand::linkUpdated(ParameterLink* pLink)
{
	if (pLink->parameter == target) updateValueFromTarget();

}

void GenericControllableCommand::onContainerParameterChanged(Parameter* p)
{
	if (p == target)
	{
		if (action == SET_VALUE)
		{
			updateValueFromTarget();
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

	if(value == nullptr) updateValueFromTarget(); //force generate if not yet

	Engine::mainEngine->removeEngineListener(this);
}

BaseCommand* GenericControllableCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	return new GenericControllableCommand((ChataigneGenericModule*)module, context, params, multiplex);
}
