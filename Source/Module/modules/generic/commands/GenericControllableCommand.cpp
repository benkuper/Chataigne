/*
  ==============================================================================

	GenericControllableCommand.cpp
	Created: 28 Apr 2017 5:51:16pm
	Author:  Ben

  ==============================================================================
*/

GenericControllableCommand::GenericControllableCommand(ChataigneGenericModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	value(nullptr),
	valueOperator(nullptr)
{
	target = addTargetParameter("Target", "Target to set the value");

	target->targetType = TargetParameter::CONTROLLABLE;
	action = (Action)(int)params.getProperty("action", SET_VALUE);

	if (action == TRIGGER) target->typesFilter.add(Trigger::getTypeStringStatic());
	else if (action == SET_VALUE)
	{
		target->excludeTypesFilter.add(Trigger::getTypeStringStatic());
		valueOperator = addEnumParameter("Operator", "The operator to apply. If you simply want to set the value, leave at the = option.", false);
	}
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
			if (isMultiplexed()) tp->clearRange();
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
		updateOperatorOptions();

		addParameter(value);
		if (!ghostValueData.isVoid()) value->loadJSONData(ghostValueData);
		if(!isCurrentlyLoadingData) linkParamToMappingIndex(value, 0);
	}
}

void GenericControllableCommand::updateOperatorOptions()
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
	valueOperator->setEnabled(valueOperator->getAllKeys().size() >= 1);

	value->hideInEditor = valueOperator->getValueDataAsEnum<Operator>() == INVERSE;
	ghostOperator = var();
}


void GenericControllableCommand::triggerInternal(int multiplexIndex)
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
						int lVal = jlimit(0, ep->enumValues.size() - 1, (int)val);
						ep->setValueWithKey((ep->enumValues[lVal]->key));
					}
					else if (val.isString())
					{
						StringArray keys = ep->getAllKeys();
						if (keys.contains(val.toString())) ep->setValueWithKey(val);
						else ep->setValueWithData(val);
					}
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

	}else if (p == valueOperator)
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

void GenericControllableCommand::loadJSONDataInternal(var data)
{
	if (Engine::mainEngine->isLoadingFile)
	{
		//DBG("Engine is loading, waiting after load");
		Engine::mainEngine->addEngineListener(this);
		dataToLoad = data;
	}
	else
	{
		BaseCommand::loadJSONDataInternal(data);
		loadGhostData(data);
	}
}

void GenericControllableCommand::endLoadFile()
{
	target->resetValue();
	loadJSONData(dataToLoad);
	loadGhostData(dataToLoad);
	dataToLoad = var();

	Engine::mainEngine->removeEngineListener(this);
}

void GenericControllableCommand::loadGhostData(var data)
{
	if (value == nullptr)
	{
		var paramsData = data.getProperty("parameters", var());
		for (int i = 0; i < paramsData.size(); i++)
		{
			if (paramsData[i].getProperty("controlAddress", "") == "/value")
			{
				ghostValueData = paramsData[i];
			}
			else if (paramsData[i].getProperty("controlAddress", "") == "/operator")
			{
				ghostOperator = paramsData[i].getProperty("value", var());
			}
		}

		updateValueFromTarget(); //force generate if not yet
	}
}

BaseCommand* GenericControllableCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	return new GenericControllableCommand((ChataigneGenericModule*)module, context, params, multiplex);
}
