#include "ChataigneSequenceManager.h"
/*
  ==============================================================================

    SequenceManager.cpp
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

juce_ImplementSingleton(ChataigneSequenceManager)

#include "Sequence/ChataigneSequence.h"

ChataigneSequenceManager::ChataigneSequenceManager() :
	SequenceManager(),
	module(this)
{
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

Sequence* ChataigneSequenceManager::showMenuAndGetSequence()
{
	return getInstance()->showMenuAndGetSequence();
}

SequenceLayer* ChataigneSequenceManager::showmMenuAndGetLayer()
{
	return getInstance()->showmMenuAndGetLayer();
}

TimeCue* ChataigneSequenceManager::showMenuAndGetCue()
{
	return getInstance()->showMenuAndGetCue();
}
