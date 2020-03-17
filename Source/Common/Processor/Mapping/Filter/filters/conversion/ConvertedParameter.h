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
	WeakReference<Parameter> outParamReference;
	EnumParameter* conversionMode;

	void setParamValueAtIndex(var value, int index);
	void onContainerParameterChangedInternal(Parameter* p) override;

	void setOutParamReference(Parameter* p);

	String getTypeString() const override { return defaultParam->getTypeString(); }
	static ConvertedParameter* create(var params) { return new ConvertedParameter(params); }

	InspectableEditor* getEditor(bool isRoot) override;


	class CPEvent {
	public:
		enum Type { OUT_PARAM_CHANGED };
		CPEvent(Type type) : type(type) {}
		Type type;
	};

	QueuedNotifier<CPEvent> cpAsyncNotifier;
	typedef QueuedNotifier<CPEvent>::Listener AsyncCPListener;

	void addAsyncCPListener(AsyncCPListener* newListener) { cpAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedCPListener(AsyncCPListener* newListener) { cpAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncCPListener(AsyncCPListener* listener) { cpAsyncNotifier.removeListener(listener); }


};
