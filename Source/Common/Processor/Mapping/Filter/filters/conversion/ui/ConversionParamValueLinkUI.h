/*
  ==============================================================================

    ConversionParamValueLinkUI.h
    Created: 4 Mar 2020 12:04:56am
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../ConversionFilter.h"
#include "ConversionConnector.h"

class ConversionParamValueLinkUI :
	public Component
{
public:
	ConversionParamValueLinkUI(ConversionConnector* sourceConnector = nullptr, ConversionConnector* outConnector = nullptr, ConversionFilter::ParamValueLink* link = nullptr);
	~ConversionParamValueLinkUI();

	ConversionFilter::ParamValueLink* link;
	ConversionConnector* sourceConnector;
	ConversionConnector* outConnector;

	Path linkPath;
	Path hitPath;

	
	void setSourceConnector(ConversionConnector* c);
	void setOutConnector(ConversionConnector* c);

	void buildHitPath();

	void paint(Graphics& g) override;
	bool hitTest(int x, int y) override;
};