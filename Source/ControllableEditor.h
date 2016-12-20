/*
  ==============================================================================

    ControllableEditor.h
    Created: 7 Oct 2016 2:04:37pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONTROLLABLEEDITOR_H_INCLUDED
#define CONTROLLABLEEDITOR_H_INCLUDED

#include "InspectableEditor.h"
#include "Controllable.h"

class ControllableEditor : 
	public InspectableEditor
{
public:
	ControllableEditor(Controllable * controllable, bool isRootEditor);  //Todo : handle full feedback if is root

	Controllable * controllable;
	
	Label label;
	ScopedPointer<ControllableUI> ui;

	void resized() override;

};



#endif  // CONTROLLABLEEDITOR_H_INCLUDED
