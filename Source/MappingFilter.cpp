/*
  ==============================================================================

    MappingFilter.cpp
    Created: 28 Oct 2016 8:08:53pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingFilter.h"
#include "MappingFilterEditor.h"

MappingFilter::MappingFilter(const String &name) :
	BaseItem(name),
sourceParam(nullptr),
filteredParameter(nullptr),
filterParams("filterParams"),
autoSetRange(true)
{
	isSelectable = false;

	filterParams.skipControllableNameInAddress = true;
	addChildControllableContainer(&filterParams);
	filterParams.addControllableContainerListener(this);
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

	if(source != nullptr) filteredParameter = setupParameterInternal(source);
	else filteredParameter = nullptr;

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
	p->setValue(source->getValue());
	p->setRange(source->minimumValue, source->maximumValue);
	return p;
}

void MappingFilter::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * p)
{
	if (cc == &filterParams) filterParamChanged((Parameter *)p);
}

Parameter * MappingFilter::process(Parameter * source)
{
	if(!enabled->boolValue()) return source; //default or disabled does nothing
	if (sourceParam == nullptr || filteredParameter == nullptr || filteredParameter.wasObjectDeleted() || source != sourceParam) return source;
	
	if (autoSetRange && (filteredParameter->minimumValue != sourceParam->minimumValue || filteredParameter->maximumValue != sourceParam->maximumValue))
	{
		filteredParameter->setRange(sourceParam->minimumValue, sourceParam->maximumValue);
	}

	processInternal(); 

	return filteredParameter;
}

var MappingFilter::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("filterParams", filterParams.getJSONData());
	return data;
}

void MappingFilter::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	filterParams.loadJSONData(data.getProperty("filterParams", var()));
}

InspectableEditor * MappingFilter::getEditor(bool isRoot)
{
	return new GenericMappingFilterEditor(this,isRoot);
}
