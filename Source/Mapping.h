/*
  ==============================================================================

    Mapping.h
    Created: 28 Oct 2016 8:06:13pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPING_H_INCLUDED
#define MAPPING_H_INCLUDED


#include "BaseItem.h"

#include "MappingInput.h"
#include "MappingFilterManager.h"
#include "MappingOutputManager.h"
#include "ConditionManager.h"

class Mapping :
	public BaseItem,
	public MappingInput::Listener
{
public:
	Mapping();
	virtual ~Mapping();

	MappingInput input;
	ConditionManager cdm;
	MappingFilterManager fm;
	MappingOutputManager om;

	bool inputIsLocked;
	void lockInputTo(Parameter * lockParam);

	void process();

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void inputReferenceChanged(MappingInput *) override;
	void inputParameterValueChanged(MappingInput *) override;

	InspectableEditor * getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Mapping)
};




#endif  // MAPPING_H_INCLUDED
