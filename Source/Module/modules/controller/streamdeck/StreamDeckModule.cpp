/*
  ==============================================================================

    StreamDeckModule.cpp
    Created: 22 Nov 2019 4:37:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "StreamDeckModule.h"
#include "hidapi.h"

StreamDeckModule::StreamDeckModule(const String& name) :
	Module(name),
	isConnected(nullptr),
	colorsCC("Colors"),
	imagesCC("Images"),
	device(nullptr)
{
	isConnected = moduleParams.addBoolParameter("Is Connected", "Is a streamdeck connected ?", false);
	isConnected->setControllableFeedbackOnly(true);

	moduleParams.addChildControllableContainer(&colorsCC);
	moduleParams.addChildControllableContainer(&imagesCC);
	colorsCC.editorIsCollapsed = true;
	imagesCC.editorIsCollapsed = true;

	brightness = moduleParams.addFloatParameter("Brightness", "Sets the brigthness of the deck's backlight", .75f, 0, 1);
	
	setDevice(StreamDeckManager::getInstance()->devices.size() > 0 ? StreamDeckManager::getInstance()->devices[0] : nullptr);
	StreamDeckManager::getInstance()->addAsyncManagerListener(this);

}

StreamDeckModule::~StreamDeckModule()
{
	if(StreamDeckManager::getInstanceWithoutCreating() != nullptr) StreamDeckManager::getInstance()->addAsyncManagerListener(this);
}

void StreamDeckModule::rebuildValues()
{
	if (device != nullptr && device->numButtons != buttonStates.size())
	{
		valuesCC.clear();
		colorsCC.clear();
		imagesCC.clear();
		buttonStates.clear();
		colors.clear();
		images.clear();

		if (device != nullptr)
		{
			for (int i = 0; i < device->numButtons; i++)
			{
				buttonStates.add(valuesCC.addBoolParameter("Button " + String(i + 1), "Is button " + String(i + 1) + " pressed ?", false));
				colors.add(colorsCC.addColorParameter("Image " + String(i + 1), "If not image, color for button " + String(i + 1), Colours::black));
				images.add(imagesCC.addFileParameter("Image " + String(i + 1), "Image for button" + String(i + 1) + " (overrides color)"));
			}
		}
	}
}

void StreamDeckModule::setDevice(StreamDeck* newDevice)
{
	if (newDevice == device) return;
	if (device != nullptr)
	{
		device->removeStreamDeckListener(this);
	}

	device = newDevice;

	if (device != nullptr)
	{
		device->addStreamDeckListener(this);
	}

	rebuildValues();
	isConnected->setValue(device != nullptr);

}

void StreamDeckModule::streamDeckButtonPressed(int button)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Button " + String(button + 1) + " pressed");
	}

	inActivityTrigger->trigger();
	buttonStates[button]->setValue(true);
}

void StreamDeckModule::streamDeckButtonReleased(int button)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Button " + String(button + 1) + " released");
	}

	inActivityTrigger->trigger();
	buttonStates[button]->setValue(false);
}

void StreamDeckModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (!enabled->boolValue()) return;
	if (device == nullptr) return;

	if (c == brightness)
	{
		device->setBrightness(brightness->floatValue());
	}
}


void StreamDeckModule::newMessage(const StreamDeckManager::StreamDeckManagerEvent& e)
{
	setDevice(StreamDeckManager::getInstance()->devices.size() > 0 ? StreamDeckManager::getInstance()->devices[0] : nullptr);
}