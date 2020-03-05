/*
  ==============================================================================

    ConversionSourceParameterUI.h
    Created: 4 Mar 2020 12:04:24am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "ConversionConnector.h"

class ConversionSourceParameterUI :
	public InspectableContentComponent
{
public:
	ConversionSourceParameterUI(Parameter* source);
	~ConversionSourceParameterUI();

	WeakReference<Parameter> source;
	std::unique_ptr<ControllableUI> sourceUI;

	OwnedArray<ConversionConnector> connectors;
	
	void paint(Graphics&) override;
	void resized() override;
};