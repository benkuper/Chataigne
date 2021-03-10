/*
  ==============================================================================

    MultiplexModule.h
    Created: 19 Feb 2021 5:57:00pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MultiplexModule :
	public Module
{
public:
	MultiplexModule();
	~MultiplexModule();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiplexModule)
};