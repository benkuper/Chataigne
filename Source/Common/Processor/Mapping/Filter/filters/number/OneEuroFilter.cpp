/*
  ==============================================================================

	OneEuroFilter.cpp
	Created: 18 Jun 2021 10:45:02am
	Author:  bkupe

  ==============================================================================
*/

OneEuroFilter::OneEuroFilter(var params, Multiplex* multiplex) :
	TimeFilter(getTypeString(), params, multiplex)
{
	processOnSameValue = true;

	freq = 50;
	dcutoff = 1;

	mincutoff = addFloatParameter("Min Cutoff", "Minimum cutoff", .3f, 0);
	beta_ = addFloatParameter("Beta", "Beta", .1f, 0);

	x.reset(new LowPassFilter(alpha(mincutoff->floatValue())));
	dx.reset(new LowPassFilter(alpha(dcutoff)));

	filterTypeFilters.add(Controllable::BOOL, Controllable::FLOAT, Controllable::INT, Controllable::COLOR, Controllable::POINT2D, Controllable::POINT3D);
}

OneEuroFilter::~OneEuroFilter()
{
}

float OneEuroFilter::alpha(float cutoff)
{
    float te = 1.0f / freq;
    float tau = 1.0f / (2 * float_Pi * cutoff);
    return 1.0f / (1.0f + tau / te);
}


Parameter* OneEuroFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	Parameter* p = nullptr;
	if (!rangeOnly && source->type == Controllable::BOOL)
	{
		p = new FloatParameter(source->niceName, source->description, source->value, 0, 1);
		p->isSavable = false;
		p->setControllableFeedbackOnly(true);
	}
	else
	{
		p = MappingFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
	}

	return p;
}


MappingFilter::ProcessResult OneEuroFilter::processSingleParameterTimeInternal(Parameter* source, Parameter* out, int multiplexIndex, double deltaTime)
{
	var oldVal = out->getValue();
	var newVal = source->getValue();

	if (source->checkValueIsTheSame(oldVal, newVal)) return UNCHANGED;

	var val;

	

	if (out->isComplex())
	{
		for (int i = 0; i < oldVal.size(); ++i)
		{
			float dvalue = ((float)newVal[i] - (float)oldVal[i]) * freq; // FIXME: 0.0f or value?
			float edvalue = dx->filterWithAlpha(dvalue, alpha(dcutoff));
			float cutoff = mincutoff->floatValue() + beta_->floatValue() * fabs(edvalue);
			val = x->filterWithAlpha((float)newVal[i], alpha(cutoff));
		}

	}
	else
	{
		// estimate the current variation per second 
		float dvalue = ((float)newVal - (float)oldVal) * freq; // FIXME: 0.0f or value?
		float edvalue = dx->filterWithAlpha(dvalue, alpha(dcutoff));
		// use it to update the cutoff frequency
		float cutoff = mincutoff->floatValue() + beta_->floatValue() * fabs(edvalue);
		// filter the given value
		val = x->filterWithAlpha((float)newVal, alpha(cutoff));
	}

	out->setValue(val);

	return CHANGED;
}


void OneEuroFilter::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	MappingFilter::onControllableFeedbackUpdateInternal(cc, c);
}