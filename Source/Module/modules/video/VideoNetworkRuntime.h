#pragma once

#include <juce_core/juce_core.h>
#include "../../../../External/ndi/include/Processing.NDI.Lib.h"
#define OMT_DYNAMIC_LOAD
#include "../../../../External/omt/include/libomt.h"
#include <mutex>

namespace VideoNetworkRuntime
{
    inline bool openLibrary(juce::DynamicLibrary& library, const juce::StringArray& names,
                            const juce::StringArray& environmentVariables)
    {
        using namespace juce;
        const auto executable = File::getSpecialLocation(File::currentExecutableFile).getParentDirectory();
        StringArray directories { executable.getFullPathName() };
#if JUCE_MAC
        directories.add(executable.getSiblingFile("Frameworks").getFullPathName());
#elif JUCE_LINUX
        directories.add(executable.getSiblingFile("lib").getFullPathName());
#endif
        for (const auto& variable : environmentVariables)
        {
            const auto directory = SystemStats::getEnvironmentVariable(variable, {});
            if (directory.isNotEmpty()) directories.add(directory);
        }
#if ! JUCE_WINDOWS
        directories.add("/usr/local/lib");
#endif
        for (const auto& directory : directories)
            for (const auto& name : names)
                if (library.open(File(directory).getChildFile(name).getFullPathName())) return true;
        for (const auto& name : names)
            if (library.open(name)) return true;
        return false;
    }

    struct NDI
    {
        juce::DynamicLibrary library;
        std::mutex lock;
        bool ready = false;
#define CHATAIGNE_NDI_FUNCTION(name) decltype(&NDIlib_##name) name = nullptr;
        CHATAIGNE_NDI_FUNCTION(initialize)
        CHATAIGNE_NDI_FUNCTION(is_supported_CPU)
        CHATAIGNE_NDI_FUNCTION(send_create)
        CHATAIGNE_NDI_FUNCTION(send_destroy)
        CHATAIGNE_NDI_FUNCTION(send_get_no_connections)
        CHATAIGNE_NDI_FUNCTION(send_send_video_v2)
#undef CHATAIGNE_NDI_FUNCTION

        bool load()
        {
            const std::lock_guard<std::mutex> guard(lock);
            if (ready) return true;
#if JUCE_WINDOWS
            const juce::StringArray names { NDILIB_LIBRARY_NAME };
#elif JUCE_MAC
            const juce::StringArray names { "libndi.dylib", "libndi.6.dylib", "libndi.5.dylib" };
#else
            const juce::StringArray names { "libndi.so.6", "libndi.so.5", "libndi.so" };
#endif
            if (!openLibrary(library, names, { "NDI_RUNTIME_DIR_V6", "NDI_RUNTIME_DIR_V5" })) return false;
#define CHATAIGNE_NDI_LOAD(name) name = reinterpret_cast<decltype(name)>(library.getFunction("NDIlib_" #name)); if (!name) return false;
            CHATAIGNE_NDI_LOAD(initialize)
            CHATAIGNE_NDI_LOAD(is_supported_CPU)
            CHATAIGNE_NDI_LOAD(send_create)
            CHATAIGNE_NDI_LOAD(send_destroy)
            CHATAIGNE_NDI_LOAD(send_get_no_connections)
            CHATAIGNE_NDI_LOAD(send_send_video_v2)
#undef CHATAIGNE_NDI_LOAD
            ready = is_supported_CPU() && initialize();
            return ready;
        }

        static NDI& get()
        {
            // Keep process-wide SDK services alive while any module can use them.
            static auto* runtime = new NDI();
            return *runtime;
        }
    };

    struct OMT
    {
        juce::DynamicLibrary library, codec;
        std::mutex lock;
        bool ready = false;
        bool libraryLoaded = false, codecLoaded = false;
#define CHATAIGNE_OMT_FUNCTION(name) decltype(&omt_##name) name = nullptr;
        CHATAIGNE_OMT_FUNCTION(send_create)
        CHATAIGNE_OMT_FUNCTION(send_destroy)
        CHATAIGNE_OMT_FUNCTION(send_setsenderinformation)
        CHATAIGNE_OMT_FUNCTION(send)
#undef CHATAIGNE_OMT_FUNCTION

        bool load()
        {
            const std::lock_guard<std::mutex> guard(lock);
            if (ready) return true;
#if JUCE_WINDOWS
            if (juce::SystemStats::getOperatingSystemType() < juce::SystemStats::Windows10) return false;
            const juce::StringArray names { "libomt.dll" }, codecs { "libvmx.dll" };
#elif JUCE_MAC
            const juce::StringArray names { "libomt.dylib" }, codecs { "libvmx.dylib" };
#else
            const juce::StringArray names { "libomt.so" }, codecs { "libvmx.so" };
#endif
            // A missing codec can otherwise terminate the NativeAOT runtime on first send.
            if (!codecLoaded) codecLoaded = openLibrary(codec, codecs, { "CHATAIGNE_OMT_RUNTIME_DIR" });
            if (!codecLoaded) return false;
            if (!libraryLoaded) libraryLoaded = openLibrary(library, names, { "CHATAIGNE_OMT_RUNTIME_DIR" });
            if (!libraryLoaded) return false;
#define CHATAIGNE_OMT_LOAD(name) name = reinterpret_cast<decltype(name)>(library.getFunction("omt_" #name)); if (!name) return false;
            CHATAIGNE_OMT_LOAD(send_create)
            CHATAIGNE_OMT_LOAD(send_destroy)
            CHATAIGNE_OMT_LOAD(send_setsenderinformation)
            CHATAIGNE_OMT_LOAD(send)
#undef CHATAIGNE_OMT_LOAD
            ready = true;
            return true;
        }

        static OMT& get()
        {
            // NativeAOT shared libraries cannot safely be unloaded with dlclose.
            static auto* runtime = new OMT();
            return *runtime;
        }
    };
}
