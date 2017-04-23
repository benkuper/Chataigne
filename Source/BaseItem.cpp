/*
  ==============================================================================

    BaseItem.cpp
    Created: 28 Oct 2016 8:04:25pm
    Author:  bkupe

  ==============================================================================
*/

#include "BaseItem.h"
#include "BaseItemEditor.h"
#include "ScriptManager.h"

BaseItem::BaseItem(const String &name, bool _canBeDisabled, bool _canHaveScripts) :
	ControllableContainer(name),
	canBeDisabled(_canBeDisabled),
	canHaveScripts(_canHaveScripts)
{
	saveAndLoadName = true;

	if (canBeDisabled)
	{
		enabled = addBoolParameter("Enabled", "Enable / Disable this component", true);
		enabled->hideInOutliner = true;
	}

	nameParam = addStringParameter("Name", "Name of the component", niceName);
	nameParam->hideInEditor = true;
	nameParam->hideInOutliner = true;
	nameParam->isTargettable = false;

	if (canHaveScripts)
	{
		scriptManager = new ScriptManager(this);
		addChildControllableContainer(scriptManager);
	}

	//For UI
	miniMode = addBoolParameter("Mini Mode", "Set the mini mode", false);
	miniMode->hideInOutliner = true;
	miniMode->hideInEditor = true;
	miniMode->isTargettable = false;

	listUISize = addFloatParameter("List Size", "Size in list", 0, 0, 500);
	listUISize->hideInEditor = true;
	listUISize->hideInOutliner = true;
	listUISize->isTargettable = false;

	viewUIPosition = addPoint2DParameter("Position", "Position the view");
	viewUIPosition->setBounds(-100, -100, 100, 100);
	viewUIPosition->hideInEditor = true;
	viewUIPosition->hideInOutliner = true;
	viewUIPosition->isTargettable = false;

	viewUISize = addPoint2DParameter("Size", "Size in the view");
	viewUISize->setBounds(30, 60, 500, 500);
	viewUISize->setPoint(200, 300);
	viewUISize->hideInEditor = true;
	viewUISize->hideInOutliner = true;
	viewUISize->isTargettable = false;
}

BaseItem::~BaseItem()
{
	clear();
}

void BaseItem::remove()
{
	baseItemListeners.call(&BaseItem::Listener::askForRemoveBaseItem, this);
}

void BaseItem::onContainerParameterChanged(Parameter * p)
{
	if (p == nameParam)
	{
		setNiceName(nameParam->stringValue());
	}

	onContainerParameterChangedInternal(p);
}

void BaseItem::onContainerNiceNameChanged()
{
	nameParam->setValue(niceName);
}

var BaseItem::getJSONData()
{
	var data = ControllableContainer::getJSONData();
	if (canHaveScripts) data.getDynamicObject()->setProperty("scripts", scriptManager->getJSONData());

	return data; 
}

void BaseItem::loadJSONDataInternal(var data)
{
	ControllableContainer::loadJSONDataInternal(data);
	if (canHaveScripts) scriptManager->loadJSONData(data.getProperty("scripts",var()));
}

InspectableEditor * BaseItem::getEditor(bool isRoot)
{
	return ControllableContainer::getEditor(isRoot);// new BaseItemEditor(this, isRoot);
}
