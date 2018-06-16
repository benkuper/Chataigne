/*
  ==============================================================================

    MappingLayerPanel.h
    Created: 20 Nov 2016 3:07:42pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef MAPPINGLAYERPANEL_H_INCLUDED
#define MAPPINGLAYERPANEL_H_INCLUDED


#include "../../../ui/SequenceLayerPanel.h"
#include "../MappingLayer.h"

class MappingLayerPanel :
	public SequenceLayerPanel,
	public Mapping::AsyncListener
{
public:
	MappingLayerPanel(MappingLayer * layer);
	~MappingLayerPanel();

	MappingLayer * mappingLayer;
	ScopedPointer<EnumParameterUI> modeUI;

	ScopedPointer<BoolImageToggleUI> snapUI;
	ScopedPointer<FloatSliderUI> snapSensitivityUI;
	ScopedPointer<ParameterUI> mappingOutputUI;

	ScopedPointer<Inspector> automationInspector;

	void resizedInternalHeader(Rectangle<int> &r) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void newMessage(const Mapping::MappingEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayerPanel)
};




#endif  // MAPPINGLAYERPANEL_H_INCLUDED
