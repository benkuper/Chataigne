/*
  ==============================================================================

    WiimotePlugin.h
    Created: 27 Dec 2016 2:44:29pm
    Author:  Ben

  ==============================================================================
*/

#ifndef WIIMOTEPLUGIN_H_INCLUDED
#define WIIMOTEMPLUGIN_H_INCLUDED

#include "JuceHeader.h"
#include "../../Source/ModuleFactory.h"

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)
#endif

#ifdef __APPLE__
#endif

var getPluginInfos()
{
	var d(new DynamicObject());
	d.getDynamicObject()->setProperty("type", "module");
	d.getDynamicObject()->setProperty("name", "Wiimote Plugin");
	d.getDynamicObject()->setProperty("version", "1.0.0");
	d.getDynamicObject()->setProperty("description", "Module that allows connecting to the wiimote");
	return d;
}



class WiimotePlugin :
	public Module
{
public:

	static WiimotePlugin * create() { return new WiimotePlugin(); }
	virtual String getTypeString() const override { return "WiimotePlugin"; }
};

ModuleDefinition * getCreateFunc()
{
	return new ModuleDefinition("Plugin", "Wiimote", &WiimotePlugin::create);
}



#endif  // WIIMOTEMODULE_H_INCLUDED
