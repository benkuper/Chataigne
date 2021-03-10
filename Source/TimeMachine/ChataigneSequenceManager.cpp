/*
  ==============================================================================

    SequenceManager.cpp
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

juce_ImplementSingleton(ChataigneSequenceManager)

ChataigneSequenceManager::ChataigneSequenceManager() :
	SequenceManager()
{
	module.reset(new SequenceModule(this));

	itemDataType = "Sequence";
	helpID = "TimeMachine";
}

ChataigneSequenceManager::~ChataigneSequenceManager()
{
}

Sequence* ChataigneSequenceManager::createItem()
{
	return new ChataigneSequence();
}

Sequence* ChataigneSequenceManager::showMenuAndGetSequenceStatic()
{
	return getInstance()->showMenuAndGetSequence();
}

SequenceLayer* ChataigneSequenceManager::showmMenuAndGetLayerStatic()
{
	return getInstance()->showmMenuAndGetLayer();
}

TimeCue* ChataigneSequenceManager::showMenuAndGetCueStatic()
{
	return getInstance()->showMenuAndGetCue();
}

AudioLayer* ChataigneSequenceManager::showMenuAndGetAudioLayerStatic()
{
	return getInstance()->showMenuAndGetAudioLayer();
}

TimeTrigger* ChataigneSequenceManager::showMenuAndGetTriggerStatic()
{
	return getInstance()->showMenuAndGetTrigger();
}
