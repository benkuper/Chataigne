/*
  ==============================================================================

	TimeColorManager.cpp
	Created: 11 Apr 2017 11:40:41am
	Author:  Ben

  ==============================================================================
*/

#include "TimeColorManager.h"

TimeColorComparator TimeColorManager::comparator;


TimeColorManager::TimeColorManager(float _maxPosition) :
	BaseManager("Colors"),
	positionMax(_maxPosition),
	gradient(Colours::red,0,0,Colours::blue,_maxPosition,0,false)
{
	
	skipControllableNameInAddress = true;
	
	position = addFloatParameter("Position", "Position in the gradient",0,0,positionMax);
	currentColor = new ColorParameter("Color", "Current color depending on time", Colours::black);
	addParameter(currentColor);

	addColorAt(0, Colours::red)->isLocked = true;
	addColorAt(positionMax / 2, Colours::green);
	addColorAt(positionMax, Colours::blue);
}


TimeColorManager::~TimeColorManager()
{
}

void TimeColorManager::setPositionMax(float val)
{
	positionMax = val;
	position->setRange(0, positionMax);
	for (auto &k : items)
	{
		//DBG("Set posMax to " << k->niceName << "/" << (int)k->position);
		k->position->setRange(0, positionMax);
	}
}

Colour TimeColorManager::getColorForPosition(const float & time) const
{
	return gradient.getColourAtPosition(time / positionMax);
}

void TimeColorManager::rebuildGradient()
{
	gradient.clearColours();
	for (auto &i : items)
	{
		i->gradientIndex = gradient.addColour(i->position->floatValue() / positionMax, i->color->getColor());
	}
}

TimeColor * TimeColorManager::addColorAt(float time, Colour color)
{
	TimeColor * t = new TimeColor(time, color);
	BaseManager::addItem(t);

	return t;
}

void TimeColorManager::addItemInternal(TimeColor * item, var data)
{
	DBG("addItem Internal here");
	item->gradientIndex = gradient.addColour(item->position->floatValue()/ positionMax, item->color->getColor());
}

void TimeColorManager::removeItemInternal(TimeColor * item)
{
	gradient.removeColour(item->gradientIndex);
}

void TimeColorManager::reorderItems()
{
	items.sort(TimeColorManager::comparator, true);
	BaseManager::reorderItems();
}

void TimeColorManager::onContainerParameterChanged(Parameter * p)
{
	if (p == position)
	{
		currentColor->setColor(getColorForPosition(position->floatValue()));
	}
}

void TimeColorManager::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	TimeColor * t = static_cast<TimeColor *>(cc);
	if (t != nullptr)
	{
		DBG("controllable feedback update " << cc->niceName);
		if (c == t->position)
		{
			int index = items.indexOf(t);
			if (index > 0 && t->position->floatValue() < items[index - 1]->position->floatValue())
			{
				items.swap(index, index - 1);
				baseManagerListeners.call(&Listener::itemsReordered);
			}
			else if (index < items.size() - 1 && t->position->floatValue() > items[index + 1]->position->floatValue())
			{
				items.swap(index, index + 1);
				baseManagerListeners.call(&Listener::itemsReordered);
			}

			rebuildGradient();
		}
		else if (c == t->color) {
			gradient.setColour(t->gradientIndex, t->color->getColor());
		}

		currentColor->setColor(getColorForPosition(position->floatValue()));
	}
}

