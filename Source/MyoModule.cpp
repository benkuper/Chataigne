/*
  ==============================================================================

    MyoModule.cpp
    Created: 10 May 2017 11:33:35pm
    Author:  Ben

  ==============================================================================
*/

#include "MyoModule.h"

MyoModule::MyoModule() :
	Module("Myo"),
	device(nullptr)
{
	MyoManager::getInstance()->addListener(this);

	yaw = valuesCC.addFloatParameter("Yaw", "Armband's yaw", 0, 0, 1);
	pitch = valuesCC.addFloatParameter("Pitch", "Armband's pitch", 0, -1, 1);
	roll = valuesCC.addFloatParameter("Roll", "Armband's roll", 0, -1, 1);
	pose = valuesCC.addEnumParameter("Pose","Detected pose");
}

MyoModule::~MyoModule()
{
	if (MyoManager::getInstanceWithoutCreating()) MyoManager::getInstance()->removeListener(this);
}

void MyoModule::setCurrentDevice(MyoDevice * d)
{
	if (device != nullptr)
		device->removeListener(this);

	device = d;

	if (device != nullptr)
		device->addListener(this);
}

void MyoModule::myoAdded(MyoDevice * d)
{
}

void MyoModule::myoRemoved(MyoDevice * d)
{
}

void MyoModule::myoOrientationUpdate(MyoDevice * d)
{
}

void MyoModule::myoPoseUpdate(MyoDevice * d)
{
}

void MyoModule::myoEMGUpdate(MyoDevice * d)
{
}
