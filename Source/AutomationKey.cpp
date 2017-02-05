/*
  ==============================================================================

	AutomationKey.cpp
	Created: 11 Dec 2016 1:22:20pm
	Author:  Ben

  ==============================================================================
*/

#include "AutomationKey.h"

AutomationKey::AutomationKey()
{
	isSelectable = false;

	position = addFloatParameter("Position", "Position of the key", 0, 0, 5);
	value = addFloatParameter("Value", "Value of the key", 0, 0, 1);

	easingType = addEnumParameter("EasingType", "Type of transition to the next key");

	easingType->addOption("Linear", Easing::LINEAR);
	easingType->addOption("Hold", Easing::HOLD);
	easingType->addOption("Bezier", Easing::BEZIER);

	easingType->setValueWithKey("Linear");

	setEasing(Easing::LINEAR);
}

AutomationKey::~AutomationKey()
{
}

void AutomationKey::setEasing(Easing::Type t)
{

	if (easing != nullptr)
	{
		if (easing->type == t) return;

		removeChildControllableContainer(easing);
	}

	switch (t)
	{
	case Easing::LINEAR:
		easing = new LinearEasing();
		break;

	case Easing::HOLD:
		easing = new HoldEasing();
		break;

	case Easing::BEZIER:
		easing = new CubicEasing();
		break;
	}

	if (easing != nullptr)
	{
		addChildControllableContainer(easing);
	}
}

float AutomationKey::getValue(AutomationKey * nextKey, const float & _pos)
{
	float relPos = jmap<float>(_pos, position->floatValue(), nextKey->position->floatValue(), 0, 1);

	jassert(relPos >= 0 && relPos <= 1);

	return easing->getValue(value->floatValue(), nextKey->value->floatValue(), relPos);
}

void AutomationKey::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == easingType)
	{
		setEasing((Easing::Type)(int)easingType->getValueData());
	}
}
