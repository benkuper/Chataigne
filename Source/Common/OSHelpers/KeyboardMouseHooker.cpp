/*
  ==============================================================================

	KeyboardHooker.cpp
	Created: 29 Sep 2021 11:19:23am
	Author:  bkupe

  ==============================================================================
*/

#if JUCE_WINDOWS



juce_ImplementSingleton(KeyboardHooker)

KeyboardHooker::KeyboardHooker() :
	keyboardhook(nullptr)
{
	keyboardhook = SetWindowsHookEx(WH_KEYBOARD_LL, &KeyboardHooker::keyboardCallback, NULL, 0);
	if (keyboardhook == nullptr)
	{
		LOGERROR("Error hooking keyboard :\n" << String(GetLastError()));
	}

	for (int i = 0; i < 192; i++)
	{
		keyMap.set(keyCodes[i], keyNames[i]);
	}
}

KeyboardHooker::~KeyboardHooker()
{
	UnhookWindowsHookEx(keyboardhook);
}

LRESULT __stdcall KeyboardHooker::keyboardCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (Engine::mainEngine != nullptr && !Engine::mainEngine->isClearing && KeyboardHooker::getInstanceWithoutCreating() != nullptr)
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
				KeyboardHooker::getInstance()->listeners.call(&Listener::keyChanged, pKeyStruct->vkCode, true);
			}
			break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				KeyboardHooker::getInstance()->listeners.call(&Listener::keyChanged, pKeyStruct->vkCode, false);
				break;
			}
		}
	}

	return CallNextHookEx(KeyboardHooker::getInstance()->keyboardhook, nCode, wParam, lParam);
}

String KeyboardHooker::getKeyName(int keyCode)
{
	if (keyMap.contains(keyCode)) return keyMap[keyCode];
	return "";
}


juce_ImplementSingleton(MouseHooker)

MouseHooker::MouseHooker() :
	mousehook(nullptr)
{
	mousehook = SetWindowsHookEx(WH_MOUSE_LL, &MouseHooker::mouseCallback, NULL, 0);
	if (mousehook == nullptr)
	{
		LOGERROR("Error hooking mouse :\n" << String(GetLastError()));
	}
}

MouseHooker::~MouseHooker()
{
	UnhookWindowsHookEx(mousehook);
}

LRESULT __stdcall MouseHooker::mouseCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (Engine::mainEngine != nullptr && !Engine::mainEngine->isClearing && MouseHooker::getInstanceWithoutCreating() != nullptr)
	{
		MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

		if (nCode == 0)
		{
			switch (wParam)
			{


			case WM_MOUSEMOVE:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseChanged, pMouseStruct->pt.x, pMouseStruct->pt.y, 0);
				break;
			case WM_MOUSEWHEEL:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseChanged, pMouseStruct->pt.x, pMouseStruct->pt.y, GET_WHEEL_DELTA_WPARAM(pMouseStruct->mouseData));
				break;

			case WM_LBUTTONDOWN:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseButtonChanged, 0, true);
				break;
			case WM_LBUTTONUP:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseButtonChanged, 0, false);
				break;

			case WM_RBUTTONDOWN:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseButtonChanged, 1, true);
				break;

			case WM_RBUTTONUP:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseButtonChanged, 1, false);
				break;

			case WM_MBUTTONDOWN:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseButtonChanged, 2, true);
				break;

			case WM_MBUTTONUP:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseButtonChanged, 2, false);
				break;

			case WM_XBUTTONDOWN:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseButtonChanged, GET_XBUTTON_WPARAM(pMouseStruct->mouseData), true);
				break;

			case WM_XBUTTONUP:
				MouseHooker::getInstance()->listeners.call(&Listener::mouseButtonChanged, GET_XBUTTON_WPARAM(pMouseStruct->mouseData), false);
				break;
			}
		}
	}

	return CallNextHookEx(MouseHooker::getInstance()->mousehook, nCode, wParam, lParam);
}


#endif