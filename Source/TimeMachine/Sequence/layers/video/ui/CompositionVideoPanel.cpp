/*
  ==============================================================================

    CompositionVideoPanel.cpp
    Created: 27 Sep 2026

    Composites the current frames of all active video layers into a single view.
    Each layer is rastered (with its Opacity/Transform parameters) into a scratch
    buffer, then blended over the composite using the clip's blend mode. The
    top-most layer ends up on top of the pile.

  ==============================================================================
*/

#include "TimeMachineIncludes.h"
#include "CompositionVideoPanel.h"
#include "CompositionRenderer.h"

CompositionVideoPanel::CompositionVideoPanel(const String& contentName) :
	ShapeShifterContentComponent(contentName)
{
	addAndMakeVisible(renderedView);
	startTimerHz(30);
}

CompositionVideoPanel::~CompositionVideoPanel()
{
	stopTimer();
}

juce::Array<CompositionVideoPanel::VideoCue> CompositionVideoPanel::gatherActiveLayers()
{
	auto cues = CompositionRenderer::gatherActiveLayers();
	juce::Array<VideoCue> result;
	for (auto& c : cues) result.add({ c.layer, c.frame });
	return result;
}

void CompositionVideoPanel::paint(juce::Graphics& g)
{
	// Plain backdrop : the actual composition is drawn into renderedView.
	g.fillAll(juce::Colour::greyLevel(0.08f));
}

void CompositionVideoPanel::resized()
{
	renderedView.setBounds(getLocalBounds());
}

void CompositionVideoPanel::renderComposite()
{
	const int w = getWidth();
	const int h = getHeight();
	if (w <= 0 || h <= 0) return;

	// (Re)size the render targets to the panel. Recreation on resize guarantees no
	// stale pixels survive a size change either.
	for (int i = 0; i < 2; ++i)
		if (!backBuffers[i].isValid() || backBuffers[i].getWidth() != w || backBuffers[i].getHeight() != h)
			backBuffers[i] = juce::Image(juce::Image::ARGB, w, h, true);

	if (!layerScratch.isValid() || layerScratch.getWidth() != w || layerScratch.getHeight() != h)
		layerScratch = juce::Image(juce::Image::ARGB, w, h, true);

	const int target = 1 - frontIndex;

	auto cues = gatherActiveLayers();
	juce::Array<CompositionRenderer::Cue> rendererCues;
	for (auto& c : cues) rendererCues.add({ c.layer, c.frame });
	CompositionRenderer::renderScene(backBuffers[target], layerScratch, rendererCues, false);

	// Swap the fully-rendered image into view. The previous frame is replaced
	// wholesale : no partial repaint can leave it lingering.
	renderedView.setImage(backBuffers[target]);
	renderedView.repaint();
	frontIndex = target;
}

void CompositionVideoPanel::timerCallback()
{
	if (!isVisible()) return;
	renderComposite();
}