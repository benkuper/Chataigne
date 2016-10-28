/*
  ==============================================================================

    ShapeShifterFactory.cpp
    Created: 18 May 2016 11:45:57am
    Author:  bkupe

  ==============================================================================
*/

#include "ShapeShifterFactory.h"

#include "Inspector.h"
#include "FlapLoggerUI.h"
#include "MainComponent.h"
#include "InputManagerUI.h"

ShapeShifterContent * ShapeShifterFactory::createContentForIndex(PanelName pn)
{
	String contentName = globalPanelNames[(int)pn];

	switch (pn)
	{
	case InputsPanel:
		return new InputManagerUI(InputManager::getInstance());
		break;
	case OutputsPanel:
		break;

	case StateMachinePanel:

		break;

	case TimeMachinePanel:
		break;

	case StatesPanel:
		break;

	case SequencesPanel:
		break;

	case InspectorPanel:
		return new InspectorViewport(contentName, Inspector::getInstance());
		break;

	case LoggerPanel:
		return new FlapLoggerUI(contentName, FlapLogger::getInstance());
		break;


	default:
		DBG("Panel not handled : " << contentName << ", index in names = " << globalPanelNames.strings.indexOf(contentName));
	}
	return nullptr;
}

ShapeShifterContent * ShapeShifterFactory::createContentForName(String name)
{
	return createContentForIndex((PanelName)globalPanelNames.strings.indexOf(name));
}
