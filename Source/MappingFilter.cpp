/*
  ==============================================================================

    MappingFilter.cpp
    Created: 28 Oct 2016 8:08:53pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingFilter.h"
#include "ControllableFactory.h"

MappingFilter::MappingFilter(const String &name) :
	BaseItem(name),
	filteredParameter(nullptr)
{
	isSelectable = false;
}

MappingFilter::~MappingFilter()
{
}

void MappingFilter::setupParameter(Parameter * source)
{
	if (filteredParameter != nullptr)
	{
		removeControllable(filteredParameter);
	}

	filteredParameter = setupParameterInternal(source);
	filteredParameter->isControllableFeedbackOnly = true;
	addParameter(filteredParameter);
}

Parameter * MappingFilter::setupParameterInternal(Parameter * source)
{
	Parameter * p = (Parameter *) ControllableFactory::createControllable(source->getTypeString());
	p->setNiceName("Out");
	p->setValue(source->value);
	p->setRange(p->minimumValue, p->maximumValue);
	return p;
}

Parameter * MappingFilter::process(Parameter * sourceParam)
{
	if(!enabled->boolValue()) return sourceParam; //default or disabled does nothing
	if (filteredParameter == nullptr) setupParameter(sourceParam);

	processInternal(sourceParam,filteredParameter); 

	return filteredParameter;
}

var MappingFilter::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("type", getTypeString());
	return data;
}
