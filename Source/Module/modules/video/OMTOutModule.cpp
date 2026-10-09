/*
  ==============================================================================

	OMTOutModule.cpp
	Created: 5 Oct 2026
	Author:

  ==============================================================================
*/

#include "OMTOutModule.h"

using namespace juce;

// OMTSenderInfo is a struct of fixed 1024 byte char arrays, so a plain bounded
// UTF-8 copy is all that is needed to fill one field.
static void setSenderField(char* dest, size_t size, const String& value)
{
	const StringRef text(value);
	const char* src = text;
	size_t i = 0;
	for (; i + 1 < size && src[i] != 0; ++i) dest[i] = src[i];
	dest[i] = 0;
}

// =============================================================================
// OMTOutModule
// =============================================================================

OMTOutModule::OMTOutModule() : Module(getTypeString()), Thread("OMT out sender")
{
	setupIOConfiguration(false, false);
	hideScripts();

	testCard = moduleParams.addBoolParameter("Test Card", "Show the generative test card on top of the output", false);
	streamName = moduleParams.addStringParameter("Stream Name", "OMT stream name", "Chataigne");
	width = moduleParams.addIntParameter("Width", "Output width in pixels", 1280, 16, 8192);
	height = moduleParams.addIntParameter("Height", "Output height in pixels", 720, 16, 8192);
	fps = moduleParams.addFloatParameter("FPS", "Output frame rate", 30.0f, 1.0f, 120.0f);

	quality = moduleParams.addEnumParameter("Quality", "Encoding quality. Default lets receivers choose, which is why a receiver asking for a low quality drags the whole stream down to a very low bitrate.");
	quality->addOption("Default", (int) OMTQuality_Default);
	quality->addOption("Low", (int) OMTQuality_Low);
	quality->addOption("Medium", (int) OMTQuality_Medium);
	quality->addOption("High", (int) OMTQuality_High);
	quality->setValueWithData((int) OMTQuality_High);

	updateOutput();

	// The sender is driven from its own thread because omt_send() blocks.
	startThread(Priority::low);
}

OMTOutModule::~OMTOutModule()
{
	// Note: omt_shutdown() is deliberately not called here. It tears down
	// process-wide logging/discovery threads and must only ever run once, after
	// every sender and receiver is gone.

	// stopThread() waits for run() to return, which destroys the sender.
	signalThreadShouldExit();
	stateSignal.notify_all();
	stopThread(-1);

	const std::lock_guard<std::mutex> l(stateLock);
	surface = nullptr;
}

bool OMTOutModule::isReady() const noexcept
{
	return testCard != nullptr && streamName != nullptr && width != nullptr
		&& height != nullptr && fps != nullptr && quality != nullptr;
}

// An empty key reads as 0, which would silently mean Default and let a receiver
// downgrade us, so an unset parameter falls back to High.
OMTQuality OMTOutModule::getQuality() const
{
	if (quality == nullptr || quality->getValueKey().isEmpty()) return OMTQuality_High;
	return (OMTQuality) (int) quality->getValueData();
}

void OMTOutModule::applySettings()
{
	const std::lock_guard<std::mutex> l(stateLock);
	if (surface == nullptr) return;

	CompositionRenderer::SceneSettings s;
	s.testCard = testCard->boolValue();
	s.blackBackground = true;
	// The card prints the real resolution it is sent at, which for this module
	// is simply the output size.
	s.testCardMonitorWidth = width->intValue();
	s.testCardMonitorHeight = height->intValue();
	surface->settings = s;
}

void OMTOutModule::updateOutput()
{
	if (!isReady()) return;

	const String name = streamName->stringValue().trim();
	const int w = jlimit(16, 8192, width->intValue());
	const int h = jlimit(16, 8192, height->intValue());
	const bool shouldRun = enabled->boolValue() && name.isNotEmpty();
	const int wantedQuality = (int) getQuality();
	const int wantedRate = jmax(1000, roundToInt(fps->floatValue() * 1000.0f));
	const double wantedInterval = 1000000.0 / wantedRate;

	{
		const std::lock_guard<std::mutex> l(stateLock);

		if (shouldRun)
		{
			if (surface == nullptr)
			{
				// Composited by the shared GL context, like the monitor output.
				// It is never added to a desktop, so it stays invisible.
				surface.reset(new CompositionRenderer::CompositionSurface(VideoGLContext::getInstance()));
				surface->setInterceptsMouseClicks(false, false);
				surface->setWantsKeyboardFocus(false);
			}

			surface->setSize(w, h);
			surface->showGL();
		}
		else if (surface != nullptr)
		{
			surface->hideGL();
			surface = nullptr;
		}

		configEnabled = shouldRun;
		configName = name;
		configQuality = wantedQuality;
		configIntervalMs = wantedInterval;
		configFrameRate = wantedRate;
		configDirty = true;
	}

	if (shouldRun) applySettings();

	stateSignal.notify_one();
}

