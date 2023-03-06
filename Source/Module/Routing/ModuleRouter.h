/*
  ==============================================================================

    ModuleRouter.h
    Created: 29 Apr 2017 10:35:18am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleRouterController;

class ModuleRouter :
	public BaseItem,
	public Inspectable::InspectableListener,
	public ContainerAsyncListener
{
public:
	ModuleRouter();
	~ModuleRouter();

	Module * sourceModule;
	Module * destModule;

	WeakReference<Inspectable> sourceModuleRef;
	WeakReference<Inspectable> destModuleRef;

	BaseManager<ModuleRouterValue> sourceValues;
	ModuleRouterController * routerController;

	Trigger * selectAllValues;
	Trigger * deselectAllValues;
	Trigger * routeAllValues;

	void setSourceModule(Module * m);
	void setDestModule(Module * m);

	void reloadSourceValues(bool keepData = true);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void newMessage(const ContainerAsyncEvent &e) override;

	ModuleRouterValue * getRouterValueForControllable(Controllable * c);

	void onContainerParameterChangedInternal(Parameter * p) override;
	void onContainerTriggerTriggered(Trigger * t) override;

	void inspectableDestroyed(Inspectable * m) override;


	class RouterListener
	{
	public:
		virtual ~RouterListener() {}
		virtual void sourceModuleChanged(ModuleRouter *) {}
		virtual void destModuleChanged(ModuleRouter *) {}
	};

	ListenerList<RouterListener> routerListeners;
	void addRouterListener(RouterListener* newListener) { routerListeners.add(newListener); }
	void removeRouterListener(RouterListener* listener) { routerListeners.remove(listener); }

};