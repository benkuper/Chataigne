/*
  ==============================================================================

    MappingFilter.cpp
    Created: 28 Oct 2016 8:08:53pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingFilter.h"
#include "ControllableFactory.h"
#include "MappingFilterEditor.h"

MappingFilter::MappingFilter(const String &name) :
	BaseItem(name),
	filteredParameter(nullptr),
	sourceParam(nullptr)
{
	isSelectable = false;
}

MappingFilter::~MappingFilter()
{
}

void MappingFilter::setupSource(Parameter * source)
{
	if (filteredParameter != nullptr)
	{
		removeControllable(filteredParameter);
	}
	
	sourceParam = source;

	filteredParameter = setupParameterInternal(source);
	if (filteredParameter != nullptr)
	{
		filteredParameter->isControllableFeedbackOnly = true;
		addParameter(filteredParameter);
	}
	
	mappingFilterListeners.call(&FilterListener::filteredParamChanged, this);
}

Parameter * MappingFilter::setupParameterInternal(Parameter * source)
{
	if (source == nullptr) return nullptr;
	Parameter * p = (Parameter *) ControllableFactory::createControllable(source->getTypeString());
	p->setNiceName("Out");
	p->setValue(source->value);
	p->setRange(p->minimumValue, p->maximumValue);
	return p;
}

Parameter * MappingFilter::process(Parameter * source)
{
	if(!enabled->boolValue()) return source; //default or disabled does nothing
	if (sourceParam == nullptr || filteredParameter == nullptr || source != sourceParam) return source;

	processInternal(sourceParam,filteredParameter); 

	return filteredParameter;
}

var MappingFilter::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("type", getTypeString());
	return data;
}

InspectableEditor * MappingFilter::getEditor(bool isRoot)
{
	return new MappingFilterEditor(this,isRoot);
}
