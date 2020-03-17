/*
  ==============================================================================

    ConversionFilter.h
    Created: 3 Mar 2020 10:14:38pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../MappingFilter.h"
#include "ConvertedParameterManager.h"
#include "ConversionParamValueLink.h"

class ConversionFilter :
	public MappingFilter,
	public ConvertedParameterManager::ManagerListener,
	public ConversionParamValueLink::ConversionLinkListener
{
public:
	ConversionFilter(var params);
	~ConversionFilter();

	ConvertedParameterManager cpm;
	OwnedArray<ConversionParamValueLink, CriticalSection> links;

	var ghostLinksData; //for after loading

	void clearItem() override;

	void itemAdded(ConvertedParameter*) override;
	void itemRemoved(ConvertedParameter*) override;
	void itemsReordered() override;

	void reorderFilterParameters();

	void createLink(WeakReference<Parameter>, int sourceValueIndex, ConvertedParameter* out, int outValueIndex);
	void removeLink(ConversionParamValueLink* link);

	void relinkGhostData();

	ConversionParamValueLink* getLinkForOut(ConvertedParameter* out, int outValueIndex);

	void setupParametersInternal() override;
	void processSingleParameterInternal(Parameter * source, Parameter *out) override;

	void askForRemove(ConversionParamValueLink* link) override;

	var getJSONData() override;
	void loadJSONDataItemInternal(var data) override;


	class ConversionFilterEvent {
	public:
		enum Type { SOURCES_UPDATED, LINKS_UPDATED };
		ConversionFilterEvent(Type type) : type(type) {}
		Type type;
	};

	QueuedNotifier<ConversionFilterEvent> conversionFilterAsyncNotifier;
	typedef QueuedNotifier<ConversionFilterEvent>::Listener AsyncConversionFilterListener;

	void addAsyncConversionFilterListener(AsyncConversionFilterListener* newListener) { conversionFilterAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedConversionFilterListener(AsyncConversionFilterListener* newListener) { conversionFilterAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncConversionFilterListener(AsyncConversionFilterListener* listener) { conversionFilterAsyncNotifier.removeListener(listener); }


	static ConversionFilter * create(var params) { return new ConversionFilter(params); }
	String getTypeString() const override { return getTypeStringStatic(); }
	static const String getTypeStringStatic() { return "Mega Converter"; }

	InspectableEditor* getEditor(bool isRoot);
};