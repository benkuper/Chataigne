/*
  ==============================================================================

    AboutWindow.h
    Created: 4 Jan 2018 7:25:13pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class AboutWindow :
	public Component
{
public:
	AboutWindow();
	~AboutWindow();

	Image aboutImage;

	void paint(Graphics &g) override;
};