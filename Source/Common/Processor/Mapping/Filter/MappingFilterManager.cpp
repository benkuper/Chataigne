/*
  ==============================================================================

    MappingFilterManager.cpp
    Created: 28 Oct 2016 8:08:47pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingFilterManager.h"
#include "filters/SimpleRemapFilter.h"
#include "filters/CurveMapFilter.h"
#include "filters/ScriptFilter.h"
#include "filters/InverseFilter.h"
#include "filters/SimpleSmoothFilter.h"
#include "filters/LagFilter.h"
#include "filters/OffsetFilter.h"
#include "filters/CropFilter.h"

MappingFilterManager::MappingFilterManager() :
	BaseManager<MappingFilter>("Filters"),
	inputSourceParam(nullptr)
{
	managerFactory = &factory;
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Inverse", &InverseFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Offset", &OffsetFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Remap", &SimpleRemapFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Crop", &CropFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Remap", "Curve Map", &CurveMapFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Physics", "Smooth", &SimpleSmoothFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Physics", "Lag", &LagFilter::create));
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("", "Script", &ScriptFilter::create));

	selectItemWhenCreated = false;
}

MappingFilterManager::~MappingFilterManager()
{
}

void MappingFilterManager::setupSource(Parameter * source)
{
	inputSourceParam = source;
	rebuildFilterChain();
}


Parameter * MappingFilterManager::processFilters()
{
	Parameter * fp = inputSourceParam;
	for (auto &f : items)
	{
		fp = f->process(fp);
	}
	
	return fp;
}

void MappingFilterManager::rebuildFilterChain()
{
	Parameter * fp = inputSourceParam;
	for (auto &f : items)
	{
		f->setupSource(fp);
		fp = f->filteredParameter;
	}
}
void MappingFilterManager::addItemInternal(MappingFilter * , var)
{
	rebuildFilterChain();
}

void MappingFilterManager::removeItemInternal(MappingFilter *)
{
	rebuildFilterChain();
}

