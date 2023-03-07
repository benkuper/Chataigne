/*
  ==============================================================================

    CommonIncludes.cpp
    Created: 10 Mar 2021 9:55:09am
    Author:  bkupe

  ==============================================================================
*/

#include "CommonIncludes.h"
#include "MainIncludes.h"

#include "DMX/DMXManager.cpp"
#include "DMX/DMXUniverse.cpp"
#include "DMX/DMXUniverseManager.cpp"
#include "DMX/device/DMXDevice.cpp"
#include "DMX/device/DMXSerialDevice.cpp"
#include "DMX/device/DMXArtNetDevice.cpp"
#include "DMX/device/DMXEnttecProDevice.cpp"
#include "DMX/device/DMXOpenUSBDevice.cpp"
#include "DMX/device/DMXSACNDevice.cpp"

#include "DMX/ui/DMXUniverseEditor.cpp"

#include "MIDI/MIDIDevice.cpp"
#include "MIDI/MIDIDeviceParameter.cpp"
#include "MIDI/MIDIManager.cpp"
#include "MIDI/MTCReceiver.cpp"
#include "MIDI/MTCSender.cpp"
#include "MIDI/ui/MIDIDeviceChooser.cpp"
#include "MIDI/ui/MIDIDeviceParameterUI.cpp"

#include "Serial/SerialDevice.cpp"
#include "Serial/SerialDeviceParameter.cpp"
#include "Serial/SerialManager.cpp"
#include "Serial/lib/cobs/cobs.cpp"
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