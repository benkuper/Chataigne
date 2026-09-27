/*
  ==============================================================================

    SequenceManager.h
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class SequenceLayer;
class TimeCue;
class ChataigneSequence;
class SequenceModule;
class TriggerLayer;

class ChataigneSequenceManager :
	public SequenceManager
{
public:
	juce_DeclareSingleton(ChataigneSequenceManager, false)

	ChataigneSequenceManager();
	~ChataigneSequenceManager();

	BoolParameter* snapKeysToFrames;

	Sequence * createItem() override;

	std::unique_ptr<SequenceModule> module;

	void createSequenceFromAudioFile(File f) override;

	static void showMenuAndGetSequenceStatic(ControllableContainer* startFromCC, std::function<void(Sequence*)> returnFunc);
	static void showMenuAndGetLayerStatic(ControllableContainer* startFromCC, std::function<void(SequenceLayer*)> returnFunc);
	static void showMenuAndGetCueStatic(ControllableContainer* startFromCC, std::function<void(TimeCue*)> returnFunc);
	static void showMenuAndGetAudioLayerStatic(ControllableContainer* startFromCC, std::function<void(AudioLayer*)> returnFunc);
	static void showMenuAndGetTriggerStatic(ControllableContainer* startFromCC, std::function<void(TimeTrigger*)> returnFunc);
	static void showMenuAndGetTriggerLayerStatic(ControllableContainer* startFromCC, std::function<void(TriggerLayer*)> returnFunc);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneSequenceManager)
};