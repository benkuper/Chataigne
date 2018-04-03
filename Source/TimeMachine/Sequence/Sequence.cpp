/*
  ==============================================================================

    Sequence.cpp
    Created: 28 Oct 2016 8:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "Sequence.h"
#include "Layer/SequenceLayerManager.h"
#include "Cue/TimeCueManager.h"
#include "Layer/layers/audio/AudioLayer.h"
#include "Module/modules/audio/AudioModule.h"

Sequence::Sequence() :
	BaseItem("Sequence",true),
	masterAudioModule(nullptr),
	hiResAudioTime(0)
{
	itemDataType = "Sequence";

	isPlaying = addBoolParameter("Is Playing", "Is the sequence playing ?", false);
	isPlaying->isControllableFeedbackOnly = true;
	isPlaying->isEditable = false; 
	isPlaying->isSavable = false;
	
	playTrigger = addTrigger("Play", "Play the sequence");
	stopTrigger = addTrigger("Stop", "Stops the sequence and set the current time at 0.");
	finishTrigger = addTrigger("Finish", "When the sequence reached naturally its end, and there is no loop");
	finishTrigger->hideInEditor = true;
	pauseTrigger = addTrigger("Pause", "Pause the sequence and keep the current time as is.");
	togglePlayTrigger = addTrigger("TogglePlay", "Toggle between play/pause or play/stop depending on sequence settings");
	
	float initTotalTime = 30; //default to 30 seconds, may be in general preferences later

	startAtLoad = addBoolParameter("Play at Load", "If selected, the sequence will start playing just after loading the file", false);

	currentTime = addFloatParameter("Current Time", "Current position in time of this sequence", 0, 0, initTotalTime);
	currentTime->defaultUI = FloatParameter::TIME;
	currentTime->isSavable = false;

	totalTime = addFloatParameter("Total Time", "Total time of this sequence, in seconds", initTotalTime, 1, 3600); //max 1h
	totalTime->defaultUI = FloatParameter::TIME;

	loopParam = addBoolParameter("Loop", "Whether the sequence plays again from the start when reached the end while playing", false);
	playSpeed = addFloatParameter("Play Speed", "Playing speed factor, 1 is normal speed, 2 is double speed and 0.5 is half speed",1,0,10);
	fps = addIntParameter("FPS", "Frame Per Second.\nDefines the number of times per seconds the sequence is evaluated, the higher the value is, the more previse the calculation will be.\n \
									This setting also sets how many messages per seconds are sent from layer with automations.", 50, 1, 100);
	
	
	prevCue = addTrigger("Prev Cue", "Jump to previous cue, if previous cue is less than 1 sec before, jump to the one before that.");
	nextCue = addTrigger("Next Cue", "Jump to the next cue");

	viewStartTime = addFloatParameter("View start time", "Start time of the view", 0, 0, initTotalTime - minViewTime);
	viewStartTime->hideInEditor = true;

	viewEndTime = addFloatParameter("View end time", "End time of the view", initTotalTime, minViewTime, initTotalTime);
	viewEndTime->hideInEditor = true;

	layerManager = new SequenceLayerManager(this);
	addChildControllableContainer(layerManager);

	cueManager = new TimeCueManager();
	addChildControllableContainer(cueManager);
	cueManager->hideInEditor = true;

	listUISize->setValue(5);

	helpID = "Sequence";
}

Sequence::~Sequence()
{
	stopTimer();
	stopTrigger->trigger();
	setMasterAudioModule(nullptr);
}

void Sequence::setCurrentTime(float time, bool forceOverPlaying)
{
	if (isPlaying->boolValue() && !forceOverPlaying) return;
	if (timeIsDrivenByAudio())
	{
		hiResAudioTime = time;
		if (!isPlaying->boolValue()) currentTime->setValue(time);
	}else currentTime->setValue(time);
}

bool Sequence::paste()
{
	SequenceLayer * p = layerManager->addItemFromClipboard(false);
	if (p == nullptr) return BaseItem::paste();
	return true;
}

void Sequence::setMasterAudioModule(AudioModule * module)
{
	if (masterAudioModule == module) return;

	if (masterAudioModule != nullptr)
	{
		masterAudioModule->am.removeAudioCallback(this);
	}

	masterAudioModule = module;

	if (masterAudioModule != nullptr)
	{
		masterAudioModule->am.addAudioCallback(this);
	}

	DBG("Set master audio module " << (masterAudioModule != nullptr ? masterAudioModule->niceName : "null"));
	sequenceListeners.call(&SequenceListener::sequenceMasterAudioModuleChanged, this);
}

bool Sequence::timeIsDrivenByAudio()
{
	return masterAudioModule != nullptr;
}

var Sequence::getJSONData()
{
	var data = BaseItem::getJSONData();
	var layerData = layerManager->getJSONData();
	if(!layerData.isVoid()) data.getDynamicObject()->setProperty("layerManager", layerData );
	var cueData = cueManager->getJSONData();
	if(!cueData.isVoid()) data.getDynamicObject()->setProperty("cueManager", cueData);
	return data;
}

void Sequence::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	layerManager->loadJSONData(data.getProperty("layerManager", var()));
	cueManager->loadJSONData(data.getProperty("cueManager", var()));

	if (startAtLoad->boolValue())
	{
		playTrigger->trigger();
	}
}

void Sequence::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled)
	{

	}
	else if (p == currentTime)
	{
		sequenceListeners.call(&SequenceListener::sequenceCurrentTimeChanged, this, prevTime, isPlaying->boolValue());
		prevTime = currentTime->floatValue();
		if (!isPlaying->boolValue() && timeIsDrivenByAudio()) hiResAudioTime = currentTime->floatValue();
	}
	else if (p == totalTime)
	{
		currentTime->setRange(0, totalTime->floatValue());
		viewStartTime->setRange(0, totalTime->floatValue() - minViewTime);
		viewEndTime->setRange(viewStartTime->floatValue()+minViewTime, totalTime->floatValue());
		sequenceListeners.call(&SequenceListener::sequenceTotalTimeChanged, this);
	} else if (p == playSpeed)
	{

	} else if (p == isPlaying)
	{
		if (isPlaying->boolValue())
		{
			prevMillis = Time::getMillisecondCounter();
			prevTime = currentTime->floatValue();
			startTimer(1000/fps->intValue());
		}
		else
		{
			stopTimer();
		}

		sequenceListeners.call(&SequenceListener::sequencePlayStateChanged, this);


	} else if (p == fps)
	{
		if (isPlaying->boolValue())
		{
			stopTimer();
			startTimer(1000/fps->intValue());
		}
	}
	else if (p == viewStartTime)
	{
		viewEndTime->setRange(viewStartTime->floatValue() + minViewTime, totalTime->floatValue()); //Should be a range value
	}
}

void Sequence::onContainerTriggerTriggered(Trigger * t)
{
	if (t == playTrigger)
	{
		if (currentTime->floatValue() >= totalTime->floatValue()) currentTime->setValue(0); //if reached the end when hit play, go to 0
		isPlaying->setValue(true);
	} else if(t == stopTrigger)
	{
		isPlaying->setValue(false);
		currentTime->setValue(0);
	} else if (t == pauseTrigger)
	{
		isPlaying->setValue(false);
	} else if (t == finishTrigger)
	{
		isPlaying->setValue(false);
	} else if (t == togglePlayTrigger)
	{
		if (isPlaying->boolValue()) pauseTrigger->trigger();
		else playTrigger->trigger();

	} else if (t == prevCue)
	{
		setCurrentTime(cueManager->getPrevCueForTime(currentTime->floatValue(), 1));
	} else if (t == nextCue)
	{
		setCurrentTime(cueManager->getNextCueForTime(currentTime->floatValue()));
	}
}

void Sequence::hiResTimerCallback()
{
	jassert(isPlaying->boolValue());

	if (timeIsDrivenByAudio())
	{
		currentTime->setValue(hiResAudioTime);
	}
	else
	{
		uint32 millis = Time::getMillisecondCounter();
		float deltaTime = (millis - prevMillis) / 1000.f;
		currentTime->setValue(currentTime->floatValue() + deltaTime * playSpeed->floatValue());
		prevMillis = millis;
	}

	if (currentTime->floatValue() >= (float)currentTime->maximumValue)
	{
		if (loopParam->boolValue())
		{
			sequenceListeners.call(&SequenceListener::sequenceLooped, this);
			setCurrentTime(0);
		}
		else finishTrigger->trigger();
	}
}


void Sequence::audioDeviceIOCallback(const float ** , int , float ** outputChannelData, int numOutputChannels, int numSamples)
{
	for(int i=0;i<numOutputChannels;i++) FloatVectorOperations::clear(outputChannelData[i], numSamples);

	AudioDeviceManager::AudioDeviceSetup s;
	masterAudioModule->am.getAudioDeviceSetup(s);
	if (isPlaying->boolValue()) hiResAudioTime += (numSamples / s.sampleRate)*playSpeed->floatValue();
}

void Sequence::audioDeviceAboutToStart(AudioIODevice *)
{
}

void Sequence::audioDeviceStopped()
{
}
