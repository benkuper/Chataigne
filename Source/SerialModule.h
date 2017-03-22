/*
  ==============================================================================

    SerialModule.h
    Created: 22 Mar 2017 4:15:10pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SERIALMODULE_H_INCLUDED
#define SERIALMODULE_H_INCLUDED

#include "Module.h"

class SerialModule :
	public Module
{
public:
	SerialModule(const String &name = "Serial");
	~SerialModule() {}


	static SerialModule * create() { return new SerialModule(); }
	virtual String getTypeString() const override { return "Serial"; }
};


#endif  // SERIALMODULE_H_INCLUDED
