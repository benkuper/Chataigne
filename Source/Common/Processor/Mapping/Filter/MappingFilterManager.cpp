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
#include "filters/number/LagFilter.h"
#include "filters/number/CropFilter.h"
#include "filters/number/MathFilter.h"

#include "filters/conversion/ConversionFilter.h"
#include "filters/conversion/SimpleConversionFilters.h"

#include "filters/color/ColorShiftFilter.h"


MappingFilterManager::MappingFilterManager() :
	BaseManager<MappingFilter>("Filters")
{
	canBeCopiedAndPasted = true;

	managerFactory = &factory;

	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Remap", &SimpleRemapFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Curve Map", &CurveMapFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Math", &MathFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Inverse", &InverseFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Crop", &CropFilter::create));
	
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Conversion", "Convert To Integer", &ToIntFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Conversion", "Convert To Float", &ToFloatFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Conversion", "Convert To Boolean", &ToBooleanFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Conversion", "Convert To String", &ToStringFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Conversion", "Convert To Point2D", &ToPoint2DFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Conversion", "Convert To Point3D", &ToPoint3DFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Conversion", "Convert To Color", &ToColorFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Conversion", ConversionFilter::getTypeStringStatic(), &ConversionFilter::create));

	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Time", "Smooth", &SimpleSmoothFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Time", "FPS", &LagFilter::create));

	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Color", ColorShiftFilter::getTypeStringStatic(), &ColorShiftFilter::create));

	factory.defs.add(Factory<MappingFilter>::Definition::createDef("", "Script", &ScriptFilter::create));

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


Array<Parameter *> MappingFilterManager::processFilters()
{
	Array<Parameter *> fp = inputSourceParams;
	for (auto &f : items)
	{
		if (f->process()) fp = Array<Parameter *>(f->filteredParameters.getRawDataPointer(), f->filteredParameters.size());
	}
	
	return fp;
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
	notifyNeedsRebuild();
	f->addMappingFilterListener(this);
}

void MappingFilterManager::removeItemInternal(MappingFilter * f)
{
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
