/*
  ==============================================================================

    SerialManager.cpp
    Created: 15 Mar 2017 10:14:26am
    Author:  Ben-Portable

  ==============================================================================
*/

#include "SerialManager.h"
juce_ImplementSingleton(SerialManager)

SerialManager::SerialManager()
{
	updateDeviceList();
	startTimer(1000); 
}

SerialManager::~SerialManager()
{
	stopTimer();
}

void SerialManager::updateDeviceList()
{
#if SERIALSUPPORT
	std::vector<serial::PortInfo> devices_found = serial::list_ports();
	std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

	OwnedArray<SerialDeviceInfo> newInfos;

	Array<SerialDeviceInfo *> portsToNotifyAdded;
	Array<SerialDeviceInfo *> portsToNotifyRemoved;

	while (iter != devices_found.end())
	{
		serial::PortInfo device = *iter++;
		SerialDeviceInfo * info = new SerialDeviceInfo(device.port, device.description, device.hardware_id);

        if(info->pid ==  0 && info->vid == 0) continue;

		newInfos.add(info);
	}

	//check added devices
	for (auto &newD : newInfos)
	{
		bool found = false;
		for (auto &sourceD : portInfos)
		{
			if (sourceD->hardwareID == newD->hardwareID && sourceD->port == newD->port)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			portsToNotifyAdded.add(newD);
		}
	}

	//check removed
	for (auto &sourceD : portInfos)
	{
		bool found = false;
		for (auto &newD : newInfos)
		{
			if (sourceD->hardwareID == newD->hardwareID && sourceD->port == newD->port)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			//create one so portInfos can delete the original
			portsToNotifyRemoved.add(sourceD);
		}
	}

	//check removed devices


	for (auto &p : portsToNotifyRemoved)
	{
		portInfos.removeObject(p, false);
		NLOG("SerialManager", "Port Removed : \n" + p->port);
		listeners.call(&SerialManagerListener::portRemoved, p);

		SerialDevice * port = getPort(p, false);
		if (port != nullptr) removePort(port);
		delete p;
	}

	for (auto &p : portsToNotifyAdded)
	{

		newInfos.removeObject(p, false);
		portInfos.add(p);
		NLOG("SerialManager", "Port Added : \n" + p->port + ", " + p->description + "\nHardwareID : " + p->hardwareID + "\nPID / VID : 0x" + String::toHexString(p->pid) + " / 0x" + String::toHexString(p->vid));
		listeners.call(&SerialManagerListener::portAdded, p);

	}
#endif
}

SerialDevice * SerialManager::getPort(SerialDeviceInfo * portInfo, bool createIfNotThere, int openBaudRate)
{
#if SERIALSUPPORT
	for (auto & sp : openedPorts)
	{

		if (sp->info->hardwareID == portInfo->hardwareID && sp->info->port == portInfo->port) return sp;
	}

	if (createIfNotThere)
	{
		try
		{
			Serial * newSerial = new Serial(portInfo->port.toStdString(), openBaudRate, serial::Timeout::simpleTimeout(1000));
			SerialDevice *p = new SerialDevice(newSerial, portInfo);
			openedPorts.add(p);
			return p;
		} catch (std::exception &e)
		{
			LOGERROR("Error trying to open port " << portInfo->port.toStdString() << "\n"+String(e.what()));
			return nullptr;
		}
	}
#endif

	return nullptr;
}

SerialDevice * SerialManager::getPort(String deviceID, String portName, bool createIfNotThere, int openBaudRate)
{
#if SERIALSUPPORT

	for (auto & pi : portInfos)
	{
		
		if (pi->deviceID == deviceID & pi->port == portName)
		{
			return getPort(pi, createIfNotThere,openBaudRate);
		}
	}
#endif

	return nullptr;
}

void SerialManager::removePort(SerialDevice * p)
{
	openedPorts.removeObject(p, true);
}

void SerialManager::timerCallback()
{
	updateDeviceList();
}