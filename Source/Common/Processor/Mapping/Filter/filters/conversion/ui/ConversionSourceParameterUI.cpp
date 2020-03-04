/*
  ==============================================================================

    ConversionSourceParameterUI.cpp
    Created: 4 Mar 2020 12:04:24am
    Author:  bkupe

  ==============================================================================
*/

#include "ConversionSourceParameterUI.h"

ConversionSourceParameterUI::ConversionSourceParameterUI(Parameter* source) :
	InspectableContentComponent(source),
	source(source)
{
	addAndMakeVisible(source->createDefaultUI());
}

ConversionSourceParameterUI::~ConversionSourceParameterUI()
{
}
