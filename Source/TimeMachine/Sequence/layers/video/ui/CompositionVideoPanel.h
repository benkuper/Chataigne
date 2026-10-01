/*
  ==============================================================================

    CompositionVideoPanel.h
    Created: 27 Sep 2026

    Dockable/floating organicui window that composites the current frame of every
    active (enabled, with an active clip) VideoLayer at once, on the shared OpenGL
    context through a CompositionSurface.

    Z-order follows the stacking order of the Time Machine : sequences from top to
    bottom, and within a sequence the tracks from top to bottom. The top-most
    sequence and track win, i.e. they are drawn on top of the pile.

  ==============================================================================
*/

#pragma once

#include "CompositionRenderer.h"

class CompositionVideoPanel :
	public ShapeShifterContentComponent,
	private juce::Timer
{
public:
	CompositionVideoPanel(const String& contentName);
	~CompositionVideoPanel();

	static ShapeShifterContent* create(const String& contentName) { return new CompositionVideoPanel(contentName); }

	CompositionRenderer::CompositionSurface* surface = nullptr;

	void paint(juce::Graphics& g) override;
	void resized() override;
	void visibilityChanged() override;
	void timerCallback() override;

private:
	void updateGLVisibility();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompositionVideoPanel)
};