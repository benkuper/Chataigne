/*
  ==============================================================================

    Comment.cpp
    Created: 4 Apr 2019 9:39:09am
    Author:  bkupe

  ==============================================================================
*/

#include "Comment.h"

Comment::Comment() :
	BaseItem("Comment",false)
{
	itemDataType = "Comment";
	text = addStringParameter("Text", "The comment text", "I Love Chataigne !");
	//text->multiline = true;

	size = addFloatParameter("Size", "The text size", 14, 0,80);
	size->customUI = FloatParameter::LABEL;

	color = addColorParameter("Color", "The color of the text", TEXT_COLOR);
	bgAlpha = addFloatParameter("Background Alpha", "The alpha", 0, 0, 1);
}

Comment::~Comment()
{
}
