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
	gradient(Colours::red, 0, 0, Colours::blue, _maxPosition, 0, false),
	positionMax(_maxPosition)
{

	editorIsCollapsed = true;
	skipControllableNameInAddress = true;
	selectItemWhenCreated = false;

	position = addFloatParameter("Position", "Position in the gradient", 0, 0, positionMax);
	position->isSavable = false;
	position->hideInEditor = true;

	currentColor = new ColorParameter("Color", "Current color depending on time", Colours::black);
	currentColor->isSavable = false;
	currentColor->setControllableFeedbackOnly(true);
	addParameter(currentColor);

	addColorAt(positionMax / 5, Colours::red);
	addColorAt(positionMax * 2 / 5, Colours::yellow);
	addColorAt(positionMax * 3 / 5, Colours::green)->interpolation->setValueWithData(ColourGradient::NONE);
	addColorAt(positionMax * 4 / 5, Colours::blue);

	rebuildGradient();

	currentColor->setColor(getColorForPosition(position->floatValue()));
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
	if (items.isEmpty()) return Colours::transparentBlack;
	if (items.size() == 1) return items[0]->color->getColor();

	return gradient.getColourAtPosition(time / positionMax);
}

void TimeColorManager::rebuildGradient()
{
	gradientLock.enter();
	gradient.clearColours();
	if (items.size() > 0 && items[0]->position->floatValue() > 0) gradient.addColour(0, items[0]->color->getColor(), items[0]->interpolation->getValueDataAsEnum<ColourGradient::Interpolation>());

	for (auto &i : items)
	{
		i->gradientIndex = gradient.addColour(i->position->floatValue() / positionMax, i->color->getColor(), i->interpolation->getValueDataAsEnum<ColourGradient::Interpolation>());
	}
	gradientLock.exit();

	currentColor->setColor(getColorForPosition(position->floatValue()));
	colorManagerListeners.call(&TimeColorManagerListener::gradientUpdated);
}

TimeColor * TimeColorManager::addColorAt(float time, Colour color)
{
	if (items.isEmpty())  color = color.withAlpha(1.0f); //if only one color, force a non-transparent one to avoid confusion
	TimeColor * t = new TimeColor(time, color);
	BaseManager::addItem(t);
	rebuildGradient();
	return t;
}

void TimeColorManager::addItemInternal(TimeColor * item, var data)
{
	item->gradientIndex = gradient.addColour(item->position->floatValue() / positionMax, item->color->getColor());
	item->position->setRange(0, positionMax);
}

void TimeColorManager::removeItemInternal(TimeColor *)
{
	rebuildGradient();
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

void TimeColorManager::onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	TimeColor * t = static_cast<TimeColor *>(cc);
	if (t != nullptr)
	{
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

		}
		else if (c == t->color) {
			gradient.setColour(t->gradientIndex, t->color->getColor());
			colorManagerListeners.call(&TimeColorManagerListener::gradientUpdated);
		}
		else if (c == t->interpolation)
		{
			gradient.setInterpolation(t->gradientIndex, t->interpolation->getValueDataAsEnum<ColourGradient::Interpolation>());
			colorManagerListeners.call(&TimeColorManagerListener::gradientUpdated);
		}

		rebuildGradient();
		currentColor->setColor(getColorForPosition(position->floatValue()));
	}
}

void TimeColorManager::loadJSONDataInternal(var data)
{
	BaseManager::loadJSONDataInternal(data);
	rebuildGradient();
}

