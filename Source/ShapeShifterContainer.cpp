/*
  ==============================================================================

	ShapeShifterContainer.cpp
	Created: 2 May 2016 3:11:25pm
	Author:  bkupe

  ==============================================================================
*/

#include "ShapeShifterContainer.h"
#include "Style.h"
#include "ShapeShifterManager.h"


ShapeShifterContainer::ShapeShifterContainer(Direction _direction) :
	ShapeShifter(ShapeShifter::CONTAINER),
	direction(_direction)
{

}

ShapeShifterContainer::~ShapeShifterContainer()
{
	clear();
}

void ShapeShifterContainer::insertShifterAt(ShapeShifter * shifter, int index)
{
	if (index == -1) index = shifters.size();

	shifters.insert(index, shifter);
	addAndMakeVisible(shifter);
	shifter->setParentContainer(this);

	if (shifters.size() > 1)
	{
		GapGrabber * gg = new GapGrabber(direction == HORIZONTAL ? GapGrabber::HORIZONTAL : GapGrabber::VERTICAL);
		grabbers.add(gg);
		addAndMakeVisible(gg);
		gg->addGrabberListener(this);
	}

	resized();
}

void ShapeShifterContainer::removeShifter(ShapeShifter * shifter, bool deleteShifter, bool silent)
{
	//DBG("Remove Shifter, deleteShifter ? " << String(deleteShifter) << ", silent " << String(silent));
	int shifterIndex = shifters.indexOf(shifter);
	shifters.removeAllInstancesOf(shifter);
	shifter->setParentContainer(nullptr);

	removeChildComponent(shifter);



	switch (shifter->shifterType)
	{
	case CONTAINER:
		((ShapeShifterContainer *)shifter)->removeShapeShifterContainerListener(this);
		if (deleteShifter) delete shifter;
		break;

	case PANEL:
		((ShapeShifterPanel *)shifter)->removeShapeShifterPanelListener(this);
		if (deleteShifter) ShapeShifterManager::getInstance()->removePanel(((ShapeShifterPanel *)shifter));
		break;
	}



	if (shifters.size() == 0)
	{
		//dispatch emptied container so parent container deletes it
		if (!silent) containerListeners.call(&ShapeShifterContainerListener::containerEmptied, this);
	} else
	{
		if (shifters.size() == 1 && !silent && parentContainer != nullptr)
		{
			containerListeners.call(&ShapeShifterContainerListener::oneShifterRemaining, this, shifters[0]);
		} else
		{
			GapGrabber * gg = grabbers[(jmin<int>(shifterIndex, grabbers.size() - 1))];
			removeChildComponent(gg);
			grabbers.remove(grabbers.indexOf(gg), true);
			resized();
		}
	}


}

ShapeShifterPanel * ShapeShifterContainer::insertPanelAt(ShapeShifterPanel * panel, int index)
{
	insertShifterAt(panel, index);
    
    DBG("container add panel listener " << panel->header.tabs[0]->content->contentName);
	panel->addShapeShifterPanelListener(this);
	return panel;
}

ShapeShifterPanel * ShapeShifterContainer::insertPanelRelative(ShapeShifterPanel * panel, ShapeShifterPanel * relativeTo, ShapeShifterPanel::AttachZone zone)
{
	switch (zone)
	{
	case  ShapeShifterPanel::AttachZone::LEFT:
		if (direction == HORIZONTAL) insertPanelAt(panel, shifters.indexOf(relativeTo));
		else movePanelsInContainer(panel, relativeTo, HORIZONTAL, false);
		break;

	case  ShapeShifterPanel::AttachZone::RIGHT:
		if (direction == HORIZONTAL) insertPanelAt(panel, shifters.indexOf(relativeTo) + 1);
		else movePanelsInContainer(panel, relativeTo, HORIZONTAL, true);
		break;

	case  ShapeShifterPanel::AttachZone::TOP:
		if (direction == VERTICAL) insertPanelAt(panel, shifters.indexOf(relativeTo));
		else movePanelsInContainer(panel, relativeTo, VERTICAL, false);
		break;

	case  ShapeShifterPanel::AttachZone::BOTTOM:
		if (direction == VERTICAL) insertPanelAt(panel, shifters.indexOf(relativeTo) + 1);
		else movePanelsInContainer(panel, relativeTo, VERTICAL, true);
		break;
	case ShapeShifterPanel::AttachZone::NONE:
	case ShapeShifterPanel::AttachZone::CENTER:
		//@ben do we need to handle these?
		jassertfalse;
		break;


	}

	return panel;
}


