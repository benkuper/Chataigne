/*
  ==============================================================================

    StreamDeckCommand.cpp
    Created: 23 Nov 2019 5:21:43pm
    Author:  bkupe

  ==============================================================================
*/

#include "StreamDeckCommand.h"

StreamDeckCommand::StreamDeckCommand(StreamDeckModule* _module, CommandContext context, var params, IteratorProcessor* iterator) :
	BaseCommand(_module, context, params, iterator),
	streamDeckModule(_module),
	row(nullptr),
	column(nullptr),
	valueParam(nullptr)
{
	action = (StreamDeckAction)(int)params.getProperty("action", SET_COLOR);

	if (action == SET_COLOR || action == SET_IMAGE)
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

	case SET_BRIGHTNESS:
		valueParam = addFloatParameter("Brightness", "The brightness to set to", .5f, 0, 1);
		break;

	}

	linkParamToMappingIndex(valueParam, 0);
}

StreamDeckCommand::~StreamDeckCommand()
{
}


void StreamDeckCommand::triggerInternal(int iterationIndex)
{
	int r = row != nullptr ? row->intValue()-1 : 0;
	int c = column != nullptr ? column->intValue()-1 : 0;

	switch (action)
	{
	case SET_COLOR:
		streamDeckModule->setColor(r,c,((ColorParameter*)valueParam)->getColor());
		break;

	case SET_IMAGE:
		streamDeckModule->setImage(r, c, valueParam->stringValue());
		break;

	case SET_ALL_COLOR:
		streamDeckModule->setAllColor(((ColorParameter*)valueParam)->getColor());
		break;

	case SET_BRIGHTNESS:
		streamDeckModule->brightness->setValue(((FloatParameter*)valueParam)->floatValue());
		break;

	}
}
