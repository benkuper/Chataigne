/*
  ==============================================================================

    MappingLayerPanel.h
    Created: 20 Nov 2016 3:07:42pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "../MappingLayer.h"
#include "Common/Processor/Mapping/Mapping.h"

class MappingLayerPanel :
	public SequenceLayerPanel,
	public Mapping::AsyncListener
{
public:
	MappingLayerPanel(MappingLayer * layer);
	~MappingLayerPanel();

	MappingLayer * mappingLayer;
	std::unique_ptr<EnumParameterUI> modeUI;

	std::unique_ptr<BoolImageToggleUI> snapUI;
	std::unique_ptr<FloatSliderUI> snapSensitivityUI;
	std::unique_ptr<ParameterUI> mappingOutputUI;

	std::unique_ptr<Inspector> automationInspector;

	void resizedInternalHeader(Rectangle<int> &r) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void newMessage(const Mapping::MappingEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayerPanel)
};