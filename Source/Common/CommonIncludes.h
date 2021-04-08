/*
  ==============================================================================

    CommonIncludes.h
    Created: 10 Mar 2021 9:55:09am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "Serial/lib/cobs/cobs.h"
#include "Serial/SerialDevice.h"
#include "Serial/SerialManager.h"
#include "Serial/SerialDeviceParameter.h"

#include "DMX/DMXManager.h"
#include "DMX/device/DMXDevice.h"
#include "DMX/device/DMXSerialDevice.h"
#include "DMX/device/DMXArtNetDevice.h"
#include "DMX/device/DMXEnttecProDevice.h"
#include "DMX/device/DMXOpenUSBDevice.h"
#include "DMX/device/DMXSACNDevice.h"

#include "MIDI/MIDIDevice.h"
#include "MIDI/MIDIManager.h"
#include "MIDI/MIDIDeviceParameter.h"
#include "MIDI/MTCReceiver.h"
#include "MIDI/MTCSender.h"
#include "MIDI/ui/MIDIDeviceChooser.h"
#include "MIDI/ui/MIDIDeviceParameterUI.h"

#include "Zeroconf/ZeroconfManager.h"