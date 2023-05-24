/*
  ==============================================================================

    MetronomeModule.h
    Created: 30 Dec 2017 10:09:22pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class MetronomeModule :
	public Module,
	public Thread
{
public:
	MetronomeModule();
	~MetronomeModule();

	BoolParameter * tick;


	enum MetroMode { FREQUENCY, TIME, BPM };
	EnumParameter* mode;
	FloatParameter * freqTimeBpm;
	FloatParameter * onTime;
	FloatParameter * random;
	Trigger * tapTempo;
	Trigger* resetTime;

	double TSTapTempoLastPressed = 0;
	Random rnd;
	Array<double> tapTempoHistory;
	IntParameter* tapTempoIntervalsMax;
	void updateFreqParam();
	
	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	virtual void run() override;

	String getTypeString() const override { return "Metronome"; }
	static MetronomeModule * create() { return new MetronomeModule(); }

	void tapTempoPressed();

};