/*
  ==============================================================================

    WiimotePairUtil.h
    Created: 9 May 2017 6:49:56pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#if JUCE_WINDOWS

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#include <windows.h>
#include <bthsdpdef.h>
#include <bthdef.h>
#include <BluetoothAPIs.h>
#include <strsafe.h>

#pragma comment(lib, "Bthprops.lib")


#elif JUCE_MAC

#endif


class WiiPairUtil
{
public:
	WiiPairUtil();
	~WiiPairUtil();

	static bool shouldStop; //safe stop for wiimote thread stop
    
    static int pair(bool forceRepairing = false);
    
#if JUCE_WINDOWS
	static DWORD ShowErrorCode(LPTSTR msg, DWORD dw);
    static String FormatBTAddress(BLUETOOTH_ADDRESS address);
    static int pairWin(bool forceRepairing = false);
#elif JUCE_MAC
    
    static int pairMac(bool forceRepairing = false);
#endif
};