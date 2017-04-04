/*
  ==============================================================================

    EasingUI.cpp
    Created: 16 Dec 2016 7:13:11pm
    Author:  Ben

  ==============================================================================
*/

#include "EasingUI.h"
#include "Style.h"

#pragma warning(disable:4244)

EasingUI::EasingUI(Easing * e) :
	InspectableContentComponent(e),
	easing(e),
	y1(0),y2(0)
{
	autoDrawHighlightWhenSelected = false;
	bringToFrontOnSelect = false; 
	setRepaintsOnMouseActivity(true);
	easing->addAsyncContainerListener(this);
}

EasingUI::~EasingUI()
{
	if(!easing.wasObjectDeleted()) easing->removeAsyncContainerListener(this);
}

void EasingUI::setKeyPositions(const int &k1, const int &k2)
{
	y1 = k1;
	y2 = k2;
	generatePath();
	repaint();
}

void EasingUI::paint(Graphics &g)
{
	Colour c = FRONT_COLOR;
	if (easing->isSelected) c = HIGHLIGHT_COLOR;
	if (isMouseOver()) c = c.brighter();

	g.setColour(c);
	g.strokePath(drawPath, PathStrokeType(isMouseOver()?2:1));

	//g.setColour(Colours::purple);
	//g.strokePath(hitPath, PathStrokeType(2));

	paintInternal(g);
}

void EasingUI::resized()
{
	generatePath();
}

void EasingUI::generatePath()
{
	drawPath.clear();
	drawPath.startNewSubPath(0, y1); 
	generatePathInternal();

	if(drawPath.getLength()) buildHitPath();

	repaint();
}

void EasingUI::generatePathInternal()
{
	autoGeneratePathWithPrecision();
}

void EasingUI::autoGeneratePathWithPrecision(int precision)
{
	if (getHeight() == 0) return;
	if (precision == 0) precision = getWidth();
	else precision = jmin(getWidth(), precision);

	for (int i = 1; i <= precision; i++)
	{
		float t = i*1.f / precision;
		float v1 = 1 - (y1*1.f / getHeight());
		float v2 = 1 - (y2*1.f / getHeight());
		float v = easing->getValue(v1,v2, t);
		drawPath.lineTo(t*getWidth(), (1-v)*getHeight());
		
	}
}

void EasingUI::buildHitPath()
{
	Array<Point<float>> hitPoints;
	
	for (int i = 0; i <= hitPathPrecision; i++)
	{
		hitPoints.add(drawPath.getPointAlongPath(drawPath.getLength()*i / (hitPathPrecision -1)));
	}

	const float margin = 5;

	hitPath.clear();
	Array<Point<float>> firstPoints;
	Array<Point<float>> secondPoints;
	for (int i = 0; i < hitPathPrecision; i++)
	{
		Point<float> tp;
		Point<float> sp;
		if (i == 0 || i == hitPathPrecision - 1)
		{
			tp = hitPoints[i].translated(0, -margin);
			sp = hitPoints[i].translated(0, margin);
		}
		else
		{
			float angle1 = 0; 
			float angle2 = 0;
			/*if (hitPoints[i].x == hitPoints[i - 1].x) angle1 = 0;
			else if (hitPoints[i].y == hitPoints[i - 1].y) angle1 = -float_Pi / 4;
			else */ angle1 = hitPoints[i].getAngleToPoint(hitPoints[i - 1]);

			/*if (hitPoints[i].x == hitPoints[i - 1].x) angle1 = 0;
			else if (hitPoints[i].y == hitPoints[i - 1].y) angle1 = -float_Pi / 4;
			else */angle2 = hitPoints[i].getAngleToPoint(hitPoints[i + 1]);

		
			if (angle1 < 0) angle1 += float_Pi * 2;
			if (angle2 < 0) angle2 += float_Pi * 2;

			float angle = (angle1 + angle2) / 2.f;

			if (angle1 < angle2) angle += float_Pi;

			tp = hitPoints[i].getPointOnCircumference(margin, angle + float_Pi);
			sp = hitPoints[i].getPointOnCircumference(margin, angle);
		}

		firstPoints.add(tp);
		secondPoints.insert(0, sp);
	}

	if (firstPoints.size() > 1)
	{
		hitPath.startNewSubPath(firstPoints[0]);
		for (int i = 1; i < firstPoints.size(); i++) hitPath.lineTo(firstPoints[i]);
		for (int i = 0; i < secondPoints.size(); i++) hitPath.lineTo(secondPoints[i]);
		hitPath.closeSubPath();
	}
}

