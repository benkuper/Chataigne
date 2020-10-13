/*
  ==============================================================================

	CustomVariablesModuleCommands.cpp
	Created: 23 Feb 2018 12:25:24am
	Author:  Ben

  ==============================================================================
*/

#include "CustomVariablesModuleCommands.h"

#include "../CustomVariablesModule.h"
#include "CustomVariables/CVGroupManager.h"
#include "CustomVariables/Preset/CVPresetManager.h"

CVCommand::CVCommand(CustomVariablesModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	target(nullptr),
	targetPreset(nullptr),
	targetPreset2(nullptr),
	presetFile(nullptr),
	time(nullptr),
    automation(nullptr),
    valueOperator(nullptr),
    value(nullptr)

{
	saveAndLoadRecursiveData = true;

	type = (Type)(int)params.getProperty("type", 0);
	manager = _module->manager;

	if (type == SET_VALUE)
	{
		target = addTargetParameter("Target Value", "The value to target for this command", CVGroupManager::getInstance());
		target->customGetTargetFunc = &CVGroupManager::showMenuAndGetVariable;
		target->defaultParentLabelLevel = 2;
		
		valueOperator = addEnumParameter("Operator", "The operator to apply. If you simply want to set the value, leave at the = option.", false);
		
	} else if (type == SET_2DTARGET || type == KILL_GO_TO_PRESET)
	{
		target = addTargetParameter("Target Group", "The group to target for this command", CVGroupManager::getInstance());
		target->targetType = TargetParameter::CONTAINER;
		target->customGetTargetContainerFunc = &CVGroupManager::showMenuAndGetGroup;
		target->showParentNameInEditor = false;

		if (type == SET_2DTARGET)
		{
			value = addPoint2DParameter("Position", "The target position in the 2D interpolator");
			addTargetMappingParameterAt(value, 0);
		}
	} else if (type == SET_PRESET || type == LERP_PRESETS || type == SET_PRESET_WEIGHT || type == SAVE_PRESET || type == LOAD_PRESET || type == GO_TO_PRESET)
	{
		targetPreset = addTargetParameter("Target Preset", "The Preset to get the values from and set the variables to", CVGroupManager::getInstance());
		targetPreset->targetType = TargetParameter::CONTAINER;
		targetPreset->customGetTargetContainerFunc = &CVGroupManager::showMenuAndGetPreset;
		targetPreset->defaultParentLabelLevel = 2;

		switch (type)
		{

		case GO_TO_PRESET:
			time = addFloatParameter("Interpolation time", "Time for the animation to go to the target preset", 1, 0);
			time->defaultUI = FloatParameter::TIME;
			time->canBeDisabledByUser = true;

			automation = new Automation("Interpolation Curve");
			automation->isSelectable = false;
			automation->length->setValue(1);
			automation->addKey(0, 0, false);
			automation->items[0]->easingType->setValueWithData(Easing::BEZIER);
			automation->addKey(1, 1, false);
			automation->selectItemWhenCreated = false;
			automation->editorIsCollapsed = true;
			automation->editorCanBeCollapsed = true;
			automation->setCanBeDisabled(true);
			automation->enabled->setValue(false);
			addChildControllableContainer(automation, true);
			break;

		case LERP_PRESETS:
		{
			targetPreset2 = addTargetParameter("Target Preset 2", "The second preset to use for the interpolation", CVGroupManager::getInstance());
			targetPreset2->targetType = TargetParameter::CONTAINER;
			targetPreset2->customGetTargetContainerFunc = &CVGroupManager::showMenuAndGetPreset;
			targetPreset2->defaultParentLabelLevel = 2;
			value = addFloatParameter("Value", "The interpolation value to weight between the 2 presets", 0, 0, 1);
			addTargetMappingParameterAt(value, 0);
		}
		break;

		case SET_PRESET_WEIGHT:
		{
			value = addFloatParameter("Weight", "The weight of the preset to set", 0, 0, 1);
			addTargetMappingParameterAt(value, 0);
		}
		break;

		case LOAD_PRESET:
		case SAVE_PRESET:
		{
			presetFile = addFileParameter("File", "The file to save/load the preset to/from");
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

void CVCommand::updateOperatorOptions()
{
	var oldData = valueOperator->getValueData();
	valueOperator->clearOptions();
	valueOperator->addOption("Equals", EQUAL, false);

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
	
	valueOperator->setValueWithData(oldData.isVoid()?EQUAL:(Operator)(int)(oldData));
	valueOperator->setEnabled(valueOperator->getAllKeys().size() > 1);

	value->hideInEditor = valueOperator->getValueDataAsEnum<Operator>() == INVERSE;
}

void CVCommand::onContainerParameterChanged(Parameter * p)
{
	if (p == target && type == SET_VALUE)
	{
		if (value != nullptr)
		{
			ghostValueData = value->getJSONData();
			clearTargetMappingParameters();
			removeControllable(value);
		}

		if (target->target != nullptr) value = ControllableFactory::createParameterFrom(target->target);
		else value = nullptr;

		if (value != nullptr)
		{
			updateOperatorOptions();

			if (!ghostValueData.isVoid()) value->loadJSONData(ghostValueData);
			value->setNiceName("Value");
		
			addParameter(value);
			addTargetMappingParameterAt(value, 0);
		}

	} else if (p == targetPreset || p == targetPreset2)
	{
		CVPreset * p1 = static_cast<CVPreset *>(targetPreset->targetContainer.get());
		CVPreset * p2 = static_cast<CVPreset *>(targetPreset->targetContainer.get());
		if (p1 != nullptr && p2 != nullptr && p1->group != p2->group)
		{
			LOGWARNING("The 2 presets are not from the same group !\nThis command won't have any effect until you choose presets from the same group.");
		}
	} else if (p == valueOperator)
	{
		if (value != nullptr)
		{
			Operator o = valueOperator->getValueDataAsEnum<Operator>();
			bool curHide = value->hideInEditor;
			value->hideInEditor = o == INVERSE;
			if (curHide != value->hideInEditor) queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, this));
		}
	}
}

void CVCommand::triggerInternal()
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
				Operator o = valueOperator->getValueDataAsEnum<Operator>();

				switch (o)
				{
				case EQUAL:
					p->setValue(value->value);
					break;

				case INVERSE:
					p->setNormalizedValue(1 - p->getNormalizedValue());
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

				case MAX:
					p->setValue(std::max(p->floatValue(),value->floatValue()));
					break;

				case MIN:
					p->setValue(std::min(p->floatValue(), value->floatValue()));
					break;
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

	case GO_TO_PRESET:
	{
		if (targetPreset->targetContainer != nullptr)
		{
			CVPreset* p1 = static_cast<CVPreset*>(targetPreset->targetContainer.get());
			p1->group->goToPreset(p1, time->enabled?time->floatValue():p1->defaultLoadTime->floatValue(), automation->enabled->boolValue()?automation:&p1->group->defaultInterpolation);
		}
	}
	break;

	case KILL_GO_TO_PRESET:
	{
		if (!target->targetContainer.wasObjectDeleted() && target->targetContainer != nullptr)
		{
			CVGroup* g = dynamic_cast<CVGroup*>(target->targetContainer.get());
			g->stopInterpolation();
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
		if (!target->targetContainer.wasObjectDeleted() && target->targetContainer != nullptr)
		{
			CVGroup * g = static_cast<CVGroup *>(target->targetContainer.get());
			if (g != nullptr && g->morpher != nullptr) g->morpher->targetPosition->setPoint(((Point2DParameter *)value)->getPoint());
		}
	}
	break;

	case LOAD_PRESET:
	case SAVE_PRESET:
	{
		if (targetPreset->targetContainer != nullptr)
		{
			CVPreset* p = static_cast<CVPreset*>(targetPreset->targetContainer.get());
			if (p != nullptr)
			{
				File f = presetFile->getFile();
				if (type == LOAD_PRESET)
				{
					if (f.exists())
					{
						std::unique_ptr<InputStream> is(f.createInputStream());
						var data = JSON::fromString(is->readEntireStreamAsString());
						p->loadValuesFromJSON(data);
					}
					else
					{
						NLOGWARNING(niceName, "Preset file does not exist");
					}
				}
				else if (type == SAVE_PRESET)
				{
					if (f.exists()) f.deleteFile();

					var data = p->getValuesAsJSON();
					std::unique_ptr<OutputStream> os(f.createOutputStream());
					JSON::writeToStream(*os, data);
					os->flush();
				}
			}
		}
	}
	break;

	}

}

BaseCommand * CVCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	return new CVCommand((CustomVariablesModule *)module, context, params);
}
