/*
  ==============================================================================

	BaseItemEditor.cpp
	Created: 18 Jan 2017 2:23:31pm
	Author:  Ben

  ==============================================================================
*/

#include "BaseItemEditor.h"
#include "AssetManager.h"
#include "ScriptManager.h"

BaseItemEditor::BaseItemEditor(BaseItem * bi, bool isRoot) :
	InspectableEditor(bi, isRoot),
	item(bi),
    headerHeight(20),
    transparentBG(false),   
	paintHeaderOnly(isRoot)
{
	if (item->canBeDisabled)
	{
		enabledUI = item->enabled->createImageToggle(AssetManager::getInstance()->getPowerBT());
		addAndMakeVisible(enabledUI);
	}

	nameUI = item->nameParam->createStringParameterUI();
	addAndMakeVisible(nameUI);

	if (!isRoot && item->userCanRemove)
	{
		removeBT = AssetManager::getInstance()->getRemoveBT();
		addAndMakeVisible(removeBT);
		removeBT->addListener(this);
	}

	if (item->canHaveScripts)
	{
		scriptManagerUI = item->scriptManager->getEditor(false);
		addAndMakeVisible(scriptManagerUI);
	}

	item->addAsyncContainerListener(this);

	if(item->canBeDisabled) setAlpha(item->enabled->boolValue() ? 1 : .6f);
}

BaseItemEditor::~BaseItemEditor()
{
	if (!inspectable.wasObjectDeleted()) item->removeAsyncContainerListener(this);
}

void BaseItemEditor::paint(Graphics & g)
{
	if (transparentBG) return;
	Rectangle<int> r = getLocalBounds();
	if (paintHeaderOnly) r.setHeight((int)headerHeight);
	Colour c = (item->canBeDisabled && !item->enabled->boolValue()) ? BG_COLOR.darker().withAlpha(.6f): BG_COLOR.brighter().withAlpha(.3f);
	g.setColour(c);
	g.fillRoundedRectangle(r.toFloat(), 2);

}

void BaseItemEditor::resized()
{
	Rectangle<int> r = getLocalBounds().withHeight((int)headerHeight).reduced(2);
	Rectangle<int> hr = Rectangle<int>(r);

	if (item->canBeDisabled)
	{
		enabledUI->setBounds(hr.removeFromLeft(hr.getHeight()));
	}
	if (!isRoot && item->userCanRemove)
	{
		removeBT->setBounds(hr.removeFromRight(hr.getHeight()));
		hr.removeFromRight(2);
	}
	resizedInternalHeader(hr);
	nameUI->setBounds(hr);

	r.translate(0, r.getBottom() + 2);
	r.setHeight(0); //if no override, ensure bottom is set
	resizedInternalContent(r);

	if (item->canHaveScripts)
	{
		r.setY(r.getBottom() + 2);
		r.setHeight(scriptManagerUI->getHeight());
		scriptManagerUI->setBounds(r);
	}

	r.setY(r.getBottom() + 2);
	r.setHeight(0); //if no override, ensure bottom is set
	resizedInternalFooter(r);

	if (isRoot) setSize(getWidth(), r.getBottom());
	else setBounds(getLocalBounds().withBottom(r.getBottom()));
}


void BaseItemEditor::newMessage(const ContainerAsyncEvent & e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:
		controllableFeedbackAsyncUpdate(e.targetControllable);
		break;
            
        default:
            break;
	}
}

void BaseItemEditor::controllableFeedbackAsyncUpdate(Controllable * c)
{
	if (item->canBeDisabled && c == item->enabled)
	{
		setAlpha(item->enabled->boolValue() ? 1 : .6f);
		repaint();
	}
}

void BaseItemEditor::childBoundsChanged(Component *)
{
	resized();
}

void BaseItemEditor::buttonClicked(Button * b)
{
	if (b == removeBT)
	{
		item->remove();
		return;
	}
}

