/*
  ==============================================================================

	LoupedeckModule.cpp
	Created: 16 Mar 2021 6:38:55pm
	Author:  bkupe

  ==============================================================================
*/

#pragma warning(push)
#pragma warning(disable:4838)

#include "Module/ModuleIncludes.h"

LoupedeckModule::LoupedeckModule() :
	SerialModule(getDefaultTypeString()),
	buttonParamsCC("Buttons"),
	sliderParamsCC("Sliders"),
	padParamsCC("Pads"),
	buttonsCC("Buttons"),
	knobsCC("Knobs"),
	touchScreenCC("Touch Screen"),
	slidersCC("Sliders"),
	padsCC("Pads")
{
	//baudRate->setValue(9600);

	portParam->setVIDPIDFilters({ 0x2ec2 }, {});

	streamingType->setValueWithData(RAW);
	streamingType->hideInEditor = true;

	autoAdd->hideInEditor = true;
	autoAdd->setValue(false);

	messageStructure->hideInEditor = true;
	firstValueIsTheName->hideInEditor = true;


	baudRate->hideInEditor = true;

	model = moduleParams.addEnumParameter("Model", "Model of the Loupedeck"); //not used right now
	model->addOption("Loupedeck Live", 0)->addOption("Loupedeck CT", 1);

	screens.add({ 0x004C, 60, 270 }); //left
	screens.add({ 0x0052, 60, 270 }); //right
	screens.add({ 0x0041, 360, 270 });//middle
	//screens.add({0x0057,240, ,240, circular = true}); //wheel

	autoRefresh = moduleParams.addBoolParameter("Auto Refresh Screen", "If checked, this will force a refresh on every change. This is useful to optimize refresh speed when changing multiple values at once", true);

	brightness = moduleParams.addFloatParameter("Screen Brightness", "Backlight intensity", .5f, 0, 1);
	knobSensitivity = moduleParams.addFloatParameter("Knob sensitivity", "Sensitivity for the relative computing of the knobs", .5f, 0, 1);
	highlightOnTouch = moduleParams.addBoolParameter("Highlight On Touch", "If checked, this will force highlighting the touched pad", true);
	vibrateOnTouch = moduleParams.addBoolParameter("Vibrate On Touch", "If checked, this will vibrate when touching the pad", false);

	const int numSliders = 2;
	const String sliderNames[numSliders] = { "Left","Right" };
	const int numPads = 12;
	const int numButtons = 8;
	const int numKnobs = 6;

	for (int i = 0; i < numButtons; i++)
	{
		ColorParameter* c = buttonParamsCC.addColorParameter("Button " + String(i) + " Color", "Color for this button", Colours::black);
		buttonColors.add(c);

		BoolParameter* b = buttonsCC.addBoolParameter("Button " + String(i), "Is this button pressed", false);
		b->setControllableFeedbackOnly(true);
		buttons.add(b);
	}


	for (int i = 0; i < numSliders; i++)
	{
		ColorParameter* c = sliderParamsCC.addColorParameter(sliderNames[i] + " Slider Color", "Color for this slider", Colours::black);
		sliderColors.add(c);

		FileParameter* fc = sliderParamsCC.addFileParameter(sliderNames[i] + " Slider Image", "Image for this pad");
		fc->fileTypeFilter = "*.png; *.jpg; *.jpeg";
		sliderImages.add(fc);

		StringParameter* sc = sliderParamsCC.addStringParameter(sliderNames[i] + " Slider Text", "Overlay text for this slider", "");
		sliderTexts.add(sc);

		BoolParameter* b = slidersCC.addBoolParameter(sliderNames[i] + " Pressed", "Is this slider pressed by at least one touch", false);
		b->setControllableFeedbackOnly(true);
		sliderTouches.add(b);

		FloatParameter* v = slidersCC.addFloatParameter(sliderNames[i], "Vertical slider position", 0, 0, 1);
		v->setControllableFeedbackOnly(true);
		sliderAbsolutes.add(v);
	}

	for (int i = 0; i < numPads; i++)
	{
		ColorParameter* c = padParamsCC.addColorParameter("Pad " + String(i + 1) + " Color", "Color for this pad", Colours::black);
		padColors.add(c);

		FileParameter* fc = padParamsCC.addFileParameter("Pad " + String(i + 1) + " Image", "Image for this pad");
		fc->fileTypeFilter = "*.png; *.jpg; *.jpeg";
		padImages.add(fc);

		StringParameter* sc = padParamsCC.addStringParameter("Pad " + String(i + 1) + " Text", "Overlay text for this pad", "");
		padTexts.add(sc);

		BoolParameter* b = padsCC.addBoolParameter("Pad " + String(i + 1) + " Pressed", "Is this pad pressed by at least one touch", false);
		b->setControllableFeedbackOnly(true);
		pads.add(b);


		Point2DParameter* p2d = padsCC.addPoint2DParameter("Pad " + String(i + 1), "Position in this pad, if touched");
		p2d->setBounds(-1, -1, 1, 1);
		p2d->setControllableFeedbackOnly(true);
		padsTouchPositions.add(p2d);
	}

	for (int i = 0; i < numKnobs; i++)
	{
		BoolParameter* b = knobsCC.addBoolParameter("Knob " + String(i + 1) + " Pressed", "Is this knob pressed", false);
		b->setControllableFeedbackOnly(true);
		knobButtons.add(b);

		FloatParameter* v = knobsCC.addFloatParameter("Knob " + String(i + 1) + " Absolute", "Absolute infinite scrolling", 0);
		v->setControllableFeedbackOnly(true);
		knobAbsolutes.add(v);

		FloatParameter* v2 = knobsCC.addFloatParameter("Knob " + String(i + 1) + " Relative", "Relative scrolling depending on knob sensitivity", 0, -1, 1);
		v2->alwaysNotify = true;
		v2->setControllableFeedbackOnly(true);
		knobRelatives.add(v2);
	}

	touchPosition = touchScreenCC.addPoint2DParameter("Touch Position", "Relative position of the first touch");
	touchPosition->setBounds(0, 0, 1, 1);
	touchPosition->setControllableFeedbackOnly(true);



	//serverPath->hideInEditor = true;
	//serverPath->setValue(getLoupedeckServerPath());

	defManager->clear();
	defManager->add(CommandDefinition::createDef(this, "Pad", "Set Color", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_COLOR)->addParam("screenTarget", LoupedeckCommand::PADS));
	defManager->add(CommandDefinition::createDef(this, "Pad", "Set Image", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_IMAGE)->addParam("screenTarget", LoupedeckCommand::PADS));
	defManager->add(CommandDefinition::createDef(this, "Pad", "Set Text", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_TEXT)->addParam("screenTarget", LoupedeckCommand::PADS));
	defManager->add(CommandDefinition::createDef(this, "Pad", "Set All Color", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_ALL_COLOR));
	defManager->add(CommandDefinition::createDef(this, "Pad", "Set All Text", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_ALL_TEXT)->addParam("screenTarget", LoupedeckCommand::PADS));

	defManager->add(CommandDefinition::createDef(this, "Slider Left", "Set Color", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_COLOR)->addParam("screenTarget", LoupedeckCommand::LEFT_SLIDER));
	defManager->add(CommandDefinition::createDef(this, "Slider Left", "Set Image", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_IMAGE)->addParam("screenTarget", LoupedeckCommand::LEFT_SLIDER));
	defManager->add(CommandDefinition::createDef(this, "Slider Left", "Set Text", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_TEXT)->addParam("screenTarget", LoupedeckCommand::LEFT_SLIDER));

	defManager->add(CommandDefinition::createDef(this, "Slider Right", "Set Color", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_COLOR)->addParam("screenTarget", LoupedeckCommand::RIGHT_SLIDER));
	defManager->add(CommandDefinition::createDef(this, "Slider Right", "Set Image", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_IMAGE)->addParam("screenTarget", LoupedeckCommand::RIGHT_SLIDER));;
	defManager->add(CommandDefinition::createDef(this, "Slider Right", "Set Text", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_TEXT)->addParam("screenTarget", LoupedeckCommand::RIGHT_SLIDER));

	defManager->add(CommandDefinition::createDef(this, "Button", "Set Button Color", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_COLOR)->addParam("screenTarget", LoupedeckCommand::BUTTONS));
	defManager->add(CommandDefinition::createDef(this, "Button", "Set All Color", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_ALL_COLOR)->addParam("screenTarget", LoupedeckCommand::BUTTONS));

	defManager->add(CommandDefinition::createDef(this, "", "Set Brightness", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_BRIGHTNESS));
	defManager->add(CommandDefinition::createDef(this, "", "Vibrate", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::VIBRATE));
	defManager->add(CommandDefinition::createDef(this, "", "Set Auto Refresh", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::SET_AUTO_REFRESH));
	defManager->add(CommandDefinition::createDef(this, "", "Refresh Screen", &LoupedeckCommand::create)->addParam("action", LoupedeckCommand::REFRESH_SCREEN));

	moduleParams.addChildControllableContainer(&buttonParamsCC);
	moduleParams.addChildControllableContainer(&sliderParamsCC);
	moduleParams.addChildControllableContainer(&padParamsCC);
	moduleParams.addChildControllableContainer(&shapeManager);

	valuesCC.addChildControllableContainer(&buttonsCC);
	valuesCC.addChildControllableContainer(&knobsCC);
	valuesCC.addChildControllableContainer(&touchScreenCC);
	valuesCC.addChildControllableContainer(&slidersCC);
	valuesCC.addChildControllableContainer(&padsCC);


	wsMode = WSMode::HANDSHAKE;
}

