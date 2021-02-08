/*
  ==============================================================================

    ConvertedParameter.h
    Created: 3 Mar 2020 10:30:57pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class ConvertedParameter :
	public BaseItem
{
public:
	ConvertedParameter(var params = var());

	enum ConversionMode { RGB, HSV };

	Parameter* defaultParam;
	Array<WeakReference<Parameter>> outParamReferences;
	EnumParameter* conversionMode;

	Array<bool> connectedSlots;

	bool areAllSlotsConnected() const;

	void connectSlot(int index);
	void disconnectSlot(int index);

	void setParamValueAtIndex(var value, int index, int multiplexIndex);
	void onContainerParameterChangedInternal(Parameter* p) override;

	void setOutParamReference(Parameter* p, int multiplexIndex);

	StringArray getValueNames() const;

	String getTypeString() const override { return defaultParam->getTypeString(); }
	static ConvertedParameter* create(var params) { return new ConvertedParameter(params); }

	InspectableEditor* getEditor(bool isRoot) override;


	class CPEvent {
	public:
		enum Type { OUT_PARAM_CHANGED, SLOT_CONNECTION_CHANGED };
		CPEvent(Type type) : type(type) {}
		Type type;
	};

	QueuedNotifier<CPEvent> cpAsyncNotifier;
	typedef QueuedNotifier<CPEvent>::Listener AsyncCPListener;

	void addAsyncCPListener(AsyncCPListener* newListener) { cpAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedCPListener(AsyncCPListener* newListener) { cpAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncCPListener(AsyncCPListener* listener) { cpAsyncNotifier.removeListener(listener); }


};
