/*
  ==============================================================================

	StringFilter.cpp
	Created: 16 Mar 2024 11:00:56am
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"
#include "StringFilter.h"

StringSliceFilter::StringSliceFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{
	start = filterParams.addIntParameter("Start", "Start", 0);
	end = filterParams.addIntParameter("End", "End", -1);

	filterTypeFilters.add(Controllable::STRING);
}

StringSliceFilter::~StringSliceFilter()
{
}

MappingFilter::ProcessResult StringSliceFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	String str = source->stringValue();

	int s = filterParams.getLinkedValue(start, multiplexIndex);
	int e = filterParams.getLinkedValue(end, multiplexIndex);

	if (s < 0) s += str.length();
	if (e < 0) e += str.length() + 1;
	if (e < s) e += str.length() + 1;

	String result = str.substring(s, e);
	if (result == str) return ProcessResult::UNCHANGED;
	out->setValue(result);

	return ProcessResult::CHANGED;
}

StringReplaceFilter::StringReplaceFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{
	pattern = filterParams.addStringParameter("Pattern", "Pattern to look for in the source", "cool");
	replace = filterParams.addStringParameter("Replace", "String to replace with", "awesome");
	regexMode = filterParams.addBoolParameter("Regex Mode", "Use regex for pattern", false);

	filterTypeFilters.add(Controllable::STRING);

}

StringReplaceFilter::~StringReplaceFilter()
{
}

MappingFilter::ProcessResult StringReplaceFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	String str = source->stringValue();

	String pat = filterParams.getLinkedValue(pattern, multiplexIndex);
	String repl = filterParams.getLinkedValue(replace, multiplexIndex);
	bool regex = filterParams.getLinkedValue(regexMode, multiplexIndex);

	String result = str;

	if (!regex)
	{
		result = str.replace(pat, repl);
	}
	else
	{
		result = std::regex_replace(str.toStdString(), std::regex(pat.toStdString()), repl.toStdString());
	}

	if (result == str) return ProcessResult::UNCHANGED;
	out->setValue(result);
	return ProcessResult::CHANGED;
}

StringOffsetFilter::StringOffsetFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(), params, multiplex)
{
	offset = filterParams.addIntParameter("Offset", "Offset", 0);

	start = filterParams.addIntParameter("Start", "Start", 0, INT32_MIN, INT32_MAX, false);
	start->canBeDisabledByUser = true;

	end = filterParams.addIntParameter("End", "End", 0, INT32_MIN, INT32_MAX, false);
	end->canBeDisabledByUser = true;

	filterTypeFilters.add(Controllable::STRING);
}

StringOffsetFilter::~StringOffsetFilter()
{
}

MappingFilter::ProcessResult StringOffsetFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
	String str = source->stringValue();

	


	String result;

	int o = filterParams.getLinkedValue(offset, multiplexIndex);

	int s = start->enabled ? (int)filterParams.getLinkedValue(start, multiplexIndex) : 0;
	int e = end->enabled ? (int)filterParams.getLinkedValue(end, multiplexIndex) : str.length() + 1;

	if (s < 0) s += str.length();
	if (e < 0) e += str.length();
	if (e < s) e += str.length();


	for (int i = 0; i < str.length(); i++)
	{
		int curOffset = i < s || i > e ? 0 : o;
		char c = (str[i] + curOffset) % 255;
		result += String::fromUTF8(&c, 1);
	};

	if (result == str) return ProcessResult::UNCHANGED;
	out->setValue(result);

	return ProcessResult::CHANGED;
}
