/*
  ==============================================================================

    ModuleEditor.cpp
    Created: 28 Dec 2016 1:37:59pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleEditor.h"
#include "AssetManager.h"

ModuleEditor::ModuleEditor(Module * _module, bool isRoot) :
	InspectableEditor(_module,isRoot),
	module(_module)
{
	enabledUI = module->enabled->createImageToggle(AssetManager::getInstance()->getPowerBT());
	nameUI = module->nameParam->createStringParameterUI();
	logIncomingUI = module->logIncomingData->createToggle();
	logOutgoingUI = module->logOutgoingData->createToggle();

	addAndMakeVisible(enabledUI);
	addAndMakeVisible(nameUI);
	addAndMakeVisible(logIncomingUI);
	addAndMakeVisible(logOutgoingUI);

}

ModuleEditor::~ModuleEditor()
{
}

void ModuleEditor::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds().withHeight(headerHeight);
	g.setColour(BG_COLOR.brighter(.1f));
	g.fillRoundedRectangle(r.toFloat(), 2);
}

void ModuleEditor::resized()
{
	Rectangle<int> r = getLocalBounds().withHeight(headerHeight);
	Rectangle<int> hr = r.reduced(2);

	enabledUI->setBounds(hr.removeFromLeft(hr.getHeight()));
	logOutgoingUI->setBounds(hr.removeFromRight(60));
	hr.removeFromRight(2); 
	logIncomingUI->setBounds(hr.removeFromRight(60));
	hr.reduce(5, 0);
	nameUI->setBounds(hr);

	r.translate(0,hr.getBottom() + 5);
	resizedInternal(r);

	setBounds(getLocalBounds().withBottom(r.getBottom()));
}

void ModuleEditor::newMessage(const ContainerAsyncEvent & e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:
		controllableFeedbackAsyncUpdate(e.targetControllable);
		break;
	}
}

void ModuleEditor::controllableFeedbackAsyncUpdate(Controllable * c)
{
	if (c == module->logIncomingData)
	{

	}
	else if (c == module->logOutgoingData)
	{

	}
}
