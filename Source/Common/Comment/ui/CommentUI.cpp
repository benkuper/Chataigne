/*
  ==============================================================================

    CommentUI.cpp
    Created: 4 Apr 2019 9:39:30am
    Author:  bkupe

  ==============================================================================
*/

#include "CommentUI.h"

CommentUI::CommentUI(Comment * comment) :
	BaseItemMinimalUI(comment)
{
	int size = Font(item->size->floatValue()).getStringWidth(item->text->stringValue());
	setSize(size + 8, item->size->floatValue() + 8);
}

CommentUI::~CommentUI()
{
}

void CommentUI::paint(Graphics & g)
{
	if (inspectable.wasObjectDeleted()) return;

	Colour c = item->color->getColor();
	float bgAlpha = item->bgAlpha->floatValue();
	
	g.setColour(c.darker().withAlpha(bgAlpha));
	g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	g.setColour(c.withAlpha(bgAlpha));
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 2);
	 
	g.setColour(c);
	g.setFont(item->size->floatValue());
	g.drawText(item->text->stringValue(), getLocalBounds().toFloat(), Justification::centred);
}

void CommentUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	BaseItemMinimalUI::controllableFeedbackUpdateInternal(c);

	if (c == item->text || c == item->size)
	{
		int size = Font(item->size->floatValue()).getStringWidth(item->text->stringValue());
		setSize(size+8, item->size->floatValue()+8);
	}
	else if (c == item->color || c == item->bgAlpha)
	{
		repaint();
	}
}
