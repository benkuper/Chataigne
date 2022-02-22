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

	static void showMenuAndGetSequenceStatic(std::function<void(Sequence*)> returnFunc);
	static void showMenuAndGetLayerStatic(std::function<void(SequenceLayer*)> returnFunc);
	static void showMenuAndGetCueStatic(std::function<void(TimeCue*)> returnFunc);
	static void showMenuAndGetAudioLayerStatic(std::function<void(AudioLayer*)> returnFunc);
	static void showMenuAndGetTriggerStatic(std::function<void(TimeTrigger*)> returnFunc);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneSequenceManager)
};