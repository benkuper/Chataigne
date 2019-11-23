/*
  ==============================================================================

    StreamDeckModule.cpp
    Created: 22 Nov 2019 4:37:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "StreamDeckModule.h"
#include "hidapi.h"
#include "commands/StreamDeckCommand.h"

StreamDeckModule::StreamDeckModule(const String& name) :
	Module(name),
	isConnected(nullptr),
	colorsCC("Colors"),
	imagesCC("Images"),
	device(nullptr)
{
	setupIOConfiguration(true, true);
	
	isConnected = moduleParams.addBoolParameter("Is Connected", "Is a streamdeck connected ?", false);
	isConnected->setControllableFeedbackOnly(true);

	moduleParams.addChildControllableContainer(&colorsCC);
	moduleParams.addChildControllableContainer(&imagesCC);
	colorsCC.editorIsCollapsed = true;
	imagesCC.editorIsCollapsed = true;

	brightness = moduleParams.addFloatParameter("Brightness", "Sets the brigthness of the deck's backlight", .75f, 0, 1);
	
	reset = moduleParams.addTrigger("Reset", "Resets the stream deck");
	colorizeImages = moduleParams.addBoolParameter("Colorize images", "If checked, this will use both colors and images to set buttons", true);
	highlightPressedButtons = moduleParams.addBoolParameter("Highlight pressed buttons", "If checked, this will automatically highlight the buttons when pressed", true);

	defManager->add(CommandDefinition::createDef(this, "", "Set Color", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_COLOR));
	defManager->add(CommandDefinition::createDef(this, "", "Set Image", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_IMAGE));
	defManager->add(CommandDefinition::createDef(this, "", "Set All", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_ALL_COLOR));
	defManager->add(CommandDefinition::createDef(this, "", "Set Brightness", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_BRIGHTNESS));

	setDevice(StreamDeckManager::getInstance()->devices.size() > 0 ? StreamDeckManager::getInstance()->devices[0] : nullptr);
	StreamDeckManager::getInstance()->addAsyncManagerListener(this);

}

StreamDeckModule::~StreamDeckModule()
{
	if(StreamDeckManager::getInstanceWithoutCreating() != nullptr) StreamDeckManager::getInstance()->addAsyncManagerListener(this);
	setDevice(nullptr);
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

	if (device != nullptr)
	{
		device->setBrightness(brightness->floatValue());

		for (int i = 0; i < device->numButtons; i++) updateButton(i);
	}

	isConnected->setValue(device != nullptr);

}

void StreamDeckModule::updateButton(int id)
{
	if (device == nullptr) return;
	File f = images[id]->getFile();

	if (f.existsAsFile())
	{
		Image image = ImageCache::getFromFile(f);
		if (logOutgoingData->boolValue()) NLOG(niceName, "Set image " << f.getFileName() << " to button " << (id + 1));
		if (colorizeImages->boolValue()) device->setImage(id, image, colors[id]->getColor(), highlightPressedButtons->boolValue()?buttonStates[id]->boolValue() :false);
		else device->setImage(id, image, highlightPressedButtons->boolValue() ? buttonStates[id]->boolValue() : false);
	}
	else
	{
		if (logOutgoingData->boolValue()) NLOG(niceName, "Set color " << colors[id]->getColor().toString() << " to button " << (id + 1));
		device->setColor(id, colors[id]->getColor(), highlightPressedButtons->boolValue() ? buttonStates[id]->boolValue() : false);
	}

	outActivityTrigger->trigger();
}

void StreamDeckModule::streamDeckButtonPressed(int button)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Button " + String(button + 1) + " pressed");
	}


	inActivityTrigger->trigger();
	buttonStates[button]->setValue(true);

	if (highlightPressedButtons) updateButton(button);
}

void StreamDeckModule::streamDeckButtonReleased(int button)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Button " + String(button + 1) + " released");
	}

	inActivityTrigger->trigger();
	buttonStates[button]->setValue(false);
	if (highlightPressedButtons) updateButton(button);
}

void StreamDeckModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (!enabled->boolValue()) return;
	if (device == nullptr) return;

	if (c == reset)
	{
		device->reset();
		for (int i = 0; i < device->numButtons; i++) updateButton(i);
	}
	else if (c == colorizeImages)
	{
		for (int i = 0; i < device->numButtons; i++) updateButton(i);
	}
	else if (c == brightness)
	{
		device->setBrightness(brightness->floatValue());
	}
	else if (c->parentContainer == &colorsCC)
	{
		int id = colors.indexOf((ColorParameter*)c);
		updateButton(id);
	}
	else if (c->parentContainer == &imagesCC)
	{
		int id = images.indexOf((FileParameter*)c);
		updateButton(id);	
	}
	

}


void StreamDeckModule::newMessage(const StreamDeckManager::StreamDeckManagerEvent& e)
{
	setDevice(StreamDeckManager::getInstance()->devices.size() > 0 ? StreamDeckManager::getInstance()->devices[0] : nullptr);
}