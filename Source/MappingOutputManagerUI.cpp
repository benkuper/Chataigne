/*
  ==============================================================================

    MappingOutputManagerUI.cpp
    Created: 28 Oct 2016 8:12:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingOutputManagerUI.h"


MappingOutputManagerUI::MappingOutputManagerUI(MappingOutputManager * _manager) :
	BaseManagerUI<MappingOutputManager, MappingOutput, MappingOutputUI>("MappingOutputs", _manager)
{
	addItemText = "Add Output";
}

MappingOutputManagerUI::~MappingOutputManagerUI()
{
}
