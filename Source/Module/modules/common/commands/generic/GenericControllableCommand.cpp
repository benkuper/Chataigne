/*
  ==============================================================================

	GenericControllableCommand.cpp
	Created: 25 May 2021 10:34:11am
	Author:  bkupe

  ==============================================================================
*/


#include "Module/ModuleIncludes.h"

GenericControllableCommand::GenericControllableCommand(Module* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	target(nullptr),
	valueOperator(nullptr),
	componentOperator(nullptr),
	loop(nullptr),
	randomAlwaysUnique(nullptr),
	value(nullptr),
	time(nullptr),
	isUpdatingContent(false)
{
	saveAndLoadRecursiveData = true;

	action = (Action)(int)params.getProperty("action", SET_VALUE);
	int64 rootPtr = (int64)params.getProperty("root", 0);
	ControllableContainer* rootCC = rootPtr == 0 ? nullptr : (ControllableContainer*)rootPtr;
	target = addTargetParameter("Target ", "The target to set the value to or trigger", rootCC);
	if (ParameterLink* pLink = getLinkedParam(target)) pLink->canLinkToMapping = false;

	if (action == TRIGGER) target->typesFilter.add(Trigger::getTypeStringStatic());
	else if (action == SET_VALUE || action == GO_TO_VALUE) target->excludeTypesFilter.add(Trigger::getTypeStringStatic());
	else if (action == SET_ENABLED) target->customTargetFilterFunc = &GenericControllableCommand::checkEnableTargetFilter;

	target->defaultParentLabelLevel = params.getProperty("labelLevel", 3);

	if (action == SET_VALUE || action == GO_TO_VALUE)
	{
		componentOperator = addEnumParameter("Component", "Component to target. Keep all to target everything. Gamgie made me do this.");
		valueOperator = addEnumParameter("Operator", "The operator to apply. If you simply want to set the value, leave at the = option.", false);
		loop = addBoolParameter("Loop", "If applicable, this will allow going from max val to min val and vice-versa.", false);
		loop->hideInEditor = true;

		randomAlwaysUnique = addBoolParameter("Random Always Unique", "If in random mode, this will ensure that the value is always different from the previous one.", false);
		randomAlwaysUnique->hideInEditor = true;

		if (action == GO_TO_VALUE)
		{
			time = addFloatParameter("Time", "Time to go to the value", 1, 0);
			time->defaultUI = FloatParameter::TIME;
			automation.reset(new Automation("Curve"));
			automation->addKey(0, 0);
			automation->addKey(1, 1);
			addChildControllableContainer(automation.get());
		}
	}
	else if (action == SET_ENABLED) value = addBoolParameter("Value", "If checked, this will enable this parameter, otherwise it will disable it. Simple. Efficient.", false);
}

GenericControllableCommand::~GenericControllableCommand()
{
	if (Parameter::ValueInterpolator::Manager::getInstanceWithoutCreating() == nullptr) return;

	if (action == GO_TO_VALUE)
	{
		if (!isMultiplexed()) Parameter::ValueInterpolator::Manager::getInstance()->removeInterpolationWith((Parameter*)target->target.get());
		else
		{
			for (int i = 0; i < getMultiplexCount(); i++)
			{
				if (Parameter* p = dynamic_cast<Parameter*>(getTargetControllableAtIndex(i)))
				{
					Parameter::ValueInterpolator::Manager::getInstance()->removeInterpolationWith(p);
				}
			}
		}
	}
}

void GenericControllableCommand::updateComponentFromTarget()
{
	if (componentOperator == nullptr) return;

	bool curUpdating = isUpdatingContent;
	isUpdatingContent = true;

	var val = componentOperator->getValueData();
	componentOperator->clearOptions();
	componentOperator->addOption("All", -1);

	Controllable* cTarget = getControllableFromTarget();

	bool hasSubComponents = false;

	if (cTarget != nullptr)
	{
		if (cTarget->type != Controllable::TRIGGER)
		{
			Parameter* p = (Parameter*)cTarget;
			if (p->isComplex())
			{
				hasSubComponents = true;

				StringArray names = p->getValuesNames();
				for (int i = 0; i < names.size(); i++) componentOperator->addOption(names[i], i);
			}
		}
	}
	componentOperator->setEnabled(hasSubComponents);


	if (!val.isVoid()) componentOperator->setValueWithData(val);
	else if (!ghostComponent.isVoid()) componentOperator->setValueWithKey(ghostComponent);

	updateValueFromTargetAndComponent();

	isUpdatingContent = curUpdating;

}

void GenericControllableCommand::updateValueFromTargetAndComponent()
{
	bool curUpdating = isUpdatingContent;
	isUpdatingContent = true;


	if (value != nullptr && !value.wasObjectDeleted())
	{
		if (ghostValueData.isVoid())
		{
			ghostValueData = value->getJSONData();
		}
		ParameterLink* pLink = getLinkedParam(value);
		if (pLink != nullptr) ghostValueParamLinkData = pLink->getJSONData();

		removeControllable(value.get());
	}

	value = nullptr;
	if (target == nullptr)
	{
		isUpdatingContent = curUpdating;
		return;
	}

	Parameter* cTarget = dynamic_cast<Parameter*>(getControllableFromTarget());

	if (cTarget != nullptr)
	{
		var compData = componentOperator->getValueData();
		if (!compData.isVoid())
		{
			int compEnum = (int)compData;
			if (compEnum == -1) value = ControllableFactory::createParameterFrom(cTarget);
			else
			{
				if (compEnum < cTarget->minimumValue.size())
				{
					float minV = cTarget->minimumValue[compEnum];
					float maxV = cTarget->maximumValue[compEnum];
					value = new FloatParameter("Value", "", 0, minV, maxV);
				}
			}
		}
	}

	if (value != nullptr)
	{
		updateOperatorOptions();


		if (!ghostValueData.isVoid())
		{
			value->loadJSONData(ghostValueData);
		}
		value->setNiceName("Value");
		if (isMultiplexed()) value->clearRange(); //don't fix a range for multilex, there could be many ranges

		addParameter(value);

		ParameterLink* pLink = getLinkedParam(value);
		if (pLink != nullptr && !ghostValueParamLinkData.isVoid()) pLink->loadJSONData(ghostValueParamLinkData);

		ghostValueData = var();
	}

	isUpdatingContent = curUpdating;
}



Controllable* GenericControllableCommand::getControllableFromTarget()
{
	return getLinkedTargetAs<Controllable>(target, 0); //use multiplex 0 to create param, should be better;
}

void GenericControllableCommand::updateOperatorOptions()
{
	bool curUpdating = isUpdatingContent;
	isUpdatingContent = true;

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

	if (value->type != Controllable::TARGET) valueOperator->addOption("Random", RANDOM);

	if (oldData.isNotEmpty()) valueOperator->setValueWithKey(oldData);
	else valueOperator->setValueWithData(EQUAL);

	valueOperator->setEnabled(valueOperator->getAllKeys().size() >= 1);

	Operator o = valueOperator->getValueDataAsEnum<Operator>();
	if (value != nullptr)
	{
		bool shouldHideValue = o == INVERSE || o == NEXT_ENUM || o == PREV_ENUM || o == RANDOM;
		value->hideInEditor = shouldHideValue;

		bool loopEnabled = o == ADD || o == SUBTRACT || o == NEXT_ENUM || o == PREV_ENUM;
		loop->hideInEditor = !loopEnabled;

	}

	randomAlwaysUnique->hideInEditor = o != RANDOM;

	isUpdatingContent = curUpdating;
}

void GenericControllableCommand::onContainerParameterChanged(Parameter* p)
{
	if (p == target && (action == SET_VALUE || action == GO_TO_VALUE))
	{
		updateComponentFromTarget();
	}
	else if (p == componentOperator)
	{
		updateValueFromTargetAndComponent();
	}
	else if (p == valueOperator)
	{
		if (value != nullptr)
		{
			Operator o = valueOperator->getValueDataAsEnum<Operator>();
			if (o != EQUAL) value->clearRange(); //to clean more
			bool curHide = value->hideInEditor;
			value->hideInEditor = o == INVERSE || o == NEXT_ENUM || o == PREV_ENUM || o == RANDOM;

			bool curHideLoop = loop->hideInEditor;
			loop->hideInEditor = !(o == ADD || o == SUBTRACT || o == NEXT_ENUM || o == PREV_ENUM);

			bool curHideRandom = randomAlwaysUnique->hideInEditor;
			randomAlwaysUnique->hideInEditor = o != RANDOM;

			if (curHide != value->hideInEditor || curHideLoop != loop->hideInEditor || curHideRandom != randomAlwaysUnique->hideInEditor) queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, this));

		}
	}
}