LoupedeckModule::~LoupedeckModule()
{
}


void LoupedeckModule::setupPortInternal()
{
	//port->setDataBits(8);
	//port->setStopBits(2);
}

void LoupedeckModule::portOpenedInternal()
{
	//Timer::callAfterDelay(100, [this]() {
	wsMode = WSMode::HANDSHAKE;

	//in case loupedeck was already in websocket mode
	Array<uint8_t> closeBytes{ 0x88, 0x80, 0x00, 0x00, 0x00, 0x00 };
	sendBytes(closeBytes);

	String req = "GET /index.html HTTP/1.1\n\
Connection: Upgrade\n\
Upgrade: websocket\n\
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==\n\n";

	this->sendMessage(req);
	//});
}

void LoupedeckModule::processDataBytesInternal(Array<uint8> bytes)
{
	if (wsMode == HANDSHAKE)
	{
		String msg = String::createStringFromData((const char*)bytes.getRawDataPointer(), bytes.size());
		if (msg.contains("\r\n\r\n"))
		{
			NLOG(niceName, "Handshake received, Loupedeck in da platz.");
			buffer.clear();
			wsMode = WSMode::DATA;

			//init screen and buttons
			//for (int i = 0; i < pads.size(); i++) updatePadContent(i, false);
			//for (int i = 0; i < buttons.size(); i++) updateButton(i);
			refreshScreen(2);
		}

		return;
	}

	if (!enabled->boolValue()) return;
	if (bytes.size() < 2) return;

	if (bytes[0] == 130)
	{
		bytes.remove(0);
		expectedLength = bytes.removeAndReturn(0);
		buffer.clear();
	}

	buffer.addArray(bytes.getRawDataPointer(), jmin<int>(bytes.size(), expectedLength - buffer.size()));

	if (buffer.size() == 0 || buffer.size() < expectedLength) return;

	uint8 id = buffer[0];

	switch (id)
	{
	case 4:
		return;
		break;

	case 5: // buttons and knobs
	{
		if (buffer.size() >= 5)
		{
			int pot = buffer[3] - 1;
			if (pot >= 6)
			{
				buttons[pot - 6]->setValue(buffer[4] == 0);
			}
			else if (pot >= 0)
			{
				if (buffer[1])
				{
					float val = buffer[4] == 255 ? -1 : 1;
					knobRelatives[pot]->setValue(val);
					knobAbsolutes[pot]->setValue(knobAbsolutes[pot]->floatValue() + val * knobSensitivity->floatValue());
				}
				else
				{
					knobButtons[pot]->setValue(buffer[4] == 0);
				}
			}
			else
			{
				DBG("Pot : " << pot << " unknown");
			}
		}
	}
	break;

	case 9: //touch
		processTouchData(buffer);
		break;
	}
}


