/*
  ==============================================================================

    TriggerLayer.cpp
    Created: 17 Nov 2016 7:59:54pm
    Author:  Ben Kuper

  ==============================================================================
*/

ChataigneTriggerLayer::ChataigneTriggerLayer(Sequence* _sequence, var params) :
	TriggerLayer(_sequence, getTypeString(), params)
{
	helpID = "ChataigneTriggerLayer";
	setManager(new ChataigneTriggerManager(this, _sequence));
}

ChataigneTriggerLayer::~ChataigneTriggerLayer()
{

}


ChataigneTriggerManager::ChataigneTriggerManager(ChataigneTriggerLayer* layer, Sequence* sequence) :
	TimeTriggerManager(layer, sequence)
{
}

ChataigneTriggerManager::~ChataigneTriggerManager()
{
}

TimeTrigger* ChataigneTriggerManager::createItem()
{
	return new ChataigneTimeTrigger();
}
