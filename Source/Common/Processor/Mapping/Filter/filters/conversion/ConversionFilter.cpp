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
	clearItem();
}

void ConversionFilter::clearItem()
{
	DBG("Clear item, links size : " << links.size());
	links.clear();
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
	
	Array<ConversionParamValueLink*> linksToRemove;
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

	conversionFilterAsyncNotifier.addMessage(new ConversionFilterEvent(ConversionFilterEvent::LINKS_UPDATED));

	reorderFilterParameters();
}

void ConversionFilter::itemsReordered()
{
	reorderFilterParameters();
}

void ConversionFilter::reorderFilterParameters()
{
	for (int i = 0; i < cpm.items.size(); ++i) filteredParameters.set(i, cpm.items[i]->outParamReference, false);
	mappingFilterListeners.call(&FilterListener::filteredParamsChanged, this);
	filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
}


void ConversionFilter::createLink(WeakReference<Parameter> source, int sourceValueIndex, ConvertedParameter* out, int outValueIndex)
{
	if (source == nullptr) return;
	jassert(sourceParams.indexOf(source) != -1);

	ConversionParamValueLink* link = getLinkForOut(out, outValueIndex);
	if (link == nullptr)
	{
		link = new ConversionParamValueLink(sourceParams.indexOf(source), sourceValueIndex, out, outValueIndex);
		link->addConversionLinkListener(this);
		links.add(link);
	}
	else
	{
		link->sourceIndex = sourceParams.indexOf(source);
		link->sourceValueIndex = sourceValueIndex;
		link->out = out;
		link->outValueIndex = outValueIndex;
	}

	out->connectSlot(outValueIndex);

	conversionFilterAsyncNotifier.addMessage(new ConversionFilterEvent(ConversionFilterEvent::LINKS_UPDATED));
}

void ConversionFilter::removeLink(ConversionParamValueLink* link)
{
	if (link->out != nullptr && link->outValueIndex >= 0)
	{
		link->out->disconnectSlot(link->outValueIndex);
	}

	links.removeObject(link);
	conversionFilterAsyncNotifier.addMessage(new ConversionFilterEvent(ConversionFilterEvent::LINKS_UPDATED));
}

void ConversionFilter::relinkGhostData()
{
	if (sourceParams.size() == 0 || !ghostLinksData.isArray()) return;

	links.clear();
	for (int i = 0; i < ghostLinksData.size(); ++i)
	{
		var linkData = ghostLinksData[i];
		if (ConvertedParameter* cp = cpm.getItemWithName(linkData.getProperty("out", "")))
		{
			createLink(sourceParams[linkData.getProperty("sourceIndex", 0)], linkData.getProperty("sourceValueIndex", 0), cp, linkData.getProperty("outValueIndex", 0));
		}
	}

	ghostLinksData = var(); //Reset ghost data
}

ConversionParamValueLink* ConversionFilter::getLinkForOut(ConvertedParameter* out, int outValueIndex)
{
	GenericScopedLock lock(links.getLock());
	for (auto& link : links) if (link->out == out && link->outValueIndex == outValueIndex) return link;
	return nullptr;
}

void ConversionFilter::setupParametersInternal()
{
	//do not call parent, we have our own filteredParameterCreation implementation

	//need here to reconnect links to new source parameters

	relinkGhostData();

	conversionFilterAsyncNotifier.addMessage(new ConversionFilterEvent(ConversionFilterEvent::SOURCES_UPDATED));
}

bool ConversionFilter::processInternal()
{
	for (auto& s : sourceParams)
	{
		processSingleParameterInternal(s, nullptr); //don't care about output as they are recreated
	}

	return true;
}

void ConversionFilter::processSingleParameterInternal(Parameter* source, Parameter*)
{
	GenericScopedLock lock(links.getLock());
	int sourceIndex = sourceParams.indexOf(source);
	for (auto& link : links)
	{
		if (link->out == nullptr) continue;
		if (link->sourceIndex != sourceIndex) continue;

		link->out->setParamValueAtIndex(source->value.isArray()?source->value[link->sourceValueIndex]:source->value, link->outValueIndex);
	}
}

void ConversionFilter::askForRemove(ConversionParamValueLink* link)
{
	removeLink(link);
}

var ConversionFilter::getJSONData()
{
	var data = MappingFilter::getJSONData();
	data.getDynamicObject()->setProperty(cpm.shortName, cpm.getJSONData());
	
	var linkData;
	for (auto& link : links)
	{
		var lData = var(new DynamicObject());
		lData.getDynamicObject()->setProperty("sourceIndex", link->sourceIndex);
		lData.getDynamicObject()->setProperty("sourceValueIndex", link->sourceValueIndex);
		lData.getDynamicObject()->setProperty("out", link->out->shortName);
		lData.getDynamicObject()->setProperty("outValueIndex", link->outValueIndex);
		linkData.append(lData);
	}

	data.getDynamicObject()->setProperty("links", linkData);

	return data;
}

void ConversionFilter::loadJSONDataItemInternal(var data)
{
	MappingFilter::loadJSONDataItemInternal(data);
	cpm.loadJSONData(data.getProperty(cpm.shortName, var()));

	ghostLinksData = data.getProperty("links", var());
	relinkGhostData();
}

InspectableEditor* ConversionFilter::getEditor(bool isRoot)
{
	return new ConversionFilterEditor(this, isRoot);
}
