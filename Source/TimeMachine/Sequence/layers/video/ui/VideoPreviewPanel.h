/*
  ==============================================================================

    VideoPreviewPanel.h
    Created: 27 Sep 2026

  ==============================================================================
*/

#pragma once

class VideoLayer;
class VlcVideoPlayer;

class VideoPreviewPanel :
	public ShapeShifterContentComponent,
	private juce::Timer
{
public:
	VideoPreviewPanel(const String& contentName);
	~VideoPreviewPanel();

	static ShapeShifterContent* create(const String& contentName) { return new VideoPreviewPanel(contentName); }

	juce::WeakReference<ControllableContainer> currentLayer;
	VlcVideoPlayer* currentPlayer = nullptr;

	VideoLayer* getCurrentVideoLayer();
	void updateCurrentVideoLayer();

	void paint(juce::Graphics& g) override;
	void resized() override;
	void timerCallback() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoPreviewPanel)
};