/*
  ==============================================================================

    ConsequenceEditor.h
    Created: 25 Jan 2017 10:07:09am
    Author:  Ben

  ==============================================================================
*/

#ifndef CONSEQUENCEEDITOR_H_INCLUDED
#define CONSEQUENCEEDITOR_H_INCLUDED

#include "BaseCommandHandlerEditor.h"
#include "Consequence.h"

class ConsequenceEditor :
	public BaseCommandHandlerEditor
{
public:
	ConsequenceEditor(Consequence *c, bool isRoot);
	~ConsequenceEditor();

	Consequence * consequence;
};


#endif  // CONSEQUENCEEDITOR_H_INCLUDED
