/*
  ==============================================================================

	NDIOutModule.cpp
	Created: 5 Oct 2026
	Author:  

  ==============================================================================
*/

#include "NDIOutModule.h"

using namespace juce;

// =============================================================================
// NDIOutModule
// =============================================================================

NDIOutModule::NDIOutModule() : Module(getTypeString()), Thread("NDI out sender")
{
	setupIOConfiguration(false, false);
	hideScripts();

	testCard = moduleParams.addBoolParameter("Test Card", "Show the generative test card on top of the output", false);
	streamName = moduleParams.addStringParameter("Stream Name", "NDI source name", "Chataigne");
	width = moduleParams.addIntParameter("Width", "Output width in pixels", 1280, 1, 16384);
	height = moduleParams.addIntParameter("Height", "Output height in pixels", 720, 1, 16384);
	fps = moduleParams.addFloatParameter("FPS", "Output frame rate", 30.0f, 1.0f, 120.0f);

	updateOutput();

	// The sender is driven from its own thread because sending a frame blocks.
	startThread(Priority::low);
}

NDIOutModule::~NDIOutModule()
{
	// Note: NDIlib_destroy() is deliberately not called here directly. It tears
	// down process-wide library threads, so it belongs to the sender lifetime and
	// only runs once every sender has been destroyed.

	// stopThread() waits for run() to return, which destroys the sender.
	stateSignal.notify_all();
	stopThread(2000);

	const std::lock_guard<std::mutex> l(stateLock);
	surface = nullptr;
}

bool NDIOutModule::isReady() const noexcept
{
	return testCard != nullptr && streamName != nullptr && width != nullptr
		&& height != nullptr && fps != nullptr;
}

void NDIOutModule::applySettings()
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

void NDIOutModule::updateOutput()
{
	if (!isReady()) return;

	const String name = streamName->stringValue().trim();
	const int w = jlimit(16, 8192, width->intValue());
	const int h = jlimit(16, 8192, height->intValue());
	const bool shouldRun = enabled->boolValue() && name.isNotEmpty();
	const int wantedRate = jmax(1, roundToInt(fps->floatValue()));
	const int wantedInterval = jmax(1, 1000 / wantedRate);

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
		configIntervalMs = wantedInterval;
		configFrameRate = wantedRate;
		configDirty = true;
	}

	if (shouldRun) applySettings();

	stateSignal.notify_one();
}

void NDIOutModule::run()
{
	uint64 nextDue = Time::getMillisecondCounter();

	while (! threadShouldExit())
	{
		bool dirty = false;
		int interval = 33;
		{
			const std::lock_guard<std::mutex> l(stateLock);
			dirty = configDirty;
			configDirty = false;
			interval = configIntervalMs;
		}

		if (dirty) applyQueuedConfig();
		if (threadShouldExit()) break;

		sendLatestFrame();

		// Keep a steady cadence: a send that overruns its slot simply runs
		// straight into the next one instead of piling latency up.
		nextDue += (uint64) interval;
		const uint64 now = Time::getMillisecondCounter();
		const int waitFor = nextDue > now ? (int) jmin((uint64) 250, nextDue - now) : 0;

		{
			std::unique_lock<std::mutex> l(stateLock);
			// Only sleep when there is genuinely nothing new to apply, so a
			// configuration change arriving mid-send is never dropped.
			if (waitFor > 0 && ! configDirty)
				stateSignal.wait_for(l, std::chrono::milliseconds(waitFor));
		}
	}

	destroySender();
}

void NDIOutModule::destroySender()
{
	if (sender != nullptr)
	{
		NDIlib_send_destroy(sender);
		sender = nullptr;
		currentSenderName.clear();
	}

	// Tearing the library down while another NDI sender still exists is
	// documented as harmless, so this is per sender rather than global.
	if (sdkInitialised)
	{
		NDIlib_destroy();
		sdkInitialised = false;
	}
}

