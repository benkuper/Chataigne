/*
  ==============================================================================

    BoolParameter.cpp
    Created: 8 Mar 2016 1:22:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "BoolParameter.h"
#include "BoolToggleUI.h"

BoolParameter::BoolParameter(const String & niceName, const String &description, bool initialValue, bool enabled) :
    Parameter(Type::BOOL, niceName, description, initialValue, false, true, enabled)
{
    setValue(initialValue);
}

BoolToggleUI * BoolParameter::createToggle(BoolParameter * target)
{
	if (target == nullptr) target = this;
    return new BoolToggleUI(target);
}


ControllableUI * BoolParameter::createDefaultUI(Controllable * targetControllable) {
	return createToggle(dynamic_cast<BoolParameter *>(targetControllable));
}
