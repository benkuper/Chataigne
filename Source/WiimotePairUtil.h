/*
  ==============================================================================

    WiimotePairUtil.h
    Created: 9 May 2017 6:49:56pm
    Author:  Ben

  ==============================================================================
*/

#ifndef WIIMOTEPAIRUTIL_H_INCLUDED
#define WIIMOTEPAIRUTIL_H_INCLUDED


#include <windows.h>
#include <bthsdpdef.h>
#include <bthdef.h>
#include <BluetoothAPIs.h>
#include <strsafe.h>

#pragma comment(lib, "Bthprops.lib")

#include "JuceHeader.h"

class WiiPairUtil
{
public:
	WiiPairUtil();
	~WiiPairUtil();


	static DWORD ShowErrorCode(LPTSTR msg, DWORD dw);

	static String FormatBTAddress(BLUETOOTH_ADDRESS address);

	static int pair(bool forceRepairing = false);
};


#endif  // WIIMOTEPAIRUTIL_H_INCLUDED
