/*
  ==============================================================================

    ChataigneVideoLayer.h
    Created: 26 Sep 2026

  ==============================================================================
*/

#pragma once

#include "Module/ModuleIncludes.h"
#include <atomic>
#include <cstdint>

class ChataigneSequence;
namespace CompositionRenderer { class SharedTextureOutput; }

class ChataigneVideoLayer :
	public VideoLayer
{
public:
	ChataigneVideoLayer(ChataigneSequence* sequence, var params);
	~ChataigneVideoLayer();

	ChataigneSequence* chataigneSequence;
	FloatParameter* alpha = nullptr;
	TargetParameter* ndiOutput = nullptr;
	// GL and PCM threads only read the stable route identifier.
	std::atomic<uint64_t> ndiRouteId { 0 };
	void updateNDIOutput();
	void syncPlaybackState() override;

	BoolParameter* spoutOutput;
	StringParameter* spoutName;
	IntParameter* spoutWidth;
	IntParameter* spoutHeight;
	std::unique_ptr<CompositionRenderer::SharedTextureOutput> sharedTextureOutput;

	void updateSharedTextureOutput();
	void onContainerParameterChangedInternal(Parameter* p) override;
	void afterLoadJSONDataInternal() override;

	// Factory : provides the mpv-backed engine for the base VideoLayer.
	virtual VideoPlayerEngine* createVideoPlayer() override;
	VideoLayerClip* createVideoClip() override;

	virtual SequenceLayerPanel* getPanel() override;
	virtual SequenceLayerTimeline* getTimelineUI() override;

	static ChataigneVideoLayer* create(Sequence* sequence, var params) { return new ChataigneVideoLayer((ChataigneSequence*)sequence, params); }

	virtual String getTypeString() const override { return "Video"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneVideoLayer)
};
