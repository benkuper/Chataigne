/*
  ==============================================================================

    ChataigneVideoLayer.cpp
    Created: 26 Sep 2026

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "MPVPlayer.h"
#include "Module/modules/video/NDIOutModule.h"
#include "Module/ModuleIncludes.h"

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
	alpha = addFloatParameter("Alpha", "Visual opacity of this video layer", 1.0f, 0.0f, 1.0f);
	ndiOutput = addTargetParameter("NDI Output", "Select the NDI output object for video and audio", ModuleManager::getInstance());
	ndiOutput->targetType = TargetParameter::CONTAINER;
	ndiOutput->maxDefaultSearchLevel = 4;
	ndiOutput->defaultContainerTypeCheckFunc = [](ControllableContainer* cc)
		{ return dynamic_cast<NDIOutput*>(cc) != nullptr; };
	ndiOutput->typesFilter.add("NDI Output");
	updateNDIOutput();

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

void ChataigneVideoLayer::updateNDIOutput()
{
	NDIOutput* output = ndiOutput != nullptr ? ndiOutput->getTargetContainerAs<NDIOutput>() : nullptr;
	const uint64_t id = output != nullptr ? output->routeId : 0;
	if (ndiRouteId.exchange(id, std::memory_order_relaxed) == id) return;
	if (moviePlayer != nullptr)
	{
		static_cast<MPVPlayer*>(moviePlayer.get())->ndiRouteId.store(id, std::memory_order_relaxed);
		NDIOutModule::removeAudioSource(moviePlayer.get());
	}
	if (overlapPlayer != nullptr)
	{
		static_cast<MPVPlayer*>(overlapPlayer.get())->ndiRouteId.store(id, std::memory_order_relaxed);
		NDIOutModule::removeAudioSource(overlapPlayer.get());
	}
}

void ChataigneVideoLayer::syncPlaybackState()
{
	// Also synchronize targets resolved after loading the project.
	updateNDIOutput();
	VideoLayer::syncPlaybackState();
}

void ChataigneVideoLayer::onContainerParameterChangedInternal(Parameter* p)
{
	VideoLayer::onContainerParameterChangedInternal(p);
	if (p == ndiOutput) updateNDIOutput();
	if (p == spoutOutput || p == spoutName || p == spoutWidth || p == spoutHeight)
		updateSharedTextureOutput();
}

void ChataigneVideoLayer::afterLoadJSONDataInternal()
{
	VideoLayer::afterLoadJSONDataInternal();
	updateNDIOutput();
}

VideoLayerClip* ChataigneVideoLayer::createVideoClip()
{
	VideoLayerClip* clip = new VideoLayerClip();
	// The clip already has an Opacity parameter. Display it as Alpha without
	// changing its saved address, so existing projects retain their values.
	clip->opacity->setCustomShortName("opacity");
	clip->opacity->setNiceName("Alpha");
	return clip;
}

VideoPlayerEngine* ChataigneVideoLayer::createVideoPlayer()
{
	auto* player = new MPVPlayer();
	player->ndiRouteId.store(ndiRouteId.load(std::memory_order_relaxed), std::memory_order_relaxed);
	return player;
}

SequenceLayerPanel* ChataigneVideoLayer::getPanel()
{
	return new ChataigneVideoLayerPanel(this);
}

SequenceLayerTimeline* ChataigneVideoLayer::getTimelineUI()
{
	return new VideoLayerTimeline(this);
}
