/*
  ==============================================================================

	TimeColorManager.cpp
	Created: 11 Apr 2017 11:40:41am
	Author:  Ben

  ==============================================================================
*/

#include "TimeColorManager.h"

TimeColorComparator TimeColorManager::comparator;

TimeColorManager::TimeColorManager(float _length) :
	BaseManager("Colors"),
	allowKeysOutside(true)
{

	editorIsCollapsed = true;
	skipControllableNameInAddress = true;
	selectItemWhenCreated = false;

	position = addFloatParameter("Position", "Position in the gradient", 0, 0, _length);
	position->isSavable = false;
	position->hideInEditor = true;

	length = addFloatParameter("Length", "Length of the gradient", _length, 0, INT32_MAX);

	currentColor = new ColorParameter("Color", "Current color depending on time", Colours::black);
	currentColor->isSavable = false;
	currentColor->setControllableFeedbackOnly(true);
	addParameter(currentColor);

	addColorAt(length->floatValue() / 5, Colours::red);
	addColorAt(length->floatValue() * 2 / 5, Colours::yellow);
	addColorAt(length->floatValue() * 3 / 5, Colours::green)->interpolation->setValueWithData(TimeColor::NONE);
	addColorAt(length->floatValue() * 4 / 5, Colours::blue);

	//rebuildGradient();

	currentColor->setColor(getColorForPosition(position->floatValue()));
}


TimeColorManager::~TimeColorManager()
{
}

void TimeColorManager::setLength(float val, bool stretch, bool stickToEnd)
{
	if (length->floatValue() == val) return;

	float stretchFactor = val / length->floatValue();
	
	float lengthDiff = val - length->floatValue();

	length->setValue(val);
	position->setRange(0, length->floatValue());
	
	
	for (auto &k : items)
	{
		//DBG("Set posMax to " << k->niceName << "/" << (int)k->position);

		if (stretch && stretchFactor < 1) k->position->setValue(k->position->floatValue()*stretchFactor); //reposition then set range

		if (!allowKeysOutside) k->position->setRange(0, length->floatValue());

		if (stretch && stretchFactor > 1) k->position->setValue(k->position->floatValue()*stretchFactor); //set range then reposition

		if (!stretch && stickToEnd) k->position->setValue(k->position->floatValue() + lengthDiff);
	}
	
}

Colour TimeColorManager::getColorForPosition(const float & time) const
{
	if (items.isEmpty()) return Colours::transparentBlack;
	if (time <= items[0]->position->floatValue()) return items[0]->color->getColor();
	if (time >= items[items.size() - 1]->position->floatValue()) return items[items.size() - 1]->color->getColor();

	TimeColor * nearest = getItemAt(time, true);

	if (nearest == nullptr) return Colours::purple;

	TimeColor::Interpolation interpolation = nearest->interpolation->getValueDataAsEnum<TimeColor::Interpolation>();

	switch (interpolation)
	{
	case TimeColor::NONE:
		return nearest->color->getColor();
		break;

	case TimeColor::LINEAR:
	{
		TimeColor * next = items[items.indexOf(nearest) + 1];
		if (nearest == nullptr || next == nullptr || nearest->position->floatValue() == next->position->floatValue()) return nearest->color->getColor();
		return nearest->color->getColor().interpolatedWith(next->color->getColor(), jmap<float>(time, nearest->position->floatValue(), next->position->floatValue(), 0, 1));
	}
	break;
	}

	return Colours::purple;
}

/*
void TimeColorManager::rebuildGradient()
{
	gradientLock.enter();
	gradient.clearColours();
	if (items.size() > 0 && items[0]->position->floatValue() > 0) gradient.addColour(0, items[0]->color->getColor(), items[0]->interpolation->getValueDataAsEnum<ColourGradient::Interpolation>());

	for (auto &i : items)
	{
		i->gradientIndex = gradient.addColour(jlimit<float>(0,1,i->position->floatValue() / length->floatValue()), i->color->getColor(), i->interpolation->getValueDataAsEnum<ColourGradient::Interpolation>());
	}
	gradientLock.exit();

	currentColor->setColor(getColorForPosition(position->floatValue()));
	colorManagerListeners.call(&TimeColorManagerListener::gradientUpdated);
}
*/

TimeColor * TimeColorManager::addColorAt(float time, Colour color)
{
	if (items.isEmpty())  color = color.withAlpha(1.0f); //if only one color, force a non-transparent one to avoid confusion
	TimeColor * t = getItemAt(time);
	if (t == nullptr)
	{
		t = new TimeColor(time, color);
		BaseManager::addItem(t);
	}
	else
	{
		t->color->setColor(color);
	}
	
	reorderItems();
	//rebuildGradient();
	return t;
}

TimeColor * TimeColorManager::getItemAt(float time, bool getNearestPreviousKeyIfNotFound) const
{
	TimeColor * nearestPrevious = nullptr;
	for (auto &t : items)
	{
		if (t->position->floatValue() == time) return t;
		if (t->position->floatValue() > time) break; //avoid looking for further keys, todo : implement dichotomy mechanism
		nearestPrevious = t;
	}

	return getNearestPreviousKeyIfNotFound ? nearestPrevious : nullptr;
}

void TimeColorManager::addItemInternal(TimeColor * item, var data)
{
	//item->gradientIndex = gradient.addColour(item->position->floatValue() / length->floatValue(), item->color->getColor());
	if(!allowKeysOutside) item->position->setRange(0, length->floatValue());
}

void TimeColorManager::removeItemInternal(TimeColor *)
{
	//rebuildGradient();
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
			//gradient.setColour(t->gradientIndex, t->color->getColor());
			colorManagerListeners.call(&TimeColorManagerListener::gradientUpdated);
		}
		else if (c == t->interpolation)
		{
			//gradient.setInterpolation(t->gradientIndex, t->interpolation->getValueDataAsEnum<ColourGradient::Interpolation>());
			colorManagerListeners.call(&TimeColorManagerListener::gradientUpdated);
		}

		//rebuildGradient();
		currentColor->setColor(getColorForPosition(position->floatValue()));
	}
}

void TimeColorManager::loadJSONDataInternal(var data)
{
	BaseManager::loadJSONDataInternal(data);
	//rebuildGradient();
}

