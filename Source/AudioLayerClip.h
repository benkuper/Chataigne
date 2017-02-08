/*
  ==============================================================================

    AudioLayerClip.h
    Created: 8 Feb 2017 12:20:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUDIOLAYERCLIP_H_INCLUDED
#define AUDIOLAYERCLIP_H_INCLUDED

#include "BaseItem.h"

class AudioLayerClip :
	public BaseItem
{
public:
	AudioLayerClip(const String &name = "Clip");
	virtual ~AudioLayerClip();

	AudioTransportSource source;

	StringParameter * filePath;

	void updateAudioSourceFile();

	void onContainerParameterChanged(Parameter *) override;
};



#endif  // AUDIOLAYERCLIP_H_INCLUDED
