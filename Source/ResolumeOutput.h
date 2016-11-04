/*
  ==============================================================================

    ResolumeOutput.h
    Created: 29 Oct 2016 7:21:01pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef RESOLUMEOUTPUT_H_INCLUDED
#define RESOLUMEOUTPUT_H_INCLUDED

#include "OSCOutput.h"

class ResolumeOutput :
	public OSCOutput
{
public:
	ResolumeOutput();
	~ResolumeOutput() {}

	static ResolumeOutput * create() { return new ResolumeOutput(); }
	virtual String getTypeString() const override { return "Resolume"; }
};




#endif  // RESOLUMEOUTPUT_H_INCLUDED
