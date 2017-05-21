/*
  ==============================================================================

    ModuleRouter.h
    Created: 29 Apr 2017 10:35:18am
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEROUTER_H_INCLUDED
#define MODULEROUTER_H_INCLUDED


#include "Module.h"
#include "ModuleRouterValue.h"

class ModuleRouter :
	public BaseItem,
	public Inspectable::InspectableListener,
	public ControllableContainer::ContainerAsyncListener
{
public:
	ModuleRouter();
	~ModuleRouter();

	Module * sourceModule;
	Module * destModule;
	BaseManager<ModuleRouterValue> sourceValues;

	Trigger * selectAllValues;
	Trigger * deselectAllValues;

	void setSourceModule(Module * m);
	void setDestModule(Module * m);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void newMessage(const ContainerAsyncEvent &e) override;

	ModuleRouterValue * getRouterValueForControllable(Controllable * c);

	void onContainerTriggerTriggered(Trigger *) override;

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

#endif  // MODULEROUTER_H_INCLUDED
