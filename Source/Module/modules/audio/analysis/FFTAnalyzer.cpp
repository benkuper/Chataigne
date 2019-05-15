/*
  ==============================================================================

    FFTAnalyzer.cpp
    Created: 14 May 2019 4:08:08pm
    Author:  bkupe

  ==============================================================================
*/

#include "FFTAnalyzer.h"
#include "ui/FFTAnalyzerEditor.h"

FFTAnalyzer::FFTAnalyzer() :
	BaseItem("Analyzer 1")
{
	position = addFloatParameter("Position", "", .5f, 0, 1);
	size = addFloatParameter("Size", "", .1f, 0, 1);

	value = new FloatParameter(niceName + " value", "", 0, 0, 1);
	value->setControllableFeedbackOnly(true);

	Random r;
	color = addColorParameter("Color", "Color to see in the visualizer", Colour::fromHSV(r.nextFloat(), 1, 1, 1));
}

FFTAnalyzer::~FFTAnalyzer()
{

}


void FFTAnalyzer::process(float* fftSamples, int numSamples)
{
	if (!enabled->boolValue()) return;

	float result = 0;
	float targetPos = position->floatValue();
	float maxDist = size->floatValue() / 2;

	int numGoodSamples = 0;
	float totalCoef = 0;
	for (int i = 0; i < numSamples; i++)
	{
		float pos = i * 1.0f / numSamples;
		float dist = jmin<float>(fabsf(targetPos - pos) / maxDist, 1);
		if (dist >= 1) continue;

		numGoodSamples++;
		float factor = cosf(dist * float_Pi / 2); //smooth

		result += fftSamples[i] * factor;
		totalCoef += factor;
	}

	if (totalCoef > 0)
	{
		result /= totalCoef;
		value->setValue(result);
	}
}

void FFTAnalyzer::onContainerNiceNameChanged()
{
	BaseItem::onContainerNiceNameChanged();
	value->setNiceName(niceName + " value");
}

InspectableEditor* FFTAnalyzer::getEditor(bool isRoot)
{
	return new FFTAnalyzerEditor(this, isRoot);
}
