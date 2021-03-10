/*
  ==============================================================================

    ConversionParamValueLinkUI.cpp
    Created: 4 Mar 2020 12:04:56am
    Author:  bkupe

  ==============================================================================
*/

#include <math.h>

ConversionParamValueLinkUI::ConversionParamValueLinkUI(ConversionConnector* sourceConnector, ConversionConnector* outConnector, ConversionParamValueLink* link) :
	Component("link"),
	link(link),
    sourceConnector(sourceConnector),
	outConnector(outConnector)
{
	setRepaintsOnMouseActivity(true);
}

ConversionParamValueLinkUI::~ConversionParamValueLinkUI()
{
	setSourceConnector(nullptr);
	setOutConnector(nullptr);
}

void ConversionParamValueLinkUI::setSourceConnector(ConversionConnector* c)
{
	if (sourceConnector != nullptr)
	{
		sourceConnector->dropCandidate = false;
		sourceConnector->repaint();
	}
	sourceConnector = c;
	if (sourceConnector != nullptr)
	{
		sourceConnector->dropCandidate = true;
		sourceConnector->repaint();
	}
}

void ConversionParamValueLinkUI::setOutConnector(ConversionConnector* c)
{
	if (outConnector != nullptr)
	{
		outConnector->dropCandidate = false;
		outConnector->repaint();
	}
	outConnector = c;
	if (outConnector != nullptr)
	{
		outConnector->dropCandidate = true;
		outConnector->repaint();
	}
}

void ConversionParamValueLinkUI::mouseDown(const MouseEvent& e)
{
	if (e.mods.isRightButtonDown())
	{
		PopupMenu p;
		p.addItem(1, "Remove connection");
		int result = p.show();

		if (result == 1) link->remove();
	}
}

void ConversionParamValueLinkUI::buildHitPath()
{
	hitPath.clear();
	Array<Point<float>> firstPoints;
	Array<Point<float>> secondPoints;

	const int numPoints = 10;
	const int margin = 5;

	Array<Point<float>> points;
	for (int i = 0; i < numPoints; ++i)
	{
		points.add(linkPath.getPointAlongPath(linkPath.getLength() * i / (numPoints-1)));
	}

	for (int i = 0; i < numPoints; ++i)
	{
		Point<float> tp;
		Point<float> sp;

		if (i == 0 || i == numPoints - 1)
		{
			tp = points[i].translated(0, -margin);
			sp = points[i].translated(0, margin);
		}
		else
		{
			float angle1 = points[i].getAngleToPoint(points[i - 1]);
			float angle2 = points[i].getAngleToPoint(points[i + 1]);

			if (angle1 < 0) angle1 += float_Pi * 2;

			if (angle2 < 0) angle2 += float_Pi * 2;

			float angle = (angle1 + angle2) / 2.f;

			if (angle1 < angle2) angle += float_Pi;

			//            DBG("Point " << i << ", angle : " << angle << " >>  " << String(angle1>angle2));

			tp = points[i].getPointOnCircumference(margin, angle + float_Pi);
			sp = points[i].getPointOnCircumference(margin, angle);
		}

		if (isnan(tp.x)) return;

		firstPoints.add(tp);
		secondPoints.insert(0, sp);
	}

	hitPath.startNewSubPath(firstPoints[0]);

	for (int i = 1; i < firstPoints.size(); ++i) hitPath.lineTo(firstPoints[i]);

	for (int i = 0; i < secondPoints.size(); ++i) hitPath.lineTo(secondPoints[i]);

	hitPath.closeSubPath();
}

void ConversionParamValueLinkUI::paint(Graphics& g)
{
	ConversionConnector* c = sourceConnector != nullptr ? sourceConnector : outConnector;
	jassert(c != nullptr);
	ConversionConnector* c2 = c == sourceConnector ? outConnector : sourceConnector;
	
	Point<int> p1 = getLocalPoint(c, c->getConnectorCenter());
	Point<int> p2 = c2 != nullptr ? getLocalPoint(c2, c2->getConnectorCenter()) : getMouseXYRelative();
	int pc1 = p1.x + jmin((p2.x - p1.x) * .3f, 20.0f);
	int pc2 = p2.x - jmin((p2.x - p1.x) * .3f, 20.0f);

	Path p;
	p.startNewSubPath(p1.toFloat());
	p.lineTo(pc1, p1.y);
	p.lineTo(pc2, p2.y);
	p.lineTo(p2.x, p2.y);
	linkPath = p.createPathWithRoundedCorners(4);
	g.setColour(isMouseOverOrDragging() ? HIGHLIGHT_COLOR : NORMAL_COLOR.brighter());
	g.strokePath(linkPath, PathStrokeType(2));

	if (linkPath.getLength() > 10)
	{
		buildHitPath();
		//g.setColour(isMouseOverOrDragging() ? HIGHLIGHT_COLOR : Colours::purple);
		//g.strokePath(hitPath, PathStrokeType(1));
	}
}



bool ConversionParamValueLinkUI::hitTest(int x, int y)
{
	return hitPath.contains(x, y);
}
