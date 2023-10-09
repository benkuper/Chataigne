/*
  ==============================================================================

    ConversionFilter.h
    Created: 3 Mar 2020 10:14:38pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConversionFilter :
	public MappingFilter,
	public ConvertedParameterManager::ManagerListener,
	public ConversionParamValueLink::ConversionLinkListener
{
public:
	ConversionFilter(var params, Multiplex* multiplex);
	~ConversionFilter();

	ConvertedParameterManager cpm;
	OwnedArray<ConversionParamValueLink, CriticalSection> links;

	var ghostLinksData; //for after loading

	void clearItem() override;

	void itemAdded(ConvertedParameter*) override;
	void itemsAdded(Array<ConvertedParameter*> items) override;
	void itemRemoved(ConvertedParameter*) override;
	void itemsRemoved(Array<ConvertedParameter*> items) override;
	void itemsReordered() override;

	void reorderFilterParameters();

	void createLink(int sourceIndex, int sourceValueIndex, ConvertedParameter* out, int outValueIndex);
	void removeLink(ConversionParamValueLink* link);

	void relinkGhostData();

	ConversionParamValueLink* getLinkForOut(ConvertedParameter* out, int outValueIndex);

	void setupParametersInternal(int multiplexIndex, bool rangeOnly) override;
	ProcessResult processInternal(Array<Parameter *> inputs, int multiplexIndex) override;

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


	String getTypeString() const override { return getTypeStringStatic(); }
	static const String getTypeStringStatic() { return "Mega Converter"; }

    InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
};
