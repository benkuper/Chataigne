/*
  ==============================================================================

    SequenceLayer.h
    Created: 28 Oct 2016 8:15:05pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCELAYER_H_INCLUDED
#define SEQUENCELAYER_H_INCLUDED


#include "BaseItem.h"
#include "Sequence.h"
#include "ColorParameter.h"

class SequenceLayerPanel;
class SequenceLayerTimeline;

class SequenceLayer :
	public BaseItem,
	public Sequence::SequenceListener
{
public:
	SequenceLayer(Sequence * _sequence = nullptr, const String &name = "New Layer");
	virtual ~SequenceLayer();

	Sequence * sequence;

	//UI
	IntParameter * uiHeight;
	ColorParameter * color;

	//Save / Load
	virtual var getJSONData() override;

	//Handlers

	//Factory
	virtual String getTypeString() const { jassert(false); return ""; } //should be overriden

	//UI
	virtual SequenceLayerPanel * getPanel();
	virtual SequenceLayerTimeline * getTimelineUI();

	InspectableEditor * getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayer)
};




#endif  // SEQUENCELAYER_H_INCLUDED
