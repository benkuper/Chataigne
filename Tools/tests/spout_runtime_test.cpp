#include <windows.h>
#include <cstdio>
#include "SpoutLibrary.h"

namespace
{
LONG WINAPI reportCrash(EXCEPTION_POINTERS* error)
{
    std::fprintf(stderr, "Spout runtime exception 0x%08lx at %p; check that SpoutLibrary.h matches the staged DLL.\n",
                 error->ExceptionRecord->ExceptionCode, error->ExceptionRecord->ExceptionAddress);
    ExitProcess(2);
}
}

int wmain(int argc, wchar_t** argv)
{
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    SetUnhandledExceptionFilter(reportCrash);
    if (argc != 2) return 3;

    auto library = LoadLibraryExW(argv[1], nullptr,
                                 LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    if (library == nullptr)
    {
        std::fprintf(stderr, "Could not load the staged Spout DLL (Windows error %lu).\n", GetLastError());
        return 4;
    }
    auto factory = reinterpret_cast<SPOUTHANDLE (WINAPI*)()>(GetProcAddress(library, "GetSpout"));
    if (factory == nullptr) return 5;

    // Exercise the real DLL's receiver, sender and manager cleanup paths.
    // No OpenGL context, sender registration or external application is needed.
    for (int i = 0; i < 32; ++i)
    {
        auto receiver = factory();
        if (receiver == nullptr) return 6;
        receiver->SetReceiverName("Chataigne runtime test");
        receiver->ReleaseReceiver();
        receiver->Release();

        auto sender = factory();
        if (sender == nullptr) return 6;
        sender->SetSenderName("Chataigne runtime test");
        sender->ReleaseSender();
        sender->Release();

        auto detector = factory();
        if (detector == nullptr) return 6;
        const auto count = detector->GetSenderCount();
        if (count < 0) return 7;
        if (count > 0)
        {
            char name[256] = {};
            detector->GetSender(0, name);
        }
        detector->ReleaseSender();
        detector->Release();
    }

    FreeLibrary(library);
    std::puts("Spout runtime passed: 32 receiver, sender and detector cleanup cycles.");
    return 0;
}
