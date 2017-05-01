/*
  ==============================================================================

    ModuleRouterValue.h
    Created: 30 Apr 2017 2:59:04pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "BaseItem.h"
#include "Module.h"

class ModuleRouterValue :
	public BaseItem
{
public:
	ModuleRouterValue(Controllable * sourceValue = nullptr, int index = 0);
	~ModuleRouterValue();

	int valueIndex;
	Controllable * sourceValue;
	Module * outModule;

	BoolParameter * doRoute;

	ScopedPointer<Module::RouteParams> routeParams;

	void setOutModule(Module * outModule);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void onContainerParameterChangedInternal(Parameter * p) override;
	void onExternalParameterChanged(Parameter * p) override;
	void onExternalTriggerTriggered(Trigger * t) override;
	

	class ValueListener
	{
	public:
		virtual ~ValueListener() {}
		virtual void routeParamsChanged(ModuleRouterValue *) {}
	};

	ListenerList<ValueListener> valueListeners;
	void addValueListener(ValueListener* newListener) { valueListeners.add(newListener); }
	void removeValueListener(ValueListener* listener) { valueListeners.remove(listener); }
};
