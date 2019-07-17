/*
  ==============================================================================

    SequenceManager.h
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Module/modules/sequence/SequenceModule.h"

class SequenceLayer;
class TimeCue;
class ChataigneSequence;

class ChataigneSequenceManager :
	public SequenceManager
{
public:
	juce_DeclareSingleton(ChataigneSequenceManager, false)

	ChataigneSequenceManager();
	~ChataigneSequenceManager();

	Sequence * createItem() override;

	SequenceModule module;

	static Sequence* showMenuAndGetSequenceStatic();
	static SequenceLayer* showmMenuAndGetLayerStatic();
	static TimeCue* showMenuAndGetCueStatic();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneSequenceManager)
};