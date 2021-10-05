/*
  ==============================================================================

    FFTAnalyzer.h
    Created: 14 May 2019 4:08:08pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class FFTAnalyzer :
	public BaseItem
{
public:
	FFTAnalyzer();
	~FFTAnalyzer();
	
	
	FloatParameter* position;
	FloatParameter* size;
	FloatParameter * value;
	ColorParameter * color;

	void process(float * fftSamples, int numSamples);

	void onContainerNiceNameChanged() override;

	InspectableEditor* getEditorInternal(bool isRoot) override;
};
