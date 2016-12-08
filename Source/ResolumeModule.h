/*
  ==============================================================================

    ResolumeModule.h
    Created: 29 Oct 2016 7:21:01pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef RESOLUMEMODULE_H_INCLUDED
#define RESOLUMEMODULE_H_INCLUDED

#include "OSCModule.h"

class ResolumeModule :
	public OSCModule
{
public:
	ResolumeModule();
	~ResolumeModule() {}

	static ResolumeModule * create() { return new ResolumeModule(); }
	virtual String getTypeString() const override { return "Resolume"; }
};




#endif  // RESOLUMEMODULE_H_INCLUDED
