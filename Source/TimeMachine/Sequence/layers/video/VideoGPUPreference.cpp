/*
  ==============================================================================

	VideoGPUPreference.cpp
	Vendor neutral "use the best GPU available" request for the video pipeline.

	Windows : OpenGL contexts are created by the driver against the adapter that
	owns the primary output, which is the integrated GPU on a laptop. The only
	supported way to change that is the per executable GPU preference exposed by
	WDDM, which is read when the process starts (there is no way to switch an
	existing WGL context). We ask for the high performance adapter, without
	naming any vendor, and never overwrite a preference the user set themselves.

	Mac : the Metal / OpenGL stack already runs on the discrete GPU.

	Linux : the choice belongs to the running stack (PRIME, render offload, EGL
	device selection), we don't touch the environment.

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "VideoGPUPreference.h"

#if JUCE_WINDOWS
 #include <windows.h>
#endif

using namespace juce;

namespace
{
#if JUCE_WINDOWS
	static const wchar_t* preferenceKeyPath = L"Software\\Microsoft\\DirectX\\UserGpuPreferences";
	static const wchar_t* highPerformance = L"GpuPreference=2;";

	struct RegistryAccess
	{
		HKEY key = nullptr;

		RegistryAccess()
		{
			if (RegCreateKeyExW(HKEY_CURRENT_USER, preferenceKeyPath, 0, nullptr,
				REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE,
				nullptr, &key, nullptr) != ERROR_SUCCESS)
			{
				key = nullptr;
			}
		}

		~RegistryAccess()
		{
			if (key != nullptr) RegCloseKey(key);
		}

		String read(const String& name) const
		{
			if (key == nullptr) return {};

			DWORD type = 0;
			DWORD size = 0;
			const juce::CharPointer_UTF16 namePtr = name.toUTF16();

			if (RegQueryValueExW(key, namePtr.getAddress(), nullptr, &type, nullptr, &size) != ERROR_SUCCESS
				|| size == 0 || (type != REG_SZ && type != REG_EXPAND_SZ))
			{
				return {};
			}

			HeapBlock<wchar_t> buffer((size / sizeof(wchar_t)) + 1);

			if (RegQueryValueExW(key, namePtr.getAddress(), nullptr, &type,
				(LPBYTE)buffer.get(), &size) != ERROR_SUCCESS)
			{
				return {};
			}

			return String(juce::CharPointer_UTF16(buffer.get()));
		}

		bool write(const String& name, const wchar_t* value) const
		{
			if (key == nullptr) return false;

			const auto needed = (DWORD)((String(value).length() + 1) * sizeof(wchar_t));
			return RegSetValueExW(key, name.toUTF16().getAddress(), 0, REG_SZ,
				(const BYTE*)value, needed) == ERROR_SUCCESS;
		}
	};

	String getCurrentExecutablePath()
	{
		return File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
	}
#endif
}

void VideoGPUPreference::applyBestAvailable()
{
#if JUCE_WINDOWS
	applyOnWindows();
#else
	NLOG("Video", "VideoGPUPreference : the system selects the best GPU on this platform");
#endif
}

#if JUCE_WINDOWS
void VideoGPUPreference::applyOnWindows()
{
	const String exePath = getCurrentExecutablePath();
	if (exePath.isEmpty()) return;

	RegistryAccess reg;
	const String existing = reg.read(exePath);

	if (existing.contains("GpuPreference=2"))
	{
		NLOG("Video", "VideoGPUPreference : high performance GPU already allowed for this executable");
		return;
	}

	if (existing.contains("GpuPreference="))
	{
		NLOG("Video", "VideoGPUPreference : GPU preference '" << existing
					<< "' was set outside of Chataigne, leaving it as is");
		return;
	}

	if (reg.write(exePath, highPerformance))
	{
		NLOG("Video", "VideoGPUPreference : requested high performance GPU for this executable,"
				  " it will be used on the next launch");
	}
	else
	{
		NLOGERROR("Video", "VideoGPUPreference : could not write the GPU preference");
	}
}
#endif

bool VideoGPUPreference::isApplied()
{
#if JUCE_WINDOWS
	const String exePath = getCurrentExecutablePath();
	if (exePath.isEmpty()) return false;

	RegistryAccess reg;
	return reg.read(exePath).contains("GpuPreference=2");
#else
	return true;
#endif
}

juce::String VideoGPUPreference::getDescription()
{
#if JUCE_WINDOWS
	return String("high performance GPU : ") + (isApplied() ? "allowed" : "not allowed");
#else
	return "system default GPU";
#endif
}
