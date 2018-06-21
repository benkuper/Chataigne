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
#include "filters/ConversionFilters.h"

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
	
	factory.defs.add(Factory<MappingFilter>::Definition::createDef("Conversion", "Convert To Integer", &ToIntFilter::create));

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
	lastEnabledFilter = nullptr;
	for (auto &f : items)
	{
		if (f->enabled->boolValue())
		{
			f->setupSource(fp);
			fp = f->filteredParameter;
			lastEnabledFilter = f;
		}
	}
}

void MappingFilterManager::addItemInternal(MappingFilter *f , var)
{
	rebuildFilterChain();
	f->addAsyncFilterListener(this);
}

void MappingFilterManager::removeItemInternal(MappingFilter *)
{
	rebuildFilterChain();
}

void MappingFilterManager::reorderItems()
{
	BaseManager::reorderItems(); 
	rebuildFilterChain();
	baseManagerListeners.call(&BaseManager::Listener::itemsReordered);
	managerNotifier.addMessage(new ManagerEvent(ManagerEvent::ITEMS_REORDERED));
}


void MappingFilterManager::newMessage(const MappingFilter::FilterEvent & e)
{
	if (e.type == MappingFilter::FilterEvent::FILTER_STATE_CHANGED)
	{
		rebuildFilterChain();
		baseManagerListeners.call(&BaseManager::Listener::itemsReordered);
		managerNotifier.addMessage(new ManagerEvent(ManagerEvent::ITEMS_REORDERED));
	}
}
