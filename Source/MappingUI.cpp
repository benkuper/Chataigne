/*
  ==============================================================================

    MappingUI.cpp
    Created: 28 Oct 2016 8:06:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingUI.h"


MappingUI::MappingUI(Mapping * mapping) :
	ProcessorUI(mapping)
{
	bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);

}

MappingUI::~MappingUI()
{

}
