/*
  ==============================================================================

	ColorParameterUI.h
	Created: 11 Apr 2017 10:42:03am
	Author:  Ben

  ==============================================================================
*/

#ifndef COLORPARAMETERUI_H_INCLUDED
#define COLORPARAMETERUI_H_INCLUDED

#include "ParameterUI.h"
#include "ColorParameter.h"

class ColorParameterUI :
	public ParameterUI,
	public ChangeListener
{

public:
	ColorParameterUI(ColorParameter * parameter = nullptr);
	~ColorParameterUI();

	ColorParameter * colorParam;

	void paint(Graphics &g) override;
	void resized() override;
	void mouseDown(const MouseEvent &e) override;

protected:
	// Inherited via ChangeListener
	virtual void changeListenerCallback(ChangeBroadcaster * source) override;
	void valueChanged(const var &) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorParameterUI)

		
};

#endif  // COLORPARAMETERUI_H_INCLUDED
