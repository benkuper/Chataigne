/*
  ==============================================================================

    MadMapperModule.h
    Created: 28 Feb 2019 10:33:28pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "generic/GenericOSCQueryModule.h"

class MadMapperModule :
	public GenericOSCQueryModule
{
public:
	MadMapperModule();
	~MadMapperModule();


	String getTypeString() const override { return "MadMapper";  }
	static MadMapperModule * create() { return new MadMapperModule(); }
};