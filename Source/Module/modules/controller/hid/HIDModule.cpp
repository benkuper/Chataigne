/*
  ==============================================================================

    HIDModule.cpp
    Created: 26 Dec 2016 2:10:14pm
    Author:  Ben

  ==============================================================================
*/

#include "HIDModule.h"

HIDModule::HIDModule(const String & name) :
	Module(name)
{
	setupIOConfiguration(true, false);

	//startTimerHz(1000);

	
	/*
	std::vector<HID::DeviceDescription> descriptions = HID::enumerateAllDevices();
	Array<HID::DeviceDescription>  descArray(&descriptions[0],descriptions.size());
	for (auto &d : descArray)
	{
		if (d.productId == 0x306 && d.vendorId == 0x057e)
		{
			DBG("Wiimote found !");
			device = new HID(d.path);
			device->readBlocking(device->)
			break;
		}
	}
	*/
}

HIDModule::~HIDModule()
{
}

void HIDModule::checkDevices()
{
	DBG("Check devices HID");
	/*
	std::vector<HID::DeviceDescription> descriptions = HID::enumerateAllDevices();
	Array<HID::DeviceDescription>  descArray(&descriptions[0],descriptions.size());
	for (auto &d : descArray)
	{
		DBG(String::toHexString(d.productId) << " / " << String::toHexString(d.vendorId) << " / "<<d.productString);
	}
	*/
}

void HIDModule::timerCallback()
{
	checkDevices();
}
