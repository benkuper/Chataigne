/*
  ==============================================================================

	LoupedeckCommands.cpp
	Created: 17 Mar 2021 11:13:59am
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

LoupedeckCommand::LoupedeckCommand(LoupedeckModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	loupedeckModule(_module),
	row(nullptr),
	column(nullptr),
	valueParam(nullptr)
{
	action = (LoupedeckAction)(int)params.getProperty("action", SET_COLOR);
	screenTarget = (ScreenTarget)(int)params.getProperty("screenTarget", PADS);

	if (screenTarget == PADS && (action == SET_COLOR || action == SET_IMAGE || action == SET_TEXT))
	{
		column = addIntParameter("Column", "The column of the button to set", 1, 1, 4);
		row = addIntParameter("Row", "The row of button to set", 1, 1, 3);
	}
	else if (screenTarget == BUTTONS && action == SET_COLOR)
	{
		column = addIntParameter("Button", "The button to change", 0, 0, 7);
	}

	switch (action)
	{
	case SET_COLOR:
	case SET_ALL_COLOR:
		valueParam = addColorParameter("Color", "The color to set the button to", Colours::black);
		break;

	case SET_IMAGE:
		valueParam = addFileParameter("Image", "The image to set the button to");
		((FileParameter*)valueParam)->fileTypeFilter = "*.png; *.jpg; *.jpeg";
		break;

	case SET_TEXT:
	case SET_ALL_TEXT:
		valueParam = addStringParameter("Text", "The text to put", "Cool");
		break;

	case SET_BRIGHTNESS:
		valueParam = addFloatParameter("Brightness", "The brightness to set to", .5f, 0, 1);
		break;

	case VIBRATE:
		valueParam = addEnumParameter("Vibration Type", "The type of vibration, choose your flavor !");
		((EnumParameter*)valueParam)->addOption("One short vibration (do)", 1)
			->addOption("Two quick vibrations (do-de)", 10)
			->addOption("Three quick vibrations (do-de-de)", 12)
			->addOption("Single ramp down (dooooooooo)", 13)
			->addOption("Medium length high-pitched (veeeee)", 14)
			->addOption("Long length high-pitched (veeeeeee)", 15)
			->addOption("Really long length high-pitched (veeeeeeee)", 16)
			->addOption("Two quick vibrations (do-do)", 27)
			->addOption("Medium length high-pitched (veeeee)", 47)
			->addOption("One medium length vibration (veeeeee)", 48)
			->addOption("One medium length vibration (vuum)", 49)
			->addOption("Short deep vibration (vum)", 50)
			->addOption("Alarm (vu-ve-ve-ve-ve)", 52)
			->addOption("Deeper Alarm (vu-ve-ve-ve-ve)", 56)
			->addOption("Ramp (vuuuuuu-vu)", 58)
			->addOption("Ramp (vuuuuuu-ve)", 63)
			->addOption("Low medium vibration (vuuuuuuuu)", 64)
			->addOption("High to low ramp (veeeeoooo)", 70)
			->addOption("High to low ramp (voooooooo)", 71)
			->addOption("High to low ramp (vooooo)", 72)
			->addOption("High to low ramp (vooo)", 73)
			->addOption("High to low ramp (voo)", 74)
			->addOption("High to low ramp (vo)", 75)
			->addOption("High to low ramp (vooooooo)", 76)
			->addOption("High to low ramp (vooooo)", 77)
			->addOption("High to low ramp (voooo)", 78)
			->addOption("High to low ramp (vooo)", 79)
			->addOption("High to low ramp (voo)", 80)
			->addOption("Ramp Up (vvvvvoooooooeeeee)", 82)
			->addOption("Ramp Up (vvvvvoooooeeee)", 83)
			->addOption("Ramp Up (vvvoooeee)", 84)
			->addOption("Ramp Up (vvooee)", 85)
			->addOption("Ramp Up (vvoe)", 86)
			->addOption("Ramp Up (voe)", 87)
			->addOption("Ramp Up (.....vvvoooeeeeee)", 88)
			->addOption("Ramp Up (...vvvoooeeeeee)", 89)
			->addOption("Ramp Up (.vvvoooeeeeee)", 90)
			->addOption("Ramp Up (vvvooe)", 91)
			->addOption("Ramp Up (vvooe)", 92)
			->addOption("Ramp Up (voe)", 93)
			->addOption("Slow and low (vrrrrmmmmmmmmmmmm)", 94)
			->addOption("Slow and low (vrrrrmmmmmm)", 95)
			->addOption("Slow and low (vrrrrmmm)", 96)
			->addOption("Slow and low (vrrrmmm)", 97)
			->addOption("Almost silent (vrm)", 98)
			->addOption("Almost silent (vr)", 99)
			->addOption("Ramp Down - Low and Subtle (vvvrrrmmmmmm)", 100)
			->addOption("Ramp Down - Low and Subtle (vvvrrrmmmmm)", 101)
			->addOption("Ramp Down - Low and Subtle (vvrrmmmm)", 102)
			->addOption("Ramp Down - Low and Subtle (vvrrmmmm)", 103)
			->addOption("Ramp Down - Low and Subtle (vvrrmmmm)", 104)
			->addOption("Ramp Down - Low and Subtle (vvrrmm)", 105)
			->addOption("Ramp Down & Up (vvrrrrmmmmmeeeeee)", 106)
			->addOption("Ramp Down & Up (vvrrrrmmmmmeeeerrr)", 107)
			->addOption("Ramp Down & Up (vvrrrmmmeeerrr)", 108)
			->addOption("Ramp Down & Up (vvrrrmmmeeerrrrr)", 109)
			->addOption("Ramp Down & Up (vvvrrreee)", 110)
			->addOption("Up & Down (ver-reh)", 111)
			->addOption("Up & Down (ver-reh-reee)", 112)
			->addOption("Up & Down (veeeeer-reeeeeh-reeh)", 113)
			->addOption("Up & Down (veeeer-reeeeh)", 114)
			->addOption("Up & Down (veeer-reeeh)", 115)
			->addOption("Up & Down (veer-reeh)", 116)
			->addOption("Up & Down (veer-reeh)", 117)
			->addOption("Extremely long constant (veeeerrrrrrrr...)", 118)
			->addOption("Down & Up (verr-eehhh)", 119)
			->addOption("Down & Up (verr-eeh)", 120)
			->addOption("Down & Up (ver-ehh)", 121)
			->addOption("Down & Up (ver-ehhh)", 122)
			->addOption("Down & Up (ver-ehhhh)", 123)
			->addOption("Ramp Up (ver-eh)", 125);
		break;

	case SET_AUTO_REFRESH:
		valueParam = addBoolParameter("Value", "If checked, this will force refreshing on every change. Otherwise this will not...", false);
		break;
	default:
		break;

	}

	/*
	if (action == SET_COLOR || action == SET_IMAGE || action == SET_TEXT)
	{
		refresh = addBoolParameter("Refresh", "If checked, this will force refreshing the screen. Deactivating this when doing multiple manipulation can help optimize performance.", true);
	}
	*/

	linkParamToMappingIndex(valueParam, 0);
}

