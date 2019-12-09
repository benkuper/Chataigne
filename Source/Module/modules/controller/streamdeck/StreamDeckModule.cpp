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

#if USE_FAKE_DEVICE
	, fakeDevice(nullptr, "fake")
#endif
{
	setupIOConfiguration(true, true);
	
	isConnected = moduleParams.addBoolParameter("Is Connected", "Is a streamdeck connected ?", false);
	isConnected->setControllableFeedbackOnly(true);
	connectionFeedbackRef = isConnected;

	moduleParams.addChildControllableContainer(&colorsCC);
	moduleParams.addChildControllableContainer(&imagesCC);
	colorsCC.editorIsCollapsed = true;
	imagesCC.editorIsCollapsed = true;


	deviceType = moduleParams.addEnumParameter("Device type", "Type of the device");
	deviceType->addOption("StreamDeck (15 buttons)", STANDARD)->addOption("StreamDeck XL (32 buttons)", XL)->addOption("StreamDeck Mini (6 buttons)", MINI);

	brightness = moduleParams.addFloatParameter("Brightness", "Sets the brigthness of the deck's backlight", .75f, 0, 1);
	
	reset = moduleParams.addTrigger("Reset", "Resets the stream deck");
	colorizeImages = moduleParams.addBoolParameter("Colorize images", "If checked, this will use both colors and images to set buttons", true);
	highlightPressedButtons = moduleParams.addBoolParameter("Highlight pressed buttons", "If checked, this will automatically highlight the buttons when pressed", true);

	defManager->add(CommandDefinition::createDef(this, "", "Set Color", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_COLOR));
	defManager->add(CommandDefinition::createDef(this, "", "Set Image", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_IMAGE));
	defManager->add(CommandDefinition::createDef(this, "", "Set All", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_ALL_COLOR));
	defManager->add(CommandDefinition::createDef(this, "", "Set Brightness", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_BRIGHTNESS));

	rebuildValues();

#if USE_FAKE_DEVICE
	setDevice(&fakeDevice);
#else
	setDevice(StreamDeckManager::getInstance()->devices.size() > 0 ? StreamDeckManager::getInstance()->devices[0] : nullptr);
#endif

	StreamDeckManager::getInstance()->addAsyncManagerListener(this);
}

StreamDeckModule::~StreamDeckModule()
{
	if(StreamDeckManager::getInstanceWithoutCreating() != nullptr) StreamDeckManager::getInstance()->removeAsyncManagerListener(this);
	setDevice(nullptr);
}

void StreamDeckModule::rebuildValues()
{ 
	int numButtons = (int)deviceType->getValueData();
	
	for (int i = buttonStates.size(); i < numButtons; i++)
	{
		String userIndex = String(buttonStates.size() + 1);
		buttonStates.add(valuesCC.addBoolParameter("Button " + userIndex, "Is button " + userIndex+ " pressed ?", false));
		colors.add(colorsCC.addColorParameter("Image " + userIndex, "If not image, color for button " +userIndex, Colours::black));
		images.add(imagesCC.addFileParameter("Image " + userIndex, "Image for button" + userIndex+ " (overrides color)"));
	}

	while (buttonStates.size() > numButtons)
	{
		buttonStates.remove(numButtons);
		valuesCC.removeControllable(valuesCC.controllables[numButtons]);
		
		colors.remove(numButtons);
		colorsCC.removeControllable(colorsCC.controllables[numButtons]);

		images.remove(numButtons);
		imagesCC.removeControllable(imagesCC.controllables[numButtons]);
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
		device->setBrightness(brightness->floatValue());

		int numButtons = (int)deviceType->getValueData();
		for (int i = 0; i < numButtons; i++) updateButton(i);
	}

	isConnected->setValue(device != nullptr);

}

void StreamDeckModule::updateButton(int id)
{
	if (device == nullptr) return;

	int numButtons = deviceType->getValueData();
	if (id < 0 || id >= numButtons) return;

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

	if (button >= buttonStates.size()) return;

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

	if (button >= buttonStates.size()) return;


	inActivityTrigger->trigger();
	buttonStates[button]->setValue(false);
	if (highlightPressedButtons) updateButton(button);
}

void StreamDeckModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	
	if (c == deviceType)
	{
		rebuildValues();
	}

	if (!enabled->boolValue()) return;
	if (device == nullptr) return;

	if (c == reset)
	{
		device->reset();
		int numButtons = (int)deviceType->getValueData();
		for (int i = 0; i < numButtons; i++) updateButton(i);
	}
	else if (c == colorizeImages)
	{
		int numButtons = (int)deviceType->getValueData();
		for (int i = 0; i < numButtons; i++) updateButton(i);
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