void NDIOutModule::applyQueuedConfig()
{
	bool enabled = false;
	String name;
	{
		const std::lock_guard<std::mutex> l(stateLock);
		enabled = configEnabled;
		name = configName;
	}

	if (! enabled)
	{
		destroySender();
		return;
	}

	// The source name is fixed when the sender is created (the API offers no
	// rename), so a new name means a fresh sender.
	if (sender != nullptr && currentSenderName == name) return;

	destroySender();

	// NDI needs SSE4.2. On a CPU without it send_create() simply fails, and
	// there is nothing useful the module can do about that.
	if (! NDIlib_is_supported_CPU()) return;

	// Not required by the SDK, but it starts the library's own threads, which is
	// what gets the best send throughput.
	sdkInitialised = NDIlib_initialize();

	// The name is UTF-8 and has to outlive the create call, so the converted
	// string is kept in a named local rather than a temporary.
	const CharPointer_UTF8 nameUtf8 = name.toUTF8();

	NDIlib_send_create_t settings;
	settings.p_ndi_name = nameUtf8.getAddress();
	// A null group list puts the source in the default NDI group.
	settings.p_groups = nullptr;
	// Video is the only stream being submitted, so it is the one that clocks
	// itself to the submitted frame rate.
	settings.clock_video = true;
	settings.clock_audio = false;

	sender = NDIlib_send_create(&settings);
	currentSenderName = name;

	if (sender == nullptr)
	{
		DBG("NDI: could not create the sender");

		if (sdkInitialised)
		{
			NDIlib_destroy();
			sdkInitialised = false;
		}
	}
}

void NDIOutModule::sendLatestFrame()
{
	if (sender == nullptr) return;

	// Despite the name, this returns how many receivers are attached, so 0 means
	// nobody is watching and the whole send path can be skipped: it reads 0
	// before a receiver attaches and 1 once one has. A receiver that connects
	// gets the very next frame, at most one interval later.
	if (NDIlib_send_get_no_connections(sender, 0) == 0) return;

	Image image;
	int frameRate = 30;
	{
		const std::lock_guard<std::mutex> l(stateLock);
		if (surface == nullptr) return;
		image = surface->getLatestImage();
		frameRate = configFrameRate;
	}

	if (! image.isValid()) return;

	const int w = image.getWidth();
	const int h = image.getHeight();
	if (w <= 0 || h <= 0) return;

	const Image::BitmapData bd(image, Image::BitmapData::readOnly);
	const int stride = bd.lineStride;
	if (stride <= 0) return;

	// The GL read-back already produced B,G,R,A bytes (see
	// CompositionSurface::renderSurfaceGL), and the module composites over
	// black, so every frame is opaque. BGRX therefore needs no conversion and
	// lets the SDK skip the alpha channel entirely.
	NDIlib_video_frame_v2_t frame(w, h,
		NDIlib_FourCC_video_type_BGRX,
		jmax(1, frameRate), 1,
		(float) w / (float) h,
		NDIlib_frame_format_type_progressive,
		NDIlib_send_timecode_synthesize,
		(uint8*) bd.getLinePointer(0),
		stride);

	// The synchronous call is deliberate: the async variant requires the frame
	// buffer to stay alive until the next send, and this one dies with the
	// function scope.
	NDIlib_send_send_video_v2(sender, &frame);
}

void NDIOutModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);

	if (p == enabled) updateOutput();
}

void NDIOutModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	// Parameters living inside moduleParams reach this callback as controllable
	// feedback, not as an onContainerParameterChanged.
	if (c == testCard)
	{
		applySettings();
	}
	else if (c == streamName || c == width || c == height || c == fps)
	{
		updateOutput();
	}
}

void NDIOutModule::clearItem()
{
	// stopThread() waits for run() to return, which destroys the sender.
	stateSignal.notify_all();
	stopThread(2000);

	{
		const std::lock_guard<std::mutex> l(stateLock);
		if (surface != nullptr) surface->hideGL();
		surface = nullptr;
	}

	Module::clearItem();
}