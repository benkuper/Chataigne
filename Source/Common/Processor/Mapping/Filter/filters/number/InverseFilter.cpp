/*
  ==============================================================================

    InverseFilter.cpp
    Created: 4 Feb 2017 5:39:15pm
    Author:  Ben

  ==============================================================================
*/

#include "InverseFilter.h"

InverseFilter::InverseFilter(var params) :
	MappingFilter(getTypeString(), params)
{
	editorCanBeCollapsed = false;
	editorIsCollapsed = true;
}

InverseFilter::~InverseFilter()
{
}

void InverseFilter::processInternal()
{
	if (sourceParam->isComplex())
	{
		var val;
		int numValToInverse = sourceParam->type != Controllable::COLOR ? sourceParam->value.size() : 3; //do not invert alpha by default (may improve to have an option)
		for (int i = 0; i < numValToInverse; i++)
		{
			float normVal = ((float)sourceParam->value[i] - (float)sourceParam->minimumValue[i]) / ((float)sourceParam->maximumValue[i] - (float)sourceParam->minimumValue[i]);
			val.append(jmap<float>(normVal, sourceParam->maximumValue[i], sourceParam->minimumValue[i]));
		}
		for (int i = numValToInverse; i < sourceParam->value.size(); i++)
		{
			val.append(sourceParam->value[i]);
		}

	
		filteredParameter->setValue(val);

		for (int i = 0; i < val.size(); i++)
		{
			DBG("Filtered Value " << i << " : " << (float)filteredParameter->value[i] << " / " << (float)val[i]);
		}

	}
	else
	{
		filteredParameter->setValue(jmap<float>(sourceParam->getNormalizedValue(), sourceParam->maximumValue, sourceParam->minimumValue));
	}
}
