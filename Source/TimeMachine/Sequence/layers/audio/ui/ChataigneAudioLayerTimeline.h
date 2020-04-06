/*
  ==============================================================================

    ChataigneAudioLayerTimeline.h
    Created: 5 Apr 2020 1:23:50pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ChataigneAudioLayer.h"



class ChataigneAudioLayerTimeline :
    public AudioLayerTimeline,
    public ChataigneAudioLayerProcessor::RecorderListener
{
public:
    ChataigneAudioLayerTimeline(ChataigneAudioLayer * cal);
    ~ChataigneAudioLayerTimeline();

    ChataigneAudioLayer* cal;
    int64 nextSampleNum;

    class RecordingThumbnail : public Component,
        private ChangeListener
    {
    public:
        RecordingThumbnail();
        ~RecordingThumbnail() override;
        AudioThumbnail& getAudioThumbnail() { return thumbnail; }
        void paint(Graphics& g) override;

    private:
        AudioFormatManager formatManager;
        AudioThumbnailCache thumbnailCache{ 10 };
        AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };
        void changeListenerCallback(ChangeBroadcaster* source) override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RecordingThumbnail)
    };

    RecordingThumbnail thumbnail;
    AudioThumbnail& audioThumbnail;

    void updateContent() override;

    void recordingStarted(int numChannels, int sampleRate) override;
    void recordingStopped() override;
    void recordingUpdated(AudioBuffer<float>& buffer, int numSamples) override;


    void controllableFeedbackUpdateInternal(Controllable* c) override;

};