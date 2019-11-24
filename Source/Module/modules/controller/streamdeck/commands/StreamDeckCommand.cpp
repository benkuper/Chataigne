/*
  ==============================================================================

    StreamDeckCommand.cpp
    Created: 23 Nov 2019 5:21:43pm
    Author:  bkupe

  ==============================================================================
*/

#include "StreamDeckCommand.h"

StreamDeckCommand::StreamDeckCommand(StreamDeckModule* _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	streamDeckModule(_module),
	buttonID(nullptr),
	valueParam(nullptr)
{
	action = (StreamDeckAction)(int)params.getProperty("action", SET_COLOR);

	if (action == SET_COLOR || action == SET_IMAGE)
	{
		buttonID = addIntParameter("Button ID", "The button to set", 1, 1, streamDeckModule->deviceType->getValueData());
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

	addTargetMappingParameterAt(valueParam, 0);
}

StreamDeckCommand::~StreamDeckCommand()
{
}


void StreamDeckCommand::triggerInternal()
{
	switch (action)
	{
	case SET_COLOR:
		if(streamDeckModule->colors.size() >= buttonID->intValue() -1) streamDeckModule->colors[buttonID->intValue() - 1]->setColor(((ColorParameter*)valueParam)->getColor());
		break;

	case SET_IMAGE:
		if (streamDeckModule->images.size() >= buttonID->intValue() - 1) streamDeckModule->images[buttonID->intValue() - 1]->setValue(valueParam->stringValue());
		break;

	case SET_ALL_COLOR:
		for(auto &c : streamDeckModule->colors) c->setColor(((ColorParameter*)valueParam)->getColor());
		break;

	case SET_BRIGHTNESS:
		streamDeckModule->brightness->setValue(((FloatParameter*)valueParam)->floatValue());
		break;

	}
}
