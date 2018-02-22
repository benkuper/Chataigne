/*
  ==============================================================================

    CVGroup.cpp
    Created: 15 Feb 2018 3:49:35pm
    Author:  Ben

  ==============================================================================
*/

#include "CVGroup.h"

CVGroup::CVGroup(const String & name) :
	BaseItem(name),
	values("Variables",false,false,false),
	pm(this)
{
	addChildControllableContainer(&values);
	addChildControllableContainer(&pm);
}

CVGroup::~CVGroup()
{
}

var CVGroup::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("variables", values.getJSONData());
	data.getDynamicObject()->setProperty("presets", pm.getJSONData());
	return data;
}

void CVGroup::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	values.loadJSONData(data.getProperty("variables", var()));
	pm.loadJSONData(data.getProperty("presets", var()));
}
