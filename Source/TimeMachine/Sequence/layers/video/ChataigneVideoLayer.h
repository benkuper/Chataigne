/*
  ==============================================================================

    ChataigneVideoLayer.h
    Created: 26 Sep 2026

  ==============================================================================
*/

#pragma once

#include "Module/ModuleIncludes.h"

class ChataigneSequence;

class ChataigneVideoLayer :
	public VideoLayer
{
public:
	ChataigneVideoLayer(ChataigneSequence* sequence, var params);
	~ChataigneVideoLayer();

	ChataigneSequence* chataigneSequence;

	// Factory : provides the mpv-backed engine for the base VideoLayer.
	virtual VideoPlayerEngine* createVideoPlayer() override;

	virtual SequenceLayerPanel* getPanel() override;
	virtual SequenceLayerTimeline* getTimelineUI() override;

	static ChataigneVideoLayer* create(Sequence* sequence, var params) { return new ChataigneVideoLayer((ChataigneSequence*)sequence, params); }

	virtual String getTypeString() const override { return "Video"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneVideoLayer)
};