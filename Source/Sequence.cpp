/*
  ==============================================================================

    Sequence.cpp
    Created: 28 Oct 2016 8:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#include "Sequence.h"
#include "SequenceLayerManager.h"

Sequence::Sequence() :
	BaseItem("Sequence")
{
	totalTime = addFloatParameter("Total Time", "Total time of this sequence, in seconds", 30, 1, 3600); //max 1h
	currentTime = addFloatParameter("Current Time", "Current position in time of this sequence", 0, 0,totalTime->floatValue());
	playSpeed = addFloatParameter("Play Speed", "Playing speed factor, 1 is normal speed, 2 is double speed and 0.5 is half speed",1,0,10);
	fps = addIntParameter("FPS", "Frame Per Second.\nDefines the number of times per seconds the sequence is evaluated, the higher the value is, the more previse the calculation will be.\n \
									This setting also sets how many messages per seconds are sent from layer with automations.", 50, 1, 100);

	playTrigger = addTrigger("Play", "Play the sequence");
	stopTrigger = addTrigger("Stop", "Stops the sequence and set the current time at 0.");
	pauseTrigger = addTrigger("Pause", "Pause the sequence and keep the current time as is.");
	togglePlayTrigger = addTrigger("TogglePlay", "Toggle between play/pause or play/stop depending on sequence settings");


	viewStartTime = addFloatParameter("View start time", "Start time of the view", 0, 0, totalTime->floatValue() - minViewTime);
	viewEndTime = addFloatParameter("View end time", "End time of the view", totalTime->floatValue(), minViewTime, totalTime->floatValue());

	isPlaying = addBoolParameter("Is Playing", "Is the sequence playing ?", false);
	isPlaying->isControllableFeedbackOnly = true;
	isPlaying->isEditable = false;

	layerManager = new SequenceLayerManager(this);
	addChildControllableContainer(layerManager);

	
}

Sequence::~Sequence()
{
	stopTimer();
	stopTrigger->trigger();
}

var Sequence::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("layerManager", layerManager->getJSONData());
	return data;
}

void Sequence::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	layerManager->loadJSONData(data.getProperty("layerManager", var()));
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
		isPlaying->setValue(true);
	} else if(t == stopTrigger)
	{
		isPlaying->setValue(false);
		currentTime->setValue(0);
	} else if (t == pauseTrigger)
	{
		isPlaying->setValue(false);
	}else if (t == togglePlayTrigger)
	{
		isPlaying->setValue(!isPlaying->boolValue());
	}
}

void Sequence::hiResTimerCallback()
{
	jassert(isPlaying->boolValue());

	uint32 millis = Time::getMillisecondCounter();
	float deltaTime = (millis - prevMillis)/1000.f;
	currentTime->setValue(currentTime->floatValue() + deltaTime);
	prevMillis = millis;

	if (currentTime->floatValue() == (float)currentTime->maximumValue)
	{
		stopTrigger->trigger();
	}
}
