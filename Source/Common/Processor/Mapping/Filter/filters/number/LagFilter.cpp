/*
  ==============================================================================

	LagFilter.cpp
	Created: 4 Feb 2017 5:39:47pm
	Author:  Ben

  ==============================================================================
*/

LagFilter::LagFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex, true)
{
	frequency = filterParams.addFloatParameter("Frequency", "Lag frequency in Hz", 5, .01f, 50);

	processOnSameValue = true;
}

LagFilter::~LagFilter()
{
}

void LagFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
	if (lastSendTime.size() != getMultiplexCount())
	{
		lastSendTime.resize(getMultiplexCount());
		lastSendTime.fill(Time::getMillisecondCounterHiRes());
	}
	MappingFilter::setupParametersInternal(multiplexIndex, rangeOnly);
}

MappingFilter::ProcessResult  LagFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	double lastTime = lastSendTime[multiplexIndex];
	double currentTime = Time::getMillisecondCounterHiRes();
	double msToWait = 1000.0 / frequency->doubleValue();


	if (currentTime - lastTime < msToWait) return UNCHANGED;
	out->setValue(source->getValue());

	lastSendTime.set(multiplexIndex, currentTime);

	return CHANGED;
}

void LagFilter::filterParamChanged(Parameter* p)
{
}