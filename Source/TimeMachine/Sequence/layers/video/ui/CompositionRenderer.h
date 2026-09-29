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
		VideoLayerClip* clip = nullptr;
		juce::Image frame;
		float fadeFactor = 1.0f;
	};

	// Collects the enabled video layers with an active clip AND a decoded frame,
	// ordered from the top-most sequence/track to the bottom-most one.
	juce::Array<Cue> gatherActiveLayers(Sequence* sequenceFilter = nullptr, VideoLayer* layerFilter = nullptr);

	// Blends the premultiplied `src` layer over the premultiplied `dst` composite
	// using one of VideoLayerClip::BlendMode. Graphic contexts can't do arbitrary
	// blend modes in this JUCE, so the modes are implemented per-pixel here.
	void blendPixels(juce::Image& dst, const juce::Image& src, int blendMode);

	// Renders all cues into a freshly-cleared `buffer` : the collection is
	// top-most first, so it is composited in reverse. `layerScratch` is a scratch
	// buffer used for the non-Normal blend paths.
	void renderScene(juce::Image& buffer, juce::Image& layerScratch, const juce::Array<Cue>& cues, bool blackBackground);

	class SharedTextureOutput :
		private SharedTextureSender::SharedTextureListener,
		private juce::Timer
	{
	public:
		SharedTextureOutput(std::function<juce::Array<Cue>()> gatherFunction);
		~SharedTextureOutput() override;

		void configure(bool enabled, const juce::String& name, int width, int height);

	private:
		void timerCallback() override;
		void drawSharedTexture(juce::Graphics& g, juce::Rectangle<int> bounds) override;

		std::function<juce::Array<Cue>()> gather;
		SharedTextureSender* sender = nullptr;
		juce::Image renderedImage;
		juce::Image renderBuffer;
		juce::Image layerScratch;
		juce::CriticalSection imageLock;
		int outputWidth = 1280;
		int outputHeight = 720;
	};
}
