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

void ChataigneSequenceManager::showMenuAndGetSequenceStatic(ControllableContainer* startFromCC, std::function<void(Sequence*)> returnFunc)
{
	getInstance()->showMenuAndGetSequence(startFromCC, returnFunc);
}

void ChataigneSequenceManager::showMenuAndGetLayerStatic(ControllableContainer* startFromCC, std::function<void(SequenceLayer*)> returnFunc)
{
	getInstance()->showMenuAndGetLayer(startFromCC, returnFunc);
}

void ChataigneSequenceManager::showMenuAndGetCueStatic(ControllableContainer* startFromCC, std::function<void(TimeCue*)> returnFunc)
{
	getInstance()->showMenuAndGetCue(startFromCC, returnFunc);
}

void ChataigneSequenceManager::showMenuAndGetAudioLayerStatic(ControllableContainer* startFromCC, std::function<void(AudioLayer*)> returnFunc)
{
	getInstance()->showMenuAndGetAudioLayer(startFromCC, returnFunc);
}

void ChataigneSequenceManager::showMenuAndGetTriggerStatic(ControllableContainer* startFromCC, std::function<void(TimeTrigger*)> returnFunc)
{
	getInstance()->showMenuAndGetTrigger(startFromCC, returnFunc);
}
