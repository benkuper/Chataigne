/*
  ==============================================================================

	CompositionRenderer.h
	GL compositor for the video signal.

	The message thread snapshots the active timeline cues. The holder's GL thread
	uses that snapshot to render each video into a framebuffer and composite the
	textures for each registered CompositionSurface. It reads the result back into
	an image for the surface to paint.

	Surfaces are plain JUCE components (no native OpenGL child windows) : that
	keeps them glitch-free inside organicui panels.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../VideoGLContext.h"

class VideoLayer;
class VideoLayerClip;
class VideoPlayerEngine;
class Sequence;

namespace CompositionRenderer
{
	struct Cue
	{
		// Used as identities by the GL thread. Timeline objects stay on the message thread.
		const Sequence* sequence = nullptr;
		const VideoLayer* layer = nullptr;
		const VideoLayerClip* clip = nullptr;
		VideoPlayerEngine* engine = nullptr;
		juce::String filePath;
		float opacity = 1.0f;
		float scaleX = 1.0f;
		float scaleY = 1.0f;
		float xPercent = 0.0f;
		float yPercent = 0.0f;
		int blendMode = 0;
		uint64_t outputId = 0;
		double thumbnailTime = -1.0;
		double thumbnailSpacing = 0.35;
		std::function<void(const juce::Image&)> cacheThumbnail;
	};

	struct SceneSettings
	{
		bool blackBackground = false;
		bool testCard = false;
		// Real pixel resolution of the monitor the test card is shown on. The
		// card prints it instead of a hardcoded value.
		int testCardMonitorWidth = 0;
		int testCardMonitorHeight = 0;

		bool featherEnabled = false;
		float featherAmount = 1.0f; // 0..1 strength at the very edge
		float featherLeft = 0;      // fade widths, in % of width/height
		float featherRight = 0;
		float featherTop = 0;
		float featherBottom = 0;
	};

	// Message thread. Publish the timeline state and remove retiring players
	// before their deferred OpenGL cleanup.
	void updateActiveCues();
	void clearActiveCues();
	void removePlayer(VideoPlayerEngine* player);

	// Holder GL thread. Render the layers and all output surfaces from one snapshot.
	void renderFrame();

	// A display surface that hosts the composition : a plain JUCE component.
	// It has no OpenGL context of its own — it is composited on the holder GL
	// thread (renderSurfaces) and publishes a CPU image that paint() blits, so
	// it can live safely inside organicui panels without a native GL child
	// window glitching the UI.
	class CompositionSurface :
		public juce::Component,
		private juce::Timer
	{
	public:
		CompositionSurface(VideoGLContext* holder);
		~CompositionSurface() override;

		SceneSettings settings;

		// Optional filtering (preview panel).
		Sequence* sequenceFilter = nullptr;
		VideoLayer* layerFilter = nullptr;
		// 0 = full composition; other values are stable NDIOutput runtime IDs.
		uint64_t outputFilter = 0;

		// Message thread. Starts or stops inclusion in the composition.
		void showGL();
		void hideGL();

		// Holder GL thread only : composites this surface and reads the result
		// back into latestImage (painted on the message thread).
		void renderSurfaceGL(const juce::Array<Cue>& cues);
		juce::Image getLatestImage();

		void paint(juce::Graphics& g) override;
		void resized() override;

	private:
		void timerCallback() override;

		// Test card. The static part (checkerboard, grid, diagonals, circles,
		// greyscale ramp, colour ramp, logo, wordmark, resolution) is baked once
		// per size; only the diagonal white sweep and the clock are refreshed per
		// frame, each as its own small texture, so nothing has to be re-rasterised
		// at full resolution while the card is on screen.
		void releaseTestCardTextures();
		void renderTestCardGL(juce::OpenGLFrameBuffer*& src,
			juce::OpenGLFrameBuffer& ping, juce::OpenGLFrameBuffer& pong,
			int w, int h);

		VideoGLContext* vidHolder = nullptr;

		bool isShown = false;

		bool reportedFirstFrame = false;
		juce::OpenGLFrameBuffer pingFB;
		juce::OpenGLFrameBuffer pongFB;
		int fbWidth = 0;
		int fbHeight = 0;

		// Static part of the test card, re-baked when the surface size or the
		// reported monitor resolution changes.
		GLuint testCardStaticTexture = 0;
		int testCardStaticWidth = 0;
		int testCardStaticHeight = 0;
		juce::String testCardStaticLabel;

		// Diagonal white sweep : re-uploaded every frame (small).
		GLuint testCardSweepTexture = 0;

		// Clock : re-uploaded only when the printed string changes.
		GLuint testCardClockTexture = 0;
		int testCardClockWidth = 0;
		int testCardClockHeight = 0;
		juce::String testCardClockLabel;

		// Image last produced on the holder GL thread, painted on the UI thread.
		juce::Image latestImage;
		juce::CriticalSection imageLock;

		// Per-surface read-back throttle (holder thread) : caps CPU read-backs
		// while the holder composites continuously.
		uint32 lastReadbackTime = 0;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CompositionSurface)
	};

	// Publishes the same GL composition used by the preview and monitor panels
	// through the existing Spout/Syphon sender.
	class SharedTextureOutput : private SharedTextureSender::SharedTextureListener
	{
	public:
		SharedTextureOutput(Sequence* sequenceFilter, VideoLayer* layerFilter);
		~SharedTextureOutput() override;
		void configure(bool enabled, const juce::String& name, int width, int height);

	private:
		void drawSharedTexture(juce::Graphics& g, juce::Rectangle<int> bounds) override;
		std::unique_ptr<CompositionSurface> surface;
		SharedTextureSender* sender = nullptr;
	};
}
