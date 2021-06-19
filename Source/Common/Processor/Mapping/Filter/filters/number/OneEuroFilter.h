/*
  ==============================================================================

    OneEuroFilter.h
    Created: 18 Jun 2021 10:45:02am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class LowPassFilter {

    float y, a, s;
    bool initialized;

    void setAlpha(float alpha) {
        if (alpha <= 0.0f || alpha > 1.0f)
            throw std::range_error("alpha should be in (0.0f., 1.0]");
        a = alpha;
    }

public:

    LowPassFilter(float alpha, float initval = 0.0f) {
        y = s = initval;
        setAlpha(alpha);
        initialized = false;
    }

    float filter(float value) {
        float result;
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

    float filterWithAlpha(float value, float alpha) {
        setAlpha(alpha);
        return filter(value);
    }

    bool hasLastRawValue(void) {
        return initialized;
    }

    float lastRawValue(void) {
        return y;
    }
};

class OneEuroFilter :
	public TimeFilter
{
public:
	OneEuroFilter(var params, Multiplex* multiplex);
	~OneEuroFilter();


    FloatParameter* beta_;
    FloatParameter * mincutoff;

    float freq;
    float dcutoff;

    std::unique_ptr<LowPassFilter> x;
    std::unique_ptr<LowPassFilter> dx;
    
    float alpha(float cutoff);

	Parameter* setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly) override;
	ProcessResult processSingleParameterTimeInternal(Parameter* source, Parameter* out, int multiplexIndex, double deltaTime) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	String getTypeString() const override { return "One Euro"; }
};