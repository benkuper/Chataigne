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
	
	bgColor = item->color->getColor().darker().withAlpha(item->bgAlpha->floatValue());

	textUI.setJustificationType(Justification::centred);
	textUI.setFont(item->size->floatValue());
	textUI.setEditable(false, true, false);
	textUI.setColour(textUI.textColourId, item->color->getColor());
	textUI.setColour(textUI.backgroundColourId, Colours::transparentWhite);
	textUI.setColour(textUI.backgroundWhenEditingColourId, Colours::darkgrey.withAlpha(.2f));
	textUI.setText(item->text->stringValue(), dontSendNotification);
	textUI.addListener(this);

	addAndMakeVisible(&textUI);
}

CommentUI::~CommentUI()
{
}

void CommentUI::paint(Graphics & g)
{
	BaseItemMinimalUI::paint(g);
	

	//if (inspectable.wasObjectDeleted()) return;
	//
	//Colour c = item->color->getColor();
	//float bgAlpha = item->bgAlpha->floatValue();
	//
	//g.setColour(c.darker().withAlpha(bgAlpha));
	//g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	//g.setColour(c.withAlpha(bgAlpha));
	//g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 2);
	// 
	//g.setColour(c);
	//g.setFont(item->size->floatValue());
	//g.drawText(item->text->stringValue(), getLocalBounds().toFloat(), Justification::centred);
}

void CommentUI::resized()
{
	textUI.setBounds(getLocalBounds());
}

void CommentUI::labelTextChanged(Label *)
{
	item->text->setValue(textUI.getText());
}

bool CommentUI::canStartDrag(const MouseEvent & e)
{
	return e.eventComponent == this || e.eventComponent == &textUI;
}

void CommentUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	BaseItemMinimalUI::controllableFeedbackUpdateInternal(c);

	if (c == item->text) textUI.setText(item->text->stringValue(), dontSendNotification);
	else if(c == item->size)
	{
		int size = Font(item->size->floatValue()).getStringWidth(item->text->stringValue());
		textUI.setFont(item->size->floatValue());
		setSize(size+8, item->size->floatValue()+8);
	}
	else if (c == item->color || c == item->bgAlpha)
	{
		bgColor = item->color->getColor().darker().withAlpha(item->bgAlpha->floatValue());
		textUI.setColour(textUI.textColourId, item->color->getColor());
		repaint();
	}
}
