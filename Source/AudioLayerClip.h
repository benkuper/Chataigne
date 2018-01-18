/*
  ==============================================================================

    AudioLayerClip.h
    Created: 8 Feb 2017 12:20:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUDIOLAYERCLIP_H_INCLUDED
#define AUDIOLAYERCLIP_H_INCLUDED

#include "JuceHeader.h"

class AudioLayerClip :
	public BaseItem
{
public:
	AudioLayerClip(float time = 0);
	virtual ~AudioLayerClip();

	AudioFormatManager formatManager;
	AudioSampleBuffer buffer;
	
	StringParameter * filePath;

	FloatParameter * time; 
	FloatParameter * clipLength;

	FloatParameter * volume;
	BoolParameter * scratch;

	double clipDuration;
	double sampleRate;
	int clipSamplePos;

	bool isCurrent;

	void setIsCurrent(bool value);
	
	bool isInRange(float time);

	void updateAudioSourceFile();
	void onContainerParameterChanged(Parameter *) override;


	class ClipListener
	{
	public:
		virtual ~ClipListener() {}
		virtual void clipIsCurrentChanged(AudioLayerClip *) {}
		virtual void audioSourceChanged(AudioLayerClip *) {}
	};

	ListenerList<ClipListener> clipListeners;
	void addClipListener(ClipListener* newListener) { clipListeners.add(newListener); }
	void removeClipListener(ClipListener* listener) { clipListeners.remove(listener); }


private:
	WeakReference<AudioLayerClip>::Master masterReference;
	friend class WeakReference<AudioLayerClip>;

};



#endif  // AUDIOLAYERCLIP_H_INCLUDED
