/*
  ==============================================================================

    MappingManagerUI.cpp
    Created: 28 Oct 2016 8:06:06pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingManagerUI.h"

MappingManagerUI::MappingManagerUI(MappingManager * _manager) :
	BaseManagerUI<MappingManager, Mapping, MappingUI>("Mappings", _manager)
{
	addItemText = "Add Mapping";

	addExistingItems();
}

MappingManagerUI::~MappingManagerUI()
{
}
