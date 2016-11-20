/*
  ==============================================================================

    MappingLayer.h
    Created: 17 Nov 2016 8:00:02pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef MAPPINGLAYER_H_INCLUDED
#define MAPPINGLAYER_H_INCLUDED

#include "SequenceLayer.h"

class MappingLayer :
	public SequenceLayer
{
public:
	MappingLayer();
	~MappingLayer();


	void init() override;

	void loadJSONDataInternal(var data) override;


	static MappingLayer * create() { return new MappingLayer(); }
	virtual String getTypeString() const override { return "Mapping"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayer)
};




#endif  // MAPPINGLAYER_H_INCLUDED
