/*
  ==============================================================================

    SequenceTransportUI.cpp
    Created: 20 Nov 2016 2:24:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceTransportUI.h"
#include "AudioModule.h"
#include "TriggerImageUI.h"

SequenceTransportUI::SequenceTransportUI(Sequence * _sequence) :
	sequence(_sequence),
	timeLabel(_sequence->currentTime)
{
	timeLabel.maxFontHeight = 16;
	addAndMakeVisible(&timeLabel);
	sequence->addSequenceListener(this);
	 
	
	togglePlayUI = sequence->togglePlayTrigger->createImageUI(AssetManager::getInstance()->getPlayImage());
	stopUI = sequence->stopTrigger->createImageUI(AssetManager::getInstance()->getStopImage());
	nextCueUI = sequence->nextCue->createImageUI(AssetManager::getInstance()->getNextCueImage());
	prevCueUI = sequence->prevCue->createImageUI(AssetManager::getInstance()->getPrevCueImage());
	
	addAndMakeVisible(togglePlayUI);
	addAndMakeVisible(stopUI);
	addAndMakeVisible(nextCueUI);
	addAndMakeVisible(prevCueUI);
	
	
}

SequenceTransportUI::~SequenceTransportUI()
{
	togglePlayUI = nullptr;
	stopUI = nullptr;
	nextCueUI = nullptr;
	prevCueUI = nullptr;

	sequence->removeSequenceListener(this);
}

void SequenceTransportUI::paint(Graphics &g)
{
	if (sequence->timeIsDrivenByAudio())
	{
		g.setColour(AUDIO_COLOR);
		Rectangle<int> r = getLocalBounds().reduced(2).removeFromTop(timeLabel.getHeight()).withLeft(timeLabel.getRight() + 5);
		g.setColour(AUDIO_COLOR.brighter(.7f));
		g.drawFittedText(sequence->masterAudioModule->niceName, r.reduced(2), Justification::centred, 1);
	}
}

void SequenceTransportUI::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);
	timeLabel.setBounds(r.removeFromTop(20).removeFromLeft(110));
	r.removeFromTop(2);
	
	
	Rectangle<int> pr = r.removeFromTop(24);
	togglePlayUI->setBounds(pr.removeFromLeft(pr.getHeight()));
	stopUI->setBounds(pr.removeFromLeft(pr.getHeight()));
	prevCueUI->setBounds(pr.removeFromLeft(pr.getHeight()));
	nextCueUI->setBounds(pr.removeFromLeft(pr.getHeight()));
	

}

void SequenceTransportUI::sequenceMasterAudioModuleChanged(Sequence *)
{
	repaint();
}

TimeLabel::TimeLabel(Parameter * p) :
	FloatParameterLabelUI(p)
{
	valueChanged(parameter->value);
}

TimeLabel::~TimeLabel()
{
}

void TimeLabel::valueChanged(const var & v)
{
	String timeString = valueToTimeString(v);
	FloatParameterLabelUI::valueChanged(timeString);
}

void TimeLabel::labelTextChanged(Label *)
{
	parameter->setValue(timeStringToValue(valueLabel.getText()));
}

#pragma warning (push)
#pragma warning(disable:4244)
String TimeLabel::valueToTimeString(float timeVal) const
{
	int hours = floor(timeVal / 3600);
	int minutes = floor(fmodf(timeVal,3600) / 60);
	float seconds = fmodf(timeVal, 60);
	return String::formatted("%02i:%02i:%06.3f", hours, minutes, seconds);
}

float TimeLabel::timeStringToValue(String str) const
{
	StringArray sa;
	if (str.endsWithChar(':')) str += "0";
	sa.addTokens(str,":","");

	float value = 0;
	
	value += sa.strings[sa.strings.size()-1].getFloatValue();

	if (sa.strings.size() >= 2)
	{
		int numMinutes = sa.strings[sa.strings.size() - 2].getIntValue() * 60;
		value += numMinutes;
		if (sa.strings.size() >= 3)
		{
			int numHours = sa.strings[sa.strings.size() - 3].getFloatValue() * 3600;
			value += numHours;
		}
	}
	
	return value;
}

#pragma warning (pop)

