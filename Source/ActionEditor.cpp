/*
  ==============================================================================

    ActionEditor.cpp
    Created: 25 Jan 2017 10:03:47am
    Author:  Ben

  ==============================================================================
*/

#include "ActionEditor.h"

ActionEditor::ActionEditor(Action * a, bool isRoot) :
	BaseItemEditor(a, isRoot),
	cdme(&a->cdm,false),
	csme(&a->csm,false),
	action(a)
{
	addAndMakeVisible(&cdme);
	addAndMakeVisible(&csme);
}

ActionEditor::~ActionEditor()
{
}

void ActionEditor::resizedInternalContent(Rectangle<int>& r)
{
	cdme.setBounds(r.withHeight(cdme.getHeight()));
	r.translate(0,cdme.getHeight() + 10);
	r.setHeight(csme.getHeight());
	csme.setBounds(r);
}
