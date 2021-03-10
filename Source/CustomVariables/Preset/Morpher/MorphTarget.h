/*
  ==============================================================================

    MorphTarget.h
    Created: 11 Jun 2019 2:11:04pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MorphTarget :
	public BaseItem
{
public:
	MorphTarget(StringRef name = "MorphTarget");
	virtual ~MorphTarget();

	FloatParameter* weight;
	//Point2DParameter* position;
	FloatParameter* attraction;
	ColorParameter* targetColor;

	String getTypeString() const override { return "MorphTarget"; }

};