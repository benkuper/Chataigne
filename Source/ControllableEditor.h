/*
  ==============================================================================

    ControllableEditor.h
    Created: 7 Oct 2016 2:04:37pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONTROLLABLEEDITOR_H_INCLUDED
#define CONTROLLABLEEDITOR_H_INCLUDED

#include "CustomEditor.h"
#include "Controllable.h"

class ControllableEditor : 
	public CustomEditor
{
public:
	ControllableEditor(InspectableComponent * sourceComponent, Controllable * controllable);

	Controllable * controllable;
	
	Label label;
	ScopedPointer<ControllableUI> ui;

	void resized() override;

	int getContentHeight() override;

};



#endif  // CONTROLLABLEEDITOR_H_INCLUDED
