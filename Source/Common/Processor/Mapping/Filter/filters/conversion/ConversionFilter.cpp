/*
  ==============================================================================

	ConversionFilter.cpp
	Created: 3 Mar 2020 10:14:38pm
	Author:  bkupe

  ==============================================================================
*/

#include "ConversionFilter.h"
#include "ui/ConversionFilterEditor.h"

ConversionFilter::ConversionFilter(var params) :
	MappingFilter(getTypeString(), params),
	conversionFilterAsyncNotifier(10)
{
	autoSetRange = false;

	cpm.addBaseManagerListener(this);
	addChildControllableContainer(&cpm);
}

ConversionFilter::~ConversionFilter() 
{
	cpm.removeBaseManagerListener(this);
}

void ConversionFilter::itemAdded(ConvertedParameter* cp)
{
	Parameter* p = ControllableFactory::createParameterFrom(cp->defaultParam, false, true);
	p->setNiceName("Out " + p->getTypeString());
	cp->setOutParamReference(p);
	p->setControllableFeedbackOnly(true);
	p->isSavable = false;
	filteredParameters.add(p);
	reorderFilterParameters();
}


void ConversionFilter::itemRemoved(ConvertedParameter* cp)
{
	filteredParameters.removeObject(cp->outParamReference);
	
	Array<ParamValueLink*> linksToRemove;
	{
		GenericScopedLock lock(links.getLock());
		for (auto& link : links)
		{
			if (link->out == cp) linksToRemove.add(link);
		}
	}

	for (auto& link : linksToRemove)
	{
		links.removeObject(link);
	}

	reorderFilterParameters();
}

void ConversionFilter::itemsReordered()
{
	reorderFilterParameters();
}

void ConversionFilter::reorderFilterParameters()
{
	for (int i = 0; i < cpm.items.size(); i++) filteredParameters.set(i, cpm.items[i]->outParamReference, false);
	mappingFilterListeners.call(&FilterListener::filteredParamsChanged, this);
	filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
}


void ConversionFilter::createLink(int sourceIndex, int sourceValueIndex, ConvertedParameter* out, int outValueIndex)
{
	ParamValueLink* link = getLinkForOut(out, outValueIndex);
	if (link == nullptr) link = new ParamValueLink();
	link->sourceIndex = sourceIndex;
	link->sourceValueIndex = sourceValueIndex;
	link->out = out;
	link->outValueIndex = outValueIndex;

	conversionFilterAsyncNotifier.addMessage(new ConversionFilterEvent(ConversionFilterEvent::LINKS_UPDATED));
}

void ConversionFilter::removeLink(ParamValueLink * link)
{
	links.removeObject(link);
	conversionFilterAsyncNotifier.addMessage(new ConversionFilterEvent(ConversionFilterEvent::LINKS_UPDATED));
}

ConversionFilter::ParamValueLink* ConversionFilter::getLinkForOut(ConvertedParameter* out, int outValueIndex)
{
	GenericScopedLock lock(links.getLock());
	for (auto& link : links) if (link->out == out && link->outValueIndex == outValueIndex) return link;
	return nullptr;
}

void ConversionFilter::setupParametersInternal()
{
	//do not call parent, we have our own filteredParameterCreation implementation

	//need here to reconnect links to new source parameters

	//conversionFilterAsyncNotifier.addMessage(new ConversionFilterEvent(ConversionFilterEvent::LINKS_UPDATED));
}

void ConversionFilter::processSingleParameterInternal(Parameter* source, Parameter* out)
{
	GenericScopedLock lock(links.getLock());
	int sourceIndex = sourceParams.indexOf(source);
	for (auto& link : links)
	{
		if (link->out == nullptr) continue;
		if (link->sourceIndex != sourceIndex || link->out->outParamReference != out) continue;

		link->out->setParamValueAtIndex(source->value[link->sourceValueIndex], link->outValueIndex);
	}
}

var ConversionFilter::getJSONData()
{
	var data = MappingFilter::getJSONData();
	data.getDynamicObject()->setProperty(cpm.shortName, cpm.getJSONData());
	return data;
}

void ConversionFilter::loadJSONDataItemInternal(var data)
{
	MappingFilter::loadJSONDataItemInternal(data);
	cpm.loadJSONData(data.getProperty(cpm.shortName, var()));
}

InspectableEditor* ConversionFilter::getEditor(bool isRoot)
{
	return new ConversionFilterEditor(this, isRoot);
}
