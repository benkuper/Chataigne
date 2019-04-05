/*
  ==============================================================================

    Comment.h
    Created: 4 Apr 2019 9:39:09am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class Comment :
	public BaseItem
{
public:
	Comment();
	~Comment();

	StringParameter * text;
	FloatParameter * size;
	ColorParameter * color;
	FloatParameter * bgAlpha;

	String getTypeString() const override { return "Comment"; }
};