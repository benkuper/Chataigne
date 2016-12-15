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
	easing = new LinearEasing();
	position = addFloatParameter("Position", "Position of the key", 0, 0, 5);
	value = addFloatParameter("Value", "Value of the key", 0, 0, 1);

	easingType = addEnumParameter("EasingType", "Type of transition to the next key");

	easingType->addOption("Linear", Easing::Type::LINEAR);
	easingType->addOption("Hold", Easing::Type::HOLD);

	easingType->setValue("Linear");
}

AutomationKey::~AutomationKey()
{
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
		switch ((Easing::Type)(int)easingType->getValueData())
		{
		case Easing::Type::LINEAR:
			easing = new LinearEasing();
			break;

		case Easing::Type::HOLD:
			easing = new HoldEasing();
			break;
		}
	}
}
