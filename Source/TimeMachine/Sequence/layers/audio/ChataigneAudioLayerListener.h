/*
  ==============================================================================

    ChataigneAudioLayerListener.h
    Created: 17 Jul 2019 12:22:45pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ChataigneAudioLayer;

class ChataigneAudioLayerListener
{
public:
    virtual ~ChataigneAudioLayerListener() {}
    virtual void targetAudioModuleChanged(ChataigneAudioLayer*) {}
};