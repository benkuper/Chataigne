/*
  ==============================================================================

    CustomOSCCommandCreator.h
    Created: 19 Dec 2016 3:06:08pm
    Author:  Ben

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMANDCREATOR_H_INCLUDED
#define CUSTOMOSCCOMMANDCREATOR_H_INCLUDED

#include "JuceHeader.h"
#include "CustomOSCCommand.h"

class CustomOSCArgumentUI :
	public Component
{
public:
	CustomOSCArgumentUI();
	ComboBox typeBox;
	Label defaultValue;
	ToggleButton getFromMapping;
	Slider mappingValueId;

	void resized() override;
};

class CustomOSCCommandCreator :
	public ResizableWindow,
	public ButtonListener
{
public:
	CustomOSCCommandCreator(CustomOSCCommand * command = nullptr);

	Label addressLabel;
	Slider numArgsSlider;
	Array<CustomOSCArgumentUI> oscParams;

	void addArgument();
	void removeLastArgument();

	void resized() override;

	void buttonClicked(Button *) override;
};



#endif  // CUSTOMOSCCOMMANDCREATOR_H_INCLUDED
