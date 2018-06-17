/*
  ==============================================================================

    ModuleFactory.cpp
    Created: 8 Dec 2016 2:36:06pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleFactory.h"

#include "modules/osc/custom/CustomOSCModule.h"
#include "modules/osc/resolume/ResolumeModule.h"
#include "modules/osc/millumin/MilluminModule.h"
#include "modules/midi/MIDIModule.h"
#include "modules/controller/hid/HIDModule.h"
#include "modules/controller/gamepad/GamepadModule.h"
#include "modules/controller/joystick/JoystickModule.h"
#include "modules/controller/wiimote/WiimoteModule.h"
#include "modules/controller/keyboard/KeyboardModule.h"
#include "modules/osc/reaper/ReaperModule.h"
#include "modules/audio/AudioModule.h"
#include "modules/controller/kinect/KinectV2Module.h"
#include "modules/serial/SerialModule.h"
#include "modules/dmx/DMXModule.h"
#include "modules/osc/live/LiveOSCModule.h"
#include "modules/controller/myo/MyoModule.h"
#include "modules/midi/Launchpad/LaunchpadModule.h"
#include "modules/osc/dlight/DLightModule.h"
#include "modules/tcp/TCPModule.h"
#include "modules/tcp/vlc/VLCModule.h"
#include "modules/generators/metronome/MetronomeModule.h"
#include "modules/generators/signal/SignalModule.h"
#include "modules/udp/UDPModule.h"
#include "modules/tcp/watchout/WatchoutModule.h"
#include "modules/system/time/TimeModule.h"
#include "modules/system//os/OSModule.h"
#include "modules/tcp/pjlink/PJLinkModule.h"
#include "modules/controller/joycon/JoyConModule.h"

juce_ImplementSingleton(ModuleFactory)

ModuleFactory::ModuleFactory() {
	moduleDefs.add(new ModuleDefinition("Protocol", "OSC", &CustomOSCModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "MIDI", &MIDIModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "DMX", &DMXModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "Serial", &SerialModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "UDP", &UDPModule::create));  
	moduleDefs.add(new ModuleDefinition("Protocol", "TCP Client", &TCPModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "PJLink", &PJLinkModule::create));

	
#if JUCE_WINDOWS
	//moduleDefs.add(new ModuleDefinition("Controller", "Myo", &MyoModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "KinectV2", &KinectV2Module::create));
#endif

	moduleDefs.add(new ModuleDefinition("Hardware", "Launchpad", &LaunchpadModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "Sound Card", &AudioModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "Wiimote", &WiimoteModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "JoyCon", &JoyConModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "Keyboard", &KeyboardModule::create));

	/*
	moduleDefs.add(new ModuleDefinition("Hardware", "HID", &HIDModule::create));
	*/
	moduleDefs.add(new ModuleDefinition("Hardware", "Gamepad", &GamepadModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "Joystick", &JoystickModule::create));
	

	moduleDefs.add(new ModuleDefinition("Software", "DLight", &DLightModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "LiveOSC2", &LiveOSCModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "Millumin", &MilluminModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "Reaper", &ReaperModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "Resolume", &ResolumeModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "VLC", &VLCModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "Watchout", &WatchoutModule::create));

	moduleDefs.add(new ModuleDefinition("Generator", "Metronome", &MetronomeModule::create));
	moduleDefs.add(new ModuleDefinition("Generator", "Signal", &SignalModule::create));
	
	moduleDefs.add(new ModuleDefinition("System", "Time", &TimeModule::create));
	moduleDefs.add(new ModuleDefinition("System", "OS", &OSModule::create));

	addCustomModules();
	buildPopupMenu();
}

void ModuleFactory::addCustomModules()
{
	File modulesFolder = File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getChildFile("Chataigne/modules");
	modulesFolder.createDirectory();

	Array<File> modulesList;
	modulesFolder.findChildFiles(modulesList, File::findDirectories, false);
	for (auto &m : modulesList)
	{
		File moduleFile = m.getChildFile("module.json");
		if (!moduleFile.existsAsFile())
		{
			LOGWARNING("Folder " << m.getFileName() << "does not contain a module.json file, skipping");
			continue;
		}

		var moduleData = JSON::parse(moduleFile);

		if (!moduleData.isObject())
		{
			LOGWARNING("Module " << m.getFileName() << " has an invalid module.json file, skipping");
			continue;
		}

		moduleData.getDynamicObject()->setProperty("modulePath", m.getFullPathName());
		
		String moduleName = moduleData.getProperty("name", "");
		String moduleType = moduleData.getProperty("type", "");
		String moduleMenuPath = moduleData.getProperty("path", "");

		if (moduleName.isNotEmpty() && moduleType.isNotEmpty())
		{
			std::function<Module*()> createFunc;
			if (moduleType == "Serial") createFunc = &SerialModule::create;
			else if (moduleType == "OSC") createFunc = &CustomOSCModule::create;
			else if (moduleType == "MIDI") createFunc = &MIDIModule::create;
			else if (moduleType == "UDP") createFunc = &UDPModule::create;
			else continue;

			LOG("Found custom module : " << moduleMenuPath << ":" << moduleName);
			ModuleDefinition * def = moduleDefs.add(new ModuleDefinition(moduleMenuPath,moduleName, createFunc));
			def->jsonData = moduleData;

		}
		
	}
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

Module * ModuleFactory::showCreateMenu()
{
	int result = getInstance()->menu.show();
	if (result == 0) return nullptr;

	ModuleDefinition * d = getInstance()->moduleDefs[result - 1];//result 0 is no result
	Module * m = d->createFunc();
	if (!d->jsonData.isVoid())
	{
		m->setupModuleFromJSONData(d->jsonData);
	}

	return m;
}

Module * ModuleFactory::createModule(const String & moduleType)
{
	for (auto &d : getInstance()->moduleDefs)
	{
		if (d->moduleType == moduleType)
		{
			Module * m = d->createFunc();
			if (!d->jsonData.isVoid()) m->setupModuleFromJSONData(d->jsonData);
			return m;
		}
	}
	return nullptr;
}
