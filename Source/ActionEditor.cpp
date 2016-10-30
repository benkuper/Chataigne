/*
  ==============================================================================

    ActionEditor.cpp
    Created: 29 Oct 2016 5:52:20pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionEditor.h"

ActionEditor::ActionEditor(Action * _action) :
	CustomEditor(_action),
	action(_action),
	cdmui(&_action->cdm),
	csmui(&_action->csm)
{
	addAndMakeVisible(&cdmui);
	addAndMakeVisible(&csmui);

	cdmui.drawContour = true;
	csmui.drawContour = true;
}

ActionEditor::~ActionEditor()
{
}

void ActionEditor::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(5);
	cdmui.setBounds(r.removeFromTop(getHeight() / 2 - 5));
	r.removeFromTop(10);
	csmui.setBounds(r);
}

int ActionEditor::getContentHeight()
{
	return 0;
}
