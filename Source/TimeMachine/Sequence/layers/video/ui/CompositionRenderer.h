/*
  ==============================================================================

    CompositionRenderer.h
    Created: 28 Sep 2026

    Shared helpers that render the composition signal : the currently active
    frames of every VideoLayer, stacked top-most-first. Used by the editable
    "Composition Video" panel and by the "Video monitor out" module output
    window so they always show the exact same picture.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class VideoLayer;

namespace CompositionRenderer
{
	struct Cue
	{
		VideoLayer* layer = nullptr;
		juce::Image frame;
	};

	// Collects the enabled video layers with an active clip AND a decoded frame,
	// ordered from the top-most sequence/track to the bottom-most one.
	juce::Array<Cue> gatherActiveLayers();

	// Blends the premultiplied `src` layer over the premultiplied `dst` composite
	// using one of VideoLayerClip::BlendMode. Graphic contexts can't do arbitrary
	// blend modes in this JUCE, so the modes are implemented per-pixel here.
	void blendPixels(juce::Image& dst, const juce::Image& src, int blendMode);

	// Renders all cues into a freshly-cleared `buffer` : the collection is
	// top-most first, so it is composited in reverse. `layerScratch` is a scratch
	// buffer used for the non-Normal blend paths.
	void renderScene(juce::Image& buffer, juce::Image& layerScratch, const juce::Array<Cue>& cues, bool blackBackground);
}