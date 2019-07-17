/*
  ==============================================================================

    TriggerLayer.cpp
    Created: 17 Nov 2016 7:59:54pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "ActionLayer.h"

#include "../../ChataigneSequence.h"
#include "TimeAction.h"

ActionLayer::ActionLayer(Sequence* _sequence, var params) :
	TriggerLayer(_sequence, "Action", params)
{
	helpID = "ActionLayer";
	color->setColor(ACTION_COLOR.darker());
	setManager(new TimeActionManager(this, _sequence));
}

ActionLayer::~ActionLayer()
{

}


TimeActionManager::TimeActionManager(ActionLayer* layer, Sequence* sequence) :
	TimeTriggerManager(layer, sequence)
{
}

TimeActionManager::~TimeActionManager()
{
}

TimeTrigger* TimeActionManager::createItem()
{
	return new TimeAction();
}
