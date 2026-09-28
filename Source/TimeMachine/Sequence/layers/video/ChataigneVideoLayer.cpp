/*
  ==============================================================================

    ChataigneVideoLayer.cpp
    Created: 26 Sep 2026

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"

ChataigneVideoLayer::ChataigneVideoLayer(ChataigneSequence* sequence, var params) :
	VideoLayer(sequence, params),
	chataigneSequence(sequence)
{
	uiHeight->setValue(160);
}

ChataigneVideoLayer::~ChataigneVideoLayer()
{
}

SequenceLayerPanel* ChataigneVideoLayer::getPanel()
{
	return new ChataigneVideoLayerPanel(this);
}

SequenceLayerTimeline* ChataigneVideoLayer::getTimelineUI()
{
	return new VideoLayerTimeline(this);
}