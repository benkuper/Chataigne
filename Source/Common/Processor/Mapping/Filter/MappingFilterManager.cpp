/*
  ==============================================================================

	MappingFilterManager.cpp
	Created: 28 Oct 2016 8:08:47pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

MappingFilterManager::MappingFilterManager(Multiplex* multiplex) :
	BaseManager<MappingFilter>("Filters"),
	MultiplexTarget(multiplex),
	isRebuilding(false)
{
	canBeCopiedAndPasted = true;

	managerFactory = &factory;

	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<SimpleRemapFilter>("Remap", "Remap", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<CurveMapFilter>("Remap", "Curve Map", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<MathFilter>("Remap", "Math", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<InverseFilter>("Remap", "Inverse", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<CropFilter>("Remap", "Crop", multiplex));

	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ToIntFilter>("Conversion", "Convert To Integer", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ToFloatFilter>("Conversion", "Convert To Float", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ToBooleanFilter>("Conversion", "Convert To Boolean", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ToStringFilter>("Conversion", "Convert To String", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ToPoint2DFilter>("Conversion", "Convert To Point2D", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ToPoint3DFilter>("Conversion", "Convert To Point3D", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ToColorFilter>("Conversion", "Convert To Color", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<MergeFilter>("Conversion", "Merge", multiplex));

	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ConversionFilter>("Conversion", ConversionFilter::getTypeStringStatic(), multiplex));

	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<SimpleSmoothFilter>("Time", "Smooth", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<DampingFilter>("Time", "Damping", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<OneEuroFilter>("Time", "One Euro", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<LagFilter>("Time", "FPS", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<DelayFilter>("Time", "Delay", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<SpeedFilter>("Time", "Speed", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<FreezeFilter>("Time", "Freeze", multiplex));

	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ColorShiftFilter>("Color", ColorShiftFilter::getTypeStringStatic(), multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ColorRemapFilter>("Color", ColorRemapFilter::getTypeStringStatic(), multiplex));

	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<StringSliceFilter>("String", StringSliceFilter::getTypeStringStatic(), multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<StringReplaceFilter>("String", StringReplaceFilter::getTypeStringStatic(), multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<StringOffsetFilter>("String", StringOffsetFilter::getTypeStringStatic(), multiplex));

	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ConditionFilter>("", "Condition", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ScriptFilter>("", "Script", multiplex));

	selectItemWhenCreated = false;
}

MappingFilterManager::~MappingFilterManager()
{
}

bool MappingFilterManager::setupSources(Array<Parameter*> sources, int multiplexIndex)
{
	if (isCurrentlyLoadingData) return false;

	inputSources.set(multiplexIndex, sources);
	filteredParameters.set(multiplexIndex, sources);

	return rebuildFilterChain(nullptr, multiplexIndex);
}


MappingFilter::ProcessResult MappingFilterManager::processFilters(Array<Parameter*> inputs, int multiplexIndex)
{
	if (getLastEnabledFilter() == nullptr)
	{
		filteredParameters.set(multiplexIndex, inputs);// Array<Parameter*>(multiplexInputSourceMap[multiplexIndex].getRawDataPointer(), multiplexInputSourceMap[multiplexIndex].size());
		return MappingFilter::CHANGED;
	}


	jassert(inputs.size() == inputSources[multiplexIndex].size());
	if (inputs.size() != inputSources[multiplexIndex].size()) return MappingFilter::STOP_HERE;

	Array<Parameter*> fp = inputs;
	MappingFilter::ProcessResult result = MappingFilter::UNCHANGED;

	for (auto& f : items)
	{
		if (!f->enabled->boolValue()) continue; //f
		MappingFilter::ProcessResult r = f->process(fp, multiplexIndex);
		if (r == MappingFilter::STOP_HERE) return MappingFilter::STOP_HERE;
		else if (r == MappingFilter::CHANGED) result = MappingFilter::CHANGED;

		if (f->filteredParameters[multiplexIndex] == nullptr) return MappingFilter::STOP_HERE;
		fp = Array<Parameter*>(f->filteredParameters[multiplexIndex]->getRawDataPointer(), f->filteredParameters[multiplexIndex]->size());
	}

	filteredParameters.set(multiplexIndex, fp);

	return result;
}

bool MappingFilterManager::rebuildFilterChain(MappingFilter* afterThisFilter, int multiplexIndex, bool rangeOnly)
{
	isRebuilding = true;

	Array<Parameter*> fp = inputSources[multiplexIndex];

	if (!rangeOnly) lastEnabledFilter = nullptr;

	bool foundFilter = afterThisFilter == nullptr ? true : false;

	for (auto& f : items)
	{
		if (f == afterThisFilter)
		{
			foundFilter = true;
			OwnedArray<Parameter>* fParams = f->filteredParameters[multiplexIndex];
			if (fParams == nullptr)
			{
				isRebuilding = false;
				return false;
			}
			fp = Array<Parameter*>(fParams->getRawDataPointer(), fParams->size());
		}
		else
		{
			if (!foundFilter) continue;
			if (!f->enabled->boolValue()) continue;

			bool setupResult = f->setupSources(fp, multiplexIndex, rangeOnly);
			if (!setupResult) continue;
		}

		if (!rangeOnly)
		{
			OwnedArray<Parameter>* fParams = f->filteredParameters[multiplexIndex];
			fp = Array<Parameter*>(fParams->getRawDataPointer(), fParams->size());
			lastEnabledFilter = f;
		}
	}

	if (!rangeOnly) filteredParameters.set(multiplexIndex, fp);

	isRebuilding = false;

	return true;
}

void MappingFilterManager::notifyNeedsRebuild(MappingFilter* afterThisFilter, bool rangeOnly)
{
	if (isCurrentlyLoadingData || isRebuilding) return;
	filterManagerListeners.call(&FilterManagerListener::filterManagerNeedsRebuild, afterThisFilter, rangeOnly);
}

Array<Parameter*> MappingFilterManager::getLastFilteredParameters(int multiplexIndex)
{
	return filteredParameters[multiplexIndex];

	//if (lastEnabledFilter != nullptr) return Array<Parameter *>(lastEnabledFilter->filteredParameters[multiplexIndex]->getRawDataPointer(), lastEnabledFilter->filteredParameters[multiplexIndex]->size());
	//else return multiplexInputSourceMap[multiplexIndex];
}

void MappingFilterManager::addItemInternal(MappingFilter* f, var)
{
	ScopedLock lock(filterLock); //avoid removing while serving
	notifyNeedsRebuild();
	f->addMappingFilterListener(this);
}

void MappingFilterManager::removeItemInternal(MappingFilter* f)
{
	ScopedLock lock(filterLock); //avoid removing while serving
	f->removeMappingFilterListener(this);
	notifyNeedsRebuild();
}

void MappingFilterManager::setItemIndex(MappingFilter* item, int index, bool addToUndo)
{
	BaseManager::setItemIndex(item, index);
	if (!addToUndo) notifyNeedsRebuild();
}

void MappingFilterManager::reorderItems()
{
	BaseManager::reorderItems();
	notifyNeedsRebuild();
}

void MappingFilterManager::filterStateChanged(MappingFilter* mf)
{
	if (isRebuilding) return;
	int prevFilterIndex = items.indexOf(mf) - 1;
	MappingFilter* prevFilter = prevFilterIndex >= 0 ? items[prevFilterIndex] : nullptr;
	notifyNeedsRebuild(prevFilter); //do not specify the filter so the complete chain is rebuild
}

void MappingFilterManager::filterNeedsProcess(MappingFilter* mf)
{
	filterManagerListeners.call(&FilterManagerListener::filterManagerNeedsProcess);
}

void MappingFilterManager::filteredParamsChanged(MappingFilter* mf)
{
	if (isRebuilding) return;
	notifyNeedsRebuild(mf);
}

void MappingFilterManager::filteredParamRangeChanged(MappingFilter* mf)
{
	if (isRebuilding) return;
	notifyNeedsRebuild(mf, true);
}

void MappingFilterManager::loadJSONDataManagerInternal(var data)
{
	BaseManager::loadJSONDataManagerInternal(data);
	notifyNeedsRebuild();
}
