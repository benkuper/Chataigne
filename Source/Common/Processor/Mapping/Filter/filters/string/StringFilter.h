/*
  ==============================================================================

	StringFilter.h
	Created: 16 Mar 2024 11:00:56am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class StringSliceFilter :
	public MappingFilter
{
public:
	StringSliceFilter(var params, Multiplex* multiplex);
	~StringSliceFilter();

	IntParameter* start;
	IntParameter* end;

	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	String getTypeString() const override { return getTypeStringStatic(); }
	static String getTypeStringStatic() { return "String Slice"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringSliceFilter)
};

class StringReplaceFilter :
	public MappingFilter
{
public:
	StringReplaceFilter(var params, Multiplex* multiplex);
	~StringReplaceFilter();

	StringParameter* pattern;
	StringParameter* replace;
	BoolParameter* regexMode;

	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	String getTypeString() const override { return getTypeStringStatic(); }
	static String getTypeStringStatic() { return "String Replace"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringReplaceFilter)
};


class StringOffsetFilter :
	public MappingFilter
{
public:
	StringOffsetFilter(var params, Multiplex* multiplex);
	~StringOffsetFilter();

	IntParameter* offset;
	IntParameter* start;
	IntParameter* end;

	ProcessResult processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex) override;

	String getTypeString() const override { return getTypeStringStatic(); }
	static String getTypeStringStatic() { return "String Offset"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringOffsetFilter)
};