void GenericControllableCommand::triggerInternal(int multiplexIndex)
{
	if (isCurrentlyLoadingData) return;
	if (isUpdatingContent) return;

	Controllable* c = getTargetControllableAtIndex(multiplexIndex);
	if (c == nullptr) return;

	switch (action)
	{

	case TRIGGER:
		if (c->type == Parameter::TRIGGER) ((Trigger*)c)->trigger();
		break;

	case SET_VALUE:
	case GO_TO_VALUE:
	{

		if (Parameter* p = static_cast<Parameter*>(c))
		{
			var targetValue;

			Operator o = valueOperator->getValueDataAsEnum<Operator>();
			int compOp = componentOperator->getValueData();

			if (value != nullptr || o == INVERSE || o == NEXT_ENUM || o == PREV_ENUM)
			{
				var val = getLinkedValue(value, multiplexIndex);

				switch (o)
				{
				case EQUAL:
				{
					if (p->type == Controllable::ENUM)
					{

						if (EnumParameter* ep = dynamic_cast<EnumParameter*>(p))
						{
							if (ep->enumValues.size() > 0)
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
						}
					}
					else if (p->type == Controllable::TARGET)
					{
						if (TargetParameter* tp = dynamic_cast<TargetParameter*>(p))
						{
							if (tp->targetType == TargetParameter::CONTROLLABLE)
							{
								if (Controllable* c = getLinkedTargetAs<Controllable>((TargetParameter*)value.get(), multiplexIndex))
								{
									tp->setValueFromTarget(c);
								}
							}
							else
							{
								if (ControllableContainer* cc = getLinkedTargetContainerAs<ControllableContainer>((TargetParameter*)value.get(), multiplexIndex))
								{
									tp->setValueFromTarget(cc);
								}
							}
						}
					}
					else
					{
						if (compOp == -1)
						{
							if (p->value.size() == val.size()) targetValue = val;
						}
						else
						{
							var pVal = p->value.clone();
							if (pVal.isArray())
							{
								pVal[compOp] = (float)val;
								targetValue = pVal;
							}
						}
					}
				}
				break;

				case INVERSE:
					if (p->type == Parameter::BOOL)  p->setValue(!p->boolValue());
					else targetValue = 1 - p->floatValue() / ((float)p->maximumValue - (float)p->minimumValue);
					break;

				case ADD:
				case SUBTRACT:
				{
					targetValue = p->value.clone();

					if (compOp == -1)
					{
						if (!p->isComplex())
						{
							targetValue = o == ADD ? p->floatValue() + (float)val : p->floatValue() - (float)val;
							if (p->hasRange() && loop != nullptr && loop->boolValue())
							{
								if ((float)targetValue > (float)p->maximumValue) targetValue = p->minimumValue;
								else if ((float)targetValue < (float)p->minimumValue) targetValue = p->maximumValue;
							}

						}
						else
						{
							for (int i = 0; i < p->value.size() && val.size(); i++)
							{
								targetValue[i] = o == ADD ? (float)p->value[i] + (float)val[i] : (float)p->value[i] - (float)val[i];
							}
						}
					}
					else
					{
						if (targetValue.isArray())
						{
							float targetCompVal = o == ADD ? (float)p->value[compOp] + (float)val : (float)p->value[compOp] - (float)val;
							if (p->hasRange() && loop != nullptr && loop->boolValue())
							{
								if (targetCompVal > (float)p->maximumValue[compOp]) targetValue = p->minimumValue[compOp];
								else if (targetCompVal < (float)p->minimumValue[compOp]) targetValue = p->maximumValue[compOp];
							}

							targetValue[compOp] = targetCompVal;
						}
					}
				}
				break;

				case MULTIPLY:
					targetValue = p->floatValue() * (float)val;
					break;

				case DIVIDE:
					targetValue = p->floatValue() / (float)val;
					break;

				case MAX:
					targetValue = std::max(p->floatValue(), (float)val);
					break;

				case MIN:
					targetValue = std::min(p->floatValue(), (float)val);
					break;

				case NEXT_ENUM:
					if (EnumParameter* ep = dynamic_cast<EnumParameter*>(p)) ep->setNext(loop != nullptr ? loop->boolValue() : false);
					break;

				case PREV_ENUM:
					if (EnumParameter* ep = dynamic_cast<EnumParameter*>(p)) ep->setPrev(loop != nullptr ? loop->boolValue() : false);
					break;

				case RANDOM:
				{
					Random r;
					if (p->type == Parameter::BOOL) p->setValue(randomAlwaysUnique->boolValue() ? !p->boolValue() : r.nextBool());
					else if (p->type == Parameter::FLOAT || p->type == Parameter::INT)
					{
						bool isInt = p->type == Parameter::INT;

						if (randomAlwaysUnique->boolValue() && p->minimumValue != p->maximumValue)
						{
							targetValue = isInt ? p->intValue() : p->floatValue();

							while ((isInt && (int)targetValue == p->intValue()) || (float)targetValue == p->floatValue())
							{
								if (p->hasRange()) targetValue = jmap<float>(r.nextFloat(), p->minimumValue, isInt ? (float)p->maximumValue + 1 : (float)p->maximumValue);
								else targetValue = isInt ? r.nextInt() : r.nextFloat();
							}


						}
						else
						{
							if (p->hasRange()) targetValue = jmap<float>(r.nextFloat(), p->minimumValue, isInt ? (float)p->maximumValue + 1 : (float)p->maximumValue);
							else targetValue = p->type == Parameter::INT ? r.nextInt() : r.nextFloat();
						}
					}
					else if (p->type == Parameter::COLOR)
					{
						for (int i = 0; i < 4; i++) targetValue.append(r.nextFloat());
					}
					else if (p->type == Parameter::ENUM)
					{
						EnumParameter* ep = (EnumParameter*)p;
						if (ep->enumValues.size() > 0)
						{
							if (randomAlwaysUnique->boolValue() && ep->enumValues.size() > 1)
							{
								String key = ep->getValueKey();
								while (key == ep->getValueKey())
								{
									int index = r.nextInt() % ep->enumValues.size();
									while (index < 0) index += ep->enumValues.size();
									key = ep->enumValues[index]->key;
								}

								ep->setValueWithKey(key);
							}
							else
							{
								int index = r.nextInt() % ep->enumValues.size();
								while (index < 0) index += ep->enumValues.size();
								ep->setValueWithKey((ep->enumValues[index]->key));
							}

						}
					}
					else if (p->isComplex())
					{
						var v;
						for (int i = 0; i < p->value.size(); i++)
						{
							float vv = p->value[i];
							if (compOp == i || compOp == -1) vv = p->hasRange() ? jmap<float>(r.nextFloat(), p->minimumValue[i], p->maximumValue[i]) : r.nextFloat();
							v.append(vv);
						}
						targetValue = v;
					}
				}
				}
			}

			if (!targetValue.isVoid())
			{
				float lTime = getLinkedValue(time, multiplexIndex);
				if (action == SET_VALUE || (action == GO_TO_VALUE && lTime == 0))
				{
					Parameter::ValueInterpolator::Manager::getInstance()->removeInterpolationWith(p);
					p->setValue(targetValue);
				}

				else if (action == GO_TO_VALUE)
				{
					Parameter::ValueInterpolator::Manager::getInstance()->interpolate(p, targetValue, lTime, automation.get());
				}
			}
		}
	}
	break;

	case SET_ENABLED:
		c->setEnabled(value->boolValue());
		break;
	}
}

