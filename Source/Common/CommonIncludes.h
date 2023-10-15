/*
  ==============================================================================

	CommonIncludes.h
	Created: 10 Mar 2021 9:55:09am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#ifndef BLE_SUPPORT
#define BLE_SUPPORT 0
#endif

#if BLE_SUPPORT
#include "simpleble/SimpleBLE.h"
using namespace SimpleBLE;

#include "BLE/BLEDevice.h"
#include "BLE/BLEManager.h"
#include "BLE/BLEDeviceParameter.h"

#endif


#include "MIDI/MIDIDevice.h"
#include "MIDI/MIDIManager.h"
#include "MIDI/MIDIDeviceParameter.h"
#include "MIDI/MTCReceiver.h"
#include "MIDI/MTCSender.h"
#include "MIDI/ui/MIDIDeviceChooser.h"
#include "MIDI/ui/MIDIDeviceParameterUI.h"


//#include "LTC/config.h"
#include "LTC/ltc.h"
#include "LTC/encoder.h"
#include "LTC/decoder.h"

#include "Zeroconf/ZeroconfManager.h"

#include "InputSystem/InputSystemManager.h"
#include "InputSystem/InputDeviceHelpers.h"

