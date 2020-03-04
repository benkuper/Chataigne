/*
  ==============================================================================

    ConversionParamValueLinkUI.h
    Created: 4 Mar 2020 12:04:56am
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../ConversionFilter.h"

class ConversionParamValueLinkUI :
	public Component
{
public:
	ConversionParamValueLinkUI(ConversionFilter::ParamValueLink* link);
	~ConversionParamValueLinkUI();

	ConversionFilter::ParamValueLink* link;

	Component* sourceConnector;
	Component* outConnector;

	void paint(Graphics& g) override;
};