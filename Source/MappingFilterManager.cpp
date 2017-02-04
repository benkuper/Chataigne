/*
  ==============================================================================

    MappingFilterManager.cpp
    Created: 28 Oct 2016 8:08:47pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingFilterManager.h"
#include "MappingFilterManagerEditor.h"
#include "MappingFilterFactory.h"

MappingFilterManager::MappingFilterManager() :
	BaseManager<MappingFilter>("Filters"),
	inputSourceParam(nullptr)
{
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

void MappingFilterManager::addItemFromData(var data)
{
	String moduleType = data.getProperty("type", "none");
	if (moduleType.isEmpty()) return;
	MappingFilter * i = MappingFilterFactory::getInstance()->createModule(moduleType);
	if (i != nullptr) addItem(i, data);
}

void MappingFilterManager::addItemInternal(MappingFilter * , var)
{
	rebuildFilterChain();
}

void MappingFilterManager::removeItemInternal(MappingFilter *)
{
	rebuildFilterChain();
}

InspectableEditor * MappingFilterManager::getEditor(bool isRoot)
{
	return new MappingFilterManagerEditor(this,isRoot);
}
