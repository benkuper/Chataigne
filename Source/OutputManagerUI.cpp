/*
  ==============================================================================

    OutputManagerUI.cpp
    Created: 28 Oct 2016 8:22:11pm
    Author:  bkupe

  ==============================================================================
*/

#include "OutputManagerUI.h"

OutputManagerUI::OutputManagerUI(OutputManager * _manager) :
	BaseManagerUI<OutputManager,Output,OutputUI>("Outputs",_manager)
{
	addItemText = "Add Output";
}

OutputManagerUI::~OutputManagerUI()
{
}
