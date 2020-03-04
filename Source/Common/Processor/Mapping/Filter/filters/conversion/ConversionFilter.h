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

class ConversionFilter :
	public MappingFilter,
	public ConvertedParameterManager::ManagerListener
{
public:
	ConversionFilter(var params);
	~ConversionFilter();

	struct ParamValueLink
	{
		int sourceIndex;
		int sourceValueIndex;
		ConvertedParameter* out;
		int outValueIndex;
	};

	ConvertedParameterManager cpm;
	OwnedArray<ParamValueLink, CriticalSection> links;

	void itemAdded(ConvertedParameter*) override;
	void itemRemoved(ConvertedParameter*) override;
	void itemsReordered() override;

	void reorderFilterParameters();

	void createLink(int sourceIndex, int sourceValueIndex, ConvertedParameter* out, int outValueIndex);
	void removeLink(ParamValueLink * link);


	ParamValueLink * getLinkForOut(ConvertedParameter* out, int outValueIndex);

	void setupParametersInternal() override;
	void processSingleParameterInternal(Parameter * source, Parameter *out) override;

	var getJSONData() override;
	void loadJSONDataItemInternal(var data) override;

	class ConversionFilterEvent {
	public:
		enum Type { LINKS_UPDATED };
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
	static const String getTypeStringStatic() { return "Conversion"; }

	InspectableEditor* getEditor(bool isRoot);
};