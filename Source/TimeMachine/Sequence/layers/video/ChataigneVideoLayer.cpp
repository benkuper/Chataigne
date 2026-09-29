/*
  ==============================================================================

    ChataigneVideoLayer.cpp
    Created: 26 Sep 2026

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"

ChataigneVideoLayer::ChataigneVideoLayer(ChataigneSequence* sequence, var params) :
	VideoLayer(sequence, params),
	chataigneSequence(sequence)
{
	uiHeight->setValue(110);
	spoutOutput = addBoolParameter("Spout Output", "Publish this layer as a Spout texture on Windows (Syphon on macOS)", false);
	spoutName = addStringParameter("Spout Name", "Shared texture sender name", "Chataigne - Video Layer");
	spoutWidth = addIntParameter("Spout Width", "Shared texture width", 1280, 16, 8192);
	spoutHeight = addIntParameter("Spout Height", "Shared texture height", 720, 16, 8192);
	sharedTextureOutput.reset(new CompositionRenderer::SharedTextureOutput([this]()
		{
			return CompositionRenderer::gatherActiveLayers(chataigneSequence, this);
		}));
	updateSharedTextureOutput();
}

ChataigneVideoLayer::~ChataigneVideoLayer()
{
	sharedTextureOutput.reset();
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

SequenceLayerPanel* ChataigneVideoLayer::getPanel()
{
	return new ChataigneVideoLayerPanel(this);
}

SequenceLayerTimeline* ChataigneVideoLayer::getTimelineUI()
{
	return new VideoLayerTimeline(this);
}
