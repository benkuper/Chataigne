/*
  ==============================================================================

    ConsequenceManagerEditor.h
    Created: 25 Jan 2017 10:06:40am
    Author:  Ben

  ==============================================================================
*/

#ifndef CONSEQUENCEMANAGEREDITOR_H_INCLUDED
#define CONSEQUENCEMANAGEREDITOR_H_INCLUDED

#include "GenericManagerEditor.h"
#include "ConsequenceManager.h"

class ConsequenceManagerEditor :
	public GenericManagerEditor<Consequence>
{
public:
	ConsequenceManagerEditor(ConsequenceManager * c, bool isRoot);
	~ConsequenceManagerEditor();
};

#endif  // CONSEQUENCEMANAGEREDITOR_H_INCLUDED
