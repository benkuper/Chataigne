/*
  ==============================================================================

	MPVPlayer.h
	Ported from MapGyver (Golden-Geek), adapted to Chataigne :
	- AudioManager is replaced by the host AudioModule (resolved from the
	  ModuleManager). When no Sound Card module exists, audio is drained from
	  the named pipe and silently dropped (video still plays).
	- GlContextHolder is replaced by VideVideoGLContext (Chataigne source).

  ==============================================================================
*/

#pragma once

#include "Module/ModuleIncludes.h"

#include "mpv/client.h"
#include "mpv/render_gl.h"

#include "juce_timeline/timeline/Sequence/Layer/layers/video/VideoPlayerEngine.h"
#include "VideoAudioProcessor.h"

#include <vector>

class MPVPlayer;

class MPVTimers :
	public Timer
{
public:
	juce_DeclareSingleton(MPVTimers, true);
	MPVTimers();
	~MPVTimers();

	Array<MPVPlayer*> players;

	void registerMPV(MPVPlayer* vm);
	void unregisterMPV(MPVPlayer* vm);

	void timerCallback() override;
};

class MPVPlayer :
	public VideoPlayerEngine,
	public AudioModule::AudioModuleListener
{
public:
	MPVPlayer();
	~MPVPlayer();

	String filePath;

	mpv_handle* mpv = nullptr;
	mpv_render_context* mpv_gl = nullptr;

	int currentPos = 0;

	void clear();
	void setupMPV();

	// VideoPlayerEngine interface implementation
	bool load(const String& filePath) override;
	void unload() override;
	void play() override;
	void pause() override;
	void stop() override;
	bool isPlaying() const override;
	void setPosition(double pos) override;
	double getPosition() const override;
	double getDuration() const override;
	void setPlaySpeed(float speed) override;
	float getPlaySpeed() const override;
	void setVolume(float volume) override;
	float getVolume() const override;
	void setLoop(bool loop) override;
	bool getLoop() const override;
	int getVideoWidth() const override;
	int getVideoHeight() const override;
	int getNumChannels() const override;
	void setupGL() override;
	void renderGL(juce::OpenGLFrameBuffer& frameBuffer) override;
	bool isFileLoaded() const override { return fileInfo.fileLoaded; }
	juce::String getFilePath() const override { return filePath; }
	bool isGLInit() const override { return mpv_gl != nullptr; }
	AudioProcessor* getAudioProcessor() override;
	void pullEvents() override;

	// MPV-specific methods
	void loadFile();
	void stopGLUpdates();
	void clearGL();
	bool isShutdownComplete() const { return shutdownComplete.load(); }
	void abandonForProcessExit();

	static void destroyAfterShutdown(std::unique_ptr<MPVPlayer> player);
	static void drainDeferredPlayers();
	static bool hasDeferredPlayers();

	// MPV Stuff
	void onMPVUpdate();
	void onMPVWakeup();

	// Internal state
	bool shouldLoop = false;
	float currentVolume = 1.0f;
	float currentSpeed = 1.0f;

	//Audio
	class AudioPipeThread : public Thread
	{
	public:
		AudioPipeThread(MPVPlayer* owner, String pipePath);
		~AudioPipeThread() override;
		void run() override;
		void shutdown();

	private:
		bool connected = false;
		MPVPlayer* owner;
		String pipePath;
#if JUCE_WINDOWS
		HANDLE pipeHandle = INVALID_HANDLE_VALUE;
#else
		int fifoFd = -1;
#endif
		std::vector<float> readBuffer;
	};

	std::unique_ptr<AudioPipeThread> pipeThread;
	String uniquePipePath;

	// Audio Processor Graph
	AudioProcessorGraph::NodeID audioNodeID;
	VideoAudioProcessor* audioProcessor = nullptr;
	AudioModule* audioModuleResolved = nullptr;
	bool audioListenerRegistered = false;
	bool usingAudioPipe = false;

	void setupAudio();
	void audioSetupChanged() override;

	//Helpers
	int getMPVIntProperty(const char* name);
	double getMPVDoubleProperty(const char* name) const;
	String getMPVStringProperty(const char* name);
	void setPlaySpeedInternal(double speed);

	struct FileInfo
	{
		bool fileLoaded = false;
		int width = 0;
		int height = 0;
		double duration = 0;
		int numChannels = 0;
	};
	FileInfo fileInfo;
	int pendingFileInfoMask = 0;
	// Set by load() when it was asked for the file the player already holds (two
	// clips of a layer can share a source, e.g. after a clip was split) : the layer
	// is notified of the load from pullEvents() instead of by an mpv event.
	bool pendingSameFileNotify = false;
	bool eofReached = false;
	std::atomic<bool> playbackActive{ false };
	// Cached mpv state, fed by observed properties : lets isPlaying()/getPosition()
	// return without a blocking mpv_get_property round-trip on the message thread.
	std::atomic<bool> pausedState{ true };
	std::atomic<double> lastTimePos{ 0.0 };
	std::atomic<double> lastSeekTarget{ -1.0 };
	std::atomic<bool> shutdownRequested{ false };
	std::atomic<bool> shutdownCommandComplete{ false };
	std::atomic<bool> shutdownComplete{ false };
	std::atomic<bool> frameUpdatePending{ false };
};
