/*
  ==============================================================================

    BoolImageToggleUI.h
    Created: 4 Jan 2017 1:33:12pm
    Author:  Ben

  ==============================================================================
*/

#ifndef BOOLIMAGETOGGLEUI_H_INCLUDED
#define BOOLIMAGETOGGLEUI_H_INCLUDED

#include "BoolParameter.h"
#include "ParameterUI.h"

class BoolImageToggleUI :
	public ParameterUI,
	public ButtonListener
{
public:
	BoolImageToggleUI(ImageButton *, BoolParameter *);
	~BoolImageToggleUI();
	
	BoolParameter * boolParam;
	ScopedPointer<ImageButton> bt;

	void resized() override;

	void valueChanged(const var &) override;

	void buttonClicked(Button *) override;
};



#endif  // BOOLIMAGETOGGLEUI_H_INCLUDED
