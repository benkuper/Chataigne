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

void ShapeShifterContainer::insertShifterAt(ShapeShifter * shifter, int index, bool resizeAfter)
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

	DBG("Insert shifter at : " << shifter->getPreferredWidth());
	if(resizeAfter) resized();
}

void ShapeShifterContainer::removeShifter(ShapeShifter * shifter, bool deleteShifter, bool silent, bool resizeAfter)
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
			if(resizeAfter) resized();
		}
	}


}

ShapeShifterPanel * ShapeShifterContainer::insertPanelAt(ShapeShifterPanel * panel, int index, bool resizeAfter)
{
	insertShifterAt(panel, index, resizeAfter);
	panel->addShapeShifterPanelListener(this);
	return panel;
}

ShapeShifterPanel * ShapeShifterContainer::insertPanelRelative(ShapeShifterPanel * panel, ShapeShifterPanel * relativeTo, ShapeShifterPanel::AttachZone zone, bool resizeAfter)
{
	switch (zone)
	{
	case  ShapeShifterPanel::AttachZone::LEFT:
		if (direction == HORIZONTAL) insertPanelAt(panel, shifters.indexOf(relativeTo),resizeAfter);
		else movePanelsInContainer(panel, relativeTo, HORIZONTAL, false);
		break;

	case  ShapeShifterPanel::AttachZone::RIGHT:
		if (direction == HORIZONTAL) insertPanelAt(panel, shifters.indexOf(relativeTo) + 1, resizeAfter);
		else movePanelsInContainer(panel, relativeTo, HORIZONTAL, true);
		break;

	case  ShapeShifterPanel::AttachZone::TOP:
		if (direction == VERTICAL) insertPanelAt(panel, shifters.indexOf(relativeTo),resizeAfter);
		else movePanelsInContainer(panel, relativeTo, VERTICAL, false);
		break;

	case  ShapeShifterPanel::AttachZone::BOTTOM:
		if (direction == VERTICAL) insertPanelAt(panel, shifters.indexOf(relativeTo) + 1,resizeAfter);
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


ShapeShifterContainer * ShapeShifterContainer::insertContainerAt(ShapeShifterContainer * container, int index, bool resizeAfter)
{
	container->addShapeShifterContainerListener(this);
	insertShifterAt(container, index, resizeAfter);	
	return container;
}


void ShapeShifterContainer::movePanelsInContainer(ShapeShifterPanel * newPanel, ShapeShifterPanel * containedPanel, Direction _newDir, bool secondBeforeFirst)
{
	int targetIndex = shifters.indexOf(containedPanel);
	removeShifter(containedPanel, false, true, false);

	ShapeShifterContainer * newContainer = new ShapeShifterContainer(_newDir);
	newContainer->insertPanelAt(containedPanel, 0,false);
	newContainer->insertPanelAt(newPanel, secondBeforeFirst ? 1 : 0, false);
	
	newContainer->setPreferredWidth(containedPanel->getPreferredWidth());
	newContainer->setPreferredHeight(containedPanel->getPreferredHeight());
	DBG("MovePanels " << containedPanel->contents[0]->contentName << "/" << containedPanel->getPreferredWidth() << " / " << newContainer->getPreferredWidth());
	
	insertContainerAt(newContainer, targetIndex);
	DBG("After insertContainerAt" << containedPanel->contents[0]->contentName << "/" << containedPanel->getPreferredWidth() << " / " << newContainer->getPreferredWidth());	

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
	if (shifters.size() == 0) return;

	if (parentContainer == nullptr && shifters.size() == 1) //Main container, only one item
	{
		shifters[0]->setBounds(getLocalBounds());
		return;
	}

	int totalSpace = (direction == HORIZONTAL) ? getWidth() : getHeight();
	
	int numShifters = shifters.size();
	int totalSpaceWithoutGap = totalSpace - gap *(numShifters - 1);

	int numFlexibleShifters = 0;
	int totalSpacePreferred = 0;
	for (auto &p : shifters)
	{
		if (p->isFlexible()) numFlexibleShifters++;
		totalSpacePreferred += (direction == HORIZONTAL) ? p->getPreferredWidth() : p->getPreferredHeight();
	}

	int spaceDiff = totalSpaceWithoutGap - totalSpacePreferred;
	if (numFlexibleShifters > 0)
	{
		int spaceDiffPerShifter = spaceDiff / numFlexibleShifters;
		for (auto &p : shifters)
		{
			if (p->isFlexible())
			{
				if (direction == HORIZONTAL) p->setPreferredWidth(p->getPreferredWidth() + spaceDiffPerShifter);
				else p->setPreferredHeight(p->getPreferredHeight() + spaceDiffPerShifter);
			}
		}
	} else
	{
		int spaceDiffPerShifter = spaceDiff / numShifters;
		for (auto &p : shifters)
		{
			if (direction == HORIZONTAL) p->setPreferredWidth(p->getPreferredWidth() + spaceDiffPerShifter);
			else p->setPreferredHeight(p->getPreferredHeight() + spaceDiffPerShifter);
		}
	}

	Rectangle<int> r = getLocalBounds();
	int index = 0;
	for (auto &p : shifters)
	{
		bool isLastShifter = index == numShifters - 1;
		
		if (direction == HORIZONTAL) p->setBounds(r.removeFromLeft(p->getPreferredWidth()));
		else p->setBounds(r.removeFromTop(p->getPreferredHeight()));

		if (!isLastShifter)
		{
			Rectangle<int> gr = (direction == HORIZONTAL) ? r.removeFromLeft(gap) : r.removeFromTop(gap);
			grabbers[index]->setBounds(gr);
		}

		index++;
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
				insertPanelAt(c, -1, false);
			} else if (t == CONTAINER)
			{
				Direction dir = (Direction)(int)sData.getDynamicObject()->getProperty("direction");
				ShapeShifterContainer * sc = new ShapeShifterContainer(dir);
				insertContainerAt(sc, -1, false);
				sc->loadLayout(sData);
			}
		}
	}

	resized();
}

void ShapeShifterContainer::grabberGrabUpdate(GapGrabber * gg, int dist)
{
	ShapeShifter * firstShifter = shifters[grabbers.indexOf(gg)];
	ShapeShifter * secondShifter = shifters[grabbers.indexOf(gg) + 1];

	switch (direction)
	{
	case HORIZONTAL:
		firstShifter->setPreferredWidth(firstShifter->getPreferredWidth() + dist);
		secondShifter->setPreferredWidth(secondShifter->getPreferredWidth() - dist);
		break;

	case VERTICAL:
		firstShifter->setPreferredHeight(firstShifter->getPreferredHeight() + dist);
		secondShifter->setPreferredHeight(secondShifter->getPreferredHeight() - dist);
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
	container->removeShifter((ShapeShifterContainer *)lastShifter, false, true,false);

	ShapeShifter * ss = nullptr;

	//DBG("Insert last shifter in parent container");
	if (lastShifter->shifterType == PANEL)  ss = insertPanelAt((ShapeShifterPanel *)lastShifter, containerIndex,false);
	else ss = insertContainerAt((ShapeShifterContainer *)lastShifter, containerIndex,false);

	ss->setPreferredWidth(container->getPreferredWidth());
	ss->setPreferredHeight(container->getPreferredHeight());

	//DBG("Remove useless container");
	removeShifter(container, true, true);

}
