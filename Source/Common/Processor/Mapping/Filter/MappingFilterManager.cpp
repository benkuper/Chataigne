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

	factory.defs.add(Factory<MappingFilter>::Definition::createDef<SimpleRemapFilter>("Remap", "Remap"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<CurveMapFilter>("Remap", "Curve Map"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<MathFilter>("Remap", "Math"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<InverseFilter>("Remap", "Inverse"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<CropFilter>("Remap", "Crop"));

	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ToIntFilter>("Conversion", "Convert To Integer"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ToFloatFilter>("Conversion", "Convert To Float"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ToBooleanFilter>("Conversion", "Convert To Boolean"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ToStringFilter>("Conversion", "Convert To String"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ToPoint2DFilter>("Conversion", "Convert To Point2D"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ToPoint3DFilter>("Conversion", "Convert To Point3D"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ToColorFilter>("Conversion", "Convert To Color"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ConversionFilter>("Conversion", ConversionFilter::getTypeStringStatic()));

	factory.defs.add(Factory<MappingFilter>::Definition::createDef<SimpleSmoothFilter>("Time", "Smooth"));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef<DampingFilter>("Time", "Damping"));
		factory.defs.add(Factory<MappingFilter>::Definition::createDef<LagFilter>("Time", "FPS"));

	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ColorShiftFilter>("Color", ColorShiftFilter::getTypeStringStatic()));

	factory.defs.add(Factory<MappingFilter>::Definition::createDef<ScriptFilter>("", "Script"));

	selectItemWhenCreated = false;
}

MappingFilterManager::~MappingFilterManager()
{
}

bool MappingFilterManager::setupSources(Array<Parameter *> sources)
{
	if (isCurrentlyLoadingData) return false;
	
	inputSourceParams = sources;
	return rebuildFilterChain();
}


bool MappingFilterManager::processFilters(int multiplexIndex)
{
	if (items.size() == 0)
	{
		filteredParameters = Array<Parameter*>(inputSourceParams.getRawDataPointer(), inputSourceParams.size());
		return true;
	}

	Array<Parameter *> fp = inputSourceParams;
	bool hasChanged = false;

	for (auto &f : items)
	{
		hasChanged |= f->process();
		fp = Array<Parameter *>(f->filteredParameters.getRawDataPointer(), f->filteredParameters.size());
	}
	
	filteredParameters = fp;

	return hasChanged;
}

bool MappingFilterManager::rebuildFilterChain(MappingFilter * afterThisFilter)
{
	Array<Parameter *> fp = inputSourceParams;
	lastEnabledFilter = nullptr;
	bool foundFilter = afterThisFilter == nullptr?true:false;
	for (auto& f : items)
	{
		
		if (f == afterThisFilter) foundFilter = true;
		else
		{
			if (!foundFilter) continue;
			if (!f->enabled->boolValue()) continue;

			bool setupResult = f->setupSources(fp);
			if (!setupResult) continue;
		}
		
		fp = Array<Parameter*>(f->filteredParameters.getRawDataPointer(), f->filteredParameters.size());
		lastEnabledFilter = f;
	}

	return true;
}

void MappingFilterManager::notifyNeedsRebuild(MappingFilter* afterThisFilter)
{
	if (isCurrentlyLoadingData) return;
	filterManagerListeners.call(&FilterManagerListener::filterManagerNeedsRebuild, afterThisFilter);
}

Array<Parameter *> MappingFilterManager::getLastFilteredParameters()
{
	if (lastEnabledFilter != nullptr) return Array<Parameter *>(lastEnabledFilter->filteredParameters.getRawDataPointer(), lastEnabledFilter->filteredParameters.size());
	else return inputSourceParams;
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
