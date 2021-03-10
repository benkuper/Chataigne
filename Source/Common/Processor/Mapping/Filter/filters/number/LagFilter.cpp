/*
  ==============================================================================

    LagFilter.cpp
    Created: 4 Feb 2017 5:39:47pm
    Author:  Ben

  ==============================================================================
*/

LagFilter::LagFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{
	frequency = filterParams.addFloatParameter("Frequency", "Lag frequency in Hz", 5, .01f, 50);
	startTimer(1000 / frequency->floatValue());

	processOnSameValue = true;
}

LagFilter::~LagFilter()
{

}

void LagFilter::setupParametersInternal(int multiplexIndex)
{
	paramTempValueMap.clear();
	MappingFilter::setupParametersInternal(multiplexIndex);
}

Parameter* LagFilter::setupSingleParameterInternal(Parameter* source, int multiplexIndex)
{
	var tmpVal = var(source->getValue()); //shoud maybe copy the values or is it enough ?
	paramTempValueMap.set(source, tmpVal);
	return MappingFilter::setupSingleParameterInternal(source, multiplexIndex);
}

MappingFilter::ProcessResult  LagFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	if (!paramTempValueMap.contains(source)) return UNCHANGED;
	if (paramTempValueMap[source] == out->getValue()) return UNCHANGED;
	
	out->setValue(paramTempValueMap[source]);
	return CHANGED;
}

void LagFilter::filterParamChanged(Parameter * p)
{
	if(p == frequency) startTimer(1000 / frequency->floatValue());
}

void LagFilter::hiResTimerCallback()
{
	for (auto& mSourceParams : sourceParams)
	{
		for (auto& s : mSourceParams)
		{
			if (s == nullptr) continue;
			paramTempValueMap.set(s, var(s->value));
		}
	}
}