ShapeShifterContainer * ShapeShifterContainer::insertContainerAt(Direction _direction, int index)
{
	ShapeShifterContainer * ssc = new ShapeShifterContainer(_direction);
	return insertContainerAt(ssc, index);
}

ShapeShifterContainer * ShapeShifterContainer::insertContainerAt(ShapeShifterContainer * container, int index)
{

	insertShifterAt(container, index);
	container->addShapeShifterContainerListener(this);
	resized();

	return container;
}


void ShapeShifterContainer::movePanelsInContainer(ShapeShifterPanel * newPanel, ShapeShifterPanel * containedPanel, Direction _newDir, bool secondBeforeFirst)
{
	int targetIndex = shifters.indexOf(containedPanel);
	removeShifter(containedPanel, false, true);
	ShapeShifterContainer * newContainer = insertContainerAt(_newDir, targetIndex);
	newContainer->insertPanelAt(containedPanel, 0);
	newContainer->insertPanelAt(newPanel, secondBeforeFirst ? 1 : 0);

}

bool ShapeShifterContainer::isFlexible()
{
	for (auto &p : shifters)
	{
		if (!p->isFlexible()) return false;
	}
	return true;
}


void ShapeShifterContainer::resized()
{

	if (parentContainer == nullptr && shifters.size() == 1) //Main container, only one item
	{
		shifters[0]->setBounds(getLocalBounds());
		return;
	}

	Rectangle<int> r = getLocalBounds();
	int gap = 6;
	int totalSpace = (direction == HORIZONTAL) ? r.getWidth() : r.getHeight();

	int numShifters = shifters.size();

	int numDefaultSpace = numShifters;
	int reservedPreferredSpace = 0;


	for (auto &p : shifters)
	{
		if (!p->isFlexible())
		{
			numDefaultSpace--;
			reservedPreferredSpace += (direction == HORIZONTAL) ? p->preferredWidth : p->preferredHeight;
		}
	}

	bool allShiftersAreFlexible = reservedPreferredSpace == 0;
	bool noShiftersAreFlexible = numDefaultSpace == 0;


	int backOffsetAmount = 0; //amount to subtract from each fixed-size panel so every panel is visible
	if (reservedPreferredSpace > totalSpace)
	{
		backOffsetAmount = (reservedPreferredSpace - totalSpace) / (numShifters - numDefaultSpace);
	}

	int defaultSpace = 0;

	if (noShiftersAreFlexible && numShifters >= 2) //auto set middle to flexible
	{
		numDefaultSpace = jmax(1, numShifters - 2);
		reservedPreferredSpace = (direction == HORIZONTAL) ? shifters[0]->preferredWidth : shifters[0]->preferredHeight;
		if (numShifters >= 3) reservedPreferredSpace += (direction == HORIZONTAL) ? shifters[numShifters-1]->preferredWidth : shifters[numShifters-1]->preferredHeight;
	}

	if(numDefaultSpace > 0) defaultSpace = (totalSpace - reservedPreferredSpace) / numDefaultSpace - gap*(numShifters - 1);

	int panelIndex = 0;
	for (auto &p : shifters)
	{
		bool lastShifter = panelIndex >= grabbers.size();
		int tp = (direction == HORIZONTAL) ? p->preferredWidth : p->preferredHeight;

		int targetSpace = defaultSpace;
		bool targetSpaceFromPreferred = false;
		
		if (noShiftersAreFlexible)
		{
			targetSpaceFromPreferred = panelIndex == 0 || (panelIndex == numShifters - 1 && numShifters >= 3);
		} else if (allShiftersAreFlexible)
		{
			targetSpaceFromPreferred = panelIndex == 0;
		} else
		{
			targetSpaceFromPreferred = !p->isFlexible();
		}
		
		if (targetSpaceFromPreferred)
		{
			targetSpace = jmax(tp - backOffsetAmount, (direction == HORIZONTAL ? p->minWidth : p->minHeight));
		}


		if (!lastShifter)
		{
			Rectangle<int> tr = (direction == HORIZONTAL) ? r.removeFromLeft(targetSpace) : r.removeFromTop(targetSpace);
			p->setBounds(tr);
		} else
		{
			p->setBounds(r);
		}

		if (!lastShifter)
		{
			Rectangle<int> gr = (direction == HORIZONTAL) ? r.removeFromLeft(gap) : r.removeFromTop(gap);
			grabbers[panelIndex]->setBounds(gr);
		}

		panelIndex++;
	}
}

