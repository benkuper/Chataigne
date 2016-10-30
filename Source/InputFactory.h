/*
  ==============================================================================

    InputFactory.h
    Created: 30 Oct 2016 1:14:32pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INPUTFACTORY_H_INCLUDED
#define INPUTFACTORY_H_INCLUDED

/*
==============================================================================

InputFactor.h
Created: 8 Mar 2016 10:25:48pm
Author:  bkupe

==============================================================================
*/

#include "CustomOSCInput.h"
#include "MIDIInputModule.h"
#include "DMXInput.h"
#include "SerialInput.h"
#include "AugmentaInput.h"
#include "BentoInput.h"
#include "BGloveInput.h"
#include "DLightInput.h"
#include "LiveInput.h"
#include "MilluminInput.h"
#include "ResolumeInput.h"
#include "MyoInput.h"
#include "MicrophoneInput.h"
#include "LeapMotionInput.h"
#include "KinectInput.h"
#include "KinectV2Input.h"

class InputDefinition
{
public:
	String menuPath;
	String inputType;
	std::function<Input*()> createFunc;

	InputDefinition(String menuPath, String type, std::function<Input*()> createFunc) :
		menuPath(menuPath), 
		inputType(type),
		createFunc(createFunc)
	{}
};

class InputFactory
{
public:
	juce_DeclareSingleton(InputFactory,true);

	OwnedArray<InputDefinition> inputDefs;
	PopupMenu menu;

	InputFactory();
	~InputFactory() {}

	void buildPopupMenu();

	static Input * showCreateMenu()
	{
		int result = getInstance()->menu.show();
		if (result == 0) return nullptr;
		else
		{
			InputDefinition * d = getInstance()->inputDefs[result-1];//result 0 is no result
			return d->createFunc();
		}
	}

	static Input * createInput(const String &inputType)
	{
		for (auto &d : getInstance()->inputDefs) if (d->inputType == inputType) return d->createFunc();
		return nullptr;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InputFactory)
};


#endif  // INPUTFACTORY_H_INCLUDED
