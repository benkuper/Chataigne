/*
  ==============================================================================

    ConversionSourceParameterUI.h
    Created: 4 Mar 2020 12:04:24am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConversionSourceParameterUI :
	public InspectableContentComponent
{
public:
	ConversionSourceParameterUI(Parameter* source);
	~ConversionSourceParameterUI();

	WeakReference<Parameter> source;
	std::unique_ptr<ControllableUI> sourceUI;

	OwnedArray<ConversionConnector> connectors;

	void mouseDown(const MouseEvent &e) override; //avoid default behaviour
	
	void paint(Graphics&) override;
	void resized() override;
};