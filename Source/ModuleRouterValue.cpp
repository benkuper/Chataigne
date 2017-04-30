/*
  ==============================================================================

	ModuleRouterValue.cpp
	Created: 30 Apr 2017 2:59:04pm
	Author:  Ben

  ==============================================================================
*/

#include "ModuleRouterValue.h"
#include "TargetParameter.h"

ModuleRouterValue::ModuleRouterValue(Controllable * _sourceValue, Module * _outModule) :
	BaseItem("Router Value"),
	outModule(_outModule)
{
	jassert(_sourceValue != nullptr);

	sourceTarget = addTargetParameter("Source Value", "Source Value");
	sourceTarget->setTarget(_sourceValue);

	doRoute = addBoolParameter("Route", "Activate the routing for this value", true);
}

ModuleRouterValue::~ModuleRouterValue()
{
}

void ModuleRouterValue::setSourceValue(Controllable * c)
{
	if (c == currentSourceValue) return;
	if (currentSourceValue != nullptr)
	{
		currentSourceValue->removeInspectableListener(this);
	}

	currentSourceValue = c;

	if (currentSourceValue != nullptr)
	{
		currentSourceValue->addInspectableListener(this);
	}
}

void ModuleRouterValue::setOutModule(Module * m)
{
	if (outModule == m) return;
	if (outModule != nullptr)
	{

	}

	outModule = m;

	if (outModule != nullptr)
	{
		routeParams = outModule->createRouteParamsForSourceValue(currentSourceValue);
	}
}

var ModuleRouterValue::getJSONData()
{
	var data = BaseItem::getJSONData();
	if (routeParams != nullptr) data.getDynamicObject()->setProperty("routeParams", routeParams->getJSONData());
	return var();
}

void ModuleRouterValue::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	if (routeParams != nullptr) routeParams->loadJSONData(data.getProperty("routeParams", var()));
}

void ModuleRouterValue::inspectableDestroyed(Inspectable * i)
{
	setSourceValue(nullptr);
}
