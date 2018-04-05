/*
  ==============================================================================

	CustomVariablesModuleCommands.cpp
	Created: 23 Feb 2018 12:25:24am
	Author:  Ben

  ==============================================================================
*/

#include "CustomVariablesModuleCommands.h"

CVCommand::CVCommand(CustomVariablesModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	target(nullptr),
	targetPreset(nullptr),
	targetPreset2(nullptr),
	valueOperator(nullptr),
	value(nullptr)
{
	type = (Type)(int)params.getProperty("type", 0);
	manager = _module->manager;

	if (type == SET_VALUE)
	{
		target = addTargetParameter("Target Value", "The value to target for this command", CVGroupManager::getInstance());
		target->customGetTargetFunc = &CVGroupManager::showMenuAndGetVariable;
		target->defaultParentLabelLevel = 2;
		
		valueOperator = addEnumParameter("Operator", "The operator to apply. If you simply want to set the value, leave at the = option.");
		valueOperator->addOption("=", EQUAL)->addOption("+", ADD)->addOption("-", SUBTRACT)->addOption("x", MULTIPLY)->addOption("/", DIVIDE);
		valueOperator->hideInEditor = true;

	} else if (type == SET_2DTARGET)
	{
		target = addTargetParameter("Target Group", "The group to target for this command", CVGroupManager::getInstance());
		target->targetType = TargetParameter::CONTAINER;
		target->defaultContainerTypeCheckFunc = &ContainerTypeChecker::checkType<CVGroup>;
		target->maxDefaultSearchLevel = 1;
		target->showParentNameInEditor = false;
		value = addPoint2DParameter("Position", "The target position in the 2D interpolator");
		setTargetMappingParameterAt(value, 0);

	} else if (type == SET_PRESET || type == LERP_PRESETS || type == SET_PRESET_WEIGHT)
	{
		targetPreset = addTargetParameter("Target Preset", "The Preset to get the values from and set the variables to", CVGroupManager::getInstance());
		targetPreset->targetType = TargetParameter::CONTAINER;
		targetPreset->customGetTargetContainerFunc = &CVGroupManager::showMenuAndGetPreset;
		targetPreset->defaultParentLabelLevel = 2;

		switch (type)
		{
		case LERP_PRESETS:
		{
			targetPreset2 = addTargetParameter("Target Preset 2", "The second preset to use for the interpolation", CVGroupManager::getInstance());
			targetPreset2->targetType = TargetParameter::CONTAINER;
			targetPreset2->customGetTargetContainerFunc = &CVGroupManager::showMenuAndGetPreset;
			targetPreset2->defaultParentLabelLevel = 2;
			value = addFloatParameter("Value", "The interpolation value to weight between the 2 presets", 0, 0, 1);
			setTargetMappingParameterAt(value, 0);
		}
		break;

		case SET_PRESET_WEIGHT:
		{
			value = addFloatParameter("Weight", "The weight of the preset to set", 0, 0, 1);
			setTargetMappingParameterAt(value, 0);
		}
		break;

		default:
			break;
		}
	}
}

CVCommand::~CVCommand()
{

}

void CVCommand::onContainerParameterChanged(Parameter * p)
{
	if (p == target && type == SET_VALUE)
	{
		if (value != nullptr) removeControllable(value);
		value = ControllableFactory::createParameterFrom(target->target);
		if (value != nullptr)
		{
			valueOperator->hideInEditor = value->type != Controllable::FLOAT && value->type != Controllable::INT;
			
			value->setNiceName("Value");
			addParameter(value);
		}

	} else if (p == targetPreset || p == targetPreset2)
	{
		CVPreset * p1 = static_cast<CVPreset *>(targetPreset->targetContainer.get());
		CVPreset * p2 = static_cast<CVPreset *>(targetPreset->targetContainer.get());
		if (p1 != nullptr && p2 != nullptr && p1->group != p2->group)
		{
			LOGWARNING("The 2 presets are not from the same group !\nThis command won't have any effect until you choose presets from the same group.");
		}
	}
}

void CVCommand::trigger()
{
	switch (type)
	{
	case SET_VALUE:
	{
		if (target->target != nullptr && value != nullptr)
		{
			Parameter * p = static_cast<Parameter *>(target->target.get());
			if (p != nullptr)
			{
				if (value->type != Controllable::FLOAT && value->type != Controllable::INT)
				{
					p->setValue(value->value);
				} else
				{

					Operator o = valueOperator->getValueDataAsEnum<Operator>();

					switch (o)
					{
					case EQUAL:
						p->setValue(value->value);
						break;

					case ADD:
						p->setValue(p->floatValue() + value->floatValue());
						break;

					case SUBTRACT:
						p->setValue(p->floatValue() - value->floatValue());
						break;

					case MULTIPLY:
						p->setValue(p->floatValue() * value->floatValue());
						break;

					case DIVIDE:
						p->setValue(p->floatValue() / value->floatValue());
						break;
					}
				}
				
			}
		}
	}
	break;

	case SET_PRESET:
	{
		if (targetPreset->targetContainer != nullptr)
		{
			CVPreset * p = static_cast<CVPreset *>(targetPreset->targetContainer.get());
			if (p != nullptr) p->group->setValuesToPreset(p);
		}
	}
	break;

	case LERP_PRESETS:
	{
		if (targetPreset->targetContainer != nullptr && targetPreset2->targetContainer != nullptr)
		{
			CVPreset * p1 = static_cast<CVPreset *>(targetPreset->targetContainer.get());
			CVPreset * p2 = static_cast<CVPreset *>(targetPreset2->targetContainer.get());
			if (p1->group != p2->group)
			{
				LOGWARNING("The 2 presets are not from the same group !\nThis command won't have any effect until you choose presets from the same group.");
			} else
			{
				p1->group->lerpPresets(p1, p2, value->floatValue());
			}
		}
	}
	break;

	case SET_PRESET_WEIGHT:
	{
		if (targetPreset->targetContainer != nullptr)
		{
			CVPreset * p = static_cast<CVPreset *>(targetPreset->targetContainer.get());
			if (p != nullptr) p->weight->setValue(value->floatValue());
		}
	}
	break;

	case SET_2DTARGET:
	{
		if (target->targetContainer != nullptr)
		{
			CVGroup * g = static_cast<CVGroup *>(target->targetContainer.get());
			if (g != nullptr) g->targetPosition->setPoint(((Point2DParameter *)value)->getPoint());
		}
	}
	break;
	}
}

BaseCommand * CVCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	return new CVCommand((CustomVariablesModule *)module, context, params);
}
