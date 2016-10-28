/*
  ==============================================================================

    GapGrabber.cpp
    Created: 2 May 2016 5:40:46pm
    Author:  bkupe

  ==============================================================================
*/

#include "GapGrabber.h"
#include "Style.h"

GapGrabber::GapGrabber(Direction _direction) : direction(_direction)
{
	setRepaintsOnMouseActivity(true);
}

GapGrabber::~GapGrabber()
{
}

void GapGrabber::paint(Graphics & g)
{
	int grabberSize = 100;
	Rectangle<int> r = getLocalBounds().reduced(2);
	Rectangle<int> tr = (direction == Direction::HORIZONTAL) ? r.withHeight(grabberSize) : r.withWidth(grabberSize);
	tr.setCentre(r.getCentre());
	Colour c = BG_COLOR.brighter(.1f);
	if (isMouseOver()) c = HIGHLIGHT_COLOR;
	if (isMouseButtonDown()) c = Colours::yellow;

	g.setColour(c);
	g.fillRoundedRectangle(tr.toFloat(), 2);
}

void GapGrabber::mouseEnter(const MouseEvent &)
{
	setMouseCursor(direction == HORIZONTAL?MouseCursor::LeftRightResizeCursor:MouseCursor::UpDownResizeCursor);
}

void GapGrabber::mouseExit(const MouseEvent &)
{
	setMouseCursor(MouseCursor::NormalCursor);
}

void GapGrabber::mouseDrag(const MouseEvent &e)
{
	listeners.call(&Listener::grabberGrabUpdate, this, direction == HORIZONTAL?e.getPosition().x:e.getPosition().y);
}
