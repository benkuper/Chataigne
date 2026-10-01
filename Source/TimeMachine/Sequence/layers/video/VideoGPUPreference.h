/*
  ==============================================================================

	VideoGPUPreference.h
	Vendor neutral "use the best GPU available" request for the video pipeline.
	See VideoGPUPreference.cpp for the per platform details.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
class VideoGPUPreference
{
public:
	// Must be called before any GL context is created : on Windows the driver
	// only reads the preference when the process is started, so the value we
	// write here is applied on the next launch of the application.
	static void applyBestAvailable();

	// True when the running process is allowed to use the high performance GPU.
	static bool isApplied();

	// Info about what happened, for logging
	static juce::String getDescription();

private:
	static void applyOnWindows();
};
