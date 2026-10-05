/*
  ==============================================================================

	OMTOutModule.h
	Created: 5 Oct 2026
	Author:  

  ==============================================================================
*/

#pragma once

#include "../../Module.h"
#include "TimeMachine/Sequence/layers/video/ui/CompositionRenderer.h"
#include <libomt.h>
#include <mutex>
#include <condition_variable>
#include <chrono>

/*
	Video module sending the composition signal out of Chataigne as an
	OpenMediaTransport source.

	The picture is produced by a CompositionSurface on the shared OpenGL context,
	exactly like the Video monitor out module, so both outputs always show the
	same content (composition + test card overlay).

	The surface reads the composite back into a CPU Image::ARGB, whose bytes are
	B,G,R,A. That is already OMT's BGRA layout, so frames are handed to the
	sender without any per-pixel conversion.

	omt_send() is synchronous and blocks for roughly 30ms on a 720p frame, so it
	is never called from the message thread: a dedicated worker thread pulls the
	latest composition and sends it, while the message thread only keeps the
	surface and the sender configuration up to date. Everything the worker reads
	is a plain copy guarded by stateLock, because the parameters themselves are
	not safe to read from another thread.
*/
class OMTOutModule : public Module,
	private juce::Thread
{
public:
	OMTOutModule();
	~OMTOutModule() override;

	static Module* create() { return new OMTOutModule(); }

	String getTypeString() const override { return "OMT out"; }

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
	EnumParameter* quality = nullptr;

private:
	// Parameters are still being built while the module constructor runs, and
	// adding them fires synchronous feedback. Guard against it.
	bool isReady() const noexcept;

	// Pushes the Test Card setting into the surface.
	void applySettings();

	// Current encoding quality, or OMTQuality_High when the parameter has not
	// been given a value yet.
	OMTQuality getQuality() const;

	// Worker thread.
	void run() override;
	void applyQueuedConfig();
	void sendLatestFrame();

	// The sender is only ever touched by the worker thread, which is what lets
	// omt_send_destroy() run without racing a send.
	omt_send_t* sender = nullptr;
	String currentSenderName;
	int currentQuality = -1;

	// State shared with the worker thread.
	std::mutex stateLock;
	std::condition_variable stateSignal;

	std::unique_ptr<CompositionRenderer::CompositionSurface> surface;

	// Desired configuration, written by the message thread under stateLock.
	bool configDirty = false;
	bool configEnabled = false;
	String configName;
	int configQuality = (int) OMTQuality_High;
	int configIntervalMs = 33;
	int configFrameRate = 30;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OMTOutModule)
};