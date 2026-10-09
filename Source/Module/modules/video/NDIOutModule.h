/*
  ==============================================================================

	NDIOutModule.h
	Created: 5 Oct 2026
	Author:  

  ==============================================================================
*/

#pragma once

#include "../../Module.h"
#include "TimeMachine/Sequence/layers/video/ui/CompositionRenderer.h"
#include "VideoNetworkRuntime.h"
#include <mutex>
#include <condition_variable>
#include <chrono>

/*
	Video module publishing the composition signal out of Chataigne as an NDI
	source.

	The picture is produced by a CompositionSurface on the shared OpenGL context,
	exactly like the Video monitor out module, so both outputs always show the
	same content (composition + test card overlay).

	The surface reads the composite back into a CPU Image::ARGB, whose bytes are
	B,G,R,A. That is already one of NDI's layouts, so frames are handed to the
	sender without any per-pixel conversion.

	NDIlib_send_send_video_v2() is synchronous and does the colour conversion,
	compression and network send before it returns, so it is never called from
	the message thread: a dedicated worker thread pulls the latest composition
	and sends it, while the message thread only keeps the surface and the sender
	configuration up to date. Everything the worker reads is a plain copy guarded
	by stateLock, because the parameters themselves are not safe to read from
	another thread.
*/
class NDIOutModule : public Module,
	private juce::Thread
{
public:
	NDIOutModule();
	~NDIOutModule() override;

	static Module* create() { return new NDIOutModule(); }

	String getTypeString() const override { return "NDI out"; }

	// Module
	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	void clearItem() override;

	// Opens/closes the output depending on the enabled state, the stream
	// settings and the current size. Message thread.
	void updateOutput();

	BoolParameter* testCard = nullptr;
	StringParameter* streamName = nullptr;
	IntParameter* width = nullptr;
	IntParameter* height = nullptr;
	FloatParameter* fps = nullptr;

private:
	// Parameters are still being built while the module constructor runs, and
	// adding them fires synchronous feedback. Guard against it.
	bool isReady() const noexcept;

	// Pushes the Test Card setting into the surface.
	void applySettings();

	// Worker thread.
	void run() override;
	void applyQueuedConfig();
	void sendLatestFrame();
	void destroySender();

	// The sender is only ever touched by the worker thread, which is what lets
	// NDIlib_send_destroy() run without racing a send.
	NDIlib_send_instance_t sender = nullptr;
	String currentSenderName;


	// State shared with the worker thread.
	std::mutex stateLock;
	std::condition_variable stateSignal;

	std::unique_ptr<CompositionRenderer::CompositionSurface> surface;

	// Desired configuration, written by the message thread under stateLock.
	bool configDirty = false;
	bool configEnabled = false;
	String configName;
	double configIntervalMs = 1000.0 / 30.0;
	int configFrameRate = 30000;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NDIOutModule)
};