void LoupedeckModule::processTouchData(Array<uint8_t> data)
{
	if (data.size() < 7) return;
	if (data[1] != 77 && data[1] != 109) return; //CT : data[1] has another data layout for events, filtering out only main screen touch right now

	bool touched = data[1] == 77;

	int touchID = data[3];
	int x = (data[4] << 8) | data[5];
	int y = (data[6] << 8) | data[7];
	//int pressure = data[8];

	Point<int> curPos = Point<int>(x, y);

	if (!touchPadMap.contains(touchID))
	{
		int pad = getPadIDForPos(curPos);
		if (pad >= padsTouchPositions.size()) return;

		touchPadMap.set(touchID, pad);
		posOnTouchMap.set(touchID, curPos);
		if (pad >= 0) padsTouchPositions[pad]->setPoint(0, 0);
	}


	int curPad = touchPadMap[touchID];
	Point<int> posOnTouch = posOnTouchMap[touchID];


	if (touched)
	{
		if (curPad == -1)
		{
			sliderTouches[0]->setValue(true);
			sliderAbsolutes[0]->setValue(1 - (y / 270.0f));
		}
		else if (curPad == -2)
		{
			sliderTouches[1]->setValue(true);
			sliderAbsolutes[1]->setValue(1 - (y / 270.0f));
		}
		else
		{
			if (!pads[curPad]->boolValue()) pads[curPad]->setValue(true);
			else
			{
				Point<float> diff = ((curPos - posOnTouch).toFloat() / Point<float>(460, 270));
				padsTouchPositions[curPad]->setPoint(diff);
			}
		}

		if (touchID == 0) touchPosition->setPoint(curPos.toFloat() / Point<float>(460, 270));
	}
	else
	{
		if (curPad == -1)  sliderTouches[0]->setValue(false);
		else if (curPad == -2) sliderTouches[1]->setValue(false);
		else  pads[curPad]->setValue(false);

		touchPadMap.remove(touchID);
		posOnTouchMap.remove(touchID);
	}
}

void LoupedeckModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	SerialModule::onControllableFeedbackUpdateInternal(cc, c);

	if (c == isConnected)
	{
		if (isConnected->boolValue())
		{

		}
	}
	else if (cc == &moduleParams)
	{
		if (c == brightness)
		{
			sendLoupedeckCommand(BacklightLevel, { (uint8)(brightness->floatValue() * 10) });
		}
		else if (c == autoRefresh)
		{
			if (autoRefresh->boolValue()) for (int i = 0; i < screens.size(); i++) refreshScreen(i);
		}
		else if (c->parentContainer == &buttonParamsCC)
		{
			if (ColorParameter* colP = dynamic_cast<ColorParameter*>(c))
			{
				uint8 index = buttonColors.indexOf(colP);
				updateButton(index);
			}
		}
		else if (c->parentContainer == &padParamsCC)
		{
			int id = -1;
			if (ColorParameter* colP = dynamic_cast<ColorParameter*>(c)) id = padColors.indexOf(colP);
			else if (FileParameter* fp = dynamic_cast<FileParameter*>(c)) id = padImages.indexOf(fp);
			else if (StringParameter* tp = dynamic_cast<StringParameter*>(c)) id = padTexts.indexOf(tp);

			if (id != -1) updatePadContent(id, autoRefresh->boolValue()); //1-based
		}
		else if (c->parentContainer == &sliderParamsCC)
		{
			int id = -1;
			if (ColorParameter* colP = dynamic_cast<ColorParameter*>(c)) id = sliderColors.indexOf(colP);
			else if (FileParameter* fp = dynamic_cast<FileParameter*>(c)) id = sliderImages.indexOf(fp);
			else if (StringParameter* tp = dynamic_cast<StringParameter*>(c)) id = sliderTexts.indexOf(tp);

			if (id != -1) updateSliderContent(id, autoRefresh->boolValue()); //1-based
		}
		else if (LoupedeckShape* s = dynamic_cast<LoupedeckShape*>(c->parentContainer.get()))
		{
			int sScreen = (int)s->screen->getValueData();
			if (sScreen < 2) updateSliderContent(sScreen, autoRefresh->boolValue());
			else
			{
				Rectangle<float> sBounds = s->getBounds().translated(60, 0);

				for (int i = 0; i < pads.size(); i++)
				{
					if (getPadCoords(i).toFloat().intersects(sBounds)) updatePadContent(i, false);
				}

				if (autoRefresh->boolValue()) refreshScreen(2);
			}
		}
	}
	else if (cc == &valuesCC)
	{
		if (c->parentContainer == &padsCC)
		{
			if (BoolParameter* p = dynamic_cast<BoolParameter*>(c))
			{
				int index = pads.indexOf(p);
				if (index >= 0) {
					if (vibrateOnTouch->boolValue() && p->boolValue()) vibrate(50);
					if (highlightOnTouch->boolValue()) updatePadContent(index, autoRefresh->boolValue());
				}
			}
		}
		else if (c->parentContainer == &slidersCC)
		{
			if (BoolParameter* p = dynamic_cast<BoolParameter*>(c))
			{
				int index = sliderTouches.indexOf(p);
				if (index >= 0) {
					if (vibrateOnTouch->boolValue() && p->boolValue()) vibrate(50);
					if (highlightOnTouch->boolValue()) updateSliderContent(index, autoRefresh->boolValue());
				}
			}
		}
	}

}

