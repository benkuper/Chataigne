/*
  ==============================================================================

    AutomationUI.cpp
    Created: 11 Dec 2016 1:22:02pm
    Author:  Ben

  ==============================================================================
*/

#include "AutomationUI.h"

AutomationUI::AutomationUI(Automation * _automation) :
	BaseManagerUI("Automation",_automation,false),
	firstROIKey(0),lastROIKey(0), currentPosition(0),
	currentUI(nullptr)
{
	transparentBG = true;
	useDefaultMenu = false;
	setViewRange(0, manager->positionMax);
	manager->addAsyncContainerListener(this);
}

AutomationUI::~AutomationUI()
{
	manager->removeAsyncContainerListener(this);
}

void AutomationUI::setCurrentPosition(float pos)
{
	currentPosition = pos;
	currentUI = getClosestKeyUIForPos(currentPosition);
	currentValue = manager->getValueForPosition(pos);
	repaint();
}

void AutomationUI::setViewRange(float start, float end)
{
	viewStartPos = start;
	viewEndPos = end;
	updateROI();
	resized();
}

void AutomationUI::updateROI()
{
	if (itemsUI.size() == 0) return;

	int len = itemsUI.size()-1;
	for (int i = 0; i < len; i++)
	{
		itemsUI[i]->setVisible(false);
		if (isInView(itemsUI[i]))
		{
			firstROIKey = jmax<int>(i-1,0);
			break;
		}
	}

	for (int i = len; i >= 0; i--)
	{
		itemsUI[i]->setVisible(false);
		if (isInView(itemsUI[i]))
		{
			lastROIKey = jmin<int>(i + 1, len);
			break;
		}
	}

	for (int i = firstROIKey; i <= lastROIKey; i++)
	{
		itemsUI[i]->setVisible(true);
	}
	
}

void AutomationUI::paint(Graphics & g)
{
	BaseManagerUI::paint(g);

	
	if (itemsUI.size() < 2) return;

	int count = 0;
	int ty = getHeight() - jmap<float>(currentValue, 0, manager->valueMax, 0, getHeight());
	Rectangle<int> vr = getLocalBounds().withTop(ty);
	g.setColour(Colours::purple.withAlpha(.1f));
	g.fillRect(vr);
	g.setColour(Colours::yellow);
	g.drawEllipse(Rectangle<int>(0, 0, 5, 5).withCentre(Point<int>(getXForPos(currentPosition), ty)).toFloat(),2);


	for (int i = firstROIKey; i < lastROIKey; i++)
	{
		drawTransition(g, itemsUI[i], itemsUI[i + 1]);
		count++;
	}
	

	
}

void AutomationUI::drawTransition(Graphics & g, AutomationKeyUI * k1, AutomationKeyUI * k2)
{
	Point<int> p1 = k1->getBounds().getCentre();
	Point<int> p2 = k2->getBounds().getCentre();
	
	Colour c = FRONT_COLOR;
	if (k1->item->isSelected) c = HIGHLIGHT_COLOR;
	if (k1 == currentUI) c = c.brighter();
	g.setColour(c); 

	Easing * e = k1->item->easing;
	if (e != nullptr)
	{
		switch (e->type)
		{
		case Easing::Type::LINEAR:
			g.drawLine(p1.x, p1.y, p2.x, p2.y, 1);
			break;

		case Easing::Type::BEZIER:
			Path p;
			CubicEasing * ce = static_cast<CubicEasing *>(e);
			p.startNewSubPath(p1.x, p1.y);
			Point<float> a = Point<float>(jmap<float>(ce->anchor1->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor1->getPoint().y, p1.y, p2.y));
			Point<float> b = Point<float>(jmap<float>(ce->anchor2->getPoint().x, p1.x, p2.x), jmap<float>(ce->anchor2->getPoint().y, p1.y, p2.y));
			p.cubicTo(a,b, p2.toFloat());
			g.strokePath(p, PathStrokeType(1));
			g.setColour(Colours::red);
			g.fillEllipse(Rectangle<int>(0, 0,5,5).toFloat().withCentre(a));
			g.setColour(Colours::blue);
			g.fillEllipse(Rectangle<int>(0, 0, 5, 5).toFloat().withCentre(b));
			break;
		}
		
	}
	
}


void AutomationUI::resized()
{
	if (itemsUI.size() == 0) return;

	for (int i = firstROIKey;i<=lastROIKey;i++)
	{
		placeKeyUI(itemsUI[i]);
	}
}

void AutomationUI::placeKeyUI(AutomationKeyUI * kui) 
{
	int tx = getXForPos(kui->item->position->floatValue());
	int ty = (1 - kui->item->value->floatValue() / manager->valueMax)*getHeight();
	Rectangle<int> kr = Rectangle<int>(0, 0, AutomationKeyUI::handleSize + 10, AutomationKeyUI::handleSize + 10).withCentre(Point<int>(tx, ty));
	kui->setBounds(kr);
}


int AutomationUI::getXForPos(float time)
{
	if (viewStartPos == viewEndPos) return 0;
	return (int)jmap<float>(time, viewStartPos, viewEndPos, 0, (float)getWidth());
}

