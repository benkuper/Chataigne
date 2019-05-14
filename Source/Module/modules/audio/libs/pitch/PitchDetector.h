/*
  ==============================================================================

    PitchDetector.h
    Created: 14 May 2019 10:29:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class PitchDetector
{
public:
	PitchDetector() {}
	virtual ~PitchDetector() {}

	virtual float getPitch(const float * samples) = 0;
	virtual unsigned int getBufferSize() = 0;
	virtual void setBufferSize(int value) = 0;
};