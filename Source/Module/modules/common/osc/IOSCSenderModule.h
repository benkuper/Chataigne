/*
  ==============================================================================

	IOSCSenderModule.h
	Created: 19 Apr 2022 1:50:39pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class IOSCSenderModule
{
public:
	virtual ~IOSCSenderModule() {}


	virtual void sendOSC(const OSCMessage& m) = 0;
	virtual OSCHelpers::ColorMode getColorMode() { return OSCHelpers::ColorMode::ColorRGBA; }
	virtual OSCHelpers::BoolMode getBoolMode() { return OSCHelpers::BoolMode::Int; }
};