void LoupedeckModule::sendLoupedeckCommand(LDCommand command, Array<uint8> data)
{
	if (!enabled->boolValue()) return;

	//insert command at begin of payload
	data.insert(0, (command >> 8) & 0xFF);
	data.insert(1, command & 0xFF);
	data.insert(2, command & 0xFF); //callback

	int payloadSize = data.size();

	uint8_t lenAndMask = 1 << 7;//first bit is mask, always 1 when sending from client
	int numExpandedLenBytes = 0;
	if (payloadSize <= 125) lenAndMask += payloadSize;
	else if (payloadSize < 65535)
	{
		lenAndMask += 126;
		numExpandedLenBytes = 2;
	}
	else
	{
		lenAndMask += 127;
		numExpandedLenBytes = 8;
	}

	Array<uint8> dataToSend;
	dataToSend.add(130); //ws data opcode
	dataToSend.add(lenAndMask);
	for (int i = 0; i < numExpandedLenBytes; i++)
	{
		uint8_t t = (payloadSize >> ((numExpandedLenBytes - i - 1) * 8)) & 0xFF;
		dataToSend.add(t);
	}

	//masking
	const uint8_t mask[4]{ 0,0,0,0 };
	dataToSend.addArray(mask, 4);


	// mask here
	//for (int i = 0; i < data.size(); i++)
	//{
	//	uint8_t mByte = mask[i % 4];
	//	dataToSend.add(data[i] ^ mByte); //websocket masking = (original byte) xor (mask byte i%4)
	//}

	dataToSend.addArray(data);

	sendBytes(dataToSend);
}

void LoupedeckModule::updateButton(int buttonId)
{
	ColorParameter* colP = buttonColors[buttonId];

	Colour col = colP->getColor().withMultipliedBrightness(col.getAlpha());

	sendLoupedeckCommand(ButtonColor, { (uint8_t)(buttonId + 7), col.getRed(), col.getGreen(), col.getBlue() });
}

void LoupedeckModule::updatePadContent(int padID, bool refresh)
{
	Rectangle<int> r = getPadCoords(padID).translated(-60, 0);

	Colour c = padColors[padID]->getColor();
	if (pads[padID]->boolValue() && highlightOnTouch->boolValue()) c = c.brighter(.2f);
	setScreenContent(2, r, ImageCache::getFromFile(padImages[padID]->getFile()), c, padTexts[padID]->stringValue(), refresh);
}

void LoupedeckModule::updateSliderContent(int sliderID, bool refresh)
{
	Rectangle<int> r(0, 0, 60, 270);
	Colour c = sliderColors[sliderID]->getColor();
	if (sliderTouches[sliderID]->boolValue() && highlightOnTouch->boolValue()) c = c.brighter(.1f);
	setScreenContent(sliderID, r, ImageCache::getFromFile(sliderImages[sliderID]->getFile()), c, sliderTexts[sliderID]->stringValue(), refresh);
}

