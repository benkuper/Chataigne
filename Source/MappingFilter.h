/*
  ==============================================================================

    MappingFilter.h
    Created: 28 Oct 2016 8:08:53pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGFILTER_H_INCLUDED
#define MAPPINGFILTER_H_INCLUDED


#include "BaseItem.h"

class MappingFilter :
	public BaseItem
{
public:
	MappingFilter(const String &name = "MappingFilter");
	virtual ~MappingFilter();

	Parameter * filteredParameter;
	void setupParameter(Parameter * source);
	virtual Parameter * setupParameterInternal(Parameter * source);

	Parameter * process(Parameter * sourceParam);
	virtual void processInternal(Parameter * /*source*/, Parameter * /*target*/) {}

	var getJSONData() override;

	virtual String getTypeString() const { jassert(false); return "[ERROR]"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilter)
};




#endif  // MAPPINGFILTER_H_INCLUDED
