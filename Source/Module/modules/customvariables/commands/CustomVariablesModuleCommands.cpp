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
	value(nullptr)

{
	saveAndLoadRecursiveData = true;

	type = (Type)(int)params.getProperty("type", 0);
	manager = _module->manager;

	if (type == SET_2DTARGET || type == KILL_GO_TO_PRESET || type == RANDOMIZE)
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
		getLinkedParam(targetPreset)->fullPresetSelectMode = true;

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
			getLinkedParam(targetPreset2)->fullPresetSelectMode = true;
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


void CVCommand::onContainerParameterChanged(Parameter* p)
{
	if (p == targetPreset || p == targetPreset2)
	{
		CVPreset* p1 = static_cast<CVPreset*>(targetPreset->targetContainer.get());
		CVPreset* p2 = static_cast<CVPreset*>(targetPreset->targetContainer.get());
		if (p1 != nullptr && p2 != nullptr && p1->group != p2->group)
		{
			LOGWARNING("The 2 presets are not from the same group !\nThis command won't have any effect until you choose presets from the same group.");
		}
	}
}

void CVCommand::triggerInternal(int multiplexIndex)
{
	if (isCurrentlyLoadingData) return;

	switch (type)
	{
	case SET_PRESET:
	{
		if (CVPreset* p = getLinkedTargetContainerAs<CVPreset>(targetPreset, multiplexIndex))
		{
			if (p != nullptr) p->group->setValuesToPreset(p);
		}
	}
	break;

	case GO_TO_PRESET:
	{
		if (CVPreset* p = getLinkedTargetContainerAs<CVPreset>(targetPreset, multiplexIndex))
		{
			p->group->goToPreset(p, time->enabled ? time->floatValue() : p->defaultLoadTime->floatValue(), automation->enabled->boolValue() ? automation : &p->group->defaultInterpolation);
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
		CVPreset* p1 = getLinkedTargetContainerAs<CVPreset>(targetPreset, multiplexIndex);
		CVPreset* p2 = getLinkedTargetContainerAs<CVPreset>(targetPreset2, multiplexIndex);

		if (p1 != nullptr && p2 != nullptr)
		{
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
		if (CVPreset* p = getLinkedTargetContainerAs<CVPreset>(targetPreset, multiplexIndex))
		{
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
		if (CVPreset* p = getLinkedTargetContainerAs<CVPreset>(targetPreset, multiplexIndex))
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
					NLOGWARNING(niceName, "Preset file does not exist or is not a valid JSON preset file");
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
	break;

	case RANDOMIZE:
	{
		if (!target->targetContainer.wasObjectDeleted() && target->targetContainer != nullptr)
		{
			CVGroup* g = dynamic_cast<CVGroup*>(target->targetContainer.get());
			g->randomizeValues();
		}
	}
	break;
	}

}

BaseCommand* CVCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	return new CVCommand((CustomVariablesModule*)module, context, params, multiplex);
}