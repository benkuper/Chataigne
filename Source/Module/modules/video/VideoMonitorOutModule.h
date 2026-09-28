/*
  ==============================================================================

    VideoMonitorOutModule.h
    Created: 28 Sep 2026

    Hardware module that opens a borderless full-screen output window on a chosen
    monitor, duplicating the composition signal (the same picture the Composition
    Video panel shows). Black background by default.

    Options :
    - Monitor : which monitor the window appears on ("None" closes it)
    - Test Card : generative SMPTE-style test card overlaid on the signal
      (colour bars, grey ramp, resolution label, program logo, live time, red
      border frame, and a slow white diagonal sweep every 10 seconds)
    - Edge feather : optional fade of the window edges to black, handy for
      projector edge-blending

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class VideoMonitorOutModule;

class VideoMonitorOutWindow :
	public juce::Component,
	public juce::Timer
{
public:
	VideoMonitorOutWindow(VideoMonitorOutModule* _module);
	~VideoMonitorOutWindow();

	void resized() override;

	void timerCallback() override;

private:
	VideoMonitorOutModule* module;

	juce::Image backBuffers[2];
	int frontIndex = 0;
	juce::Image layerScratch;
	juce::ImageComponent renderedView;

	// Rasterizes the composition into a fresh back buffer, applies the test card
	// and the edge feather, then swaps the finished image into view.
	void renderComposite();

	// Draws the generative test card on top of the composition.
	void drawTestCard(juce::Image& img);

	// The slow white diagonal sweep, scheduled every 10 seconds.
	void drawSweep(juce::Image& img);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoMonitorOutWindow)
};

class VideoMonitorOutModule :
	public Module
{
public:
	VideoMonitorOutModule();
	~VideoMonitorOutModule();

	EnumParameter* monitor;
	BoolParameter* testCard;

	BoolParameter* featherEnabled;
	FloatParameter* featherAmount;
	FloatParameter* featherLeft;
	FloatParameter* featherRight;
	FloatParameter* featherTop;
	FloatParameter* featherBottom;

	std::unique_ptr<VideoMonitorOutWindow> window;

	// Applies the optional per-pixel edge fade to the rendered image.
	void applyEdgeFeather(juce::Image& img);

	// Immediately closes the output window, if any (Monitor reset to None).
	void closeVideoOutputWindow();

	String getTypeString() const override { return "Video monitor out"; }
	static VideoMonitorOutModule* create() { return new VideoMonitorOutModule(); }

	// Module
	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	void clearItem() override;

private:
	// (Re)builds the Monitor combo box from the current display setup.
	void updateMonitorOptions();

	// Opens/closes the output window depending on the enabled state and the
	// selected monitor.
	void updateWindow();
};