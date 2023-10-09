/*
  ==============================================================================

	ConversionFilter.cpp
	Created: 3 Mar 2020 10:14:38pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

ConversionFilter::ConversionFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex),
	conversionFilterAsyncNotifier(10)
{
	autoSetRange = false;

	cpm.addBaseManagerListener(this);
	addChildControllableContainer(&cpm);

	while (filteredParameters.size() < getMultiplexCount()) filteredParameters.add(new OwnedArray<Parameter>());
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
	for (int i = 0; i < filteredParameters.size(); i++)
	{
		Parameter* p = ControllableFactory::createParameterFrom(cp->defaultParam, false, true);
		p->setNiceName("Out " + p->getTypeString());
		cp->setOutParamReference(p, i);
		p->setControllableFeedbackOnly(true);
		p->isSavable = false;

		filteredParameters[i]->add(p);
	}

	reorderFilterParameters();
}

void ConversionFilter::itemsAdded(Array<ConvertedParameter*> items)
{
	for (auto& cp : items)
	{
		for (int i = 0; i < filteredParameters.size(); i++)
		{
			Parameter* p = ControllableFactory::createParameterFrom(cp->defaultParam, false, true);
			p->setNiceName("Out " + p->getTypeString());
			cp->setOutParamReference(p, i);
			p->setControllableFeedbackOnly(true);
			p->isSavable = false;

			filteredParameters[i]->add(p);
		}
	}

	reorderFilterParameters();
}


void ConversionFilter::itemRemoved(ConvertedParameter* cp)
{
	for (int i = 0; i < filteredParameters.size(); i++)
	{
		filteredParameters[i]->removeObject(cp->outParamReferences[i]);
	}

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

void ConversionFilter::itemsRemoved(Array<ConvertedParameter*> items)
{
	for (auto& cp : items)
	{

		for (int i = 0; i < filteredParameters.size(); i++)
		{
			filteredParameters[i]->removeObject(cp->outParamReferences[i]);
		}

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
	for (int mi = 0; mi < filteredParameters.size(); mi++)
	{
		for (int i = 0; i < cpm.items.size(); ++i)
		{
			filteredParameters[mi]->set(i, cpm.items[i]->outParamReferences[mi], false);
		}

		mappingFilterListeners.call(&FilterListener::filteredParamsChanged, this);
		filterAsyncNotifier.addMessage(new FilterEvent(FilterEvent::FILTER_REBUILT, this));
	}
}

void ConversionFilter::createLink(int sourceIndex, int sourceValueIndex, ConvertedParameter* out, int outValueIndex)
{
	jassert(sourceIndex != -1);

	ConversionParamValueLink* link = getLinkForOut(out, outValueIndex);
	if (link == nullptr)
	{
		link = new ConversionParamValueLink(sourceIndex, sourceValueIndex, out, outValueIndex);
		link->addConversionLinkListener(this);
		links.add(link);
	}
	else
	{
		link->sourceIndex = sourceIndex;
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
			createLink(linkData.getProperty("sourceIndex", 0), linkData.getProperty("sourceValueIndex", 0), cp, linkData.getProperty("outValueIndex", 0));
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

void ConversionFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	//do not call parent, we have our own filteredParameterCreation implementation

	if (!rangeOnly)
	{
		while (filteredParameters.size() < getMultiplexCount()) filteredParameters.add(new OwnedArray<Parameter>());
		//need here to reconnect links to new source parameters
		relinkGhostData();
		conversionFilterAsyncNotifier.addMessage(new ConversionFilterEvent(ConversionFilterEvent::SOURCES_UPDATED));
	}
}

MappingFilter::ProcessResult ConversionFilter::processInternal(Array<Parameter*> inputs, int multiplexIndex)
{
	GenericScopedLock lock(links.getLock());

	int index = 0;
	for (int i = 0; i < inputs.size(); i++)
	{
		for (auto& link : links)
		{
			if (link->out == nullptr) continue;
			if (link->sourceIndex != i) continue;

			link->out->setParamValueAtIndex(inputs[i]->value.isArray() ? inputs[i]->value[link->sourceValueIndex] : inputs[i]->value, link->outValueIndex, multiplexIndex);
		}

		index++;
	}

	return CHANGED;
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

InspectableEditor* ConversionFilter::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ConversionFilterEditor(this, isRoot);
}
