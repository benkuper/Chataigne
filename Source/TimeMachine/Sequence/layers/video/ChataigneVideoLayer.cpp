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

	uiHeight->setValue(110);
}

ChataigneVideoLayer::~ChataigneVideoLayer()
{
	// The mpv engine must be torn down on the message thread, with its deferred
	// GL cleanup running on the holder's OpenGL thread once mpv's asynchronous
	// stop command has completed (see MPVDeferredCleaner). Simply letting the
	// unique_ptr in VideoLayer die here would call mpv_terminate_destroy() with
	// no GL context current and could also run on the wrong thread.
	if (moviePlayer != nullptr)
	{
		std::unique_ptr<MPVPlayer> mpvPlayer(dynamic_cast<MPVPlayer*>(moviePlayer.release()));
		if (mpvPlayer != nullptr)
			MPVPlayer::destroyAfterShutdown(std::move(mpvPlayer));
	}
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