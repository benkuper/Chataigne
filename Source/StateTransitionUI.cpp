/*
  ==============================================================================

    StateTransitionUI.cpp
    Created: 28 Oct 2016 8:20:06pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateTransitionUI.h"
#include "StateManagerUI.h"

StateTransitionUI::StateTransitionUI(StateTransition * st, StateViewUI * _sourceSUI, StateViewUI * _destSUI) :
	BaseItemMinimalUI(st),
	sourceSUI(_sourceSUI),
	destSUI(_destSUI)
{
	jassert(sourceSUI != nullptr && destSUI != nullptr);

	setWantsKeyboardFocus(true);

	sourceSUI->addStateViewUIListener(this);
	sourceSUI->addItemUIListener(this);
	destSUI->addStateViewUIListener(this);
	destSUI->addItemUIListener(this);
	setRepaintsOnMouseActivity(true);
	updateBounds();
}

StateTransitionUI::~StateTransitionUI()
{
	sourceSUI->removeStateViewUIListener(this);
	sourceSUI->removeItemUIListener(this);
	destSUI->removeStateViewUIListener(this);
	destSUI->removeItemUIListener(this);
}

void StateTransitionUI::updateBounds()
{
	Rectangle<int> r = getParentComponent()->getLocalArea(sourceSUI, sourceSUI->getLocalBounds());
	Rectangle<int> r2 = getParentComponent()->getLocalArea(destSUI, destSUI->getLocalBounds());
	Rectangle<int> tr = Rectangle<int>::leftTopRightBottom(jmin<int>(r.getX(), r2.getX()), jmin<int>(r.getY(), r2.getY()), jmax<int>(r.getRight(), r2.getRight()), jmax<int>(r.getBottom(), r2.getBottom()));
	setBounds(tr);
}

void StateTransitionUI::mouseDown(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDown(e);

	if (e.mods.isRightButtonDown())
	{
		PopupMenu p;
		p.addItem(1, "Remove");
		int result = p.show();
		switch (result)
		{
		case 1:
			item->remove();
			break;
		}
	}
}

void StateTransitionUI::mouseMove(const MouseEvent &e) 
{
	
    repaint();
}

bool StateTransitionUI::keyPressed(const KeyPress & e)
{
	if (item->isSelected)
	{
		if (e.getKeyCode() == e.backspaceKey || e.getKeyCode() == e.deleteKey)
		{
			item->remove();
			return true;
		}
	}
	
	return BaseItemMinimalUI::keyPressed(e);
}


void StateTransitionUI::paint(Graphics & g)
{
	if (item->sourceState.wasObjectDeleted() || item->destState.wasObjectDeleted()) return;

	Rectangle<int> sourceRect = getLocalArea(sourceSUI, sourceSUI->getLocalBounds()).expanded(2,2);
	Rectangle<int> destRect = getLocalArea(destSUI, destSUI->getLocalBounds()).expanded(2,2);
	Point<int> midPoint = (sourceRect.getCentre() + destRect.getCentre()) / 2;
	midPoint = midPoint.getPointOnCircumference(10, sourceRect.getCentre().getAngleToPoint(destRect.getCentre()) + float_Pi / 2).toInt();

	Point<int> sourceP = sourceRect.getConstrainedPoint(midPoint);
	Point<int> destP = destRect.getConstrainedPoint(midPoint);

	hitPath.clear();
	hitPath.startNewSubPath(sourceP.toFloat());
	hitPath.lineTo(destP.x, destP.y);

	if (hitPath.getLength() == 0) return;
	
	Line<int> line = Line<int>(sourceP.x, sourceP.y, destP.x, destP.y);
	float angle = line.getAngle();
	Point<float> ap = hitPath.getPointAlongPath(hitPath.getLength()-10);

	Path arrow;
	arrow.startNewSubPath(destP.toFloat());
	arrow.lineTo(ap.getPointOnCircumference(5, angle + float_Pi / 2));
	arrow.lineTo(ap.getPointOnCircumference(5, angle - float_Pi / 2));
	arrow.closeSubPath();

	Path rectP;
	rectP.addRoundedRectangle(sourceRect, 2);
	Point<float> sourceRP;
	float distOnPath = rectP.getNearestPoint(sourceP.toFloat(), sourceRP);
	
	Path base;
	float baseLength = 16;
	float startDist = distOnPath - baseLength/2;
	base.startNewSubPath(rectP.getPointAlongPath(startDist));
	int precision = 4;
	for (int i = 1; i <= precision; i++)
	{
		base.lineTo(rectP.getPointAlongPath(startDist + i*baseLength / precision));
	}
	
	/*
	Point<float> p1 = Point<float>(jmax<int>(sourceP.x - 10, sourceRect.getX()), sourceP.y);
	Point<float> p2 = Point<float>(jmin<int>(sourceP.x + 10, sourceRect.getRight()), sourceP.y);
	Point<float> p3 = Point<float>(sourceP.x, jmax<int>(sourceP.y - 10, sourceRect.getY()));
	Point<float> p4 = Point<float> (sourceP.x, jmin<int>(sourceP.y + 10, sourceRect.getBottom()));

	if (sourceP.x > sourceRect.getCentreX())
	{
		base.startNewSubPath(p1);
		base.lineTo(p2);
	} else
	{
		base.startNewSubPath(p2);
		base.lineTo(p1);
	}

	if (sourceP.y < sourceRect.getCentreY())
	{
		base.startNewSubPath(p3);
		base.lineTo(p4);
	} else
	{
		base.startNewSubPath(p4);
		base.lineTo(p3);
	}
	*/
	
	Colour c = NORMAL_COLOR;
	if (item->isSelected) c = HIGHLIGHT_COLOR;
	else if (sourceSUI->item->isSelected) c = GREEN_COLOR;
	else if (destSUI->item->isSelected) c = BLUE_COLOR;
	if (isMouseOver()) c = c.brighter();
	g.setColour(c);

	g.drawLine(sourceP.x, sourceP.y,ap.x,ap.y, c == NORMAL_COLOR ? 1 : 2);
	g.fillPath(arrow);
	g.strokePath(base, PathStrokeType(2));
}

void StateTransitionUI::paintOverChildren(Graphics & g)
{
}

void StateTransitionUI::resized()
{
}


bool StateTransitionUI::hitTest(int x, int y)
{
	Point<float> pol;
	Point<float> mousePos = getMouseXYRelative().toFloat();
	hitPath.getNearestPoint(mousePos, pol);

	int dist = pol.getDistanceFrom(mousePos);
	return dist  < 10;
}



void StateTransitionUI::itemUIGrabbed(BaseItemUI<State> *)
{
	updateBounds();
	
}

void StateTransitionUI::itemUIMiniModeChanged(BaseItemUI<State> *)
{
	updateBounds();
}

void StateTransitionUI::editorSelectionChanged(StateViewUI *)
{
	repaint();
}

