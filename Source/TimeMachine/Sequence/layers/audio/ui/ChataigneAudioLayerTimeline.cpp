/*
  ==============================================================================

    ChataigneAudioLayerTimeline.cpp
    Created: 5 Apr 2020 1:23:50pm
    Author:  bkupe

  ==============================================================================
*/

#include "ChataigneAudioLayerTimeline.h"

ChataigneAudioLayerTimeline::ChataigneAudioLayerTimeline(ChataigneAudioLayer* layer) :
    AudioLayerTimeline(layer),
    cal(layer),
    nextSampleNum(0),
    audioThumbnail(thumbnail.getAudioThumbnail())
    
{
    needle.timeBarColor = cal->arm->boolValue() ? RED_COLOR : needle.defaultTimeBarColor;
    if(ChataigneAudioLayerProcessor* calp = (ChataigneAudioLayerProcessor*)cal->currentProcessor) calp->addAudioRecorderListener(this);
}

ChataigneAudioLayerTimeline::~ChataigneAudioLayerTimeline()
{
    if (!inspectable.wasObjectDeleted())
    {
        if (ChataigneAudioLayerProcessor* calp = (ChataigneAudioLayerProcessor*)cal->currentProcessor) calp->removeAudioRecorderListener(this);
    }
}

void ChataigneAudioLayerTimeline::updateContent()
{
    AudioLayerTimeline::updateContent();
    if (thumbnail.isShowing())
    {
        float startX = getXForTime(cal->timeAtStartRecord);
        float endX = getXForTime(cal->sequence->currentTime->floatValue());
        thumbnail.setBounds(getLocalBounds().withLeft(startX).withRight(endX));
    }
}

void ChataigneAudioLayerTimeline::recordingStarted(int numChannels, int sampleRate)
{
    audioThumbnail.reset(numChannels, sampleRate);
    addAndMakeVisible(&thumbnail);
    nextSampleNum = 0;
    
    MessageManagerLock mmLock;
    updateContent();
}

void ChataigneAudioLayerTimeline::recordingStopped()
{
    removeChildComponent(&thumbnail);
}

void ChataigneAudioLayerTimeline::recordingUpdated(AudioBuffer<float>& buffer, int numSamples)
{
    audioThumbnail.addBlock(nextSampleNum, buffer, 0, buffer.getNumSamples());
    nextSampleNum += numSamples;
    MessageManagerLock mmLock;
    updateContent();
}

void ChataigneAudioLayerTimeline::controllableFeedbackUpdateInternal(Controllable* c)
{
    AudioLayerTimeline::controllableFeedbackUpdateInternal(c);

    if (c == cal->arm)
    {
        needle.timeBarColor = cal->arm->boolValue() ? RED_COLOR : needle.defaultTimeBarColor;
        repaint();
    }
}

// ----------------------------------------------------------------------

ChataigneAudioLayerTimeline::RecordingThumbnail::RecordingThumbnail()
{
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener(this);
}

ChataigneAudioLayerTimeline::RecordingThumbnail::~RecordingThumbnail()
{
    thumbnail.removeChangeListener(this);
}

void ChataigneAudioLayerTimeline::RecordingThumbnail::paint(Graphics& g)
{
    g.fillAll(RED_COLOR);
    g.setColour(RED_COLOR.brighter());

    if (thumbnail.getTotalLength() > 0.0)
    {
        thumbnail.drawChannels(g, getLocalBounds().reduced(2), 0.0, thumbnail.getTotalLength(), 1.0f);
    }
}

void ChataigneAudioLayerTimeline::RecordingThumbnail::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &thumbnail)
        repaint();
}
