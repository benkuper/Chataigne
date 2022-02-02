/*
  ==============================================================================

	WindowsHooker.cpp
	Created: 29 Sep 2021 11:19:23am
	Author:  bkupe

  ==============================================================================
*/

#if JUCE_WINDOWS



juce_ImplementSingleton(WindowsHooker)

WindowsHooker::WindowsHooker() :
	keyboardhook(nullptr)
{
	keyboardhook = SetWindowsHookEx(WH_KEYBOARD_LL, &WindowsHooker::keyboardCallback, NULL, 0);
	if (keyboardhook == nullptr)
	{
		LOGERROR("Error hooking keyboard :\n" << String(GetLastError()));
	}

	for (int i = 0; i < 192; i++)
	{
		keyMap.set(keyCodes[i], keyNames[i]);
	}
}

WindowsHooker::~WindowsHooker()
{
}

LRESULT __stdcall WindowsHooker::keyboardCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (Engine::mainEngine != nullptr && !Engine::mainEngine->isClearing && WindowsHooker::getInstanceWithoutCreating() != nullptr)
	{

		KBDLLHOOKSTRUCT* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;

		if (nCode == 0)
		{
			switch (wParam)
			{
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				//printf_s("Not Sys Key\n");
				WindowsHooker::getInstance()->listeners.call(&Listener::keyChanged, pKeyStruct->vkCode, true);
			}
			break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				WindowsHooker::getInstance()->listeners.call(&Listener::keyChanged, pKeyStruct->vkCode, false);
				break;
			}
		}
	}

	return CallNextHookEx(WindowsHooker::getInstance()->keyboardhook, nCode, wParam, lParam);
}

String WindowsHooker::getKeyName(int keyCode)
{
	if (keyMap.contains(keyCode)) return keyMap[keyCode];
	return "";
}

#endif