void LoupedeckModule::setScreenContent(int screenIndex, const Rectangle<int>& r, const Image& img, const Colour& color, const String& text, bool refresh)
{
	int w = r.getWidth();
	int h = r.getHeight();

	Image iconImage(Image::RGB, w, h, true);
	Graphics g(iconImage);
	g.setColour(img.isNull() ? color : Colours::black);
	g.fillAll();
	if (img.isValid())
	{
		g.drawImage(img, g.getClipBounds().toFloat());
		g.setColour(color.withMultipliedAlpha(.5f));
		g.fillAll();
	}

	shapeManager.draw(g, screenIndex, r);

	if (text.isNotEmpty())
	{
		MessageManagerLock mmLock;
		g.setColour(color.getPerceivedBrightness() > .5f ? Colours::black : Colours::white);
		g.drawFittedText(text, g.getClipBounds().reduced(5), Justification::centred, 5);
	}

	LDScreen screen = screens[screenIndex];

	Array<uint8> data = { (screen.id >> 8) & 0xFF, screen.id & 0xFF,
		(r.getX() >> 8) & 0xFF, r.getX() & 0xFF,
		(r.getY() >> 8) & 0xFF, r.getY() & 0xFF,
		(r.getWidth() >> 8) & 0xFF, w & 0xFF,
		(r.getHeight() >> 8) & 0xFF, h & 0xFF
		,0x00
	};

	data.resize(r.getWidth() * r.getHeight() * 2 + 11);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			Colour c = iconImage.getPixelAt(j, i);
			uint8 r = c.getRed();
			uint8 g = c.getGreen();
			uint8 b = c.getBlue();

			short rgb565 = (((r & 0xf8) << 8) + ((g & 0xfc) << 3) + (b >> 3));

			int pixIndex = i * w + j;
			int byteIndex = 11 + pixIndex * 2;
			data.set(byteIndex, ((rgb565 >> 8) & 0xFF));
			data.set(byteIndex + 1, (rgb565 & 0xFF));
		}
	}

	sendLoupedeckCommand(ScreenImage, data);
	if (refresh) refreshScreen(screenIndex);
}

void LoupedeckModule::refreshScreen(int screenIndex)
{
	short id = screens[screenIndex].id;
	sendLoupedeckCommand(RefreshScreen, { (id >> 8) & 0xFF, id & 0xFF });
}

int LoupedeckModule::getPadIDForPos(Point<int> pos)
{
	if (pos.x <= 60) return -1;
	if (pos.x >= 400) return -2;

	int tx = floorf((pos.x - 60) / 90);
	int ty = floor(pos.y / 90);
	return (tx + ty * 4); //0 based
}

Rectangle<int> LoupedeckModule::getPadCoords(int padID)
{
	//1 based
	int tx = padID % 4;
	int ty = floorf(padID / 4);
	return Rectangle<int>(60 + tx * 90, ty * 90, 90, 90);
}

void LoupedeckModule::vibrate(int vibrationIndex)
{
	sendLoupedeckCommand(Vibrate, { vibrationIndex });
}

String LoupedeckModule::getLoupedeckServerPath() const
{
	Array<IPAddress> ips;
	IPAddress::findAllAddresses(ips);

	for (auto& ipa : ips)
	{
		String ip = ipa.toString();
		if (ip.startsWith("100.127"))
		{
			StringArray ipSplit;
			ipSplit.addTokens(ip, ".", "");
			return "100.127." + ipSplit[2] + ".1:80";
		}
	}

	return String();
}


var LoupedeckModule::getJSONData()
{
	var data = SerialModule::getJSONData();
	data.getDynamicObject()->setProperty("shapeManager", shapeManager.getJSONData());
	return data;
}

void LoupedeckModule::loadJSONDataItemInternal(var data)
{
	shapeManager.loadJSONData(data.getProperty("shapeManager", var()));
	SerialModule::loadJSONDataItemInternal(data);
}

#pragma warning(pop)
