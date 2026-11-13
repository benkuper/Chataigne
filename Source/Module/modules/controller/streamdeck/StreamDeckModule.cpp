/*
  ==============================================================================

	StreamDeckModule.cpp
	Created: 22 Nov 2019 4:37:02pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

StreamDeckModule::StreamDeckModule(const String& name) :
	Module(name),
	isConnected(nullptr),
	device(nullptr),
	numRows(0),
	numColumns(0),
	numDials(0),
	textSize(nullptr),
	colorsCC("Colors"),
	imagesCC("Images"),
	textsCC("Texts"),
	dialSettingsCC("Dials"),
	dialsCC("Dials"),
	stripCC("Touch Strip"),
	stripNeedsRedraw(false)
#if USE_FAKE_DEVICE
	, fakeDevice(nullptr, "fake")
#endif
{
	setupIOConfiguration(true, true);

	isConnected = moduleParams.addBoolParameter("Is Connected", "Is a streamdeck connected ?", false);
	isConnected->setControllableFeedbackOnly(true);
	connectionFeedbackRef = isConnected;

	colorsCC.saveAndLoadRecursiveData = true;
	imagesCC.saveAndLoadRecursiveData = true;
	textsCC.saveAndLoadRecursiveData = true;

	moduleParams.addChildControllableContainer(&colorsCC);
	moduleParams.addChildControllableContainer(&imagesCC);
	moduleParams.addChildControllableContainer(&textsCC);
	colorsCC.editorIsCollapsed = true;
	imagesCC.editorIsCollapsed = true;
	textsCC.editorIsCollapsed = true;


	deviceType = moduleParams.addEnumParameter("Device type", "Type of the device");
	deviceType->addOption("StreamDeck (15 buttons)", STANDARD)->addOption("StreamDeck XL (32 buttons)", XL)->addOption("StreamDeck Mini (6 buttons)", MINI)->addOption("StreamDeck + XL (36 buttons, 6 dials)", PLUS_XL);

	dialSettingsCC.saveAndLoadRecursiveData = true;
	moduleParams.addChildControllableContainer(&dialSettingsCC);
	dialSensitivity = dialSettingsCC.addFloatParameter("Sensitivity", "How much each dial click changes the dial's Value", .01f, 0, 1);
	dialFineMultiplier = dialSettingsCC.addFloatParameter("Fine Adjust Multiplier", "Multiplier applied to Sensitivity while the dial is held down and turned (below 1 is finer)", .2f, 0, 5);
	dialRangeMin = dialSettingsCC.addFloatParameter("Range Min", "Lower bound of each dial's Value", 0, -100000, 100000);
	dialRangeMax = dialSettingsCC.addFloatParameter("Range Max", "Upper bound of each dial's Value", 1, -100000, 100000);
	dialWrapAround = dialSettingsCC.addBoolParameter("Wrap Around", "If checked, a dial's Value wraps around the range instead of stopping at the bounds", false);

	valuesCC.addChildControllableContainer(&dialsCC);
	valuesCC.addChildControllableContainer(&stripCC);
	stripPosition = stripCC.addFloatParameter("Position", "Where the strip was last touched, from 0 (left) to 1 (right)", 0, 0, 1);
	stripTap = stripCC.addTrigger("Tap", "Fires when the strip is tapped. Position is set first.");
	stripLongPress = stripCC.addTrigger("Long Press", "Fires when the strip is held. Position is set first.");
	stripSwipeLeft = stripCC.addTrigger("Swipe Left", "Fires when the strip is swiped right to left");
	stripSwipeRight = stripCC.addTrigger("Swipe Right", "Fires when the strip is swiped left to right");

	brightness = moduleParams.addFloatParameter("Brightness", "Sets the brigthness of the deck's backlight", .75f, 0, 1);
	textSize = moduleParams.addIntParameter("Text size", "Sets the size of the text on the buttons", 10, 1, 50);

	reset = moduleParams.addTrigger("Reset", "Resets the stream deck");
	colorizeImages = moduleParams.addBoolParameter("Colorize images", "If checked, this will use both colors and images to set buttons", false);
	highlightPressedButtons = moduleParams.addBoolParameter("Highlight pressed buttons", "If checked, this will automatically highlight the buttons when pressed", true);

	defManager->add(CommandDefinition::createDef(this, "", "Set Color", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_COLOR));
	defManager->add(CommandDefinition::createDef(this, "", "Set Image", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_IMAGE));
	defManager->add(CommandDefinition::createDef(this, "", "Set Text", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_TEXT));
	defManager->add(CommandDefinition::createDef(this, "", "Set All Color", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_ALL_COLOR));
	defManager->add(CommandDefinition::createDef(this, "", "Clear Texts", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::CLEAR_TEXTS));
	defManager->add(CommandDefinition::createDef(this, "", "Set Brightness", &StreamDeckCommand::create)->addParam("action", StreamDeckCommand::SET_BRIGHTNESS));


	rebuildValues();

#if USE_FAKE_DEVICE
	setDevice(&fakeDevice);
#else
	setDevice(StreamDeckManager::getInstance()->devices.size() > 0 ? StreamDeckManager::getInstance()->devices[0] : nullptr);
#endif

	StreamDeckManager::getInstance()->addStreamDeckManagerListener(this);

}

StreamDeckModule::~StreamDeckModule()
{
	stopTimer();
	if (StreamDeckManager::getInstanceWithoutCreating() != nullptr) StreamDeckManager::getInstance()->removeStreamDeckManagerListener(this);
	setDevice(nullptr);
}

void StreamDeckModule::rebuildValues()
{
	//if (isCurrentlyLoadingData) return;

	//GenericScopedLock lock(rebuildingLock);

	DeviceType t = deviceType->getValueDataAsEnum<DeviceType>();

	switch (t)
	{
	case STANDARD:
		numRows = 3;
		numColumns = 5;
		break;
	case MINI:
		numRows = 2;
		numColumns = 3;
		break;
	case XL:
		numRows = 4;
		numColumns = 8;
		break;
	case PLUS_XL:
		numRows = 4;
		numColumns = 9;
		break;
	}

	while (buttonStates.size() > numRows)
	{
		buttonStates.remove(numRows);
		valuesCC.removeChildControllableContainer(buttonRowsCC[numRows]);
		buttonRowsCC.remove(numRows);

		colors.remove(numRows);
		colorsCC.removeChildControllableContainer(colorsCC.controllableContainers[numRows]);

		images.remove(numRows);
		imagesCC.removeChildControllableContainer(imagesCC.controllableContainers[numRows]);

		texts.remove(numRows);
		textsCC.removeChildControllableContainer(textsCC.controllableContainers[numRows]);
	}


	while (buttonStates.size() < numRows)
	{
		buttonStates.add(new Array<BoolParameter*>());
		ControllableContainer* btCC = new ControllableContainer("Row " + String(buttonStates.size()));
		buttonRowsCC.add(btCC);
		valuesCC.addChildControllableContainer(btCC, true);

		colors.add(new Array<ColorParameter*>());
		ControllableContainer* cCC = new ControllableContainer("Row " + String(colors.size()));
		colorsCC.addChildControllableContainer(cCC, true);

		images.add(new Array<FileParameter*>());
		ControllableContainer* iCC = new ControllableContainer("Row " + String(images.size()));
		imagesCC.addChildControllableContainer(iCC, true);

		texts.add(new Array<StringParameter*>());
		ControllableContainer* tCC = new ControllableContainer("Row " + String(texts.size()));
		textsCC.addChildControllableContainer(tCC, true);
	}

	for (int r = 0; r < numRows; r++)
	{
		Array<BoolParameter*>* btStates = buttonStates[r];
		Array<ColorParameter*>* cColors = colors[r];
		Array<FileParameter*>* cImages = images[r];
		Array<StringParameter*>* cTexts = texts[r];
		ControllableContainer* btCC = buttonRowsCC[r];
		ControllableContainer* cCC = colorsCC.controllableContainers[r];
		ControllableContainer* iCC = imagesCC.controllableContainers[r];
		ControllableContainer* tCC = textsCC.controllableContainers[r];

		while (btStates->size() > numColumns)
		{
			btStates->remove(numColumns);
			btCC->removeControllable(btCC->controllables[numColumns]);

			cColors->remove(numColumns);
			cCC->removeControllable(cCC->controllables[numColumns]);

			cImages->remove(numColumns);
			iCC->removeControllable(iCC->controllables[numColumns]);

			cTexts->remove(numColumns);
			tCC->removeControllable(tCC->controllables[numColumns]);
		}

		while (btStates->size() < numColumns)
		{
			String btId = String(btStates->size() + 1);
			btStates->add(btCC->addBoolParameter("Button " + btId, "Is row " + String(r + 1) + ", button " + btId + " pressed ?", false));
			cColors->add(cCC->addColorParameter("Color " + btId, "If not image, color for row " + String(r + 1) + ", button " + btId, Colours::black));
			cImages->add(iCC->addFileParameter("Image " + btId, "Image for row " + String(r + 1) + ", button" + btId));
			cTexts->add(tCC->addStringParameter("Text " + btId, "Text for row " + String(r + 1) + ", button" + btId, ""));
		}
	}

	rebuildDials();

	// The + XL keys are 112 px, so 10 px text is hard to read. Only the default changes:
	// a size the user set (or loaded from a file) is kept. This redraws every key,
	// so it must run after the button parameters above exist.
	if (textSize != nullptr) textSize->setDefaultValue(t == PLUS_XL ? 20 : 10, !textSize->isOverriden);
}

void StreamDeckModule::rebuildDials()
{
	numDials = deviceType->getValueDataAsEnum<DeviceType>() == PLUS_XL ? 6 : 0;

	while (dialValues.size() > numDials)
	{
		int i = dialValues.size() - 1;
		dialsCC.removeControllable(dialPressed[i]);
		dialsCC.removeControllable(dialValues[i]);
		dialsCC.removeControllable(dialIncrements[i]);
		dialsCC.removeControllable(dialDecrements[i]);
		dialSettingsCC.removeControllable(dialLabels[i]);
		dialSettingsCC.removeControllable(dialColors[i]);
		dialPressed.remove(i);
		dialValues.remove(i);
		dialIncrements.remove(i);
		dialDecrements.remove(i);
		dialLabels.remove(i);
		dialColors.remove(i);
	}

	while (dialValues.size() < numDials)
	{
		String id = String(dialValues.size() + 1);
		dialPressed.add(dialsCC.addBoolParameter("Dial " + id + " Pressed", "Is dial " + id + " pressed ?", false));
		FloatParameter* v = dialsCC.addFloatParameter("Dial " + id + " Value", "Value of dial " + id + ", changed by turning it", 0);
		v->setRange(dialRangeMin->floatValue(), dialRangeMax->floatValue());
		dialValues.add(v);
		dialIncrements.add(dialsCC.addTrigger("Dial " + id + " Increment", "Fires once per click when dial " + id + " is turned clockwise"));
		dialDecrements.add(dialsCC.addTrigger("Dial " + id + " Decrement", "Fires once per click when dial " + id + " is turned counter-clockwise"));
		dialLabels.add(dialSettingsCC.addStringParameter("Dial " + id + " Label", "Label shown on the touch strip above dial " + id, "Dial " + id));
		dialColors.add(dialSettingsCC.addColorParameter("Dial " + id + " Color", "Background of the box above dial " + id, Colours::darkgrey));
	}

	bool hide = numDials == 0;
	if (dialSettingsCC.hideInEditor != hide)
	{
		dialSettingsCC.hideInEditor = hide;
		dialsCC.hideInEditor = hide;
		stripCC.hideInEditor = hide;
		moduleParams.queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, &moduleParams));
		valuesCC.queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, &valuesCC));
	}

	stripNeedsRedraw = true;
}

void StreamDeckModule::timerCallback()
{
	if (!stripNeedsRedraw.exchange(false)) return;
	if (device == nullptr || device->numEncoders == 0 || !enabled->boolValue()) return;

	Array<StreamDeck::EncoderDisplay> displays;
	for (int i = 0; i < jmin(numDials, device->numEncoders); ++i)
	{
		displays.add({ dialLabels[i]->stringValue(), dialColors[i]->getColor(), String(dialValues[i]->floatValue(), 2), dialValues[i]->getNormalizedValue() });
	}

	device->setEncoderDisplays(displays);
	outActivityTrigger->trigger();
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

		switch (device->model)
		{
		case StreamDeck::STANDARD_V1:
		case StreamDeck::STANDARD_V2:
			deviceType->setValueWithData(STANDARD);
			break;

		case StreamDeck::MINI:
			deviceType->setValueWithData(MINI);
			break;

		case StreamDeck::XL:
			deviceType->setValueWithData(XL);
			break;

		case StreamDeck::PLUS_XL:
			deviceType->setValueWithData(PLUS_XL);
			break;
		}

		// Strip redraws are coalesced so a fast dial spin doesn't send one image per click
		if (device->numEncoders > 0)
		{
			stripNeedsRedraw = true;
			startTimerHz(30);
		}

		//for (int i = 0; i < numRows; ++i)
		//{
		//	for (int j = 0; j < numColumns; j++)
		//	{
		//		updateButton(i, j);
		//	}
		//}
	}

	//rebuildValues();

	if (device == nullptr || device->numEncoders == 0) stopTimer();

	isConnected->setValue(device != nullptr);

}

void StreamDeckModule::updateButton(int row, int column)
{
	if (device == nullptr) return;
	//GenericScopedLock lock(rebuildingLock);

	// numRows/numColumns change before the parameter arrays during a rebuild, so check the arrays
	if (row < 0 || row >= images.size() || column < 0 || column >= images[row]->size()) return;

	File f = (*images[row])[column]->getFile();

	if (f.existsAsFile())
	{
		Image image = ImageCache::getFromFile(f);
		String overlayText = (*texts[row])[column]->stringValue();

		if (logOutgoingData->boolValue())
		{
			NLOG(niceName, "Set image " << f.getFileName() << " to button " << String(column + 1) << ":" << String(row + 1) << (overlayText.isNotEmpty() ? "\nOverlay Text : " + overlayText : ""));
		}

		if (colorizeImages->boolValue())
		{
			device->setImage(row, column, image, (*colors[row])[column]->getColor(), highlightPressedButtons->boolValue() ? (*buttonStates[row])[column]->boolValue() : false, (*texts[row])[column]->stringValue(), textSize->intValue());
		}
		else
		{
			device->setImage(row, column, image, highlightPressedButtons->boolValue() ? (*buttonStates[row])[column]->boolValue() : false, overlayText, textSize->intValue());
		}
	}
	else
	{
		String overlayText = (*texts[row])[column]->stringValue();

		if (logOutgoingData->boolValue())
		{
			NLOG(niceName, "Set color " << (*colors[row])[column]->getColor().toString() << " to button " << String(column + 1) << ":" << String(row + 1) << (overlayText.isNotEmpty() ? "\nOverlay Text : " + overlayText : ""));
		}

		device->setColor(row, column, (*colors[row])[column]->getColor(), highlightPressedButtons->boolValue() ? (*buttonStates[row])[column]->boolValue() : false, overlayText, textSize->intValue());
	}

	outActivityTrigger->trigger();
}

void StreamDeckModule::setColor(int row, int column, const Colour& c)
{
	//GenericScopedLock lock(rebuildingLock);
	if (row < 0 || row >= numRows) return;
	if (column < 0 || column >= numColumns) return;
	(*colors[row])[column]->setColor(c);
}

void StreamDeckModule::setAllColor(const Colour& color)
{
	for (auto& cc : colors)
	{
		for (auto& c : *cc) c->setColor(color);
	}
}

void StreamDeckModule::setImage(int row, int column, const String& path)
{
	//GenericScopedLock lock(rebuildingLock);
	if (row < 0 || row >= numRows) return;
	if (column < 0 || column >= numColumns) return;
	(*images[row])[column]->setValue(path);
}

void StreamDeckModule::setText(int row, int column, const String& text)
{
	//GenericScopedLock lock(rebuildingLock);
	if (row < 0 || row >= numRows) return;
	if (column < 0 || column >= numColumns) return;
	(*texts[row])[column]->setValue(text);
}

void StreamDeckModule::clearTexts()
{
	for (auto& tt : texts)
	{
		for (auto& t : *tt) t->setValue("");
	}
}

void StreamDeckModule::streamDeckButtonPressed(int row, int column)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Button " + String(column + 1) << ":" << String(row + 1) + " pressed");
	}

	if (row >= numRows) return;
	if (column >= numColumns) return;

	inActivityTrigger->trigger();
	(*buttonStates[row])[column]->setValue(true);

	if (highlightPressedButtons) updateButton(row, column);
}

void StreamDeckModule::streamDeckButtonReleased(int row, int column)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Button " + String(column + 1) << ":" << String(row + 1) + " released");
	}

	if (row >= buttonStates.size()) return;
	if (column >= buttonStates[row]->size()) return;

	inActivityTrigger->trigger();
	(*buttonStates[row])[column]->setValue(false);
	if (highlightPressedButtons) updateButton(row, column);
}

void StreamDeckModule::streamDeckEncoderRotated(int index, int delta)
{
	if (logIncomingData->boolValue()) NLOG(niceName, "Dial " << String(index + 1) << " turned " << String(delta));
	if (index < 0 || index >= dialValues.size()) return;

	inActivityTrigger->trigger();

	Trigger* t = delta > 0 ? dialIncrements[index] : dialDecrements[index];
	for (int i = 0; i < std::abs(delta); ++i) t->trigger();

	float sensitivity = dialSensitivity->floatValue() * (dialPressed[index]->boolValue() ? dialFineMultiplier->floatValue() : 1.0f);
	float newValue = dialValues[index]->floatValue() + delta * sensitivity;

	float rangeMin = dialRangeMin->floatValue();
	float rangeMax = dialRangeMax->floatValue();
	if (dialWrapAround->boolValue() && rangeMax > rangeMin)
	{
		float range = rangeMax - rangeMin;
		while (newValue < rangeMin) newValue += range;
		while (newValue >= rangeMax) newValue -= range;
	}

	dialValues[index]->setValue(newValue);
}

void StreamDeckModule::streamDeckEncoderPressed(int index)
{
	if (logIncomingData->boolValue()) NLOG(niceName, "Dial " << String(index + 1) << " pressed");
	if (index < 0 || index >= dialPressed.size()) return;
	inActivityTrigger->trigger();
	dialPressed[index]->setValue(true);
}

void StreamDeckModule::streamDeckEncoderReleased(int index)
{
	if (logIncomingData->boolValue()) NLOG(niceName, "Dial " << String(index + 1) << " released");
	if (index < 0 || index >= dialPressed.size()) return;
	inActivityTrigger->trigger();
	dialPressed[index]->setValue(false);
}

void StreamDeckModule::streamDeckStripTapped(float x, bool longPress)
{
	if (logIncomingData->boolValue()) NLOG(niceName, "Strip " << (longPress ? "long press" : "tap") << " at " << String(x, 3));
	if (numDials == 0) return;
	inActivityTrigger->trigger();
	stripPosition->setValue(x);
	(longPress ? stripLongPress : stripTap)->trigger();
}

void StreamDeckModule::streamDeckStripSwiped(bool toRight)
{
	if (logIncomingData->boolValue()) NLOG(niceName, "Strip swiped " << (toRight ? "right" : "left"));
	if (numDials == 0) return;
	inActivityTrigger->trigger();
	(toRight ? stripSwipeRight : stripSwipeLeft)->trigger();
}

void StreamDeckModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == deviceType)
	{
		rebuildValues();
	}

	if (c == dialRangeMin || c == dialRangeMax)
	{
		for (auto* v : dialValues) v->setRange(dialRangeMin->floatValue(), dialRangeMax->floatValue());
	}

	if (c == enabled || c == reset || dialLabels.contains((StringParameter*)c) || dialColors.contains((ColorParameter*)c) || dialValues.contains((FloatParameter*)c))
	{
		stripNeedsRedraw = true;
		if (c != enabled && c != reset) return;
	}

	if (!enabled->boolValue()) return;
	if (device == nullptr) return;

	if (c == reset || c == colorizeImages || c == textSize)
	{
		if (c == reset) device->reset();

		for (int i = 0; i < numRows; ++i)
		{
			for (int j = 0; j < numColumns; j++)
			{
				updateButton(i, j);
			}
		}
	}
	else if (c == brightness)
	{
		device->setBrightness(brightness->floatValue());
	}
	else if (c->parentContainer->parentContainer == &colorsCC)
	{
		int row = colorsCC.controllableContainers.indexOf(c->parentContainer);
		int column = colors[row]->indexOf((ColorParameter*)c);
		updateButton(row, column);
	}
	else if (c->parentContainer->parentContainer == &imagesCC)
	{
		int row = imagesCC.controllableContainers.indexOf(c->parentContainer);
		int column = images[row]->indexOf((FileParameter*)c);
		updateButton(row, column);
	}
	else if (c->parentContainer->parentContainer == &textsCC)
	{
		int row = textsCC.controllableContainers.indexOf(c->parentContainer);
		int column = texts[row]->indexOf((StringParameter*)c);
		updateButton(row, column);
	}
}

void StreamDeckModule::deviceAdded(StreamDeck* d)
{
	if (device == nullptr)
	{
		setDevice(d);
	}
}

void StreamDeckModule::deviceRemoved(StreamDeck* d)
{
	if (device == d || StreamDeckManager::getInstance()->devices.size() == 0)
	{
		setDevice(nullptr);
	}
}

void StreamDeckModule::afterLoadJSONDataInternal()
{
	Module::afterLoadJSONDataInternal();
	//rebuildValues();
}
