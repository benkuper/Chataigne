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
	easing(e),
	y1(0),y2(0)
{
	setRepaintsOnMouseActivity(true);
	easing->addAsyncContainerListener(this);
}

EasingUI::~EasingUI()
{
	easing->removeAsyncContainerListener(this);
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
	/*
	if (isMouseOver())
	{
		g.setColour(Colours::purple.withAlpha(.2f));
		g.fillPath(hitPath);
	}
	*/
	Colour c = FRONT_COLOR;
	if (isMouseOver()) c = HIGHLIGHT_COLOR;
	//if (k1 == currentUI) c = c.brighter();
	g.setColour(c);
	g.strokePath(drawPath, PathStrokeType(isMouseOver()?2:1));

	paintInternal(g);
	//g.setColour(Colours::purple);
	//g.strokePath(hitPath, PathStrokeType(1));
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
}

void EasingUI::buildHitPath()
{
	Array<Point<float>> hitPoints;
	const int numPoints = 10;
	for (int i = 0; i <= numPoints; i++)
	{
		hitPoints.add(drawPath.getPointAlongPath(drawPath.getLength()*i / (numPoints-1)));
	}

	const float margin = 5;

	hitPath.clear();
	Array<Point<float>> firstPoints;
	Array<Point<float>> secondPoints;
	for (int i = 0; i < numPoints; i++)
	{
		Point<float> tp;
		Point<float> sp;
		if (i == 0 || i == numPoints - 1)
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

	hitPath.startNewSubPath(firstPoints[0]);
	for (int i = 1; i < firstPoints.size(); i++) hitPath.lineTo(firstPoints[i]);
	for (int i = 0; i < secondPoints.size(); i++) hitPath.lineTo(secondPoints[i]);
	hitPath.closeSubPath();
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

}

void CubicEasingUI::generatePathInternal()
{
	Point<int> p1 = Point<int>(0, y1);
	Point<int> p2 = Point<int>(getWidth(), y2);

	CubicEasing * ce = static_cast<CubicEasing *>(easing);
	
	Point<float> a = Point<float>(jmap<float>(ce->anchor1->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor1->getPoint().y, p1.y, p2.y));
	Point<float> b = Point<float>(jmap<float>(ce->anchor2->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor2->getPoint().y, p1.y, p2.y));

	drawPath.cubicTo(a, b, p2.toFloat());

	
}

void CubicEasingUI::paintInternal(Graphics & g)
{
	CubicEasing * ce = static_cast<CubicEasing *>(easing);
	Point<int> p1 = Point<int>(0, y1);
	Point<int> p2 = Point<int>(getWidth(), y2);
	Point<float> a = Point<float>(jmap<float>(ce->anchor1->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor1->getPoint().y, p1.y, p2.y));
	Point<float> b = Point<float>(jmap<float>(ce->anchor2->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor2->getPoint().y, p1.y, p2.y));

	g.setColour(Colours::red);
	g.drawLine(p1.x, p1.y, a.x, a.y);
	g.fillEllipse(Rectangle<int>(0, 0, 5, 5).toFloat().withCentre(a));
	g.setColour(Colours::blue);
	g.drawLine(p2.x, p2.y, b.x, b.y);
	g.fillEllipse(Rectangle<int>(0, 0, 5, 5).toFloat().withCentre(b));
}

void CubicEasingUI::easingControllableFeedbackUpdate(Controllable * c)
{
	CubicEasing * ce = static_cast<CubicEasing *>(easing);
	if (c == ce->anchor1 || c == ce->anchor2)
	{
		//repaint();
	}
}
