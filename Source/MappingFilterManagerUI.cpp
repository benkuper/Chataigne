/*
  ==============================================================================

    MappingFilterManagerUI.cpp
    Created: 28 Oct 2016 8:09:00pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingFilterManagerUI.h"


MappingFilterManagerUI::MappingFilterManagerUI(MappingFilterManager * _manager) :
	BaseManagerUI<MappingFilterManager, MappingFilter, MappingFilterUI>("MappingFilters", _manager)
{
	addItemText = "Add Filter";
}

MappingFilterManagerUI::~MappingFilterManagerUI()
{
}
