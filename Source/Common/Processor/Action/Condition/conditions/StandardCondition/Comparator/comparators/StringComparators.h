/*
  ==============================================================================

    StringComparators.h
    Created: 2 Nov 2016 8:58:35pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class StringComparator :
	public BaseComparator
{
public:
	StringComparator(Parameter * sourceParam, Multiplex * multiplex);
	virtual ~StringComparator();

	const Identifier equalsId = "=";
	const Identifier differentId = "!=";
	
	const Identifier containsId = "contains";
	const Identifier containsIgnoreCaseId = "containsIgnoreCase";

    const Identifier regexpMatchesId = "regexpMatch";
    const Identifier notRegexpMatchesId = "notRegexpMatch";


	const Identifier lengthIsId = "lengthIs";
	const Identifier lengthIsNotId = "lengthIsNot";
	const Identifier lengthLongerThanId = "lengthLonger";
	const Identifier lengthShorterThanId = "lengthShorter";

	const Identifier notContainsId = "notContains";
	const Identifier notContainsIgnoreCaseId = "notContainsIgnoreCase";
	
	const Identifier startsWithId = "startsWith";
	const Identifier startsWithIgnoreCaseId = "startsWithIgnoreCase";
	
	const Identifier notStartsWithId = "notStartsWith";
	const Identifier notStartsWithIgnoreCaseId = "notStartsWithIgnoreCase";
	
	const Identifier endsWithId = "endsWith";
	const Identifier endsWithIgnoreCaseId = "endsWithIgnoreCase";
	
	const Identifier notEndsWithId = "notEndsWith";
	const Identifier notEndsWithIgnoreCaseId = "notEndsWithIgnoreCase";
	
	
	virtual void compareFunctionChanged() override;
	virtual void setupReferenceParam();
	virtual bool compareInternal(Parameter* sourceParam, int multiplexIndex) override;
    static bool regexpMatch(const juce::String& regexpString, const juce::String& stringToTest, const juce::String& flags);

};