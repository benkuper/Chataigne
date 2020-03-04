/*
  ==============================================================================

    ConversionConnector.h
    Created: 4 Mar 2020 12:28:30am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ConversionConnector :
	public Component
{
public:
	ConversionConnector(StringRef label, int index, bool labelOnLeft);
	~ConversionConnector();

	String label;
	int index;
	bool labelOnLeft;

	void paint(Graphics& g) override;
	bool hitTest(int x, int y) override;
};