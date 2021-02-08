/*
  ==============================================================================

    MappingFilterManager.cpp
    Created: 28 Oct 2016 8:08:47pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingFilterManager.h"

#include "filters/ScriptFilter.h"

#include "filters/number/SimpleRemapFilter.h"
#include "filters/number/CurveMapFilter.h"
#include "filters/number/InverseFilter.h"
#include "filters/number/SimpleSmoothFilter.h"
#include "filters/number/DampingFilter.h"
#include "filters/number/LagFilter.h"
#include "filters/number/CropFilter.h"
#include "filters/number/MathFilter.h"

#include "filters/conversion/ConversionFilter.h"
#include "filters/conversion/SimpleConversionFilters.h"

#include "filters/color/ColorShiftFilter.h"


MappingFilterManager::MappingFilterManager(Multiplex * multiplex) :
	BaseManager<MappingFilter>("Filters"),
	MultiplexTarget(multiplex)
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

	if (!isMultiplexed()) //Right now, filters that have different in <> out parameter layout are not supported in multiplex
	{
		factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ConversionFilter>("Conversion", ConversionFilter::getTypeStringStatic(), multiplex));
	}
		
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<SimpleSmoothFilter>("Time", "Smooth", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<DampingFilter>("Time", "Damping", multiplex));
	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<LagFilter>("Time", "FPS", multiplex));

	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ColorShiftFilter>("Color", ColorShiftFilter::getTypeStringStatic(), multiplex));

	factory.defs.add(MultiplexTargetDefinition<MappingFilter>::createDef<ScriptFilter>("", "Script", multiplex));

	selectItemWhenCreated = false;
}

MappingFilterManager::~MappingFilterManager()
{
}

bool MappingFilterManager::setupSources(Array<Parameter *> sources, int multiplexIndex)
{
	if (isCurrentlyLoadingData) return false;
	
	multiplexInputSourceMap.set(multiplexIndex, sources);

	filteredParamMap.set(multiplexIndex, sources);

	return rebuildFilterChain(nullptr, multiplexIndex);
}


bool MappingFilterManager::processFilters(Array<Parameter *> inputs, int multiplexIndex)
{
	if (getLastEnabledFilter() == nullptr)
	{
		filteredParamMap.set(multiplexIndex, inputs);// Array<Parameter*>(multiplexInputSourceMap[multiplexIndex].getRawDataPointer(), multiplexInputSourceMap[multiplexIndex].size());
		return true;
	}


	jassert(inputs.size() == multiplexInputSourceMap[multiplexIndex].size());

	Array<Parameter *> fp = inputs;
	bool hasChanged = false;

	for (auto &f : items)
	{
		hasChanged |= f->process(inputs, multiplexIndex);
		fp = Array<Parameter *>(f->filteredParameters[multiplexIndex]->getRawDataPointer(), f->filteredParameters[multiplexIndex]->size());
	}
	
	filteredParamMap.set(multiplexIndex, fp);

	return hasChanged;
}

bool MappingFilterManager::rebuildFilterChain(MappingFilter * afterThisFilter, int multiplexIndex)
{
	Array<Parameter *> fp = multiplexInputSourceMap[multiplexIndex];
	lastEnabledFilter = nullptr;
	bool foundFilter = afterThisFilter == nullptr?true:false;
	for (auto& f : items)
	{
		
		if (f == afterThisFilter) foundFilter = true;
		else
		{
			if (!foundFilter) continue;
			if (!f->enabled->boolValue()) continue;

			bool setupResult = f->setupSources(fp, multiplexIndex);
			if (!setupResult) continue;
		}
		
		fp = Array<Parameter*>(f->filteredParameters[multiplexIndex]->getRawDataPointer(), f->filteredParameters[multiplexIndex]->size());
		lastEnabledFilter = f;
	}

	return true;
}

void MappingFilterManager::notifyNeedsRebuild(MappingFilter* afterThisFilter)
{
	if (isCurrentlyLoadingData) return;
	filterManagerListeners.call(&FilterManagerListener::filterManagerNeedsRebuild, afterThisFilter);
}

Array<Parameter *> MappingFilterManager::getLastFilteredParameters(int multiplexIndex)
{
	return filteredParamMap[multiplexIndex];
	
	//if (lastEnabledFilter != nullptr) return Array<Parameter *>(lastEnabledFilter->filteredParameters[multiplexIndex]->getRawDataPointer(), lastEnabledFilter->filteredParameters[multiplexIndex]->size());
	//else return multiplexInputSourceMap[multiplexIndex];
}

void MappingFilterManager::addItemInternal(MappingFilter *f , var)
{
	ScopedLock lock(filterLock); //avoid removing while serving
	notifyNeedsRebuild();
	f->addMappingFilterListener(this);
}

void MappingFilterManager::removeItemInternal(MappingFilter * f)
{
	ScopedLock lock(filterLock); //avoid removing while serving
	f->removeMappingFilterListener(this);
	notifyNeedsRebuild();
}

void MappingFilterManager::setItemIndex(MappingFilter* item, int index)
{
	BaseManager::setItemIndex(item, index);
	notifyNeedsRebuild();
}

void MappingFilterManager::reorderItems()
{
	BaseManager::reorderItems(); 
	notifyNeedsRebuild();
}

void MappingFilterManager::filterStateChanged(MappingFilter* mf)
{
	notifyNeedsRebuild(); //do not specify the filter so the complete chain is rebuild
}

void MappingFilterManager::filterNeedsProcess(MappingFilter* mf)
{
	filterManagerListeners.call(&FilterManagerListener::filterManagerNeedsProcess);
}

void MappingFilterManager::filteredParamsChanged(MappingFilter* mf)
{
	notifyNeedsRebuild(mf);
}

void MappingFilterManager::filteredParamRangeChanged(MappingFilter* mf)
{
	notifyNeedsRebuild(mf);
}

void MappingFilterManager::loadJSONDataManagerInternal(var data)
{
	BaseManager::loadJSONDataManagerInternal(data);
	notifyNeedsRebuild();
}
