/*
  ==============================================================================

    CommonIncludes.cpp
    Created: 10 Mar 2021 9:55:09am
    Author:  bkupe

  ==============================================================================
*/

#include "CommonIncludes.h"
#include "MainIncludes.h"

#include "MIDI/MIDIDevice.cpp"
#include "MIDI/MIDIDeviceParameter.cpp"
#include "MIDI/MIDIManager.cpp"
#include "MIDI/MTCReceiver.cpp"
#include "MIDI/MTCSender.cpp"
#include "MIDI/ui/MIDIDeviceChooser.cpp"
#include "MIDI/ui/MIDIDeviceParameterUI.cpp"


#include "Zeroconf/ZeroconfManager.cpp" 

#include "LTC/ltc.c"
#include "LTC/timecode.c"
#include "LTC/encoder.c"
#include "LTC/decoder.c"

#include "InputSystem/InputSystemManager.cpp"
#include "InputSystem/InputDeviceHelpers.cpp"

#if BLE_SUPPORT
#include "BLE/BLEDevice.cpp"
#include "BLE/BLEManager.cpp"
#include "BLE/BLEDeviceParameter.cpp"
#endif