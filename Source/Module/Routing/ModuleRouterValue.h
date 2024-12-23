/*
  ==============================================================================

    ModuleRouterValue.h
    Created: 30 Apr 2017 2:59:04pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleRouterValue :
	public BaseItem
{
public:
	ModuleRouterValue(Controllable * sourceValue = nullptr, int index = 0);
	~ModuleRouterValue();

	
	int valueIndex;
	StringParameter * customName;
	WeakReference<Controllable> sourceValue;
	Module * outModule;
	WeakReference<Inspectable> outModuleRef;

	bool forceDisabled; //for router enable

	std::unique_ptr<Module::RouteParams> routeParams;

	void setSourceAndOutModule(Module * sourceModule, Module * outModule);

	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataInternal(var data) override;

	void onContainerParameterChangedInternal(Parameter * p) override;
	void onExternalParameterValueChanged(Parameter * p) override;
	void onExternalTriggerTriggered(Trigger * t) override;
	void childStructureChanged(ControllableContainer* cc) override;
	
	
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
