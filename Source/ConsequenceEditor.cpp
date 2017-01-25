/*
  ==============================================================================

    ConsequenceEditor.cpp
    Created: 25 Jan 2017 10:07:09am
    Author:  Ben

  ==============================================================================
*/

#include "ConsequenceEditor.h"

ConsequenceEditor::ConsequenceEditor(Consequence * c, bool isRoot) :
	BaseCommandHandlerEditor(c, isRoot),
	consequence(c)
{
}

ConsequenceEditor::~ConsequenceEditor()
{
}
