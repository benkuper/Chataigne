/*
  ==============================================================================

	GuideComponents.cpp
	Created: 7 Nov 2018 2:57:12pm
	Author:  Ben

  ==============================================================================
*/

#include "GuideComponents.h"

GuideFocusComponent::GuideFocusComponent()
{
	setAlpha(0);
}

GuideFocusComponent::~GuideFocusComponent()
{
}

bool GuideFocusComponent::hitTest(int x, int y)
{
	Point<int> p(x, y);
	return getLocalBounds().contains(p) && !area.contains(p);
}

void GuideFocusComponent::show(Rectangle<int> _area, FocusLook _look, const String &_text)
{
	this->look = _look;
	this->area = _area;
	this->text = _text;
	
	resized();
	repaint(); 
	
	animator.fadeIn(this, 1000);
}

void GuideFocusComponent::setFocus(Rectangle<int> _area, FocusLook _look, const String &_text)
{
	
	if (text.isNotEmpty())
	{
		animator.fadeOut(this, 500);
		auto showFunc = std::bind(&GuideFocusComponent::show, this, _area, _look, _text);
		Timer::callAfterDelay(500, showFunc);
	} else
	{
		show(_area, _look, _text);
	}
}

void GuideFocusComponent::resized()
{
	path.clear();
	path.addRectangle(getLocalBounds());
	path.setUsingNonZeroWinding(false);
	
	switch (look)
	{
		case RECTANGLE: path.addRectangle(area.toFloat()); break;
		case CIRCLE: path.addEllipse(area.toFloat()); break;
	}
}

void GuideFocusComponent::paint(Graphics & g)
{
	g.setColour(Colours::black.withAlpha(.6f));
	g.fillPath(path);

	g.setColour(Colours::orange);
	switch (look)
	{
	case RECTANGLE: g.drawRect(area.toFloat(),3); break;
	case CIRCLE: g.drawEllipse(area.toFloat(),3); break;
	}

	g.setColour(TEXT_COLOR);
	g.setFont(50);
	g.drawFittedText(text, getLocalBounds(), Justification::centred, 5);
}
