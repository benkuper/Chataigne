/*
  ==============================================================================

    ConversionParamValueLinkUI.h
    Created: 4 Mar 2020 12:04:56am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConversionParamValueLinkUI :
	public Component
{
public:
	ConversionParamValueLinkUI(ConversionConnector* sourceConnector = nullptr, ConversionConnector* outConnector = nullptr, ConversionParamValueLink* link = nullptr);
	~ConversionParamValueLinkUI();

	ConversionParamValueLink* link;
	WeakReference<ConversionConnector> sourceConnector;
	WeakReference<ConversionConnector> outConnector;

	Path linkPath;
	Path hitPath;

	
	void setSourceConnector(ConversionConnector* c);
	void setOutConnector(ConversionConnector* c);

	void mouseDown(const MouseEvent &e) override;

	void buildHitPath();

	void paint(Graphics& g) override;
	bool hitTest(int x, int y) override;
};