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
	public InspectableEditor,
	public ButtonListener
{
public:
	ControllableEditor(Controllable * controllable, bool isRoot, int initHeight = 16);  //Todo : handle full feedback if is root

	Controllable * controllable;
	
	Label label;
	ScopedPointer<ControllableUI> ui;
	ScopedPointer<ImageButton> removeBT;

	void resized() override;

	void buttonClicked(Button * b);
};



#endif  // CONTROLLABLEEDITOR_H_INCLUDED
