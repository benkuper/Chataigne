/*
  ==============================================================================

    ConsequenceManagerEditor.cpp
    Created: 25 Jan 2017 10:06:40am
    Author:  Ben

  ==============================================================================
*/

#include "ConsequenceManagerEditor.h"

ConsequenceManagerEditor::ConsequenceManagerEditor(ConsequenceManager * c, bool isRoot) :
	GenericManagerEditor(c,isRoot)
{
	noItemText = "Add output commands here";
}

ConsequenceManagerEditor::~ConsequenceManagerEditor()
{
}
