/*
  ==============================================================================

	OneEuroFilter.h
	Created: 18 Jun 2021 10:45:02am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class LowPassFilter {

	double y, a, s;
	bool initialized;

	void setAlpha(double alpha) {
		a = juce::jlimit<double>(0, 1, alpha);
	}

public:

	LowPassFilter(double alpha = 0, double initval = 0.0f) {
		y = s = initval;
		setAlpha(alpha);
		initialized = false;
	}

	double filter(double value) {
		double result;
		if (initialized)
			result = a * value + (1.0f - a) * s;
		else {
			result = value;
			initialized = true;
		}
		y = value;
		s = result;
		return result;
	}

	double filterWithAlpha(double value, double alpha) {
		setAlpha(alpha);
		return filter(value);
	}

	bool hasLastRawValue(void) {
		return initialized;
	}

	double lastRawValue(void) {
		return y;
	}
};


class OneEuroFilter
{
public:
	OneEuroFilter(int dimensions);
	~OneEuroFilter();

	int dimensions;
	double freq;
	double minCutOff;
	double beta;
	double derivativeCutOff;

	juce::OwnedArray<LowPassFilter> x;
	juce::OwnedArray<LowPassFilter> dx;

	double alpha(double cutoff);
	juce::Array<double> filter(juce::Array<double> oldValues, juce::Array<double>  newValue, double deltaTime);
	juce::var filter(juce::var oldValue, juce::var newValue, double deltaTime);

};



class OneEuroFloatFilter :
	public OneEuroFilter
{
public:
	OneEuroFloatFilter();
	~OneEuroFloatFilter();

	double filter(double oldValue, double newValue, double deltaTime);
};

