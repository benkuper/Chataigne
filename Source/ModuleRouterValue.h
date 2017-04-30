/*
  ==============================================================================

    ModuleRouterValue.h
    Created: 30 Apr 2017 2:59:04pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "BaseItem.h"

class ModuleRouterValue :
	public BaseItem,
	public Inspectable::InspectableListener
{
public:
	ModuleRouterValue();
	~ModuleRouterValue();

	TargetParameter * sourceValue;
	BoolParameter * doRoute;
	
	RouteParams routeParams;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void inspectableDestroyed(Inspectable * i) override;
};

class RouteParams :
	public ControllableContainer
{
	StringParameter * outputName;
};