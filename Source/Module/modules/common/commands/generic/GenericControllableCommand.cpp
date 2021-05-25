/*
  ==============================================================================

	GenericControllableCommand.cpp
	Created: 25 May 2021 10:34:11am
	Author:  bkupe

  ==============================================================================
*/

GenericControllableCommand::GenericControllableCommand(Module* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	target(nullptr),
	valueOperator(nullptr),
	value(nullptr)
{
	saveAndLoadRecursiveData = true;

	action = (Action)(int)params.getProperty("action", 0);
	int64 rootPtr = (int64)params.getProperty("root", 0);
	ControllableContainer* rootCC = rootPtr == 0 ? nullptr : (ControllableContainer*)rootPtr;
	target = addTargetParameter("Target ", "The target to set the value to or trigger", rootCC);
	if (action == TRIGGER) target->typesFilter.add(Trigger::getTypeStringStatic());
	else target->excludeTypesFilter.add(Trigger::getTypeStringStatic());

	if (action == SET_VALUE) valueOperator = addEnumParameter("Operator", "The operator to apply. If you simply want to set the value, leave at the = option.", false);
}

GenericControllableCommand::~GenericControllableCommand()
{

}

void GenericControllableCommand::updateValueFromTarget()
{
	if (value != nullptr && !value.wasObjectDeleted())
	{
		ghostValueData = value->getJSONData();
		removeControllable(value.get());
	}

	if (target == nullptr) return;

	Controllable* cTarget = getLinkedTargetAs<Controllable>(target, 0); //use multiplex 0 to create param, should be better

	if (cTarget != nullptr) value = ControllableFactory::createParameterFrom(cTarget);
	else value = nullptr;

	if (value != nullptr)
	{
		updateOperatorOptions();

		if (!ghostValueData.isVoid()) value->loadJSONData(ghostValueData);
		value->setNiceName("Value");
		if (isMultiplexed()) value->clearRange(); //don't fix a range for multilex, there could be many ranges

		addParameter(value);
		linkParamToMappingIndex(value, 0);
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

	case Controllable::ENUM:
		valueOperator->addOption("Next Option", NEXT_ENUM)->addOption("Previous Option", PREV_ENUM);
		break;

	default:
		break;
	}

	if (oldData.isNotEmpty()) valueOperator->setValueWithKey(oldData);
	else valueOperator->setValueWithData(EQUAL);

	valueOperator->setEnabled(valueOperator->getAllKeys().size() >= 1);

	if (value != nullptr)
	{
		Operator o = valueOperator->getValueDataAsEnum<Operator>();
		bool shouldHideValue = o == INVERSE || o == NEXT_ENUM || o == PREV_ENUM;
		value->hideInEditor = shouldHideValue;
	}

}

void GenericControllableCommand::onContainerParameterChanged(Parameter* p)
{
	if (p == target && action == SET_VALUE)
	{
		updateValueFromTarget();
	}
	else if (p == valueOperator)
	{
		if (value != nullptr)
		{
			Operator o = valueOperator->getValueDataAsEnum<Operator>();
			if (o != EQUAL) value->clearRange(); //to clean more
			bool curHide = value->hideInEditor;
			value->hideInEditor = o == INVERSE || o == NEXT_ENUM || o == PREV_ENUM;
			if (curHide != value->hideInEditor) queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, this));
		}
	}
}

void GenericControllableCommand::triggerInternal(int multiplexIndex)
{
	if (isCurrentlyLoadingData) return;

	Controllable* c = getLinkedTargetAs<Controllable>(target, multiplexIndex);
	if (c == nullptr) return;

	switch (action)
	{

	case TRIGGER:
		if (c->type == Parameter::TRIGGER) ((Trigger*)c)->trigger();
		break;

	case SET_VALUE:
	{
		if (Parameter* p = static_cast<Parameter*>(c))
		{
			Operator o = valueOperator->getValueDataAsEnum<Operator>();

			if (value != nullptr || o == INVERSE || o == NEXT_ENUM || o == PREV_ENUM)
			{
				var val = getLinkedValue(value, multiplexIndex);

				switch (o)
				{
				case EQUAL:
				{
					if (EnumParameter* ep = dynamic_cast<EnumParameter*>(p))
					{
						if (val.isInt() || val.isDouble())
						{
							int index = (int)val % ep->enumValues.size();
							while (index < 0) index += ep->enumValues.size();
							ep->setValueWithKey((ep->enumValues[index]->key));
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
					else p->setNormalizedValue(1 - p->floatValue() / ((float)p->maximumValue - (float)p->minimumValue));
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

				case NEXT_ENUM:
					if (EnumParameter* ep = dynamic_cast<EnumParameter*>(p)) ep->setNext();
					break;

				case PREV_ENUM:
					if (EnumParameter* ep = dynamic_cast<EnumParameter*>(p)) ep->setPrev();
					break;
				}
			}
		}
	}
	break;
	}
}

void GenericControllableCommand::linkUpdated(ParameterLink* pLink)
{
	if (pLink->parameter == target) updateValueFromTarget();
}

void GenericControllableCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);
	//loadGhostData(data);
	if (target->target == nullptr && Engine::mainEngine->isLoadingFile)
	{
		ghostData = data;
		Engine::mainEngine->addEngineListener(this);
	}
}

void GenericControllableCommand::endLoadFile()
{
	Engine::mainEngine->removeEngineListener(this);
	loadGhostData(ghostData);
	ghostData = var();
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
	return new GenericControllableCommand((CustomVariablesModule*)module, context, params, multiplex);
}
