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

	spoutOutput = addBoolParameter("Spout Output", "Publish the full composition as a Spout texture on Windows (Syphon on macOS)", false);
	spoutName = addStringParameter("Spout Name", "Shared texture sender name", "Chataigne - Composition");
	spoutWidth = addIntParameter("Spout Width", "Shared texture width", 1280, 16, 8192);
	spoutHeight = addIntParameter("Spout Height", "Shared texture height", 720, 16, 8192);
	sharedTextureOutput.reset(new CompositionRenderer::SharedTextureOutput(nullptr, nullptr));
	updateSharedTextureOutput();
}

ChataigneSequenceManager::~ChataigneSequenceManager()
{
	sharedTextureOutput.reset();
}

void ChataigneSequenceManager::updateSharedTextureOutput()
{
	if (sharedTextureOutput != nullptr)
		sharedTextureOutput->configure(spoutOutput->boolValue(), spoutName->stringValue(), spoutWidth->intValue(), spoutHeight->intValue());
}

void ChataigneSequenceManager::onContainerParameterChanged(Parameter* p)
{
	SequenceManager::onContainerParameterChanged(p);
	if (p == spoutOutput || p == spoutName || p == spoutWidth || p == spoutHeight)
		updateSharedTextureOutput();
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

void ChataigneSequenceManager::showMenuAndGetTriggerLayer(ControllableContainer* startFromCC, std::function<void(TriggerLayer*)> returnFunc)
{
	Array<TriggerLayer*> triggerLayers;

	auto getMenuForSequence = [&triggerLayers](Sequence* sequence)
		{
			PopupMenu sequenceMenu;
			for (auto* layer : sequence->layerManager->items)
			{
				if (auto* triggerLayer = dynamic_cast<TriggerLayer*>(layer))
				{
					triggerLayers.add(triggerLayer);
					sequenceMenu.addItem(triggerLayers.size(), triggerLayer->niceName);
				}
			}
			return sequenceMenu;
		};

	PopupMenu menu;
	if (auto* sequence = dynamic_cast<Sequence*>(startFromCC))
	{
		menu = getMenuForSequence(sequence);
	}
	else
	{
		for (auto* sequence : items)
			menu.addSubMenu(sequence->niceName, getMenuForSequence(sequence));
	}

	menu.showMenuAsync(PopupMenu::Options(), [triggerLayers, returnFunc](int result)
		{
			if (isPositiveAndBelow(result - 1, triggerLayers.size()))
				returnFunc(triggerLayers[result - 1]);
		});
}

void ChataigneSequenceManager::createSequenceFromVideoFile(File f)
{
	ChataigneSequence* seq = new ChataigneSequence();
	addItem(seq);
	seq->setNiceName(f.getFileNameWithoutExtension());

	ChataigneVideoLayer* l = new ChataigneVideoLayer(seq, var());
	seq->layerManager->addItem(l);
	l->uiHeight->setValue(110);

	VideoLayerClip* clip = l->createVideoClip();
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

void ChataigneSequenceManager::showMenuAndGetTriggerLayerStatic(ControllableContainer* startFromCC, std::function<void(TriggerLayer*)> returnFunc)
{
	getInstance()->showMenuAndGetTriggerLayer(startFromCC, returnFunc);
}
