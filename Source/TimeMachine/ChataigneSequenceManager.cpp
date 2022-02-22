/*
  ==============================================================================

    SequenceManager.cpp
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

juce_ImplementSingleton(ChataigneSequenceManager)

ControllableContainer* getAppSettings();

ChataigneSequenceManager::ChataigneSequenceManager() :
	SequenceManager()
{
	module.reset(new SequenceModule(this));

	itemDataType = "Sequence";
	helpID = "TimeMachine";

	snapKeysToFrames = getAppSettings()->addBoolParameter("Snap Keys to Frames", "If checked, all mapping keys in sequences, will be automatically snapped to frames", false);
}

ChataigneSequenceManager::~ChataigneSequenceManager()
{
}

Sequence* ChataigneSequenceManager::createItem()
{
	return new ChataigneSequence();
}

void ChataigneSequenceManager::showMenuAndGetSequenceStatic(std::function<void(Sequence*)> returnFunc)
{
	getInstance()->showMenuAndGetSequence(returnFunc);
}

void ChataigneSequenceManager::showMenuAndGetLayerStatic(std::function<void(SequenceLayer*)> returnFunc)
{
	getInstance()->showMenuAndGetLayer(returnFunc);
}

void ChataigneSequenceManager::showMenuAndGetCueStatic(std::function<void(TimeCue*)> returnFunc)
{
	getInstance()->showMenuAndGetCue(returnFunc);
}

void ChataigneSequenceManager::showMenuAndGetAudioLayerStatic(std::function<void(AudioLayer*)> returnFunc)
{
	getInstance()->showMenuAndGetAudioLayer(returnFunc);
}

void ChataigneSequenceManager::showMenuAndGetTriggerStatic(std::function<void(TimeTrigger*)> returnFunc)
{
	getInstance()->showMenuAndGetTrigger(returnFunc);
}
