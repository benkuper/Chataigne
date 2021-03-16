#include "LoupedeckModule.h"
/*
  ==============================================================================

	LoupedeckModule.cpp
	Created: 16 Mar 2021 6:38:55pm
	Author:  bkupe

  ==============================================================================
*/

LoupedeckModule::LoupedeckModule() :
	WebSocketClientModule(getDefaultTypeString(), "100.127.1.1:80"),
	buttonParamsCC("Buttons"),
	sliderParamsCC("Sliders"),
	padParamsCC("Pads"),
	buttonsCC("Buttons"),
	knobsCC("Knobs"),
	touchScreenCC("Touch Screen"),
	slidersCC("Sliders"),
	padsCC("Pads")
{
	model = moduleParams.addEnumParameter("Model", "Model of the Loupedeck"); //not used right now
	model->addOption("Loupedeck Live", 0)->addOption("Loupedeck CT", 1);

	screens.add({ 0x004C, 60, 270 }); //left
	screens.add({ 0x0052, 60, 270 }); //right
	screens.add({ 0x0041, 360, 270 });//middle
	//screens.add({0x0057,240, ,240, circular = true}); //wheel

	brightness = moduleParams.addFloatParameter("Screen Brightness", "Backlight intensity", .5f, 0, 1);
	knobSensitivity = moduleParams.addFloatParameter("Knob sensitivity", "Sensitivity for the relative computing of the knobs", .5f, 0, 1);

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
		ColorParameter* c = sliderParamsCC.addColorParameter(sliderNames[i] + " Color", "Color for this slider", Colours::black);
		sliderColors.add(c);

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

	streamingType->setValueWithData(RAW);
	streamingType->hideInEditor = true;

	autoAdd->hideInEditor = true;
	autoAdd->setValue(false);

	messageStructure->hideInEditor = true;
	firstValueIsTheName->hideInEditor = true;

	serverPath->hideInEditor = true;
	serverPath->setValue(getLoupedeckServerPath());

	defManager->clear();

	moduleParams.addChildControllableContainer(&buttonParamsCC);
	moduleParams.addChildControllableContainer(&sliderParamsCC);
	moduleParams.addChildControllableContainer(&padParamsCC);

	valuesCC.addChildControllableContainer(&buttonsCC);
	valuesCC.addChildControllableContainer(&knobsCC);
	valuesCC.addChildControllableContainer(&touchScreenCC);
	valuesCC.addChildControllableContainer(&slidersCC);
	valuesCC.addChildControllableContainer(&padsCC);

}

LoupedeckModule::~LoupedeckModule()
{
}

void LoupedeckModule::dataReceived(const MemoryBlock& data)
{
	WebSocketClientModule::dataReceived(data);

	Array<uint8> bytes((const uint8*)data.getData(), data.getSize());
	uint8 id = bytes[0];

	switch (id)
	{
	case 4:

		break;

	case 5:
	{
		int pot = bytes[3] - 1;

		if (pot >= 6)
		{
			buttons[pot - 6]->setValue(bytes[4] == 0);
		}
		else
		{
			if (bytes[1])
			{
				float val = bytes[4] == 255 ? -1 : 1;
				knobRelatives[pot]->setValue(val);
				knobAbsolutes[pot]->setValue(knobAbsolutes[pot]->floatValue() + val * knobSensitivity->floatValue());
			}
			else
			{
				knobButtons[pot]->setValue(bytes[4] == 0);
			}
		}
	}
	break;

	case 9:
		processTouchData(bytes);
		break;

	}
}

void LoupedeckModule::processTouchData(Array<uint8_t> data)
{
	bool touched = data[1] == 77;

	int touchId = data[3];
	int x = (data[4] << 8) | data[5];
	int y = (data[6] << 8) | data[7];
	//int pressure = data[8];

	int btID = getPadIDForPos(Point<int>(x, y));

	if (touched)
	{
		if (posOnTouch.x < 0 && posOnTouch.y < 0)
		{
			posOnTouch.setXY(x, y);
		}
		else
		{
			btID = getPadIDForPos(posOnTouch);
		}
	}
	else
	{
		btID = getPadIDForPos(posOnTouch);
	}

	if (touched)
	{
		if (btID == -1)
		{
			sliderTouches[0]->setValue(true);
			sliderAbsolutes[0]->setValue(1 - (y / 270.0f));
		}
		else if (btID == -2)
		{
			sliderTouches[1]->setValue(true);
			sliderAbsolutes[1]->setValue(1 - (y / 270.0f));
		}
		else
		{
			if (!pads[btID - 1]->boolValue())
			{
				pads[btID - 1]->setValue(true);
				padsTouchPositions[btID - 1]->setPoint(0, 0);
			}
			else
			{
				Point<float> diff = (Point<int>(x, y) - posOnTouch).toFloat() / Point<float>(460, 270);
				padsTouchPositions[btID - 1]->setPoint(diff);
			}
		}
	}
	else
	{
		if (btID == -1) //slider1
		{
			sliderTouches[0]->setValue(false);
		}
		else if (btID == -2) //slider2
		{
			sliderTouches[1]->setValue(false);
		}
		else //middle
		{
			int btID = getPadIDForPos(posOnTouch);
			if (btID >= 1) pads[btID - 1]->setValue(false);
		}
		posOnTouch.setXY(-1, -1);
	}



}

void LoupedeckModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	if (cc == &moduleParams)
	{
		if (c == brightness)
		{
			sendLoupedeckCommand(BacklightLevel, { (uint8)(brightness->floatValue() * 10) });
		}
		else if (c->parentContainer == &buttonParamsCC)
		{
			if (ColorParameter* colP = dynamic_cast<ColorParameter*>(c))
			{
				Colour col = colP->getColor();
				col.withMultipliedBrightness(col.getAlpha());
				uint8 index = buttonColors.indexOf(colP) + 7;
				sendLoupedeckCommand(ButtonColor, { index, col.getRed(), col.getGreen(), col.getBlue() });
			}
		}
		else if (c->parentContainer == &padParamsCC)
		{
			int id = -1;
			if (ColorParameter* colP = dynamic_cast<ColorParameter*>(c)) id = padColors.indexOf(colP);
			else if (FileParameter* fp = dynamic_cast<FileParameter*>(c)) id = padImages.indexOf(fp);
			else if (StringParameter* tp = dynamic_cast<FileParameter*>(c)) id = padTexts.indexOf(tp);

			if (id != -1) updatePadContent(id+1); //1-based
		}
	}
}

void LoupedeckModule::sendLoupedeckCommand(LDCommand command, Array<uint8> data)
{
	Array<uint8> dataToSend;
	dataToSend.add((command >> 8) & 0xFF);
	dataToSend.add(command & 0xFF);
	dataToSend.add(command & 0xFF); //callback
	dataToSend.addArray(data);

	sendBytes(dataToSend);
}

void LoupedeckModule::updatePadContent(int padID)
{
	Rectangle<int> r = getPadCoords(padID).translated(-60, 0);

	LDScreen screen = screens[2];

	//updateScreenImage(, imageBytes, { x = x, y = y, w = 90,h = 90 }, callbackFn)

	Array<uint8> data = { (screen.id >> 8) & 0xFF, screen.id & 0xFF, 
		(r.getX() >> 8) & 0xFF, r.getX() & 0xFF, 
		(r.getY() >> 8) & 0xFF, r.getY() & 0xFF, 
		(r.getWidth() >> 8) & 0xFF, r.getWidth() & 0xFF, 
		(r.getHeight() >> 8) & 0xFF, r.getHeight() & 0xFF
		,0x00
	};

	Colour bgColor = padColors[padID-1]->getColor();

	for (int x = 0; x< r.getWidth(); x++)
	{
		for (int y = 0; y < r.getHeight(); y++)
		{
			int r = bgColor.getRed();
			int g = bgColor.getGreen();
			int b = bgColor.getBlue();
			short rgb565 = (((r & 0xf8) << 8) + ((g & 0xfc) << 3) + (b >> 3));

			data.add((rgb565 >> 8) & 0xFF);
			data.add(rgb565 & 0xFF);
		}
	}

	sendLoupedeckCommand(ScreenImage, data);

	sendLoupedeckCommand(RefreshScreen, {(screen.id >> 8) & 0xFF, screen.id & 0xFF});
}

int LoupedeckModule::getPadIDForPos(Point<int> pos)
{
	if (pos.x <= 60) return -1;
	if (pos.x >= 400) return -2;

	int tx = floorf((pos.x - 60) / 90);
	int ty = floor(pos.y / 90);
	return (tx + ty * 4) + 1; //1 based
}

Rectangle<int> LoupedeckModule::getPadCoords(int buttonID)
{
	//1 based
	int tx = (buttonID - 1) % 4;
	int ty = floorf((buttonID - 1) / 4);
	return Rectangle<int>(60 + tx * 90, ty * 90, 90, 90);
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

void LoupedeckModule::timerCallback()
{
	if (!isConnected->boolValue())
	{
		String path = getLoupedeckServerPath();
		if (path.isEmpty()) return;


		if (path != serverPath->stringValue())
		{
			serverPath->setValue(path);
			return;
		}
	}

	WebSocketClientModule::timerCallback();
}
