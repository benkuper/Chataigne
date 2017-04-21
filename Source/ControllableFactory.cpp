/*
  ==============================================================================

    ControllableFactory.cpp
    Created: 2 Nov 2016 1:44:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "ControllableFactory.h"


juce_ImplementSingleton(ControllableFactory)

ControllableFactory::ControllableFactory() {
	controllableDefs.add(new ControllableDefinition("Trigger", &Trigger::create));
	controllableDefs.add(new ControllableDefinition("Boolean", &BoolParameter::create));
	controllableDefs.add(new ControllableDefinition("Float", &FloatParameter::create));
	controllableDefs.add(new ControllableDefinition("Integer", &IntParameter::create));
	controllableDefs.add(new ControllableDefinition("Enum", &EnumParameter::create));
	controllableDefs.add(new ControllableDefinition("String", &StringParameter::create));
	controllableDefs.add(new ControllableDefinition("Point2D", &Point2DParameter::create));
	controllableDefs.add(new ControllableDefinition("Point3D", &Point3DParameter::create));
	controllableDefs.add(new ControllableDefinition("Target", &TargetParameter::create));
	controllableDefs.add(new ControllableDefinition("Color", &ColorParameter::create));
	buildPopupMenu();
}

inline void ControllableFactory::buildPopupMenu()
{
	for (int i = 0; i < controllableDefs.size(); i++)
	{
		menu.addItem(i + 1, controllableDefs[i]->controllableType);
	}
}
