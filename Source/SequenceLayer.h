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

class SequenceLayer :
	public BaseItem,
	public Sequence::SequenceListener
{
public:
	SequenceLayer(const String &name = "New Layer");
	virtual ~SequenceLayer();

	Sequence * sequence;

	//UI
	IntParameter * uiHeight;

	//Methods
	void setSequence(Sequence * _sequence);

	virtual void init() {}; //to override after setSequence

	//Save / Load
	virtual var getJSONData() override;
	
	//Handlers
	void sequenceCurrentTimeChanged(Sequence * _sequence, bool evaluateSkippedData);


	//Factory
	virtual String getTypeString() const { jassert(false); return ""; } //should be overriden

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayer)
};




#endif  // SEQUENCELAYER_H_INCLUDED
