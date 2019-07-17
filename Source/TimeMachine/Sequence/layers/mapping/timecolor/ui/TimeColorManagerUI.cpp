/*
  ==============================================================================

	TimeColorManagerUI.cpp
	Created: 11 Apr 2017 11:40:47am
	Author:  Ben

  ==============================================================================
*/

#include "TimeColorManagerUI.h"

TimeColorManagerUI::TimeColorManagerUI(TimeColorManager * manager) :
	BaseManagerUI(manager->niceName,manager,false),
	Thread("Colors "+String(manager->niceName)),
	shouldRepaint(true),
	shouldUpdateImage(true)
{
	setShowAddButton(false);
	animateItemOnAdd = false;

	transparentBG = true;
	setViewRange(0, manager->length->floatValue());

	resizeOnChildBoundsChanged = false;

	manager->addAsyncCoalescedContainerListener(this);
	addExistingItems();

	startTimerHz(20);
	startThread();
}

TimeColorManagerUI::~TimeColorManagerUI()
{
	manager->removeAsyncContainerListener(this);
	signalThreadShouldExit();
	stopThread(1000);
}

void TimeColorManagerUI::setViewRange(float start, float end)
{
	
	viewStartPos = start;
	viewEndPos = end;
	
	shouldUpdateImage = true;
	resized();
}

void TimeColorManagerUI::paint(Graphics & g)
{
	BaseManagerUI::paint(g);

	Rectangle<int> r = getLocalBounds();
	g.setColour(manager->currentColor->getColor());
	g.fillRect(r.removeFromBottom(16).reduced(2, 7));

	g.fillCheckerBoard(r.toFloat(), 12, 12, Colours::white, Colours::white.darker(.2f));

	imageLock.enter();
	g.setColour(Colours::white);
	g.drawImage(viewImage, r.toFloat());
	imageLock.exit();
	
	/*
	g.setColour(manager->currentColor->getColor());
	g.fillRect(r.removeFromBottom(16).reduced(2,7));z

	manager->gradient.point1.setX(getXForPos(0));
	manager->gradient.point2.setX(getXForPos(manager->length->floatValue()));

	g.fillCheckerBoard(r.toFloat(), 12, 12, Colours::white, Colours::white.darker(.2f));
	if (!manager->items.isEmpty())
	{
		g.setGradientFill(manager->gradient);
		g.fillRect(r);
	}
	*/
}

void TimeColorManagerUI::resized()
{

	if (getParentComponent() == nullptr) return;
	if (getWidth() == 0 || getHeight() == 0) return;
	if (itemsUI.size() == 0) return;

	shouldUpdateImage = true;

	
	for (auto &tui : itemsUI)
	{
		tui->setVisible(isInView(tui));
		if(tui->isVisible()) placeItemUI(tui);
	}

}

void TimeColorManagerUI::addItemUIInternal(TimeColorUI * item)
{
	item->addMouseListener(this, true);
	placeItemUI(item);
}

void TimeColorManagerUI::removeItemUIInternal(TimeColorUI * item)
{
	item->removeMouseListener(this);
}

void TimeColorManagerUI::mouseDoubleClick(const MouseEvent & e)
{
	if (e.originalComponent == this)
	{
		float pos = getPosForX(e.getMouseDownX());
		manager->addColorAt(pos, manager->getColorForPosition(pos));
	}
	
}



void TimeColorManagerUI::mouseDrag(const MouseEvent & e)
{
	if (e.originalComponent == this)
	{

	}
	else
	{
		TimeColorUI * tui = dynamic_cast<TimeColorUI *>(e.eventComponent);
		if (tui == nullptr)
		{
			tui = dynamic_cast<TimeColorUI *>(e.eventComponent->getParentComponent());
		}

		if (tui != nullptr)
		{
			if (e.mods.isLeftButtonDown())
			{
				Point<int> mp = e.getEventRelativeTo(this).getPosition();
				float pos = getPosForX(mp.x);
				tui->item->position->setValue(pos);
				placeItemUI(tui);
				repaint();
			}
		}
	}
}

void TimeColorManagerUI::placeItemUI(TimeColorUI * tui)
{
	int tx = getXForPos(tui->item->position->floatValue());
	tui->setBounds(tx-6,getHeight()-18,12,16);
}

int TimeColorManagerUI::getXForPos(float time)
{
	if (viewStartPos == viewEndPos) return 0;
	return (int)jmap<float>(time, viewStartPos, viewEndPos, 0, (float)getWidth());
}

float TimeColorManagerUI::getPosForX(int tx, bool offsetStart)
{
	float viewRange = viewEndPos - viewStartPos;
	float mapStart = offsetStart ? viewStartPos : 0;
	return jmap<float>((float)tx, 0, (float)getWidth(), mapStart, mapStart + viewRange);
}

bool TimeColorManagerUI::isInView(TimeColorUI * kui)
{
	return kui->item->position->floatValue() >= viewStartPos && kui->item->position->floatValue() <= viewEndPos;
}

void TimeColorManagerUI::newMessage(const ContainerAsyncEvent & e)
{
	if (e.type == ContainerAsyncEvent::ControllableFeedbackUpdate)
	{
		//if (ControllableUtil::findParentAs<TimeColorManager>(e.targetControllable) != nullptr)
		//{
			shouldUpdateImage = true;
			shouldRepaint = true;
		//}
	}
}

void TimeColorManagerUI::run()
{
	while (!threadShouldExit())
	{
		sleep(50); //20ms is plenty enough

		if (Engine::mainEngine->isLoadingFile || Engine::mainEngine->isClearing) continue;
		if (!shouldUpdateImage) continue;


		imageLock.enter();

		const int resX = getWidth();
		const int resY = 1;

		if (resX == 0 || resY == 0)
		{
			imageLock.exit();
			return;
		}

		if (resX != viewImage.getWidth() || resY != viewImage.getHeight()) viewImage = Image(Image::ARGB, resX, resY, true);
		else viewImage.clear(viewImage.getBounds());

		manager->gradientLock.enter();
		for (int tx = 0; tx < resX; tx++)
		{
			if (threadShouldExit())
			{
				manager->gradientLock.exit(); 
				imageLock.exit();
				return;
			}

			Colour col = manager->getColorForPosition(getPosForX(tx));
			viewImage.setPixelAt(tx, 0, col);
		}
		manager->gradientLock.exit();

		imageLock.exit();
		
		shouldUpdateImage = false;
		shouldRepaint = true;
	}

	imageLock.enter();
	viewImage.clear(viewImage.getBounds());
	imageLock.exit();

}

void TimeColorManagerUI::timerCallback()
{
	if (shouldRepaint)
	{
		repaint();
		shouldRepaint = false;
	}
}