void OMTOutModule::run()
{
	auto nextDue = std::chrono::steady_clock::now();

	while (! threadShouldExit())
	{
		bool dirty = false;
		double interval = 1000.0 / 30.0;
		{
			const std::lock_guard<std::mutex> l(stateLock);
			dirty = configDirty;
			configDirty = false;
			interval = configIntervalMs;
		}

		if (dirty) applyQueuedConfig();
		if (threadShouldExit()) break;

		sendLatestFrame();

		const auto step = std::chrono::duration_cast<std::chrono::steady_clock::duration>(
			std::chrono::duration<double, std::milli>(interval));
		nextDue += step;
		const auto now = std::chrono::steady_clock::now();
		// Drop accumulated lateness without adding a second SDK pacing delay.
		if (nextDue < now) nextDue = now;
		std::unique_lock<std::mutex> guard(stateLock);
		stateSignal.wait_until(guard, nextDue, [this] { return configDirty || threadShouldExit(); });

	}

	if (sender != nullptr)
	{
		VideoNetworkRuntime::OMT::get().send_destroy(sender);
		sender = nullptr;
		currentSenderName.clear();
		currentQuality = -1;
	}
}

void OMTOutModule::applyQueuedConfig()
{
	bool enabled = false;
	String name;
	int wantedQuality = (int) OMTQuality_High;
	{
		const std::lock_guard<std::mutex> l(stateLock);
		enabled = configEnabled;
		name = configName;
		wantedQuality = configQuality;
	}

	if (! enabled)
	{
		if (sender != nullptr)
		{
			VideoNetworkRuntime::OMT::get().send_destroy(sender);
			sender = nullptr;
			currentSenderName.clear();
			currentQuality = -1;
		}
		return;
	}

	// Both the source name and the encoding quality are fixed when the sender is
	// created (the API offers no rename and no later quality change), so any
	// change to either means a fresh sender on a fresh port.
	if (sender != nullptr && currentSenderName == name && currentQuality == wantedQuality) return;

	if (sender != nullptr)
	{
		VideoNetworkRuntime::OMT::get().send_destroy(sender);
		sender = nullptr;
		currentSenderName.clear();
		currentQuality = -1;
	}

	if (!VideoNetworkRuntime::OMT::get().load())
	{
		setWarningMessage("OMT runtime or VMX codec unavailable or incompatible. Install the OMT libraries for this platform.", "OMT");
		return;
	}
	setWarningMessage({}, "OMT");

	const StringRef nameRef(name);
	sender = VideoNetworkRuntime::OMT::get().send_create(nameRef, (OMTQuality) wantedQuality);
	currentSenderName = name;
	currentQuality = wantedQuality;
	if (sender == nullptr) setWarningMessage("Could not create OMT sender", "OMT");

	if (sender != nullptr)
	{
		// Identify the sender to receivers.
		OMTSenderInfo info = {};
		setSenderField(info.ProductName, sizeof(info.ProductName), "Chataigne");
		setSenderField(info.Manufacturer, sizeof(info.Manufacturer), "Chataigne");
		setSenderField(info.Version, sizeof(info.Version), ProjectInfo::versionString);
		VideoNetworkRuntime::OMT::get().send_setsenderinformation(sender, &info);
	}
}

void OMTOutModule::sendLatestFrame()
{
	Image image;
	int frameRate = 30000;
	{
		const std::lock_guard<std::mutex> l(stateLock);
		if (surface == nullptr) return;
		image = surface->getLatestImage();
		frameRate = configFrameRate;
	}

	if (sender == nullptr || ! image.isValid()) return;

	const int w = image.getWidth();
	const int h = image.getHeight();
	if (w <= 0 || h <= 0) return;

	const Image::BitmapData bd(image, Image::BitmapData::readOnly);
	const int stride = bd.lineStride;
	if (stride <= 0) return;

	// The GL read-back already produced B,G,R,A bytes (see
	// CompositionSurface::renderSurfaceGL), which is exactly OMT's BGRA
	// layout, so the buffer is passed through untouched.
	OMTMediaFrame frame = {};
	frame.Type = OMTFrameType_Video;
	frame.Codec = OMTCodec_BGRA;
	frame.Width = w;
	frame.Height = h;
	frame.Stride = stride;
	// No alpha flag: the module composites over black, so frames are opaque and
	// OMT forwards them as BGRX.
	frame.Flags = OMTVideoFlags_None;
	frame.FrameRateN = jmax(1000, frameRate);
	frame.FrameRateD = 1000;
	frame.AspectRatio = (float) w / (float) h;
	// Same default the SDK applies when the color space is left undefined.
	frame.ColorSpace = h < 720 ? OMTColorSpace_BT601 : OMTColorSpace_BT709;
	// -1 lets the sender timestamp the frame and throttle to FrameRateN/D,
	// which is what keeps a burst of sends in sync.
	frame.Timestamp = -1;
	frame.Data = bd.getLinePointer(0);
	frame.DataLength = stride * h;

	VideoNetworkRuntime::OMT::get().send(sender, &frame);
}

void OMTOutModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);

	if (p == enabled) updateOutput();
}

void OMTOutModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	// Parameters living inside moduleParams reach this callback as controllable
	// feedback, not as an onContainerParameterChanged.
	if (c == testCard)
	{
		applySettings();
	}
	else if (c == streamName || c == width || c == height || c == fps || c == quality)
	{
		updateOutput();
	}
}

void OMTOutModule::clearItem()
{
	// stopThread() waits for run() to return, which destroys the sender.
	signalThreadShouldExit();
	stateSignal.notify_all();
	stopThread(-1);

	{
		const std::lock_guard<std::mutex> l(stateLock);
		if (surface != nullptr) surface->hideGL();
		surface = nullptr;
	}

	Module::clearItem();
}