float AutomationUI::getPosForX(int tx, bool offsetStart)
{
	float viewRange = viewEndPos - viewStartPos;
	float mapStart = offsetStart ? viewStartPos: 0;
	return jmap<float>((float)tx, 0, (float)getWidth(), mapStart, mapStart + viewRange);
}

bool AutomationUI::isInView(AutomationKeyUI * kui)
{
	return kui->item->position->floatValue() >= viewStartPos && kui->item->position->floatValue() <= viewEndPos;
}

AutomationKeyUI * AutomationUI::getClosestKeyUIForPos(float pos, int start, int end)
{
	if (itemsUI.size() == 0) return nullptr;

	if (start == -1) start = 0;
	if (end == -1) end = itemsUI.size() - 1;


	if (pos < itemsUI[0]->item->position->floatValue()) return itemsUI[0];
	if (pos > itemsUI[itemsUI.size() - 1]->item->position->floatValue()) return itemsUI[itemsUI.size() - 1];

	
	if (end-start <= 1) return itemsUI[start];
	
	int midIndex = (int)floor((start + end) / 2);
	float medPos = itemsUI[midIndex]->item->position->floatValue();

	if (pos == medPos) return itemsUI[midIndex];

	else if (pos > medPos)
	{
		return getClosestKeyUIForPos(pos,midIndex, end);
	}
	else
	{
		return getClosestKeyUIForPos(pos,start, midIndex);
	}
}

void AutomationUI::itemAdded(AutomationKey * k)
{
	BaseManagerUI::itemAdded(k);
	updateROI();
	resized();
}

void AutomationUI::itemsReordered()
{
	BaseManagerUI::itemsReordered();
	updateROI();
	repaint();
}

void AutomationUI::addItemUIInternal(AutomationKeyUI * kui)
{
	kui->addMouseListener(this, false);
}

void AutomationUI::removeItemUIInternal(AutomationKeyUI * kui)
{
	kui->removeMouseListener(this);
	updateROI();
	repaint();
}

void AutomationUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);

	if (e.eventComponent == this)
	{
		if (e.mods.isCtrlDown())
		{
			manager->addItem(getPosForX(e.getPosition().x), (1 - e.getPosition().y*1.f / getHeight())*manager->valueMax);
		}
	}
	
}

void AutomationUI::mouseDrag(const MouseEvent & e)
{
	if (e.originalComponent != this)
	{
		AutomationKeyUI * kui = dynamic_cast<AutomationKeyUI *>(e.originalComponent);
		if (kui != nullptr)
		{
			if (e.mods.isLeftButtonDown())
			{
				Point<int> mp = e.getEventRelativeTo(this).getPosition();
				float pos = getPosForX(mp.x);
				float val = (1 - mp.y*1.f / getHeight())*manager->valueMax;
				kui->item->position->setValue(pos);
				kui->item->value->setValue(val);
			}
			else if (e.mods.isRightButtonDown())
			{
				Easing * es = kui->item->easing;
				switch (es->type)
				{
					/*
				case Easing::QUADRATIC:
					
					if (itemsUI.indexOf(kui) < itemsUI.size() - 1)
					{
						AutomationKeyUI * k2 = itemsUI[itemsUI.indexOf(kui) + 1];
						Point<int> mp = e.getEventRelativeTo(this).getPosition();
						QuadraticEasing * qe = static_cast<QuadraticEasing *>(es);

						Point<float> targetPoint = Point<float>(
							jmap<float>(mp.x, kui->getBounds().getCentreX(), k2->getBounds().getCentreX(), 0, 1),
							jmap<float>(mp.y, kui->getBounds().getCentreY(), k2->getBounds().getCentreY(), 0, 1)
						);
						qe->anchor->setPoint(targetPoint);
						repaint();
					}
					
					
					break;
					*/
				case Easing::BEZIER:
					if (itemsUI.indexOf(kui) < itemsUI.size() - 1)
					{
						AutomationKeyUI * k2 = itemsUI[itemsUI.indexOf(kui) + 1];
						Point<int> mp = e.getEventRelativeTo(this).getPosition();
						CubicEasing * qe = static_cast<CubicEasing *>(es);

						Point<float> targetPoint = Point<float>(
							jmap<float>(mp.x, kui->getBounds().getCentreX(), k2->getBounds().getCentreX(), 0, 1),
							jmap<float>(mp.y, kui->getBounds().getCentreY(), k2->getBounds().getCentreY(), 0, 1)
							);
						
						if(e.mods.isCtrlDown()) qe->anchor2->setPoint(targetPoint);
						else qe->anchor1->setPoint(targetPoint);
						repaint();
					}
					break;
				}
			}			
		}
	}
}

void AutomationUI::newMessage(const ContainerAsyncEvent & e)
{
	if (e.type == ContainerAsyncEvent::EventType::ControllableFeedbackUpdate)
	{
		if (e.targetControllable != nullptr)
		{
			AutomationKey * k = dynamic_cast<AutomationKey *>(e.targetControllable->parentContainer);
			if (k != nullptr)
			{
				if (e.targetControllable == k->easingType)
				{
					repaint();
				}
				else if (e.targetControllable == k->position || e.targetControllable == k->value)
				{
					placeKeyUI(getUIForItem(k));
					repaint();
				}
			}
		}
		
	}
	
}

