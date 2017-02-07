/*
  ==============================================================================

    ModuleFactory.cpp
    Created: 8 Dec 2016 2:36:06pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleFactory.h"

#include "CustomOSCModule.h"
#include "ResolumeModule.h"
#include "MIDIModule.h"
#include "HIDModule.h"
#include "GamepadModule.h"
#include "WiimoteModule.h"
#include "ReaperModule.h"
#include "AudioModule.h"

juce_ImplementSingleton(ModuleFactory)

ModuleFactory::ModuleFactory() {
	moduleDefs.add(new ModuleDefinition("Generic", "OSC", &CustomOSCModule::create));
	moduleDefs.add(new ModuleDefinition("Generic", "MIDI", &MIDIModule::create));
	moduleDefs.add(new ModuleDefinition("Generic", "HID", &HIDModule::create));
	moduleDefs.add(new ModuleDefinition("Generic", "Gamepad", &GamepadModule::create));

	moduleDefs.add(new ModuleDefinition("Controller", "Wiimote", &WiimoteModule::create));
	
	moduleDefs.add(new ModuleDefinition("Audio", "Audio Device", &AudioModule::create));
	moduleDefs.add(new ModuleDefinition("Audio", "Reaper", &ReaperModule::create));

	moduleDefs.add(new ModuleDefinition("Video", "Resolume", &ResolumeModule::create));

	buildPopupMenu();
}

void ModuleFactory::buildPopupMenu()
{
	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;

	for (auto &d : moduleDefs)
	{
		int itemID = moduleDefs.indexOf(d) + 1;//start at 1 for menu

		if (d->menuPath.isEmpty())
		{
			menu.addItem(itemID, d->moduleType);
			continue;
		}

		int subMenuIndex = -1;

		for (int i = 0; i < subMenus.size(); i++)
		{
			if (subMenuNames[i] == d->menuPath)
			{
				subMenuIndex = i;
				break;
			}
		}
		if (subMenuIndex == -1)
		{
			subMenuNames.add(d->menuPath);
			subMenus.add(new PopupMenu());
			subMenuIndex = subMenus.size() - 1;
		}

		subMenus[subMenuIndex]->addItem(itemID, d->moduleType);
	}

	for (int i = 0; i < subMenus.size(); i++) menu.addSubMenu(subMenuNames[i], *subMenus[i]);
}
