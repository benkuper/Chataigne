/*
  ==============================================================================

	WiimotePairUtil.cpp
	Created: 9 May 2017 6:49:56pm
	Author:  Ben

  ==============================================================================
*/

#pragma warning(disable:4995)

bool WiiPairUtil::shouldStop = false;

WiiPairUtil::WiiPairUtil()
{

}

WiiPairUtil::~WiiPairUtil()
{
}

int WiiPairUtil::pair(bool forceRepairing)
{
#if JUCE_WINDOWS
    return pairWin(forceRepairing);
#elif JUCE_MAC
    return pairMac(forceRepairing);
#else
    return 0;
#endif
}

#if JUCE_WINDOWS
DWORD WiiPairUtil::ShowErrorCode(LPTSTR msg, DWORD dw)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);

	NLOGERROR("Wiimote Pairing", String(msg) << " : "<< String((LPSTR)lpMsgBuf));

	LocalFree(lpMsgBuf);

	return dw;
}

String WiiPairUtil::FormatBTAddress(BLUETOOTH_ADDRESS address)
{
	String s;
	for (int i = 5; i >= 0; i--)
	{
		s += String::toHexString(address.rgBytes[i]);
		if (i > 0) s += ":";
	}
	
	return s;
}

int WiiPairUtil::pairWin(bool forceRepairing)
{
	HANDLE hRadios[256];
	int nRadios;
	int nPaired = 0;


	///////////////////////////////////////////////////////////////////////
	// Enumerate BT radios
	///////////////////////////////////////////////////////////////////////
	{
		HBLUETOOTH_RADIO_FIND hFindRadio;
		BLUETOOTH_FIND_RADIO_PARAMS radioParam;

		radioParam.dwSize = sizeof(BLUETOOTH_FIND_RADIO_PARAMS);

		nRadios = 0;
		hFindRadio = BluetoothFindFirstRadio(&radioParam, &hRadios[nRadios++]);
		if (hFindRadio)
		{
			//while (BluetoothFindNextRadio(&radioParam, &hRadios[nRadios++]));
			BluetoothFindRadioClose(hFindRadio);
		} else
		{
			DBG("Error enumerating radios : " << String(GetLastError()));
			return 0;
		}
		//nRadios--;
		DBG("Found " << nRadios << " radios");
	}

	///////////////////////////////////////////////////////////////////////
	// Keep looping until we pair with a Wii device
	///////////////////////////////////////////////////////////////////////

	while (nPaired == 0 && !shouldStop)
	{
		int radio;

		for (radio = 0; radio < nRadios; radio++)
		{
			BLUETOOTH_RADIO_INFO radioInfo;
			HBLUETOOTH_DEVICE_FIND hFind;
			BLUETOOTH_DEVICE_INFO btdi;
			BLUETOOTH_DEVICE_SEARCH_PARAMS srch;

			radioInfo.dwSize = sizeof(radioInfo);
			btdi.dwSize = sizeof(btdi);
			srch.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);

			//DBG("BluetoothGetRadioInfo " << String(BluetoothGetRadioInfo(hRadios[radio], &radioInfo)));
			//DBG("Radio " << radio << " (" << String(radioInfo.szName) << ") : " << FormatBTAddress(radioInfo.address));
	
			srch.fReturnAuthenticated = TRUE;
			srch.fReturnRemembered = TRUE;
			srch.fReturnConnected = TRUE;
			srch.fReturnUnknown = TRUE;
			srch.fIssueInquiry = TRUE;
			srch.cTimeoutMultiplier = 2;
			srch.hRadio = hRadios[radio];

			//DBG("Scanning...");

			hFind = BluetoothFindFirstDevice(&srch, &btdi);

			if (hFind == NULL)
			{
				if (GetLastError() == ERROR_NO_MORE_ITEMS)
				{
					//DBG("No bluetooth devices found.");
				} else
				{
					//DBG("Error enumerating devices : " << String(GetLastError()));
					return 0;
				}
			} else
			{
				do
				{
					//DBG("Found: " << btdi.szName);

					if (!wcscmp(btdi.szName, L"Nintendo RVL-WBC-01") || !wcscmp(btdi.szName, L"Nintendo RVL-CNT-01"))
					{
						WCHAR pass[6];
						DWORD pcServices = 16;
						GUID guids[16];
						BOOL error = FALSE;

						bool alreadyPaired = false;
						if (!error)
						{
							if (btdi.fRemembered)
							{
								if (forceRepairing)
								{
									// Make Windows forget pairing
									if (ShowErrorCode(("BluetoothRemoveDevice"), BluetoothRemoveDevice(&btdi.Address)) != ERROR_SUCCESS)
										error = TRUE;
								}else
								{
									alreadyPaired = true;
								}
							} 
						}

						if (!alreadyPaired)
						{

							// MAC address is passphrase
							pass[0] = radioInfo.address.rgBytes[0];
							pass[1] = radioInfo.address.rgBytes[1];
							pass[2] = radioInfo.address.rgBytes[2];
							pass[3] = radioInfo.address.rgBytes[3];
							pass[4] = radioInfo.address.rgBytes[4];
							pass[5] = radioInfo.address.rgBytes[5];

							if (!error)
							{
								// Pair with Wii device
								if (ShowErrorCode(("BluetoothAuthenticateDevice"), BluetoothAuthenticateDevice(NULL, hRadios[radio], &btdi, pass, 6)) != ERROR_SUCCESS)
									error = TRUE;
							}

							if (!error)
							{
								// If this is not done, the Wii device will not remember the pairing
								if (ShowErrorCode(("BluetoothEnumerateInstalledServices"), BluetoothEnumerateInstalledServices(hRadios[radio], &btdi, &pcServices, guids)) != ERROR_SUCCESS)
									error = TRUE;
							}

							if (!error)
							{
								// Activate service
								if (ShowErrorCode(("BluetoothSetServiceState"), BluetoothSetServiceState(hRadios[radio], &btdi, &HumanInterfaceDeviceServiceClass_UUID, BLUETOOTH_SERVICE_ENABLE)) != ERROR_SUCCESS)
									error = TRUE;
							}
						}

						if (!error)
						{
							nPaired++;
						}
					} // if (!wcscmp(btdi.szName, L"Nintendo RVL-WBC-01") || !wcscmp(btdi.szName, L"Nintendo RVL-CNT-01"))
				} while (BluetoothFindNextDevice(hFind, &btdi));
			} // if (hFind == NULL)
		} // for (radio = 0; radio < nRadios; radio++)


		if (shouldStop) return false;
		Sleep(1000);
	}

	///////////////////////////////////////////////////////////////////////
	// Clean up
	///////////////////////////////////////////////////////////////////////

	{
		int radio;

		for (radio = 0; radio < nRadios; radio++)
		{
			CloseHandle(hRadios[radio]);
		}
	}

	DBG(nPaired << " Wii devices paired");

	return nPaired;
}

#endif



#if JUCE_MAC



int WiiPairUtil::pairMac(bool forceRepairing)
{
    return 0;
}
#endif
