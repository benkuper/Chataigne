/*
  ==============================================================================

    ConversionConnector.h
    Created: 4 Mar 2020 12:28:30am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "../ConvertedParameter.h"

class ConversionConnector :
	public Component
{
public:
	ConversionConnector(StringRef label, Parameter* param, int valueIndex);
	ConversionConnector(StringRef label, ConvertedParameter * param, int valueIndex);
	~ConversionConnector();

	bool isSource;
	String label;
	WeakReference<Parameter> param;
	ConvertedParameter * convertedParam;
	int valueIndex;
	bool dropCandidate;


	void paint(Graphics& g) override;
	bool hitTest(int x, int y) override;

	Point<int> getConnectorCenter();
};