/*
 ==============================================================================

 Trigger.cpp
 Created: 8 Mar 2016 1:09:29pm
 Author:  bkupe

 ==============================================================================
 */

#include "Trigger.h"

#include "TriggerButtonUI.h"
#include "TriggerBlinkUI.h"

//#include "JsHelpers.h"

Trigger::Trigger(const String & niceName, const String &description, bool enabled) :
Controllable(TRIGGER, niceName, description, enabled),
isTriggering(false),
queuedNotifier(5)
{
}

TriggerButtonUI * Trigger::createButtonUI(Trigger * target)
{
	if (target == nullptr) target = this;
    return new TriggerButtonUI(target);
}

TriggerBlinkUI * Trigger::createBlinkUI(Trigger * target)
{
	if (target == nullptr) target = this;
	return new TriggerBlinkUI(target);
}

ControllableUI * Trigger::createDefaultUI(Controllable * targetControllable){
    return createBlinkUI(dynamic_cast<Trigger *>(targetControllable));
}

void Trigger::trigger()
{
	if (enabled && !isTriggering) {
		isTriggering = true;
		listeners.call(&Listener::triggerTriggered, this);
		queuedNotifier.addMessage(new WeakReference<Trigger>(this));
		isTriggering = false;
	}
}

/*
DynamicObject * Trigger::createDynamicObject()
{
	DynamicObject* dObject = Controllable::createDynamicObject();
	dObject->setMethod(jsTriggerIdentifier, setControllableValue);
	dObject->setProperty(jsPtrIdentifier, (int64)this);

	return dObject;
}
*/