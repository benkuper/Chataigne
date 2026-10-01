/*
  ==============================================================================

    VideoPreviewPanel.h
    Created: 27 Sep 2026

    Shows the video of the current VideoLayer in its own dockable/floating
    organicui window, composited live by a CompositionSurface on the shared
    OpenGL context (single-layer filter).

  ==============================================================================
*/

#pragma once

#include "CompositionRenderer.h"

class VideoLayer;

class VideoPreviewPanel :
	public ShapeShifterContentComponent,
	private juce::Timer
{
public:
	VideoPreviewPanel(const String& contentName);
	~VideoPreviewPanel();

	static ShapeShifterContent* create(const String& contentName) { return new VideoPreviewPanel(contentName); }

	juce::WeakReference<ControllableContainer> currentLayer;
	CompositionRenderer::CompositionSurface* surface = nullptr;

	VideoLayer* getCurrentVideoLayer();
	void updateCurrentVideoLayer();

	void paint(juce::Graphics& g) override;
	void resized() override;
	void visibilityChanged() override;
	void timerCallback() override;

private:
	void updateGLVisibility();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoPreviewPanel)
};