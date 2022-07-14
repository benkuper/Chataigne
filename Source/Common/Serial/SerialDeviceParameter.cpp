/*
  ==============================================================================

	SerialDeviceParameter.cpp
	Created: 15 Mar 2017 10:14:56am
	Author:  Ben-Portable

  ==============================================================================
*/

SerialDeviceParameter::SerialDeviceParameter(const String& name, const String& description, bool enabled) :
	EnumParameter(name, description, enabled),
	currentDevice(nullptr),
	openBaudRate(9600)
{
	SerialManager::getInstance()->addSerialManagerListener(this);
	updatePortList();
}

SerialDeviceParameter::~SerialDeviceParameter()
{
	if (SerialManager::getInstanceWithoutCreating()) SerialManager::getInstance()->removeSerialManagerListener(this);
}

SerialDevice* SerialDeviceParameter::getDevice()
{
	return currentDevice;
}

void SerialDeviceParameter::setValueInternal(var& v)
{
	EnumParameter::setValueInternal(v);
	var data = getValueData();
	if (data.isVoid()) currentDevice = nullptr;
	else currentDevice = SerialManager::getInstance()->getPort(data.getProperty("deviceID", "").toString(), data.getProperty("port", "").toString(), true, openBaudRate);
	//DBG("current device from setValueInternal : " << (int)currentDevice);
}

void SerialDeviceParameter::updatePortList()
{
	//DBG("num ports :" << SerialManager::getInstance()->portInfos.size());

	String s;
	if (currentDevice != nullptr)
	{
		s = currentDevice->info->uniqueDescription;
	}

	//clearOptions();
	enumValues.clear();

	if (SerialManager::getInstance()->portInfos.size() > 0) addOption("Not connected or disconnected", var(), false);
	for (auto& p : SerialManager::getInstance()->portInfos)
	{
		if (!vidFilters.isEmpty() && !vidFilters.contains(p->vid)) continue;
		if (!pidFilters.isEmpty() && !pidFilters.contains(p->pid)) continue;

		var v(new DynamicObject());
		//DBG("Add option : " << p->port << ":" << p->hardwareID);
		v.getDynamicObject()->setProperty("port", p->port);
		v.getDynamicObject()->setProperty("deviceID", p->deviceID);
		String desc = p->uniqueDescription;
		addOption(desc, v, false);
	}

	if (s.isNotEmpty()) setValueWithKey(s);

}


void SerialDeviceParameter::portAdded(SerialDeviceInfo* s)
{
	//DBG("param : port added");
	updatePortList();
	if (getValueKey() == "" && ghostData == s->uniqueDescription)
	{
		setValueWithKey(ghostData);
		ghostData = "";
	}
}

void SerialDeviceParameter::portRemoved(SerialDeviceInfo* s)
{
	//DBG("param : port removed !");
	if (s->uniqueDescription == getValueKey())
	{
		ghostData = s->uniqueDescription;
		setValue("");
	}

	updatePortList();
}