bool EasingUI::hitTest(int tx, int ty)
{
	return hitPath.contains((float)tx, (float)ty);
}

/*
void EasingUI::resized()
{
	//
}
*/

void EasingUI::newMessage(const ContainerAsyncEvent & e)
{
	if (e.type == ContainerAsyncEvent::ControllableFeedbackUpdate)
	{
		easingControllableFeedbackUpdate(e.targetControllable);
	}
}



LinearEasingUI::LinearEasingUI(LinearEasing * e) :
	EasingUI(e)
{
	
}

void LinearEasingUI::generatePathInternal()
{
	
	drawPath.lineTo(getWidth(), y2);
}

HoldEasingUI::HoldEasingUI(HoldEasing * e) :
	EasingUI(e)
{

}

void HoldEasingUI::generatePathInternal()
{
	drawPath.lineTo(getWidth(), y1);
	drawPath.lineTo(getWidth(), y2);

}

CubicEasingUI::CubicEasingUI(CubicEasing * e) :
	EasingUI(e)
{
	addChildComponent(h1);
	addChildComponent(h2);
	h1.setVisible(easing->isSelected);
	h2.setVisible(easing->isSelected);

	h1.addMouseListener(this,false);
	h2.addMouseListener(this,false);
}

bool CubicEasingUI::hitTest(int tx, int ty)
{
	bool result = EasingUI::hitTest(tx, ty);

	if(easing->isSelected)
	{
		if (&h1 != nullptr) result |= h1.getLocalBounds().contains(h1.getMouseXYRelative());
		if(&h2 != nullptr) result |= h2.getLocalBounds().contains(h2.getMouseXYRelative());
	}

	return result;
}

void CubicEasingUI::resized()
{
	Point<int> p1 = Point<int>(0, y1);
	Point<int> p2 = Point<int>(getWidth(), y2);

	CubicEasing * ce = static_cast<CubicEasing *>(easing.get());

	Point<float> a = Point<float>(jmap<float>(ce->anchor1->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor1->getPoint().y, p1.y, p2.y));
	Point<float> b = Point<float>(jmap<float>(ce->anchor2->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor2->getPoint().y, p1.y, p2.y));
	 
	h1.setBounds(Rectangle<int>(0, 0, 16,16).withCentre(a.toInt()));
	h2.setBounds(Rectangle<int>(0, 0, 16,16).withCentre(b.toInt()));
}

void CubicEasingUI::generatePathInternal()
{
	Point<int> p1 = Point<int>(0, y1);
	Point<int> p2 = Point<int>(getWidth(), y2);

	CubicEasing * ce = static_cast<CubicEasing *>(easing.get());
	
	Point<float> a = Point<float>(jmap<float>(ce->anchor1->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor1->getPoint().y, p1.y, p2.y));
	Point<float> b = Point<float>(jmap<float>(ce->anchor2->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor2->getPoint().y, p1.y, p2.y));

	drawPath.cubicTo(a, b, p2.toFloat());
}

void CubicEasingUI::paintInternal(Graphics & g)
{ 
	if (!easing->isSelected) return;

	Point<int> p1 = Point<int>(0, y1);
	Point<int> p2 = Point<int>(getWidth(), y2);

	Colour c = LIGHTCONTOUR_COLOR;
	if (isMouseOver()) c = c.brighter();
	g.setColour(c);

	g.drawLine(p1.x, p1.y, h1.getBounds().getCentreX(), h1.getBounds().getCentreY());
	g.drawLine(p2.x, p2.y, h2.getBounds().getCentreX(), h2.getBounds().getCentreY());
	
}

void CubicEasingUI::inspectableSelectionChanged(Inspectable *)
{
	if (easing.wasObjectDeleted()) return;
	h1.setVisible(easing->isSelected);
	h2.setVisible(easing->isSelected);
	resized();
	repaint();
}

void CubicEasingUI::easingControllableFeedbackUpdate(Controllable * c)
{
	CubicEasing * ce = static_cast<CubicEasing *>(easing.get());
	if (c == ce->anchor1 || c == ce->anchor2)
	{
		generatePath();
		resized();
	}
}

