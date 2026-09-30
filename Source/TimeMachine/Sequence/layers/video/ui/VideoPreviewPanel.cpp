/*
  ==============================================================================

    VideoPreviewPanel.cpp
    Created: 27 Sep 2026

    Shows the video of the current VideoLayer in its own dockable/floating
    organicui window. The engine is owned by the layer and rendered on the
    shared holder context ; this panel composites it through a
    CompositionSurface filtered to the current layer.

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "VideoPreviewPanel.h"

VideoPreviewPanel::VideoPreviewPanel(const String& contentName) :
	ShapeShifterContentComponent(contentName)
{
	surface = new CompositionRenderer::CompositionSurface(VideoGLContext::getInstance());
	addAndMakeVisible(surface);
	surface->setWantsKeyboardFocus(false);
	surface->setInterceptsMouseClicks(false, false);

	startTimerHz(4);
	updateCurrentVideoLayer();
}

VideoPreviewPanel::~VideoPreviewPanel()
{
	stopTimer();

	if (surface != nullptr)
	{
		surface->layerFilter = nullptr;
		surface->sequenceFilter = nullptr;
		surface->hideGL();
		delete surface;
		surface = nullptr;
	}
}

VideoLayer* VideoPreviewPanel::getCurrentVideoLayer()
{
	if (InspectableSelectionManager::mainSelectionManager != nullptr)
	{
		for (auto& wi : InspectableSelectionManager::mainSelectionManager->currentInspectables)
		{
			if (wi.wasObjectDeleted() || wi == nullptr) continue;

			if (ControllableContainer* cc = dynamic_cast<ControllableContainer*>(wi.get()))
			{
				ControllableContainer* c = cc;
				while (c != nullptr)
				{
					if (VideoLayer* vl = dynamic_cast<VideoLayer*>(c)) return vl;
					c = c->parentContainer.get();
				}
			}
		}
	}

	if (ChataigneSequenceManager::getInstanceWithoutCreating() != nullptr)
	{
		for (auto& seq : ChataigneSequenceManager::getInstance()->items)
		{
			for (auto& layer : seq->layerManager->items)
			{
				if (layer == nullptr) continue;
				if (VideoLayer* vl = dynamic_cast<VideoLayer*>(layer)) return vl;
			}
		}
	}

	return nullptr;
}

void VideoPreviewPanel::updateCurrentVideoLayer()
{
	if (currentLayer.wasObjectDeleted()) currentLayer = nullptr;

	VideoLayer* layer = getCurrentVideoLayer();

	if (layer == currentLayer.get())
	{
		if (surface != nullptr) surface->setBounds(getLocalBounds());
		return;
	}

	currentLayer = layer;

	if (surface != nullptr)
	{
		surface->layerFilter = layer;
		surface->sequenceFilter = layer != nullptr ? layer->sequence : nullptr;
	}

	repaint();
}

void VideoPreviewPanel::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colour::greyLevel(0.08f));
}

void VideoPreviewPanel::resized()
{
	if (surface != nullptr)
		surface->setBounds(getLocalBounds());
}

void VideoPreviewPanel::visibilityChanged()
{
	updateGLVisibility();
}

void VideoPreviewPanel::updateGLVisibility()
{
	if (surface == nullptr) return;

	if (isVisible())
		surface->showGL();
	else
		surface->hideGL();
}

void VideoPreviewPanel::timerCallback()
{
	// Safety net : visibilityChanged() is not called when the panel is already
	// visible at construction time, and the surface can only attach once the
	// holder's GL context exists.
	updateGLVisibility();

	// Detect the active video layer (selection based, falling back to the first
	// video layer) and retarget the surface filter if it changed.
	if (!isVisible()) return;

	updateCurrentVideoLayer();
}