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
	if (!rangeOnly) paramTempValueMap.clear();
	lastSendTime.resize(getMultiplexCount());
	lastSendTime.fill(Time::getMillisecondCounterHiRes());
	MappingFilter::setupParametersInternal(multiplexIndex, rangeOnly);
}

Parameter* LagFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex, bool rangeOnly)
{
	if (!rangeOnly)
	{
		var tmpVal = var(source->getValue()); //shoud maybe copy the values or is it enough ?
		paramTempValueMap.set(source, tmpVal);
	}
	return MappingFilter::setupSingleParameterInternal(source, multiplexIndex, rangeOnly);
}

MappingFilter::ProcessResult  LagFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (!paramTempValueMap.contains(source)) return UNCHANGED;

	paramTempValueMap.set(source, source->getValue());
	if (paramTempValueMap[source] == out->getValue()) return UNCHANGED;

	double lastTime = lastSendTime[multiplexIndex];
	double currentTime = Time::getMillisecondCounterHiRes();
	double msToWait = 1000.0 / frequency->doubleValue();


	if (currentTime - lastTime < msToWait) return UNCHANGED;
	out->setValue(paramTempValueMap[source]);

	lastSendTime.set(multiplexIndex, currentTime);

	return CHANGED;
}

void LagFilter::filterParamChanged(Parameter* p)
{
}