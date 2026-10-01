/*
  ==============================================================================

    ChataigneVideoLayer.cpp
    Created: 26 Sep 2026

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "MPVPlayer.h"

ChataigneVideoLayer::ChataigneVideoLayer(ChataigneSequence* sequence, var params) :
	VideoLayer(sequence, params),
	chataigneSequence(sequence)
{
	// VideoLayer::createVideoPlayer() is called from the base constructor, where
	// virtual dispatch can only reach the base implementation : the engine created
	// there is always the NullVideoPlayer. Swap in the real one now that the
	// object is fully constructed.
	if (moviePlayer != nullptr)
	{
		moviePlayer->removeListener(this);
		moviePlayer.reset(createVideoPlayer());
		moviePlayer->addListener(this);
	}
	if (overlapPlayer != nullptr)
	{
		overlapPlayer->removeListener(this);
		overlapPlayer.reset(createVideoPlayer());
		overlapPlayer->addListener(this);
	}

	uiHeight->setValue(110);
	spoutOutput = addBoolParameter("Spout Output", "Publish this layer as a Spout texture on Windows (Syphon on macOS)", false);
	spoutName = addStringParameter("Spout Name", "Shared texture sender name", "Chataigne - Video Layer");
	spoutWidth = addIntParameter("Spout Width", "Shared texture width", 1280, 16, 8192);
	spoutHeight = addIntParameter("Spout Height", "Shared texture height", 720, 16, 8192);
	sharedTextureOutput.reset(new CompositionRenderer::SharedTextureOutput(chataigneSequence, this));
	updateSharedTextureOutput();
}

ChataigneVideoLayer::~ChataigneVideoLayer()
{
	sharedTextureOutput.reset();
	// The mpv engine must be torn down on the message thread, with its deferred
	// GL cleanup running on the holder's OpenGL thread once mpv's asynchronous
	// stop command has completed (see MPVDeferredCleaner). Simply letting the
	// unique_ptr in VideoLayer die here would call mpv_terminate_destroy() with
	// no GL context current and could also run on the wrong thread.
	if (moviePlayer != nullptr)
	{
		moviePlayer->removeListener(this);
		std::unique_ptr<MPVPlayer> mpvPlayer(static_cast<MPVPlayer*>(moviePlayer.release()));
		if (mpvPlayer != nullptr)
			MPVPlayer::destroyAfterShutdown(std::move(mpvPlayer));
	}
	if (overlapPlayer != nullptr)
	{
		overlapPlayer->removeListener(this);
		std::unique_ptr<MPVPlayer> mpvPlayer(static_cast<MPVPlayer*>(overlapPlayer.release()));
		MPVPlayer::destroyAfterShutdown(std::move(mpvPlayer));
	}
}

void ChataigneVideoLayer::updateSharedTextureOutput()
{
	if (sharedTextureOutput != nullptr)
		sharedTextureOutput->configure(spoutOutput->boolValue(), spoutName->stringValue(), spoutWidth->intValue(), spoutHeight->intValue());
}

void ChataigneVideoLayer::onContainerParameterChangedInternal(Parameter* p)
{
	VideoLayer::onContainerParameterChangedInternal(p);
	if (p == spoutOutput || p == spoutName || p == spoutWidth || p == spoutHeight)
		updateSharedTextureOutput();
}

VideoPlayerEngine* ChataigneVideoLayer::createVideoPlayer()
{
	return new MPVPlayer();
}

SequenceLayerPanel* ChataigneVideoLayer::getPanel()
{
	return new ChataigneVideoLayerPanel(this);
}

SequenceLayerTimeline* ChataigneVideoLayer::getTimelineUI()
{
	return new VideoLayerTimeline(this);
}
