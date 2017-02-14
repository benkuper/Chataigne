/*
  ==============================================================================

    ModuleInputValueChooserUI.cpp
    Created: 8 Feb 2017 9:38:57am
    Author:  Ben

  ==============================================================================
*/

#include "ModuleInputValueChooserUI.h"
#include "ModuleManager.h"

ModuleInputValueChooserUI::ModuleInputValueChooserUI(TargetParameter * p, bool _parametersOnly) :
	TargetParameterUI(p),
	parametersOnly(_parametersOnly)
{
	updateLabel();
}

ModuleInputValueChooserUI::~ModuleInputValueChooserUI()
{
}

void ModuleInputValueChooserUI::updateLabel()
{
	if (targetParameter->target != nullptr)
	{
		String moduleName = targetParameter->target->parentContainer->parentContainer->niceName;
		label.setText(moduleName + ":"+targetParameter->target->niceName, dontSendNotification);
	} else
	{
		TargetParameterUI::updateLabel();
	}
}

void ModuleInputValueChooserUI::showPopupAndGetTarget()
{
	PopupMenu p = ModuleManager::getInstance()->getAllModulesInputValuesMenu(parametersOnly);
	int result = p.show();
	Controllable * c = ModuleManager::getInstance()->getControllableForItemID(result);
	if (c != nullptr) ((TargetParameter *)parameter.get())->setValueFromTarget(c);
}
