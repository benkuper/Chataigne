/*
  ==============================================================================

    InputFactory.cpp
    Created: 30 Oct 2016 3:11:43pm
    Author:  bkupe

  ==============================================================================
*/

#include "InputFactory.h"

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

juce_ImplementSingleton(InputFactory)

InputFactory::InputFactory() {
	  inputDefs.add(new InputDefinition("Generic", "OSC", &CustomOSCInput::create));
	  inputDefs.add(new InputDefinition("Generic", "MIDI", &MIDIInputModule::create));
	  inputDefs.add(new InputDefinition("Generic", "DMX", &DMXInput::create));
	  inputDefs.add(new InputDefinition("Generic", "Serial", &SerialInput::create));

	  inputDefs.add(new InputDefinition("Audio", "Microphone", &MicrophoneInput::create));
	  inputDefs.add(new InputDefinition("Audio", "Live", &LiveInput::create));

	  inputDefs.add(new InputDefinition("Video", "Resolume", &ResolumeInput::create));
	  inputDefs.add(new InputDefinition("Video", "Millumin", &MilluminInput::create));

	  inputDefs.add(new InputDefinition("Controller", "Myo", &MyoInput::create));
	  inputDefs.add(new InputDefinition("Controller", "BGlove", &BGloveInput::create));
	  inputDefs.add(new InputDefinition("Controller", "Bento", &BentoInput::create));

	  inputDefs.add(new InputDefinition("Light", "DLight", &DLightInput::create));

	  inputDefs.add(new InputDefinition("Vision", "Augmenta", &AugmentaInput::create));
	  inputDefs.add(new InputDefinition("Vision", "Kinect", &KinectInput::create));
	  inputDefs.add(new InputDefinition("Vision", "KinectV2", &KinectV2Input::create));
	  inputDefs.add(new InputDefinition("Vision", "Leap Motion", &LeapMotionInput::create));


	  buildPopupMenu();
  }

inline void InputFactory::buildPopupMenu()
{
	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;

	for (auto &d : inputDefs)
	{
		int subMenuIndex = -1;
		for (int i = 0; i < subMenus.size(); i++)
		{
			if (subMenuNames[i] == d->menuPath)
			{
				subMenuIndex = i;
				break;
			}
		}
		if (subMenuIndex == -1)
		{
			subMenuNames.add(d->menuPath);
			subMenus.add(new PopupMenu());
			subMenuIndex = subMenus.size() - 1;
		}

		int itemID = inputDefs.indexOf(d) + 1;//start at 1 for menu
		subMenus[subMenuIndex]->addItem(itemID, d->inputType);
	}

	for (int i = 0; i < subMenus.size(); i++) menu.addSubMenu(subMenuNames[i], *subMenus[i]);
}
