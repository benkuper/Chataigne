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
	public BaseItem,
	public Thread //async loading
{
public:
	AudioLayerClip(float time = 0);
	virtual ~AudioLayerClip();

	AudioFormatManager formatManager;
	AudioSampleBuffer buffer;
	
	FileParameter * filePath;

	FloatParameter * time; 
	FloatParameter * clipLength;

	FloatParameter * volume;
	BoolParameter * scratch;

	BoolParameter * isLocked;

	double clipDuration;
	double sampleRate;
	int clipSamplePos;

	bool isCurrent;
	bool isLoading;

	void setIsCurrent(bool value);
	
	bool isInRange(float time);

	void updateAudioSourceFile();
	void onContainerParameterChanged(Parameter *) override;
	
	void run() override;

	class ClipListener
	{
	public:
		virtual ~ClipListener() {}
		virtual void clipIsCurrentChanged(AudioLayerClip *) {}
	};

	ListenerList<ClipListener> clipListeners;
	void addClipListener(ClipListener* newListener) { clipListeners.add(newListener); }
	void removeClipListener(ClipListener* listener) { clipListeners.remove(listener); }

	class ClipEvent {
	public:
		enum Type { CLIP_IS_CURRENT_CHANGED, SOURCE_LOAD_START, SOURCE_LOAD_END };
		ClipEvent(Type type, AudioLayerClip * i) : type(type), clip(i) {}
		Type type;
		AudioLayerClip *clip;
	};

	QueuedNotifier<ClipEvent> audioClipAsyncNotifier;
	typedef QueuedNotifier<ClipEvent>::Listener AsyncListener;

	void addAsyncClipListener(AsyncListener* newListener) { audioClipAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedClipListener(AsyncListener* newListener) { audioClipAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncClipListener(AsyncListener* listener) { audioClipAsyncNotifier.removeListener(listener); }


private:
	WeakReference<AudioLayerClip>::Master masterReference;
	friend class WeakReference<AudioLayerClip>;

};



#endif  // AUDIOLAYERCLIP_H_INCLUDED