void ShapeShifterContainer::clear()
{
	while (shifters.size() > 0)
	{
		removeShifter(shifters[0], true, true);
	}
}

var ShapeShifterContainer::getCurrentLayout()
{
	var layout = ShapeShifter::getCurrentLayout();
	layout.getDynamicObject()->setProperty("direction", (int)direction);

	var sData;
	for (auto &s : shifters)
	{
		sData.append(s->getCurrentLayout());
	}
	layout.getDynamicObject()->setProperty("shifters", sData);
	return layout;
}

void ShapeShifterContainer::loadLayoutInternal(var layout)
{

	Array<var> * sArray = layout.getDynamicObject()->getProperty("shifters").getArray();

	if (sArray != nullptr)
	{
		for (auto &sData : *sArray)
		{
			Type t = (Type)(int)(sData.getDynamicObject()->getProperty("type"));
			if (t == PANEL)
			{
				ShapeShifterPanel * c = ShapeShifterManager::getInstance()->createPanel(nullptr);
				c->loadLayout(sData);
				insertPanelAt(c);
			} else if (t == CONTAINER)
			{
				Direction dir = (Direction)(int)sData.getDynamicObject()->getProperty("direction");
				ShapeShifterContainer * sc = insertContainerAt(dir);
				sc->loadLayout(sData);
			}
		}
	}
}

void ShapeShifterContainer::grabberGrabUpdate(GapGrabber * gg, int dist)
{
	ShapeShifter * firstShifter = shifters[grabbers.indexOf(gg)];
	ShapeShifter * secondShifter = shifters[grabbers.indexOf(gg) + 1];

	switch (direction)
	{
	case HORIZONTAL:
		if (!secondShifter->isFlexible())  secondShifter->setPreferredWidth(secondShifter->preferredWidth - dist);
		else firstShifter->setPreferredWidth(firstShifter->preferredWidth + dist);
		break;

	case VERTICAL:
		if (!secondShifter->isFlexible())  secondShifter->setPreferredHeight(secondShifter->preferredHeight - dist);
		else firstShifter->setPreferredHeight(firstShifter->preferredHeight + dist);
		break;

	case NONE:
		//@ben what to we do for that?
		//@martin never gonna happen :)
		jassertfalse;
		break;
	}

	resized();
}


void ShapeShifterContainer::panelDetach(ShapeShifterPanel * panel)
{
	Rectangle<int> panelBounds = panel->getScreenBounds();
	removeShifter(panel, false);
	ShapeShifterManager::getInstance()->showPanelWindow(panel, panelBounds);
}

void ShapeShifterContainer::panelEmptied(ShapeShifterPanel * panel)
{
    DBG("container, panelEmptied ");
	removeShifter(panel, true, false);
}

void ShapeShifterContainer::panelDestroyed(ShapeShifterPanel * panel)
{
	removeShifter(panel, false, true);
}

void ShapeShifterContainer::containerEmptied(ShapeShifterContainer * container)
{
	removeShifter(container, true);
}

void ShapeShifterContainer::oneShifterRemaining(ShapeShifterContainer * container, ShapeShifter * lastShifter)
{
	int containerIndex = shifters.indexOf(container);

	//DBG("Remove shifter from child container");
	container->removeShifter((ShapeShifterContainer *)lastShifter, false, true);

	ShapeShifter * ss = nullptr;

	//DBG("Insert last shifter in parent container");
	if (lastShifter->shifterType == PANEL)  ss = insertPanelAt((ShapeShifterPanel *)lastShifter, containerIndex);
	else ss = insertContainerAt((ShapeShifterContainer *)lastShifter, containerIndex);

	ss->setPreferredWidth(container->preferredWidth);
	ss->setPreferredHeight(container->preferredHeight);

	//DBG("Remove useless container");
	removeShifter(container, true, true);

}
