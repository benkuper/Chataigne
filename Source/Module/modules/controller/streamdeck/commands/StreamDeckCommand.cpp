/*
  ==============================================================================

	StreamDeckCommand.cpp
	Created: 23 Nov 2019 5:21:43pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

StreamDeckCommand::StreamDeckCommand(StreamDeckModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	streamDeckModule(_module),
	row(nullptr),
	column(nullptr),
	valueParam(nullptr)
{
	action = (StreamDeckAction)(int)params.getProperty("action", SET_COLOR);

	if (action == SET_COLOR || action == SET_IMAGE || action == SET_TEXT)
	{
		column = addIntParameter("Column", "The column of the button to set", 1, 1, STREAMDECK_MAX_COLUMNS);
		row = addIntParameter("Row", "The row of button to set", 1, 1, STREAMDECK_MAX_ROWS);
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
		valueParam = addStringParameter("Text", "The text to set the button to", "");
		break;

	case SET_BRIGHTNESS:
		valueParam = addFloatParameter("Brightness", "The brightness to set to", .5f, 0, 1);
		break;
	default:
		break;
	}

	linkParamToMappingIndex(valueParam, 0);
}

StreamDeckCommand::~StreamDeckCommand()
{
}


void StreamDeckCommand::triggerInternal(int multiplexIndex)
{
	int r = row != nullptr ? jmax((int)getLinkedValue(row, multiplexIndex) - 1, 0) : 0;
	int c = column != nullptr ? jmax((int)getLinkedValue(column, multiplexIndex) - 1, 0) : 0;

	var val = getLinkedValue(valueParam, multiplexIndex);

	switch (action)
	{
	case SET_COLOR:
		if (val.size() >= 4) streamDeckModule->setColor(r, c, Colour::fromFloatRGBA(val[0], val[1], val[2], val[3]));
		break;

	case SET_IMAGE:
		streamDeckModule->setImage(r, c, val);
		break;

	case SET_ALL_COLOR:if (val.size() >= 4)
		if (val.size() >= 4) streamDeckModule->setAllColor(Colour::fromFloatRGBA(val[0], val[1], val[2], val[3]));
		break;

	case SET_BRIGHTNESS:
		streamDeckModule->brightness->setValue(val);
		break;

	case SET_TEXT:
		streamDeckModule->setText(r, c, val);
		break;

	case CLEAR_TEXTS:
		streamDeckModule->clearTexts();
		break;
	}
}
