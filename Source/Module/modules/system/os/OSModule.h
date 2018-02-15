/*
  ==============================================================================

    OSModule.h
    Created: 5 Jan 2018 3:41:34pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"

class OSModule :
	public Module
{
public:
	OSModule();
	~OSModule();
	
	enum OSType { OS_WIN, OS_MAC, OS_LINUX };

	EnumParameter * osType;
	StringParameter * osName;

	virtual String getDefaultTypeString() const override { return "OS"; }
	static OSModule * create() { return new OSModule(); }

};