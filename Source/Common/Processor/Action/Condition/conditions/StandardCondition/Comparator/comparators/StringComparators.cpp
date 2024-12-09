/*
  ==============================================================================

    StringComparators.cpp
    Created: 2 Nov 2016 8:58:35pm
    Author:  bkupe

  ==============================================================================
*/

StringComparator::StringComparator(Parameter *sourceParam, Multiplex* multiplex) :
	BaseComparator(multiplex)
{
    setupReferenceParam();
	reference->setValue(sourceParam->stringValue(), false, true, true);

	addCompareOption("=", equalsId);
	addCompareOption("!=", differentId);

	addCompareOption("Contains", containsId);
	addCompareOption("Contains (ignoring case)", containsIgnoreCaseId);

	addCompareOption("Does not contain", notContainsId);
	addCompareOption("Does not contain (ignoring case)", notContainsIgnoreCaseId);

	addCompareOption("Matches regexp", regexpMatchesId);
	addCompareOption("Does not match regexp", notRegexpMatchesId);

	addCompareOption("Length is", lengthIsId);
	addCompareOption("Length is not", lengthIsNotId);
	addCompareOption("Length is less than", lengthShorterThanId);
	addCompareOption("Length is greater than", lengthLongerThanId);

	addCompareOption("Starts with", startsWithId);
	addCompareOption("Starts with (ignoring case)", startsWithIgnoreCaseId);

	addCompareOption("Does not start with", notStartsWithId);
	addCompareOption("Does not start with (ignoring case)", notStartsWithIgnoreCaseId);

	addCompareOption("Ends with", endsWithId);
	addCompareOption("Ends with (ignoring case)", endsWithIgnoreCaseId);

	addCompareOption("Does not end with", notEndsWithId);
	addCompareOption("Does not end with (ignoring case)", notEndsWithIgnoreCaseId);

	addCompareOption("Change", changeId);
}

StringComparator::~StringComparator()
{
}

void StringComparator::compareFunctionChanged()
{
	setupReferenceParam();
}


void StringComparator::setupReferenceParam()
{
	if (currentFunctionId.toString().contains("length"))
	{
	    int defaultValue = 0;
        if (reference != nullptr && reference->type == Parameter::INT)
        {
            defaultValue = reference->intValue();
        }
		setReferenceParam(new IntParameter("Reference", "Comparison Reference to check against source value", defaultValue, 0,INT32_MAX));
	} else {
        String defaultValue = "";
	    if (reference != nullptr && reference->type == Parameter::STRING)
    	{
    	    defaultValue = reference->stringValue();
    	}
    	setReferenceParam(new StringParameter("Reference", "Comparison Reference to check against source value", defaultValue));
	}
}


bool StringComparator::compareInternal(Parameter* sourceParam, int multiplexIndex)
{
	String value = isMultiplexed() ? refLink->getLinkedValue(multiplexIndex).toString() : reference->stringValue();


	if (currentFunctionId == equalsId)				            return sourceParam->stringValue() == value;
	else if (currentFunctionId == differentId)		            return sourceParam->stringValue() != value;

	else if (currentFunctionId == containsId)		            return sourceParam->stringValue().contains(value);
	else if (currentFunctionId == containsIgnoreCaseId)		    return sourceParam->stringValue().containsIgnoreCase(value);

	else if (currentFunctionId == notContainsId)	            return !sourceParam->stringValue().contains(value);
	else if (currentFunctionId == notContainsIgnoreCaseId)	    return !sourceParam->stringValue().containsIgnoreCase(value);

    // @TODO: Add UI that enables setting the flags string value (or checkboxes). (See third parameter).
    // If it is complicated, we could copy PHP's/sed's way of doing it by specifying regexp with wildcard:   #fooregexp#im.
	else if (currentFunctionId == regexpMatchesId)		        return StringComparator::regexpMatch(value,sourceParam->stringValue(),"");
	else if (currentFunctionId == notRegexpMatchesId)		    return !StringComparator::regexpMatch(value,sourceParam->stringValue(),"");

	else if (currentFunctionId == lengthIsId)	                return sourceParam->stringValue().length()==value.getIntValue();
	else if (currentFunctionId == lengthIsNotId)	            return sourceParam->stringValue().length()!=value.getIntValue();
	else if (currentFunctionId == lengthLongerThanId)	        return sourceParam->stringValue().length()>value.getIntValue();
	else if (currentFunctionId == lengthShorterThanId)	        return sourceParam->stringValue().length()<value.getIntValue();

	else if (currentFunctionId == startsWithId)		            return sourceParam->stringValue().startsWith(value);
	else if (currentFunctionId == startsWithIgnoreCaseId)	    return sourceParam->stringValue().startsWithIgnoreCase(value);

	else if (currentFunctionId == notStartsWithId)	            return !sourceParam->stringValue().startsWith(value);
	else if (currentFunctionId == notStartsWithIgnoreCaseId)    return !sourceParam->stringValue().startsWithIgnoreCase(value);

	else if (currentFunctionId == endsWithId)		            return sourceParam->stringValue().endsWith(value);
	else if (currentFunctionId == endsWithIgnoreCaseId)		    return sourceParam->stringValue().endsWithIgnoreCase(value);

	else if (currentFunctionId == notEndsWithId)	            return !sourceParam->stringValue().endsWith(value);
	else if (currentFunctionId == notEndsWithIgnoreCaseId)	    return !sourceParam->stringValue().endsWithIgnoreCase(value);

	return false;
}

/** Checks if the given string matches the regex wildcard with the specified flags. */
bool  StringComparator::regexpMatch(const juce::String& regexpString, const juce::String& stringToTest, const juce::String& flags)
{
    try
    {
        // Determine regex flags
        std::regex::flag_type regexFlags = std::regex::ECMAScript; // Default syntax
        /*
        if (flags.containsChar('i')) {
            regexFlags |= std::regex::icase; // Case-insensitive
        }

        if (flags.containsChar('m')) {
            // Beware: This is only available from C++17, this broke the windows builds back when this was written.
            // regexFlags |= std::regex::multiline;  // Specifies that ^ shall match the beginning of a line and $ shall match the end of a line, if the ECMAScript engine is selected.
        }
        */

        // Compile the regex with the specified flags
        std::regex reg(regexpString.toStdString(), regexFlags);

        // Perform the regex match
        return std::regex_search(stringToTest.toStdString(), reg);
    }
    catch (const std::regex_error& e)
    {
        DBG("Regex error: " << e.what());
        return false;
    }
}
