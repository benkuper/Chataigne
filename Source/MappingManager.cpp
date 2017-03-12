/*
  ==============================================================================

    MappingManager.cpp
    Created: 28 Oct 2016 8:06:00pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingManager.h"

MappingManager::MappingManager() :
	BaseManager<Mapping>("Mappings"),
	forceDisabled(false)
{
	
}

MappingManager::~MappingManager()
{
}

void MappingManager::setForceDisabled(bool value)
{
	if (forceDisabled == value) return;
	forceDisabled = value;
	for (auto &i : items) i->forceDisabled = forceDisabled;
}

void MappingManager::addItemInternal(Mapping * item, var data)
{
	item->forceDisabled = forceDisabled;
}
