/*
  ==============================================================================

    VideoMonitorOutModule.h
    Created: 28 Sep 2026

    Hardware module that opens a borderless full-screen output window on a chosen
    monitor, duplicating the composition signal (the same picture the Composition
    Video panel shows). Black background by default.

    The whole signal (composition, test card overlay, edge feather) is produced
    by a CompositionSurface on the shared OpenGL context.

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
#include "TimeMachine/Sequence/layers/video/ui/CompositionRenderer.h"

class VideoMonitorOutModule;

class VideoMonitorOutWindow :
	public juce::Component
{
public:
	VideoMonitorOutWindow(VideoMonitorOutModule* _module);
	~VideoMonitorOutWindow();

	void resized() override;

	// Pushes the module's Test Card / Edge Feather parameters into the surface.
	void updateSettings();

private:
	VideoMonitorOutModule* module;
	CompositionRenderer::CompositionSurface* surface = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoMonitorOutWindow)
};

class VideoMonitorOutModule :
	public Module
#if JUCE_WINDOWS
	, public KeyboardHooker::Listener
#endif
{
public:
	VideoMonitorOutModule();
	~VideoMonitorOutModule();

	EnumParameter* monitor = nullptr;
	BoolParameter* testCard = nullptr;

	BoolParameter* featherEnabled = nullptr;
	FloatParameter* featherAmount = nullptr;
	FloatParameter* featherLeft = nullptr;
	FloatParameter* featherRight = nullptr;
	FloatParameter* featherTop = nullptr;
	FloatParameter* featherBottom = nullptr;

	std::unique_ptr<VideoMonitorOutWindow> window;

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

#if JUCE_WINDOWS
	// Global keyboard hook : fires even when the app has no OS focus.
	void keyChanged(int keyCode, bool pressed) override;
#endif
};