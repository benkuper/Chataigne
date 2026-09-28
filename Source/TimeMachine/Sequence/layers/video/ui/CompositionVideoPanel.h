/*
  ==============================================================================

    CompositionVideoPanel.h
    Created: 27 Sep 2026

    Dockable/floating organicui window that composites the current frame of every
    active (enabled, with an active clip) VideoLayer at once.

    Z-order follows the stacking order of the Time Machine : sequences from top to
    bottom, and within a sequence the tracks from top to bottom. The top-most
    sequence and track win, i.e. they are drawn on top of the pile.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class VideoLayer;

class CompositionVideoPanel :
	public ShapeShifterContentComponent,
	private juce::Timer
{
public:
	CompositionVideoPanel(const String& contentName);
	~CompositionVideoPanel();

	static ShapeShifterContent* create(const String& contentName) { return new CompositionVideoPanel(contentName); }

	struct VideoCue
	{
		VideoLayer* layer = nullptr;
		juce::Image frame;
	};

	// Collects the enabled video layers with an active clip AND a decoded frame,
	// ordered from the top-most sequence/track to the bottom-most one.
	juce::Array<VideoCue> gatherActiveLayers();

	void paint(juce::Graphics& g) override;
	void timerCallback() override;
	void resized() override;

private:
	// The whole scene is rasterized each tick into a back buffer, which is then
	// swapped into the ImageComponent as a whole. Swapping a fully-rendered image
	// makes it impossible for a previous frame to linger on screen (stale-surface
	// "trails", i.e. the previous composite persisting under the new one).
	juce::Image backBuffers[2];
	int frontIndex = 0;
	juce::Image layerScratch;
	juce::ImageComponent renderedView;

	// Renders the current frames of all active layers into a fresh back buffer and
	// swaps it into view. Called from the timer.
	void renderComposite();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompositionVideoPanel)
};