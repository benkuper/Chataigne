/*
  ==============================================================================

	MPVPlayer.cpp
	Ported from MapGyver (Golden-Geek), adapted to Chataigne :
	- AudioManager is replaced by the host AudioModule (resolved from the
	  ModuleManager). When no Sound Card module exists, audio is drained from
	  the named pipe and silently dropped (video still plays).
	- GlContextHolder is replaced by VideoGLContext (Chataigne source).

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "MPVPlayer.h"
#if !JUCE_WINDOWS
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "VideoGLContext.h"

#include <map>
#include <vector>
#include <memory>
#include <cstring>

using namespace juce;
using namespace juce::gl;

namespace {
	constexpr uint64_t kReqDuration = 1;
	constexpr uint64_t kReqWidth = 2;
	constexpr uint64_t kReqHeight = 3;
	constexpr uint64_t kReqChannels = 4;
	constexpr uint64_t kReqShutdownStop = 5;

	constexpr int kMaskDuration = 1 << 0;
	constexpr int kMaskWidth = 1 << 1;
	constexpr int kMaskHeight = 1 << 2;
	constexpr int kMaskChannels = 1 << 3;
	constexpr int kMaskAll = kMaskDuration | kMaskWidth | kMaskHeight | kMaskChannels;
}

static AudioModule* findAudioModule()
{
	if (ModuleManager::getInstanceWithoutCreating() == nullptr) return nullptr;
	for (auto& m : ModuleManager::getInstance()->items)
	{
		AudioModule* am = dynamic_cast<AudioModule*>(m);
		if (am != nullptr) return am;
	}
	return nullptr;
}

static bool isModuleAlive(Module* m)
{
	if (m == nullptr || ModuleManager::getInstanceWithoutCreating() == nullptr) return false;
	return ModuleManager::getInstance()->items.contains(m);
}

class MPVDeferredCleaner final : private Timer
{
public:
	static MPVDeferredCleaner& getInstance()
	{
		auto*& instance = instanceStorage();
		if (instance == nullptr)
			instance = new MPVDeferredCleaner();
		return *instance;
	}

	static void drainAndDelete()
	{
		auto*& instance = instanceStorage();
		if (instance == nullptr)
			return;

		instance->drain();
		delete instance;
		instance = nullptr;
	}

	static bool hasPendingPlayers()
	{
		auto* instance = instanceStorage();
		return instance != nullptr && !instance->players.empty();
	}

	void add(std::unique_ptr<MPVPlayer> player)
	{
		jassert(MessageManager::getInstance()->isThisTheMessageThread());
		players.push_back(std::move(player));
		startTimer(10);
	}

	void drain()
	{
		jassert(MessageManager::getInstance()->isThisTheMessageThread());
		stopTimer();

		// This path is used only while the application is exiting. Never enter MPV
		// or wait for its GL/decoder threads here: detach application-owned resources
		// and leave the native handles for the operating system's process teardown.
		for (auto& player : players)
		{
			player->abandonForProcessExit();
			(void)player.release();
		}
		players.clear();
	}

private:
	std::vector<std::unique_ptr<MPVPlayer>> players;

	static MPVDeferredCleaner*& instanceStorage()
	{
		static MPVDeferredCleaner* instance = nullptr;
		return instance;
	}

	void cleanupReadyPlayers()
	{
		for (int i = (int)players.size(); --i >= 0;)
		{
			auto* player = players[(size_t)i].get();
			if (!player->isShutdownComplete())
				continue;

			VideoGLContext* glHolder = VideoGLContext::getInstanceWithoutCreating();
			if (glHolder == nullptr)
				continue;

			// MPV has completed the asynchronous stop command, so freeing the render
			// context no longer waits for an active video chain. Waiting for this short
			// GL task keeps ownership on the message thread and avoids leaving a raw
			// pointer in MessageManager::callAsync during application shutdown.
			glHolder->callOnGLThread([player](juce::OpenGLContext&)
				{
					player->clearGL();
				}, true);

			players.erase(players.begin() + i);
		}
	}

	void timerCallback() override
	{
		// Do not rely solely on MPVTimers: that singleton is deliberately torn
		// down during application shutdown.
		for (auto& player : players)
			player->pullEvents();

		cleanupReadyPlayers();

		if (players.empty())
			stopTimer();
	}
};

// ==============================================================================
// HELPER: Windows OpenGL Loading
// ==============================================================================
void* get_gl_proc_address(void* ctx, const char* name) {

#if JUCE_WINDOWS
	static HMODULE glModule = GetModuleHandleA("opengl32.dll");
	using wglProc = void* (__stdcall*)(const char*);
	static wglProc wgl_get_proc_address = (wglProc)GetProcAddress(glModule, "wglGetProcAddress");

	void* p = nullptr;
	if (wgl_get_proc_address) p = wgl_get_proc_address(name);
	if (p == nullptr || p == (void*)0x1 || p == (void*)0x2 || p == (void*)0x3 || p == (void*)-1) {
		p = (void*)GetProcAddress(glModule, name);
	}
	return p;
#else
	// OpenGL symbols are provided by the active JUCE context on macOS/Linux.
	void* symbol = dlsym(RTLD_DEFAULT, name);
#if JUCE_LINUX
	if (symbol == nullptr)
	{
		using GLXGetProcAddress = void* (*)(const unsigned char*);
		auto glxGetProcAddress = reinterpret_cast<GLXGetProcAddress>(dlsym(RTLD_DEFAULT, "glXGetProcAddressARB"));
		if (glxGetProcAddress != nullptr)
			symbol = glxGetProcAddress(reinterpret_cast<const unsigned char*>(name));
	}
#endif
	return symbol;
#endif
}


void mpv_update(void* ctx) { ((MPVPlayer*)ctx)->onMPVUpdate(); }
void mpv_wakeup(void* ctx) { ((MPVPlayer*)ctx)->onMPVWakeup(); }
// ==============================================================================


MPVPlayer::MPVPlayer()
{
	// 1. Setup Unique Pipe Name
	Uuid uuid;
#if JUCE_WINDOWS
	uniquePipePath = "\\\\.\\pipe\\mpv_audio_" + uuid.toString();
#else
	uniquePipePath = File::getSpecialLocation(File::tempDirectory)
		.getChildFile("chataigne_mpv_audio_" + uuid.toString()).getFullPathName();
#endif

	// 2. Start Pipe Thread (Must happen before MPV connects)
	pipeThread.reset(new AudioPipeThread(this, uniquePipePath));

	// Start the drain unconditionally : even with no Sound Card module the pipe
	// must always be consumed, otherwise MPV's ao=pcm write blocks as soon as the
	// 64k pipe buffer fills (~170ms of audio) and playback freezes on one frame.
	// The run() loop already drops the data when audioProcessor is null.
	pipeThread->startThread();

	setupMPV();

	MPVTimers::getInstance()->registerMPV(this);
}

MPVPlayer::~MPVPlayer()
{
	clear();
}

void MPVPlayer::clear()
{
	if (MPVTimers::getInstanceWithoutCreating())
		MPVTimers::getInstance()->unregisterMPV(this);

	if (audioModuleResolved != nullptr)
	{
		if (audioListenerRegistered)
		{
			if (isModuleAlive(audioModuleResolved))
				audioModuleResolved->removeAudioModuleListener(this);
			audioListenerRegistered = false;
		}

		if (audioProcessor != nullptr && isModuleAlive(audioModuleResolved))
			audioModuleResolved->graph.removeNode(audioNodeID);
		audioModuleResolved = nullptr;
	}
	audioProcessor = nullptr;

	// Cleanup Pipe Thread
	if (pipeThread) {
		pipeThread->shutdown();
		pipeThread.reset();
	}

	if (mpv_gl)
	{
		// mpv_render_context_free() must run on the GL thread that created the
		// render context, otherwise the driver may corrupt the shared context.
		auto* glHolder = VideoGLContext::getInstanceWithoutCreating();
		if (glHolder != nullptr && juce::OpenGLContext::getCurrentContext() != &glHolder->context)
			glHolder->callOnGLThread([this](juce::OpenGLContext&) { clearGL(); }, true);
		else
			clearGL();
	}

	if (mpv)
	{
		mpv_terminate_destroy(mpv);
		mpv = nullptr;
	}
}

void MPVPlayer::clearGL()
{
	if (mpv_gl == nullptr)
		return;

	// The OpenGL render context must be destroyed while the same OpenGL
	// context used to create it is current. VideoMedia calls this from its
	// openGLContextClosing callback before unregistering from the GL thread.
	stopGLUpdates();
	mpv_render_context_free(mpv_gl);
	mpv_gl = nullptr;
}

void MPVPlayer::stopGLUpdates()
{
	if (mpv_gl != nullptr)
		mpv_render_context_set_update_callback(mpv_gl, nullptr, nullptr);
}

void MPVPlayer::destroyAfterShutdown(std::unique_ptr<MPVPlayer> player)
{
	if (player != nullptr)
		MPVDeferredCleaner::getInstance().add(std::move(player));
}

void MPVPlayer::drainDeferredPlayers()
{
	MPVDeferredCleaner::drainAndDelete();
}

bool MPVPlayer::hasDeferredPlayers()
{
	return MPVDeferredCleaner::hasPendingPlayers();
}

void MPVPlayer::abandonForProcessExit()
{
	// Do not call any MPV API here: this method exists specifically for an MPV
	// instance which stopped responding. The layer already disabled GL updates
	// before it transferred ownership to the deferred cleaner, and this object is
	// intentionally kept alive until the process exits.

	if (auto* timers = MPVTimers::getInstanceWithoutCreating())
		timers->unregisterMPV(this);

	if (pipeThread)
	{
		pipeThread->shutdown();
		pipeThread.reset();
	}

	if (audioModuleResolved != nullptr)
	{
		if (isModuleAlive(audioModuleResolved))
		{
			audioModuleResolved->removeAudioModuleListener(this);
			audioModuleResolved->graph.removeNode(audioNodeID);
		}
		audioModuleResolved = nullptr;
	}
	audioProcessor = nullptr;
}

void MPVPlayer::setupMPV()
{
	mpv = mpv_create();
	if (!mpv)
	{
		NLOGERROR("MPV Player", "Could not create mpv context");
		return;
	}

	auto setOption = [this](const char* name, const char* value)
		{
			const int optionResult = mpv_set_option_string(mpv, name, value);
			if (optionResult < 0)
				NLOGWARNING("MPV Player", "Could not set " << name << "=" << value << ": " << mpv_error_string(optionResult));
		};

	// This is a GUI application. Verbose terminal logging adds work for every
	// active decoder and has nowhere useful to go.
	setOption("terminal", "no");
	mpv_set_option_string(mpv, "vo", "libmpv");
	mpv_set_option_string(mpv, "force-window", "yes");
	mpv_set_option_string(mpv, "hr-seek", "yes");
	mpv_set_option_string(mpv, "hr-seek-framedrop", "yes");
	mpv_set_option_string(mpv, "keep-open", "yes");
	mpv_set_option_string(mpv, "keep-open-pause", "yes");

	// Leave the alpha channel untouched so layers clipped in the compositor keep
	// their transparency. The default blends the frame over a checkerboard
	// pattern ("tiles"), which paints squares over every transparent area.
	if (mpv_set_option_string(mpv, "background", "none") < 0)
		setOption("background", "0/0/0/0"); // mpv before 0.38 uses a color value.

	// "auto" is good, but sometimes picks copy-back methods.
	// "nvdec" (Nvidia) or "vaapi" (Intel/Linux) are strictly keep-in-VRAM.
	// "auto-safe" defaults to the best available hardware method.
	mpv_set_option_string(mpv, "hwdec", "auto-safe");

	// 2. GPU CONTEXT
	// Explicitly tell MPV we are in an OpenGL environment so it attempts
	// to map the HW surface directly to a GL Texture.
	mpv_set_option_string(mpv, "gpu-api", "opengl");

	// Do not force every decoder to reserve a large direct-rendering surface pool.
	// Multiple simultaneous 4K videos otherwise consume several GB of GPU memory.
	setOption("vd-lavc-dr", "no");
	setOption("hwdec-extra-frames", "1");
	setOption("vd-lavc-threads", "4");

	// Bound the per-player demux cache while retaining useful network history.
	setOption("demuxer-max-bytes", "32MiB");
	setOption("demuxer-max-back-bytes", "8MiB");
	setOption("cache-secs", "10");

	// --- AUDIO ROUTING CONFIGURATION ---
	AudioModule* am = findAudioModule();
	double sampleRate = am != nullptr ? (double)am->currentSampleRate : 48000.0;

	setOption("audio-samplerate", String(sampleRate).toRawUTF8());
	setOption("audio-format", "float");
	setOption("audio-channels", "stereo");

	// mpv's PCM writer is untimed. Without a Sound Card module there is no
	// consumer to pace it, so use the timed null output to keep video at speed.
	usingAudioPipe = am != nullptr && am->currentSampleRate > 0;
	setOption("ao", usingAudioPipe ? "pcm" : "null");
	setOption("ao-pcm-waveheader", "no"); // The pipe carries raw interleaved floats.
	setOption("ao-pcm-file", uniquePipePath.toRawUTF8());

	int result = mpv_initialize(mpv);

	if (result != MPV_ERROR_SUCCESS)
	{
		NLOGERROR("MPV Player", "could not initialize mpv context : " << result << " (" << mpv_error_string(result) << ")");
		return;
	}

	mpv_observe_property(mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
	mpv_observe_property(mpv, 0, "pause", MPV_FORMAT_FLAG);
	mpv_observe_property(mpv, 0, "eof-reached", MPV_FORMAT_FLAG);
	mpv_observe_property(mpv, 0, "idle-active", MPV_FORMAT_FLAG);
	mpv_set_wakeup_callback(mpv, mpv_wakeup, this);
}

void MPVPlayer::loadFile()
{
	if (mpv_gl == nullptr)
	{
		NLOGWARNING("MPV Player", "Not loading now, needs GL to be initialized");
		return;
	}

	// mpv starts a freshly loaded file in the paused/playing state the core
	// currently has. Chataigne drives playback explicitly from the timeline
	// (syncPlaybackState calls play() only while the sequence is playing), so mpv
	// must never auto-play on its own : force paused before the load, otherwise
	// videos would start running the instant a clip is dropped on the timeline.
	{
		int paused = 1;
		mpv_set_property_async(mpv, 0, "pause", MPV_FORMAT_FLAG, &paused);
	}

	const char* cmd[] = { "loadfile", filePath.toRawUTF8(), NULL };
	shutdownRequested = false;
	shutdownCommandComplete = false;
	shutdownComplete = false;
	NLOG("MPV Player", "loadfile '" << filePath << "'");
	mpv_command_async(mpv, 0, cmd);
}

void MPVPlayer::setupGL()
{
	mpv_opengl_init_params gl_init_params[1] = { get_gl_proc_address, nullptr };
	mpv_render_param params[]{
		{MPV_RENDER_PARAM_API_TYPE, const_cast<char*>(MPV_RENDER_API_TYPE_OPENGL)},
		{MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &gl_init_params},
		{MPV_RENDER_PARAM_INVALID, nullptr}
	};

	int result = mpv_render_context_create(&mpv_gl, mpv, params);

	if (result != MPV_ERROR_SUCCESS)
	{
		NLOGERROR("MPV Player", "failed to initialize mpv GL context : " << result << " (" << mpv_error_string(result) << ")");
		return;
	}

	mpv_render_context_set_update_callback(mpv_gl, mpv_update, this);

	// FLIP_Y and frame-timing overrides are only honoured in advanced-control mode.
	{
		const char* advancedControl = "yes";
		mpv_render_param advancedParam{ MPV_RENDER_PARAM_ADVANCED_CONTROL, const_cast<char*>(advancedControl) };
		mpv_render_context_set_parameter(mpv_gl, advancedParam);
	}

	loadFile();
}

void MPVPlayer::renderGL(juce::OpenGLFrameBuffer& frameBuffer)
{
	if (mpv_gl == nullptr)
	{
		setupGL();
		return;
	}

	if (!fileInfo.fileLoaded)
	{
		return;
	}

	// libmpv must be polled before every render : it returns true when a new frame is
	// available. Skipping this call is what makes mpv stop producing frames entirely.
	mpv_render_context_update(mpv_gl);

	int frameBufferToPasstoMPV = static_cast<int>(frameBuffer.getFrameBufferID());
	mpv_opengl_fbo mpfbo{
		frameBufferToPasstoMPV,
		frameBuffer.getWidth(),
		frameBuffer.getHeight(),
		0x8058 // Internal format RGBA8
	};

	int flip_y = 1;
	mpv_render_param params[] = {
		{MPV_RENDER_PARAM_OPENGL_FBO, &mpfbo},
		{MPV_RENDER_PARAM_FLIP_Y, &flip_y},
		{MPV_RENDER_PARAM_INVALID, nullptr}
	};

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // FIX ALIGNMENT
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_BLEND);

	int result = mpv_render_context_render(mpv_gl, params);

	if (result != MPV_ERROR_SUCCESS)
	{
		NLOGERROR("MPV Player", "mpv rendering error : " << result << " (" << mpv_error_string(result) << ")");
	}
	else
	{
		static bool reportedFirstRender = false;
		if (!reportedFirstRender)
		{
			reportedFirstRender = true;
			NLOG("MPV Player", "first mpv GL frame rendered into " << frameBuffer.getWidth() << "x" << frameBuffer.getHeight());
		}
	}

	// Tells mpv the frame was presented, so it can schedule the next one.
	mpv_render_context_report_swap(mpv_gl);
}

bool MPVPlayer::load(const String& path)
{
	if (path.isEmpty()) return false;
	if (mpv == nullptr) return false;

	filePath = path;

	NLOG("MPV Player", "load '" << path << "' (gl ready : " << (mpv_gl != nullptr ? "yes" : "no") << ")");

	// The GL context may not exist yet : the holder calls setupGL() as soon as
	// this player takes part in a cue, and setupGL() calls loadFile(). Accepting
	// the path here is what lets the layer complete its (asynchronous) load
	// bookkeeping - returning false would leave the clip without duration.
	if (mpv_gl != nullptr)
		loadFile();

	return true;
}

void MPVPlayer::unload()
{
	if (audioProcessor != nullptr) audioProcessor->onAudioFlush(0);
	if (mpv)
	{
		shutdownRequested = true;
		shutdownCommandComplete = false;
		shutdownComplete = false;
		const char* cmd[] = { "stop", NULL };
		if (mpv_command_async(mpv, kReqShutdownStop, cmd) < 0)
			shutdownComplete = true;
	}
	fileInfo.fileLoaded = false;
}

void MPVPlayer::play()
{
	if (mpv == nullptr) return;
	int paused = 0;
	pausedState = false;
	NLOG("MPV Player", "play");
	mpv_set_property_async(mpv, 0, "pause", MPV_FORMAT_FLAG, &paused);
}

void MPVPlayer::pause()
{
	if (mpv == nullptr) return;
	int paused = 1;
	pausedState = true;
	NLOG("MPV Player", "pause");
	mpv_set_property_async(mpv, 0, "pause", MPV_FORMAT_FLAG, &paused);
}

void MPVPlayer::stop()
{
	pause();
	setPosition(0.0);
}

bool MPVPlayer::isPlaying() const
{
	if (mpv == nullptr) return false;
	return !pausedState.load(std::memory_order_relaxed);
}

void MPVPlayer::setPosition(double pos)
{
	if (mpv == nullptr) return;

	// Avoid re-seeks to the same position : the base layer re-issues setPosition()
	// on every sync tick while scrubbing even when nothing moved, and each seek
	// forces mpv to decode from the target keyframe (expensive).
	const double lastTarget = lastSeekTarget.load(std::memory_order_relaxed);
	if (lastTarget >= 0.0 && std::fabs(pos - lastTarget) < 0.005
		&& std::fabs(pos - lastTimePos.load(std::memory_order_relaxed)) < 0.02
		&& pausedState.load(std::memory_order_relaxed))
		return;

	// Cache the target immediately so getPosition() never blocks behind the
	// (asynchronous) seek we are about to enqueue.
	lastSeekTarget.store(jmax(0.0, pos), std::memory_order_relaxed);
	if (pos >= 0.0) lastTimePos.store(pos, std::memory_order_relaxed);
	if (audioProcessor != nullptr) audioProcessor->onAudioFlush(0);
	mpv_node args[3];
	args[0].format = MPV_FORMAT_STRING; args[0].u.string = const_cast<char*> ("seek");
	args[1].format = MPV_FORMAT_DOUBLE; args[1].u.double_ = pos;
	args[2].format = MPV_FORMAT_STRING; args[2].u.string = const_cast<char*> ("absolute");

	mpv_node command;
	mpv_node_list list{ 3, args };
	command.format = MPV_FORMAT_NODE_ARRAY;
	command.u.list = &list;

	mpv_command_node_async(mpv, 0, &command);
}

void MPVPlayer::setPlaySpeedInternal(double speed)
{
	if (mpv == nullptr) return;
	mpv_set_property_async(mpv, 0, "speed", MPV_FORMAT_DOUBLE, const_cast<double*>(&speed));
}

double MPVPlayer::getPosition() const
{
	return lastTimePos.load(std::memory_order_relaxed);
}

double MPVPlayer::getDuration() const
{
	return fileInfo.duration;
}

void MPVPlayer::setPlaySpeed(float speed)
{
	currentSpeed = speed;
	setPlaySpeedInternal((double)speed);
}

float MPVPlayer::getPlaySpeed() const
{
	return currentSpeed;
}

void MPVPlayer::setVolume(float volume)
{
	currentVolume = volume;
	if (mpv == nullptr) return;
	double vol = volume * 100.0;
	mpv_set_property(mpv, "volume", MPV_FORMAT_DOUBLE, &vol);
}

float MPVPlayer::getVolume() const
{
	return currentVolume;
}

void MPVPlayer::setLoop(bool loop)
{
	shouldLoop = loop;
	if (mpv == nullptr) return;
	const char* value = loop ? "inf" : "no";
	mpv_set_property_string(mpv, "loop-file", value);
}

bool MPVPlayer::getLoop() const
{
	return shouldLoop;
}

int MPVPlayer::getVideoWidth() const
{
	return fileInfo.width;
}

int MPVPlayer::getVideoHeight() const
{
	return fileInfo.height;
}

int MPVPlayer::getNumChannels() const
{
	return fileInfo.numChannels;
}

AudioProcessor* MPVPlayer::getAudioProcessor()
{
	return audioProcessor;
}


void MPVPlayer::setupAudio()
{
	AudioModule* am = findAudioModule();
	if (fileInfo.numChannels <= 0 || am == nullptr || am->currentSampleRate <= 0)
	{
		return;
	}
	if (!usingAudioPipe && mpv != nullptr)
	{
		if (mpv_set_property_string(mpv, "ao", "pcm") < 0)
			return;
		usingAudioPipe = true;
	}

	const bool isFirstSetup = audioProcessor == nullptr;
	if (isFirstSetup)
	{
		audioNodeID = AudioProcessorGraph::NodeID(am->uidIncrement++);
		std::unique_ptr<VideoAudioProcessor> processor(new VideoAudioProcessor(this));
		audioProcessor = processor.get();
		am->graph.addNode(std::move(processor), audioNodeID);

		if (!audioListenerRegistered)
		{
			am->addAudioModuleListener(this);
			audioListenerRegistered = true;
		}
	}
	else
	{
		// Reconfigure the existing node after an audio-device change. Allocating a
		// new node here leaked the previous processor and duplicated connections.
		am->graph.disconnectNode(audioNodeID);
	}

	int sampleRate = (int)am->currentSampleRate;
	int bufferSize = am->currentBufferSize;
	if (sampleRate <= 0) sampleRate = 48000;
	if (bufferSize <= 0) bufferSize = 512;
	int numChannels = 2; // We enforced 2 channels in MPV options

	// Prepare the processor
	audioProcessor->setPlayConfigDetails(0, numChannels, sampleRate, bufferSize);
	audioProcessor->prepareToPlay(sampleRate, bufferSize);

	// Connect to Output Mixer
	int numOutputs = am->graph.getMainBusNumOutputChannels();
	if (numOutputs <= 0) numOutputs = numChannels;
	int outChannels = jmin(numChannels, numOutputs);
	for (int ch = 0; ch < outChannels; ++ch)
	{
		if (am->graph.canConnect({ { audioNodeID, ch }, { AUDIO_OUTPUTMIXER_GRAPH_ID, ch } }))
		{
			am->graph.addConnection({ { audioNodeID, ch }, { AUDIO_OUTPUTMIXER_GRAPH_ID, ch } });
		}
	}

	if (isFirstSetup && pipeThread != nullptr)
		pipeThread->startThread();
}

void MPVPlayer::audioSetupChanged()
{
	setupAudio();
}


void MPVPlayer::onMPVUpdate()
{
	// libmpv invokes this callback from an arbitrary internal thread. JUCE's
	// listener lists may only be traversed from their owning message thread.
	frameUpdatePending.store(true, std::memory_order_release);
}

void MPVPlayer::onMPVWakeup()
{
}

int MPVPlayer::getMPVIntProperty(const char* name)
{
	if (mpv == nullptr) return 0;
	int64_t result = 0;
	mpv_get_property(mpv, name, MPV_FORMAT_INT64, &result);
	return (int)result;
}

double MPVPlayer::getMPVDoubleProperty(const char* name) const
{
	if (mpv == nullptr) return 0;
	double result = 0;
	mpv_get_property(mpv, name, MPV_FORMAT_DOUBLE, &result);
	return result;
}

String MPVPlayer::getMPVStringProperty(const char* name)
{
	if (mpv == nullptr) return "";
	char* result = nullptr;
	mpv_get_property(mpv, name, MPV_FORMAT_STRING, &result);
	String strResult = String(result);
	mpv_free(result);
	return strResult;
}


void MPVPlayer::pullEvents()
{
	if (frameUpdatePending.exchange(false, std::memory_order_acq_rel))
	{
		notifyFrameUpdate();
	}

	if (mpv == nullptr) return;

	while (true)
	{
		mpv_event* e = mpv_wait_event(mpv, 0);
		if (e->event_id == MPV_EVENT_NONE) break;

		switch (e->event_id)
		{
		case MPV_EVENT_START_FILE:
			playbackActive = true;
			break;

		case MPV_EVENT_FILE_LOADED:
		{
			NLOG("MPV Player", "FILE_LOADED '" << filePath << "'");
			fileInfo.fileLoaded = false;
			pendingFileInfoMask = kMaskAll;
			mpv_get_property_async(mpv, kReqDuration, "duration", MPV_FORMAT_DOUBLE);
			mpv_get_property_async(mpv, kReqWidth, "width", MPV_FORMAT_INT64);
			mpv_get_property_async(mpv, kReqHeight, "height", MPV_FORMAT_INT64);
			mpv_get_property_async(mpv, kReqChannels, "audio-params/channel-count", MPV_FORMAT_INT64);
			eofReached = false;
		}
		break;

		case MPV_EVENT_GET_PROPERTY_REPLY:
		{
			auto* prop = static_cast<mpv_event_property*>(e->data);
			switch (e->reply_userdata)
			{
			case kReqDuration:
				if (e->error == MPV_ERROR_SUCCESS && prop && prop->format == MPV_FORMAT_DOUBLE && prop->data)
					fileInfo.duration = *static_cast<double*>(prop->data);
				pendingFileInfoMask &= ~kMaskDuration;
				break;
			case kReqWidth:
				if (e->error == MPV_ERROR_SUCCESS && prop && prop->format == MPV_FORMAT_INT64 && prop->data)
					fileInfo.width = (int)*static_cast<int64_t*>(prop->data);
				pendingFileInfoMask &= ~kMaskWidth;
				break;
			case kReqHeight:
				if (e->error == MPV_ERROR_SUCCESS && prop && prop->format == MPV_FORMAT_INT64 && prop->data)
					fileInfo.height = (int)*static_cast<int64_t*>(prop->data);
				pendingFileInfoMask &= ~kMaskHeight;
				break;
			case kReqChannels:
				if (e->error == MPV_ERROR_SUCCESS && prop && prop->format == MPV_FORMAT_INT64 && prop->data)
					fileInfo.numChannels = (int)*static_cast<int64_t*>(prop->data);
				pendingFileInfoMask &= ~kMaskChannels;
				break;
			default:
				break;
			}

			if (pendingFileInfoMask == 0 && !fileInfo.fileLoaded)
			{
				NLOG("MPV Player", "file info : duration=" << fileInfo.duration
							<< " " << fileInfo.width << "x" << fileInfo.height
							<< " channels=" << fileInfo.numChannels);
				setupAudio();
				fileInfo.fileLoaded = true;
				notifyFileLoaded();
			}
		}
		break;

		case MPV_EVENT_COMMAND_REPLY:
			if (e->reply_userdata == kReqShutdownStop && shutdownRequested.load())
			{
				shutdownCommandComplete = true;
				// MPV defines COMMAND_REPLY as completion of the asynchronous
				// command. Once stop has completed, the render context can be
				// released without waiting for a second notification which is
				// not emitted when there was no active file.
				shutdownComplete = true;
			}
			break;

		case MPV_EVENT_END_FILE:
		{
			auto* endFile = static_cast<mpv_event_end_file*>(e->data);
			NLOG("MPV Player", "END_FILE reason=" << (endFile != nullptr ? (int)endFile->reason : -1)
					  << " error=" << (endFile != nullptr ? mpv_error_string(endFile->error) : "?"));
			playbackActive = false;
			if (shutdownRequested.load() && shutdownCommandComplete.load())
				shutdownComplete = true;
			if (endFile != nullptr && (endFile->reason == MPV_END_FILE_REASON_EOF
				|| endFile->reason == MPV_END_FILE_REASON_ERROR))
				notifyFileEnd();
		}
		break;

		case MPV_EVENT_IDLE:
			playbackActive = false;
			if (shutdownRequested.load() && shutdownCommandComplete.load())
				shutdownComplete = true;
			break;

		case MPV_EVENT_SHUTDOWN:
			playbackActive = false;
			shutdownComplete = true;
			break;

		case MPV_EVENT_LOG_MESSAGE:
		{
			auto* logMessage = static_cast<mpv_event_log_message*>(e->data);
			if (logMessage != nullptr && logMessage->level != nullptr
				&& (String(logMessage->level) == "error" || String(logMessage->level) == "fatal"))
			{
				NLOGERROR("MPV Player", String(logMessage->prefix) << " : " << String(logMessage->text));
			}
		}
		break;

		case MPV_EVENT_PROPERTY_CHANGE:
		{
			mpv_event_property* prop = (mpv_event_property*)e->data;
			String pName = String(prop->name);
			if (pName == "idle-active" && prop->format == MPV_FORMAT_FLAG) {
				const bool isIdle = prop->data != nullptr && *(int*)prop->data != 0;
				playbackActive = !isIdle;
				if (isIdle && shutdownRequested.load() && shutdownCommandComplete.load())
					shutdownComplete = true;
			}
			else if (pName == "time-pos" && prop->format == MPV_FORMAT_DOUBLE) {
				double time = *(double*)prop->data;
				lastTimePos.store(time, std::memory_order_relaxed);
				notifyTimeChanged(time);
			}
			else if (pName == "pause" && prop->format == MPV_FORMAT_FLAG) {
				pausedState = prop->data != nullptr && *(int*)prop->data != 0;
			}
			else if (pName == "eof-reached" && prop->format == MPV_FORMAT_FLAG) {
				int reached = prop->data ? *(int*)prop->data : 0;
				if (reached != 0 && !eofReached)
				{
					eofReached = true;
					notifyFileEnd();
				}
				else if (reached == 0)
				{
					eofReached = false;
				}
			}
		}
		break;

		}
	}
}




// PIPE THREAD

MPVPlayer::AudioPipeThread::AudioPipeThread(MPVPlayer* owner, String path)
	: Thread("MPV Audio Pipe"), owner(owner), pipePath(path)
{
#if JUCE_WINDOWS
	// 1. Create the Named Pipe immediately so it exists when MPV tries to open it
	pipeHandle = CreateNamedPipeA(
		pipePath.toRawUTF8(),
		PIPE_ACCESS_INBOUND,        // Read only
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT,
		1,                          // Max instances (1)
		65536, 65536,               // Buffer sizes (64k)
		0,
		NULL
	);
	if (pipeHandle != INVALID_HANDLE_VALUE)
		ConnectNamedPipe(pipeHandle, NULL); // PIPE_NOWAIT starts listening immediately.
#else
	if (::mkfifo(pipePath.toRawUTF8(), 0600) != 0)
		NLOGERROR("MPV Player", "Could not create audio FIFO : " << pipePath);
#endif

	readBuffer.resize(4096); // 4k buffer for reading chunks
}

MPVPlayer::AudioPipeThread::~AudioPipeThread()
{
	shutdown();
}

void MPVPlayer::AudioPipeThread::shutdown()
{
	stopThread(1000);
#if JUCE_WINDOWS
	if (pipeHandle != INVALID_HANDLE_VALUE) {
		DisconnectNamedPipe(pipeHandle);
		CloseHandle(pipeHandle);
		pipeHandle = INVALID_HANDLE_VALUE;
	}
#else
	if (fifoFd >= 0)
	{
		::close(fifoFd);
		fifoFd = -1;
	}
	::unlink(pipePath.toRawUTF8());
#endif
}

void MPVPlayer::AudioPipeThread::run()
{
	auto waitForAudioSpace = [this](size_t bytes) -> bool
	{
		const int frames = (int) ((bytes + sizeof(float) * 2 - 1) / (sizeof(float) * 2));
		while (!threadShouldExit() && owner != nullptr && owner->audioProcessor != nullptr
			&& owner->audioProcessor->getFreeFrames() < frames)
			wait(5);
		return !threadShouldExit();
	};
#if JUCE_WINDOWS
	if (pipeHandle == INVALID_HANDLE_VALUE) return;
	size_t pendingBytes = 0;

	while (!threadShouldExit())
	{
		if (!connected)
		{
			connected = ConnectNamedPipe(pipeHandle, NULL) ? true : GetLastError() == ERROR_PIPE_CONNECTED;
			if (!connected) { wait(5); continue; }
		}

		DWORD bytesAvail = 0;
		if (PeekNamedPipe(pipeHandle, NULL, 0, NULL, &bytesAvail, NULL))
		{
			if (bytesAvail > 0)
			{
				DWORD bytesRead = 0;
				auto* bytes = reinterpret_cast<char*>(readBuffer.data());
				const size_t capacity = readBuffer.size() * sizeof(float);
				const DWORD bytesToRead = jmin((DWORD)(capacity - pendingBytes), bytesAvail);
				if (!waitForAudioSpace(pendingBytes + bytesToRead)) break;
				if (ReadFile(pipeHandle, bytes + pendingBytes, bytesToRead, &bytesRead, NULL) && bytesRead > 0)
				{
					const size_t totalBytes = pendingBytes + (size_t)bytesRead;
					const int numFrames = (int)(totalBytes / (sizeof(float) * 2));
					if (owner && owner->audioProcessor)
						owner->audioProcessor->onAudioPlay(readBuffer.data(), numFrames, 0);
					pendingBytes = totalBytes - (size_t)numFrames * sizeof(float) * 2;
					if (pendingBytes > 0)
						std::memmove(bytes, bytes + totalBytes - pendingBytes, pendingBytes);
				}
			}
			else wait(5);
		}
		else
		{
			// mpv closes and reopens the PCM output on file changes and seeks.
			DisconnectNamedPipe(pipeHandle);
			connected = false;
			pendingBytes = 0;
			ConnectNamedPipe(pipeHandle, NULL);
		}
	}
#else
	fifoFd = ::open(pipePath.toRawUTF8(), O_RDONLY | O_NONBLOCK);
	if (fifoFd < 0) return;
	size_t pendingBytes = 0;
	while (!threadShouldExit())
	{
		auto* bytes = reinterpret_cast<char*>(readBuffer.data());
		const size_t capacity = readBuffer.size() * sizeof(float);
		if (!waitForAudioSpace(capacity)) break;
		const ssize_t count = ::read(fifoFd, bytes + pendingBytes, capacity - pendingBytes);
		if (count <= 0)
		{
			wait(5);
			continue;
		}
		const size_t totalBytes = pendingBytes + (size_t)count;
		const int frames = (int)(totalBytes / (sizeof(float) * 2));
		if (frames > 0 && owner != nullptr && owner->audioProcessor != nullptr)
			owner->audioProcessor->onAudioPlay(readBuffer.data(), frames, 0);
		pendingBytes = totalBytes - (size_t)frames * sizeof(float) * 2;
		if (pendingBytes > 0)
			memmove(bytes, bytes + totalBytes - pendingBytes, pendingBytes);
	}
#endif
}



// MPV Timers

juce_ImplementSingleton(MPVTimers);

MPVTimers::MPVTimers()
{
	startTimerHz(60);
}

MPVTimers::~MPVTimers() {
	stopTimer();
	players.clear();
}

void MPVTimers::registerMPV(MPVPlayer* vm)
{
	players.addIfNotAlreadyThere(vm);
}

void MPVTimers::unregisterMPV(MPVPlayer* vm)
{
	players.removeAllInstancesOf(vm);
}

void MPVTimers::timerCallback()
{
	for (auto p : players)
	{
		p->pullEvents();
	}
}
