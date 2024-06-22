/*
  ==============================================================================

	GuideComponents.cpp
	Created: 7 Nov 2018 2:57:12pm
	Author:  Ben

  ==============================================================================
*/

GuideFocusComponent::GuideFocusComponent() :
	nextBT("NextBT")
{
	nextBT.addListener(this);
	nextBT.setWantsKeyboardFocus(false);
	addChildComponent(&nextBT);

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

void GuideFocusComponent::show(Rectangle<int> _area, FocusLook _look, const String &_text, const String& _nextText)
{
	this->look = _look;
	this->area = _area;
	this->text = _text;
	
	if (_nextText.isNotEmpty()) nextBT.setButtonText(_nextText);
	nextBT.setVisible(_nextText.isNotEmpty());

	resized();
	repaint(); 
	
	animator.fadeIn(this, 500);
}

void GuideFocusComponent::setFocus(Rectangle<int> _area, FocusLook _look, const String &_text, const String &_nextText)
{
	if (text.isNotEmpty())
	{
		animator.fadeOut(this, 300);
		auto showFunc = std::bind(&GuideFocusComponent::show, this, _area, _look, _text, _nextText);
		Timer::callAfterDelay(300, showFunc);
	} else
	{
		show(_area, _look, _text, _nextText);
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
		default:break;
	}

	if (nextBT.isVisible()) nextBT.setBounds(getLocalBounds().removeFromBottom(getHeight() / 3).withSizeKeepingCentre(200, 60));
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
	default:break;
	}

	g.setColour(TEXT_COLOR);
	g.setFont(FontOptions(30));
	g.drawFittedText(text, getLocalBounds().reduced(30), Justification::centred, 5);
}

void GuideFocusComponent::buttonClicked(Button* b)
{
	if (b == &nextBT) guideComponentListeners.call(&GuideComponentListener::askForNextStep);
}
