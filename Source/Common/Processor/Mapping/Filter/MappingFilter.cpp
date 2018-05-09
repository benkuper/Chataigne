/*
  ==============================================================================

    MappingFilter.cpp
    Created: 28 Oct 2016 8:08:53pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingFilter.h"
#include "ui/MappingFilterEditor.h"

MappingFilter::MappingFilter(const String &name, var params) :
	BaseItem(name),
	sourceParam(nullptr),
	filteredParameter(nullptr),
	filterParams("filterParams"),
	forceOutParameterType(""),
	needsContinuousProcess(false),
	autoSetRange(true),
	mappingFilterAsyncNotifier(10)
{
	isSelectable = false;

	filterParams.hideEditorHeader = true;
	filterParams.skipControllableNameInAddress = true;
	addChildControllableContainer(&filterParams);
	filterParams.addControllableContainerListener(this);

	forceOutParameterType = params.getProperty("forceType", "");
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
	mappingFilterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_PARAM_CHANGED, this));
}

Parameter * MappingFilter::setupParameterInternal(Parameter * source)
{
	if (source == nullptr) return nullptr;
	Parameter * p = (Parameter *) ControllableFactory::createControllable(forceOutParameterType.isNotEmpty()? forceOutParameterType :source->getTypeString());
	p->setNiceName("Out");
	p->setRange(source->minimumValue, source->maximumValue);
	p->setValue(source->getValue());
	p->hideInEditor = true;
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

	if (filteredParameter != nullptr) processInternal();  //avoid cross-thread crash

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
	return new MappingFilterEditor(this, isRoot);
}