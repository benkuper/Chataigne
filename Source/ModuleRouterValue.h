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
	public BaseItem,
	public Inspectable::InspectableListener
{
public:
	ModuleRouterValue(Controllable * sourceValue = nullptr, Module * outModule = nullptr);
	~ModuleRouterValue();

	Module * outModule;

	TargetParameter * sourceTarget;
	BoolParameter * doRoute;

	Controllable * currentSourceValue;

	ScopedPointer<Module::RouteParams> routeParams;

	void setSourceValue(Controllable * c);
	void setOutModule(Module * outModule);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void inspectableDestroyed(Inspectable * i) override;
};
