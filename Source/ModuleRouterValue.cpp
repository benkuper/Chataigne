/*
  ==============================================================================

	ModuleRouterValue.cpp
	Created: 30 Apr 2017 2:59:04pm
	Author:  Ben

  ==============================================================================
*/

#include "ModuleRouterValue.h"

#include "Module.h"

ModuleRouterValue::ModuleRouterValue(Controllable * _sourceValue, int _index) :
	BaseItem(_sourceValue->niceName,false),
	valueIndex(_index),
	sourceValue(_sourceValue),
	outModule(nullptr)
{
	jassert(sourceValue != nullptr);

	userCanRemove = false;
	doRoute = addBoolParameter("Route", "Activate the routing for this value", false);

}

ModuleRouterValue::~ModuleRouterValue()
{
	if (sourceValue == nullptr || sourceValue.wasObjectDeleted()) return;

	if (sourceValue->type == Controllable::TRIGGER) ((Trigger *)sourceValue.get())->removeTriggerListener(this);
	else ((Parameter *)sourceValue.get())->removeParameterListener(this);
}

void ModuleRouterValue::setSourceAndOutModule(Module * s, Module * m)
{
	if (outModule == m) return;

	var prevData = var();

	if (outModule != nullptr)
	{
		if (routeParams != nullptr)
		{
			removeChildControllableContainer(routeParams);
			prevData = routeParams->getJSONData();
		}
		routeParams = nullptr;
	}

	outModule = m;

	if (outModule != nullptr)
	{
		routeParams = outModule->createRouteParamsForSourceValue(s, sourceValue,valueIndex);
		routeParams->loadJSONData(prevData);
		addChildControllableContainer(routeParams);
	}

	valueListeners.call(&ValueListener::routeParamsChanged, this);
}

var ModuleRouterValue::getJSONData()
{
	var data = BaseItem::getJSONData();
	if (routeParams != nullptr) data.getDynamicObject()->setProperty("routeParams", routeParams->getJSONData());
	return data;
}

void ModuleRouterValue::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	if (routeParams != nullptr) routeParams->loadJSONData(data.getProperty("routeParams", var()));
}

void ModuleRouterValue::onContainerParameterChangedInternal(Parameter * p)
{
	if (sourceValue == nullptr || sourceValue.wasObjectDeleted()) return;

	if (p == doRoute)
	{
		if (doRoute->boolValue())
		{
			if (sourceValue->type == Controllable::TRIGGER) ((Trigger *)sourceValue.get())->addTriggerListener(this);
			else ((Parameter *)sourceValue.get())->addParameterListener(this);
		} else
		{
			if (sourceValue->type == Controllable::TRIGGER) ((Trigger *)sourceValue.get())->removeTriggerListener(this);
			else ((Parameter *)sourceValue.get())->removeParameterListener(this);
		}
	}
}

void ModuleRouterValue::onExternalParameterChanged(Parameter * p)
{
	if (outModule == nullptr) return;
	if(p == sourceValue) outModule->handleRoutedModuleValue(sourceValue, routeParams);
}

void ModuleRouterValue::onExternalTriggerTriggered(Trigger * t)
{
	if (outModule == nullptr) return;
	if(t == sourceValue) outModule->handleRoutedModuleValue(sourceValue, routeParams);
}