void CubicEasingUI::mouseDrag(const MouseEvent & e)
{
	if (e.eventComponent == &h1 || e.eventComponent == &h2)
	{
		CubicEasing * ce = static_cast<CubicEasing *>(easing.get());
		
		Point2DParameter * targetAnchor = (e.eventComponent == &h1) ? ce->anchor1 : ce->anchor2;
		Point<int> mp = e.getEventRelativeTo(this).getPosition();
		
		Point<float> targetPoint = Point<float>(mp.x*1.f/ getWidth(), jmap<float>(mp.y,y1,y2,0,1));
		targetAnchor->setPoint(targetPoint);
	}
	else
	{
		CubicEasing * ce = static_cast<CubicEasing *>(easing.get());

		Point<int> p1 = Point<int>(0, y1);
		Point<int> p2 = Point<int>(getWidth(), y2);
		Point<int> mp = e.getEventRelativeTo(this).getPosition();
		Point<int> mp1;
		Point<int> mp2;
		
		if (e.mods.isShiftDown())
		{
			mp1 = Point<int>(mp.x, p1.y);
			mp2 = Point<int>(p2.x - mp.x, p2.y);
		}
		else if (e.mods.isAltDown())
		{
			mp1 = Point<int>(p1.x, mp.y);
			mp2 = Point<int>(p2.x, p1.y + (p2.y-mp.y));
		}
		else
		{
			mp1 = (p1 + mp) / 2;
			mp2 = (p2 + mp) / 2;
		}
		
		Point<float> t1 = Point<float>(mp1.x*1.f / getWidth(), jmap<float>(mp1.y, y1, y2, 0, 1));
		Point<float> t2 = Point<float>(mp2.x*1.f / getWidth(), jmap<float>(mp2.y, y1, y2, 0, 1));

		ce->anchor1->setPoint(t1);
		ce->anchor2->setPoint(t2);
	}
}


EasingUI::EasingHandle::EasingHandle() 
{
	setRepaintsOnMouseActivity(true);
}

void EasingUI::EasingHandle::paint(Graphics & g)
{
	Colour c =LIGHTCONTOUR_COLOR;
	if (isMouseOver()) c = c.brighter(.8f);
	g.setColour(c);
	g.fillEllipse(getLocalBounds().reduced(isMouseOver()?3:5).toFloat());
}

SineEasingUI::SineEasingUI(SineEasing * e) :
	EasingUI(e)
{
	hitPathPrecision = jmin<float>(1 / e->freqAmp->floatValue(), 100);
	generatePath();

	addChildComponent(h1);
	h1.setVisible(easing->isSelected);
	h1.addMouseListener(this, false);
}

bool SineEasingUI::hitTest(int tx, int ty)
{
	bool result = EasingUI::hitTest(tx, ty);

	if (easing->isSelected)
	{
		if (&h1 != nullptr) result |= h1.getLocalBounds().contains(h1.getMouseXYRelative());
	}

	return result;
}

void SineEasingUI::resized()
{
	Point<int> p1 = Point<int>(0, y1);
	Point<int> p2 = Point<int>(getWidth(), y2);

	SineEasing * ce = static_cast<SineEasing *>(easing.get());

	Point<float> a = Point<float>(jmap<float>(ce->freqAmp->getPoint().x, p1.x, p2.x), jmap<float>(ce->freqAmp->getPoint().y, p1.y, p2.y));

	h1.setBounds(Rectangle<int>(0, 0, 16, 16).withCentre(a.toInt()));
}

void SineEasingUI::paintInternal(Graphics & g)
{
	if (!easing->isSelected) return;

	Colour c = LIGHTCONTOUR_COLOR;
	if (isMouseOver()) c = c.brighter();
	g.setColour(c);

	Point<int> hp = h1.getBounds().getCentre();
	g.drawLine(0, y1, hp.x, y1);
	g.drawLine(hp.x, y1, hp.x,hp.y,2);
}

void SineEasingUI::inspectableSelectionChanged(Inspectable *)
{
	if (easing.wasObjectDeleted()) return;
	h1.setVisible(easing->isSelected);
	resized();
	repaint();
}

void SineEasingUI::easingControllableFeedbackUpdate(Controllable  * c)
{
	SineEasing * ce = static_cast<SineEasing *>(easing.get());
	if (c == ce->freqAmp)
	{
		hitPathPrecision = jmin<float>(10 / ce->freqAmp->floatValue(),100);
		generatePath();
		resized();
	}
}

void SineEasingUI::mouseDrag(const MouseEvent & e)
{
	if (e.eventComponent == &h1)
	{
		CubicEasing * ce = static_cast<CubicEasing *>(easing.get());

		Point2DParameter * targetAnchor = (e.eventComponent == &h1) ? ce->anchor1 : ce->anchor2;
		Point<int> mp = e.getEventRelativeTo(this).getPosition();

		Point<float> targetPoint = Point<float>(mp.x*1.f / getWidth(), jmap<float>(mp.y, y1, y2, 0, 1));
		targetAnchor->setPoint(targetPoint);
	}
}
