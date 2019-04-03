/*
  ==============================================================================

    LagFilter.h
    Created: 4 Feb 2017 5:39:47pm
    Author:  Ben

  ==============================================================================
*/

#ifndef LAGFILTER_H_INCLUDED
#define LAGFILTER_H_INCLUDED

#include "../../MappingFilter.h"

class LagFilter :
	public MappingFilter,
	public Timer
{
public:
	LagFilter(var params);
	~LagFilter();

	var tempVal;
	FloatParameter * frequency;

	void processInternal() override;

	void filterParamChanged(Parameter * p) override;

	static LagFilter * create(var params) { return new LagFilter(params); }
	String getTypeString() const override { return "FPS"; }


	// Inherited via Timer
	virtual void timerCallback() override;

};


#endif  // LAGFILTER_H_INCLUDED
