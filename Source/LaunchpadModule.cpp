/*
  ==============================================================================

    LaunchpadModule.cpp
    Created: 21 Jul 2017 11:56:28am
    Author:  Ben

  ==============================================================================
*/

#include "LaunchpadModule.h"

LaunchpadModule::LaunchpadModule(const String & name) :
	MIDIModule(name,false) 
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			buttons.add(valuesCC.addBoolParameter(String(i + 1) + "-" + String(j + 1), "Button on the grid", false));
		}
	}

	for (int i = 0; i < 8; i++)
	{
		specials.add(valuesCC.addBoolParameter("Special " + String(i + 1), "Special button",false));
	}


	String lc[8]{ "A","B","C","D","E","F","G","H" };
	for(int i = 0; i < 8; i++)
	{
		letters.add(valuesCC.addBoolParameter(lc[i], "Letter button", false));
	}
}

LaunchpadModule::~LaunchpadModule()
{
	buttons.clear();
	specials.clear();
	letters.clear();
}

void LaunchpadModule::noteOnReceived(const int & channel, const int & pitch, const int & velocity)
{
	int column = pitch % 16;
	int row = floor(pitch / 16);
	if (column < 8)
	{
		buttons[row * 8 + column]->setValue(velocity > 0);
	} else if (column == 8)
	{
		letters[row]->setValue(velocity > 0);
	}

}

void LaunchpadModule::noteOffReceived(const int & channel, const int & pitch, const int & velocity)
{
	int row = floor(pitch / 16);
	int column = pitch % 16;
	if (column < 8)
	{
		buttons[row * 8 + column]->setValue(false);
	} else if (column == 8)
	{
		letters[row]->setValue(false);
	}
}

void LaunchpadModule::controlChangeReceived(const int & channel, const int & number, const int & value)
{
	if (number >= 104 && number <= 111)
	{
		specials[number - 104]->setValue(value > 0);
	}
}
