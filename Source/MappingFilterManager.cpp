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
	BaseManager<MappingFilter>("Filters")
{
	selectItemWhenCreated = false;
}

MappingFilterManager::~MappingFilterManager()
{
}

Parameter * MappingFilterManager::processFilters(Parameter * p)
{
	Parameter * fp = p;
	for (auto &f : items)
	{
		fp = f->process(fp);
	}
	
	return fp;
}

void MappingFilterManager::addItemFromData(var data)
{
	String moduleType = data.getProperty("type", "none");
	if (moduleType.isEmpty()) return;
	MappingFilter * i = MappingFilterFactory::getInstance()->createModule(moduleType);
	if (i != nullptr) addItem(i, data);
}

InspectableEditor * MappingFilterManager::getEditor(bool isRoot)
{
	return new MappingFilterManagerEditor(this,isRoot);
}
