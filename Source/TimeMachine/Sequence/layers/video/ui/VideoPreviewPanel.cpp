/*
  ==============================================================================

    VideoPreviewPanel.cpp
    Created: 27 Sep 2026

    Shows the video of the current VideoLayer in its own dockable/floating
    organicui window. The movie player is owned by the layer, it is only
    parented here while this panel is visible.

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "VideoPreviewPanel.h"

VideoPreviewPanel::VideoPreviewPanel(const String& contentName) :
	ShapeShifterContentComponent(contentName)
{
	startTimerHz(4);
	updateCurrentVideoLayer();
}

VideoPreviewPanel::~VideoPreviewPanel()
{
	stopTimer();

	if (!currentLayer.wasObjectDeleted() && currentPlayer != nullptr && currentPlayer->getParentComponent() == this)
	{
		removeChildComponent(currentPlayer);
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
	// The player is owned by the layer : if it died, its player is gone as well.
	if (currentLayer.wasObjectDeleted()) currentPlayer = nullptr;

	VideoLayer* layer = getCurrentVideoLayer();
	VlcVideoPlayer* player = (layer != nullptr) ? layer->moviePlayer.get() : nullptr;

	if (layer == currentLayer.get() && player == currentPlayer)
	{
		if (player != nullptr && player->getParentComponent() == this)
		{
			player->setBounds(getLocalBounds());
		}
		return;
	}

	if (currentPlayer != nullptr && currentPlayer->getParentComponent() == this)
	{
		removeChildComponent(currentPlayer);
	}

	currentLayer = layer;
	currentPlayer = player;

	if (currentPlayer != nullptr)
	{
		addAndMakeVisible(currentPlayer);
		currentPlayer->setBounds(getLocalBounds());
	}

	repaint();
}

void VideoPreviewPanel::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colour::greyLevel(0.08f));
}

void VideoPreviewPanel::resized()
{
	if (currentPlayer != nullptr && currentPlayer->getParentComponent() == this)
	{
		currentPlayer->setBounds(getLocalBounds());
	}
}

void VideoPreviewPanel::timerCallback()
{
	// Detect the active video layer (selection based, falling back to the first
	// video layer) and re-parent the player if it changed.
	if (!isVisible()) return;

	updateCurrentVideoLayer();
}
