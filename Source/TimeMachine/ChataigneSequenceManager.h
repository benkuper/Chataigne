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

	Sequence * createItem() override;

	std::unique_ptr<SequenceModule> module;

	static Sequence* showMenuAndGetSequenceStatic();
	static SequenceLayer* showmMenuAndGetLayerStatic();
	static TimeCue* showMenuAndGetCueStatic();
	static AudioLayer* showMenuAndGetAudioLayerStatic();
	static TimeTrigger * showMenuAndGetTriggerStatic();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneSequenceManager)
};