Controllable* GenericControllableCommand::getTargetControllableAtIndex(int multiplexIndex)
{
	return getLinkedTargetAs<Controllable>(target, multiplexIndex);
}

void GenericControllableCommand::linkUpdated(ParameterLink* pLink)
{
	if (pLink->parameter == target)
	{
		if (!isCurrentlyLoadingData) updateComponentFromTarget();
	}
}

void GenericControllableCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);

	if (target->target == nullptr)
	{
		ghostData = data;
		if (Engine::mainEngine->isLoadingFile) Engine::mainEngine->addEngineListener(this);
		else loadGhostData(data);
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
	//if (value == nullptr)
	//{


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
		else if (paramsData[i].getProperty("controlAddress", "") == "/component")
		{
			ghostComponent = paramsData[i].getProperty("value", var());
		}
	}
	ParamLinkContainer::loadJSONDataInternal(data); //force refreshing links

	if (action == SET_VALUE || action == GO_TO_VALUE) updateComponentFromTarget(); //force generate if not yet
	//}

}

bool GenericControllableCommand::checkEnableTargetFilter(Controllable* c)
{
	return c->canBeDisabledByUser;
}


BaseCommand* GenericControllableCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	return new GenericControllableCommand((CustomVariablesModule*)module, context, params, multiplex);
}