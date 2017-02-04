/*
  ==============================================================================

    MappingFilterManager.h
    Created: 28 Oct 2016 8:08:47pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGFILTERMANAGER_H_INCLUDED
#define MAPPINGFILTERMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "MappingFilter.h"

class MappingFilterManager :
	public BaseManager<MappingFilter>
{
public:
	MappingFilterManager();
	~MappingFilterManager();

	Parameter * processFilters(Parameter * p);

	void addItemFromData(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilterManager)

};



#endif  // MAPPINGFILTERMANAGER_H_INCLUDED
