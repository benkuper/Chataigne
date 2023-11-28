#include "JuceHeader.h"
#include "TimeMachineIncludes.h"
#include "ChataigneSequenceManager.h"
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

void ChataigneSequenceManager::createSequenceFromAudioFile(File f)
{
	if (ModuleManager::getInstance()->getItemsWithType<AudioModule>().size() == 0) {
		AudioModule* m = new AudioModule();
		ModuleManager::getInstance()->addItem(m);
	}

	ChataigneSequence* seq = new ChataigneSequence();
	addItem(seq);
	seq->setNiceName(f.getFileNameWithoutExtension());

	ChataigneAudioLayer* l = new ChataigneAudioLayer(seq, var());
	seq->layerManager->addItem(l);
	l->uiHeight->setValue(80);

	AudioLayerClip* clip = new AudioLayerClip();
	l->clipManager.addItem(clip);
	clip->filePath->setValue(f.getFullPathName());
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
