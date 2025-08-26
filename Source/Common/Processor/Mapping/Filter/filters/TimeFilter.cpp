/*
  ==============================================================================

	TimeFilter.cpp
	Created: 18 Jun 2021 8:44:36pm
	Author:  bkupe

  ==============================================================================
*/

TimeFilter::TimeFilter(StringRef name, var params, Multiplex* multiplex) :
	MappingFilter(name, params, multiplex, true)
{
	deltaTimes.resize(getMultiplexCount());
	for (int i = 0; i < timesAtLastUpdate.size(); i++) timesAtLastUpdate.set(i, Time::getMillisecondCounter() / 1000.0);

	timesAtLastUpdate.resize(getMultiplexCount());
	deltaTimes.fill(0.000001);
}

TimeFilter::~TimeFilter()
{
}

void TimeFilter::multiplexCountChanged()
{
	deltaTimes.resize(getMultiplexCount());
	for (int i = 0; i < timesAtLastUpdate.size(); i++) timesAtLastUpdate.set(i, Time::getMillisecondCounter() / 1000.0);

	timesAtLastUpdate.resize(getMultiplexCount());
	deltaTimes.fill(0.000001);
}

MappingFilter::ProcessResult TimeFilter::processInternal(Array<Parameter*> sources, int multiplexIndex)
{
	double curTime = Time::getMillisecondCounterHiRes() / 1000.0;
	double lastUpdate = timesAtLastUpdate.size() > multiplexIndex ? timesAtLastUpdate.getUnchecked(multiplexIndex) : curTime;
	deltaTimes.set(multiplexIndex, jmax<double>(curTime - lastUpdate, 0.00001));

	ProcessResult r = MappingFilter::processInternal(sources, multiplexIndex);

	timesAtLastUpdate.set(multiplexIndex, curTime);
	return r;
}

MappingFilter::ProcessResult TimeFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	return processSingleParameterTimeInternal(source, out, multiplexIndex, deltaTimes[multiplexIndex]);
}
