/*
  ==============================================================================

	CustomVariablesModuleCommands.cpp
	Created: 23 Feb 2018 12:25:24am
	Author:  Ben

  ==============================================================================
*/

#include "CustomVariables/CustomVariablesIncludes.h"
#include "CustomVariablesModuleCommands.h"

CVCommand::CVCommand(CustomVariablesModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
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

	}
	else if (type == SET_2DTARGET || type == KILL_GO_TO_PRESET)
	{
		target = addTargetParameter("Target Group", "The group to target for this command", CVGroupManager::getInstance());
		target->targetType = TargetParameter::CONTAINER;
		target->customGetTargetContainerFunc = &CVGroupManager::showMenuAndGetGroup;
		target->showParentNameInEditor = false;

		if (type == SET_2DTARGET)
		{
			value = addPoint2DParameter("Position", "The target position in the 2D interpolator");
			linkParamToMappingIndex(value, 0);
		}
	}
	else if (type == SET_PRESET || type == LERP_PRESETS || type == SET_PRESET_WEIGHT || type == SAVE_PRESET || type == LOAD_PRESET || type == GO_TO_PRESET)
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
			linkParamToMappingIndex(value, 0);
		}
		break;

		case SET_PRESET_WEIGHT:
		{
			value = addFloatParameter("Weight", "The weight of the preset to set", 0, 0, 1);
			linkParamToMappingIndex(value, 0);
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

void CVCommand::updateValueFromTarget()
{
	if (value != nullptr)
	{
		ghostValueData = value->getJSONData();
		removeControllable(value);
	}

	Controllable* cvTarget = getLinkedTargetAs<Controllable>(target, 0); //use multiplex 0 to create param, should be better

	if (cvTarget != nullptr) value = ControllableFactory::createParameterFrom(cvTarget);
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

void CVCommand::updateOperatorOptions()
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
	value->hideInEditor = valueOperator->getValueDataAsEnum<Operator>() == INVERSE;
}

void CVCommand::onContainerParameterChanged(Parameter* p)
{
	if (p == target && type == SET_VALUE)
	{
		updateValueFromTarget();

	}
	else if (p == targetPreset || p == targetPreset2)
	{
		CVPreset* p1 = static_cast<CVPreset*>(targetPreset->targetContainer.get());
		CVPreset* p2 = static_cast<CVPreset*>(targetPreset->targetContainer.get());
		if (p1 != nullptr && p2 != nullptr && p1->group != p2->group)
		{
			LOGWARNING("The 2 presets are not from the same group !\nThis command won't have any effect until you choose presets from the same group.");
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

void CVCommand::triggerInternal(int multiplexIndex)
{
	switch (type)
	{
	case SET_VALUE:
	{
		Controllable* c = getLinkedTargetAs<Controllable>(target, multiplexIndex);

		if (c != nullptr && value != nullptr)
		{
			Parameter* p = static_cast<Parameter*>(c);

			if (p != nullptr)
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

	case SET_PRESET:
	{
		if (targetPreset->targetContainer != nullptr)
		{
			CVPreset* p = static_cast<CVPreset*>(targetPreset->targetContainer.get());
			if (p != nullptr) p->group->setValuesToPreset(p);
		}
	}
	break;

	case GO_TO_PRESET:
	{
		if (targetPreset->targetContainer != nullptr)
		{
			CVPreset* p1 = static_cast<CVPreset*>(targetPreset->targetContainer.get());
			p1->group->goToPreset(p1, time->enabled ? time->floatValue() : p1->defaultLoadTime->floatValue(), automation->enabled->boolValue() ? automation : &p1->group->defaultInterpolation);
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
			CVPreset* p1 = static_cast<CVPreset*>(targetPreset->targetContainer.get());
			CVPreset* p2 = static_cast<CVPreset*>(targetPreset2->targetContainer.get());
			if (p1->group != p2->group)
			{
				LOGWARNING("The 2 presets are not from the same group !\nThis command won't have any effect until you choose presets from the same group.");
			}
			else
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
			CVPreset* p = static_cast<CVPreset*>(targetPreset->targetContainer.get());
			if (p != nullptr) p->weight->setValue(getLinkedValue(value, multiplexIndex));
		}
	}
	break;

	case SET_2DTARGET:
	{
		if (!target->targetContainer.wasObjectDeleted() && target->targetContainer != nullptr)
		{
			CVGroup* g = static_cast<CVGroup*>(target->targetContainer.get());
			var val = getLinkedValue(value, multiplexIndex);
			Point<float> f(val[0], val[1]);
			if (g != nullptr && g->morpher != nullptr) g->morpher->targetPosition->setPoint(f);
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

void CVCommand::linkUpdated(ParameterLink* pLink)
{
	if (pLink->parameter == target) updateValueFromTarget();
}

void CVCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);
	loadGhostData(data);
}

void CVCommand::loadGhostData(var data)
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

BaseCommand* CVCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	return new CVCommand((CustomVariablesModule*)module, context, params, multiplex);
}