LoupedeckCommand::~LoupedeckCommand()
{
}


void LoupedeckCommand::triggerInternal(int multiplexIndex)
{
	int r = row != nullptr ? jmax((int)getLinkedValue(row, multiplexIndex) - 1, 0) : 0;
	int c = column != nullptr ? jmax((int)getLinkedValue(column, multiplexIndex) - 1, 0) : 0;

	int index = screenTarget == PADS ? r * 4 + c : screenTarget == BUTTONS ? c + 1 : (int)screenTarget;

	var val;
	if (valueParam != nullptr) val = getLinkedValue(valueParam, multiplexIndex);

	switch (action)
	{
	case SET_COLOR:
	{
		Array<ColorParameter*>* arr = screenTarget == BUTTONS ? &loupedeckModule->buttonColors : (screenTarget == PADS ? &loupedeckModule->padColors : &loupedeckModule->sliderColors);
		arr->getUnchecked(index)->setColor(Colour::fromFloatRGBA(val[0], val[1], val[2], val[3]));
	}
	break;

	case SET_IMAGE:
	{
		Array<FileParameter*>* arr = screenTarget == PADS ? &loupedeckModule->padImages : &loupedeckModule->sliderImages;
		arr->getUnchecked(index)->setValue(val);
	}
	break;

	case SET_TEXT:
	{
		Array<StringParameter*>* arr = screenTarget == PADS ? &loupedeckModule->padTexts : &loupedeckModule->sliderTexts;
		arr->getUnchecked(index)->setValue(val);
	}
	break;

	case SET_ALL_COLOR:
	{
		Colour col = Colour::fromFloatRGBA(val[0], val[1], val[2], val[3]);
		if (screenTarget == BUTTONS)
		{
			for (auto& cp : loupedeckModule->buttonColors) cp->setColor(col);
		}
		else if (screenTarget == PADS)
		{
			for (int i = 0; i < loupedeckModule->padColors.size(); i++)
			{
				loupedeckModule->padColors[i]->setColor(col);
				loupedeckModule->updatePadContent(i, false);
			}
			if (loupedeckModule->autoRefresh->boolValue()) loupedeckModule->refreshScreen(2);

		}
		else if (screenTarget == LEFT_SLIDER || screenTarget == RIGHT_SLIDER)
		{
			loupedeckModule->sliderColors[(int)screenTarget]->setColor(col);
			loupedeckModule->updateSliderContent((int)screenTarget, loupedeckModule->autoRefresh->boolValue());
		}
	}
	break;

	case SET_ALL_TEXT:
	{
		if (screenTarget == PADS)
		{
			for (int i = 0; i < loupedeckModule->padTexts.size(); i++)
			{
				loupedeckModule->padTexts[i]->setValue(val.toString());
				loupedeckModule->updatePadContent(i, false);
			}
			if (loupedeckModule->autoRefresh->boolValue()) loupedeckModule->refreshScreen(2);

		}
	}
	break;

	case SET_BRIGHTNESS:
		loupedeckModule->brightness->setValue(val);
		break;

	case VIBRATE:
		loupedeckModule->vibrate(val);
		break;

	case SET_AUTO_REFRESH:
		loupedeckModule->autoRefresh->setValue(val);
		break;

	case REFRESH_SCREEN:
		loupedeckModule->refreshScreen((int)screenTarget);
		break;
	}
}
