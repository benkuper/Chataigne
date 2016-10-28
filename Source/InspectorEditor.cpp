/*
  ==============================================================================

    InspectorEditor.cpp
    Created: 9 May 2016 7:08:50pm
    Author:  bkupe

  ==============================================================================
*/

#include "InspectorEditor.h"

InspectorEditor::InspectorEditor(InspectableComponent * _sourceComponent) :
	sourceComponent(_sourceComponent)
{
}

InspectorEditor::~InspectorEditor()
{
}


void InspectorEditor::resized()
{
	
}

int InspectorEditor::getContentHeight()
{
	return 0; //will default to parent's bounds
}

void InspectorEditor::clear()
{
}
