/*
  ==============================================================================

    GenericOSCQueryCommand.cpp
    Created: 28 Feb 2019 10:33:50pm
    Author:  bkupe

  ==============================================================================
*/

GenericOSCQueryCommand::GenericOSCQueryCommand(GenericOSCQueryModule * _oscQuerymodule, CommandContext context, var params, Multiplex * multiplex) :
	BaseCommand(_oscQuerymodule, context, params, multiplex),
	oscQueryModule(_oscQuerymodule),
	value(nullptr),
	valueOperator(nullptr)
{
	target = addTargetParameter("Target", "Target to set the value", &oscQueryModule->valuesCC);

	target->targetType = TargetParameter::CONTROLLABLE;
	action = (Action)(int)params.getProperty("action", SET_VALUE);

	if (action == TRIGGER) target->typesFilter.add(Trigger::getTypeStringStatic());
	else if (action == SET_VALUE)
	{
		target->excludeTypesFilter.add(Trigger::getTypeStringStatic());
		valueOperator = addEnumParameter("Operator", "The operator to apply. If you simply want to set the value, leave at the = option.", false);
	}
}

GenericOSCQueryCommand::~GenericOSCQueryCommand()
{
}

void GenericOSCQueryCommand::updateValueFromTarget()
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
			if (isMultiplexed()) tp->clearRange();
			setValueParameter(tp);
		}

	}
}

void GenericOSCQueryCommand::setValueParameter(Parameter* p)
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
		updateOperatorOptions();

		addParameter(value);
		if (!ghostValueData.isVoid()) value->loadJSONData(ghostValueData);
		ghostValueData = var();

		linkParamToMappingIndex(value, 0);
	}
}

void GenericOSCQueryCommand::updateOperatorOptions()
{
	String oldData = ghostOperator.isVoid() ? valueOperator->getValueKey() : ghostOperator.toString();
	valueOperator->clearOptions();
	valueOperator->addOption("Equals", EQUAL, false);

	if (value == nullptr)
	{
		ghostOperator = oldData;
		return;
	}

	switch (value->type)
	{
	case Controllable::FLOAT:
	case Controllable::INT:
		valueOperator->addOption("Add", ADD)->addOption("Inverse", INVERSE)->addOption("Subtract", SUBTRACT)->addOption("Multiply", MULTIPLY)->addOption("Divide", DIVIDE)->addOption("Max", MAX)->addOption("Min", MIN);
		break;

	case Controllable::BOOL:
		valueOperator->addOption("Inverse", INVERSE, false);
		break;

	default:
		break;
	}

	if (oldData.isNotEmpty()) valueOperator->setValueWithKey(oldData);
	else valueOperator->setValueWithData(EQUAL);
	valueOperator->setEnabled(valueOperator->getAllKeys().size() > 1);

	value->hideInEditor = valueOperator->getValueDataAsEnum<Operator>() == INVERSE;
	ghostOperator = var();
}


void GenericOSCQueryCommand::triggerInternal(int multiplexIndex)
{
	BaseCommand::triggerInternal(multiplexIndex);

	Controllable* c = getLinkedTargetAs<Controllable>(target, multiplexIndex);
	if (c == nullptr) return;


	if (action == SET_VALUE)
	{
		if (value == nullptr) return;
	
		if (Parameter* p = dynamic_cast<Parameter*>(c))
		{
			var val = getLinkedValue(value, multiplexIndex);

			Operator o = valueOperator->getValueDataAsEnum<Operator>();

			switch (o)
			{
			case EQUAL:
			{
				if (EnumParameter* ep = dynamic_cast<EnumParameter*>(p))
				{
					if (val.isInt() || val.isDouble())
					{
						if ((int)val < ep->enumValues.size()) ep->setValueWithKey((ep->enumValues[val]->key));
					}
					else if (val.isString()) ep->setValueWithKey(val);
					else ep->setValueWithData(val);
				}
				else
				{
					p->setValue(val);
				}
			}
			break;

			case INVERSE:
				if (p->type == Parameter::BOOL)  p->setValue(!p->boolValue());
				else p->setNormalizedValue(1 - (float)val / ((float)p->maximumValue - (float)p->minimumValue));
				break;

			case ADD:
				p->setValue(p->floatValue() + (float)val);
				break;

			case SUBTRACT:
				p->setValue(p->floatValue() - (float)val);
				break;

			case MULTIPLY:
				p->setValue(p->floatValue() * (float)val);
				break;

			case DIVIDE:
				p->setValue(p->floatValue() / (float)val);
				break;

			case MAX:
				p->setValue(std::max(p->floatValue(), (float)val));
				break;

			case MIN:
				p->setValue(std::min(p->floatValue(), (float)val));
				break;
			}
		}
	}
	else if (action == TRIGGER)
	{
		if (c->type == Parameter::TRIGGER) ((Trigger*)c)->trigger();
	}
}

void GenericOSCQueryCommand::linkUpdated(ParameterLink* pLink)
{
	if (pLink->parameter == target) updateValueFromTarget();

}

void GenericOSCQueryCommand::onContainerParameterChanged(Parameter* p)
{
	if (p == target)
	{
		if (action == SET_VALUE)
		{
			updateValueFromTarget();
		}

	}
	else if (p == valueOperator)
	{
		if (value != nullptr)
		{
			Operator o = valueOperator->getValueDataAsEnum<Operator>();
			if (o != SET_VALUE) value->clearRange(); //to clean more
			bool curHide = value->hideInEditor;
			value->hideInEditor = o == INVERSE;
			if (curHide != value->hideInEditor) queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, this));

		}
	}
}

void GenericOSCQueryCommand::loadJSONDataInternal(var data)
{
	if (Engine::mainEngine->isLoadingFile)
	{
		//DBG("Engine is loading, waiting after load");
		Engine::mainEngine->addEngineListener(this);
		dataToLoad = data;
	}
	else BaseCommand::loadJSONDataInternal(data);


}

void GenericOSCQueryCommand::endLoadFile()
{
	//reset data we want to reload
	target->setValue("", true);
	//DBG("Engine after load, load command data");

	loadJSONData(dataToLoad);
	dataToLoad = var();

	if (value == nullptr) updateValueFromTarget(); //force generate if not yet

	if (value == nullptr)
	{
		var paramsData = dataToLoad.getProperty("parameters", var());
		for (int i = 0; i < paramsData.size(); i++)
		{
			if (paramsData[i].getProperty("controlAddress", "") == "/operator")
			{
				ghostOperator = paramsData[i].getProperty("value", var());
				break;
			}
		}
	}

	Engine::mainEngine->removeEngineListener(this);
}