/*
  ==============================================================================

    CrashHandler.cpp
    Created: 29 Oct 2017 1:40:04pm
    Author:  Ben

  ==============================================================================
*/

#include "JuceHeader.h"

#ifdef JUCE_WINDOWS

#include <windows.h>
#include <DbgHelp.h>
#include <tchar.h>


/**
* Crash handler for Windows.
*/
LONG WINAPI createMiniDump(LPEXCEPTION_POINTERS exceptionPointers)
{
	//Analytics::getInstance()->logEvent("crash", {});
	

	auto dumpFileName = "crash.log";

	HANDLE hFile = CreateFile(dumpFileName, GENERIC_READ | GENERIC_WRITE,
		0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile != nullptr && hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION exceptionInformation;

		exceptionInformation.ThreadId = GetCurrentThreadId();
		exceptionInformation.ExceptionPointers = exceptionPointers;
		exceptionInformation.ClientPointers = FALSE;

		MINIDUMP_TYPE dumpType = MiniDumpNormal;

		BOOL dumpWriteResult = MiniDumpWriteDump(GetCurrentProcess(),
			GetCurrentProcessId(),
			hFile,
			dumpType,
			exceptionPointers != nullptr ? &exceptionInformation : 0,
			nullptr,
			nullptr);

		if (!dumpWriteResult)
			_tprintf(_T("MiniDumpWriteDump failed. Error: %u \n"), GetLastError());
		else
			_tprintf(_T("Minidump created.\n"));

		CloseHandle(hFile);
	} else
	{
		_tprintf(_T("CreateFile failed. Error: %u \n"), GetLastError());
	}

	int selectedButtonId = MessageBox(nullptr,
		_T("Application crashed.  Send error report to vendor on application restart?"),
		_T("Crash"),
		MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON1
	);

	_tprintf(_T("User pushed: %d \n"), selectedButtonId);

	if (selectedButtonId == IDNO)
		DeleteFile(dumpFileName);

	return EXCEPTION_EXECUTE_HANDLER;
}

#endif
