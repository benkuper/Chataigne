/*

    IMPORTANT! This file is auto-generated each time you save your
    project - if you alter its contents, your changes may be overwritten!

    There's a section below where you can add your own custom code safely, and the
    Projucer will preserve the contents of that block, but the best way to change
    any of these definitions is by using the Projucer's project settings.

    Any commented-out settings will assume their default values.

*/

#pragma once

//==============================================================================
// [BEGIN_USER_CODE_SECTION]

// (You can add your own code in this section, and the Projucer will not overwrite it)

// [END_USER_CODE_SECTION]

#define JUCE_PROJUCER_VERSION 0x80000

//==============================================================================
#define JUCE_MODULE_AVAILABLE_juce_audio_basics          1
#define JUCE_MODULE_AVAILABLE_juce_audio_devices         1
#define JUCE_MODULE_AVAILABLE_juce_audio_formats         1
#define JUCE_MODULE_AVAILABLE_juce_audio_processors      1
#define JUCE_MODULE_AVAILABLE_juce_audio_utils           1
#define JUCE_MODULE_AVAILABLE_juce_core                  1
#define JUCE_MODULE_AVAILABLE_juce_cryptography          1
#define JUCE_MODULE_AVAILABLE_juce_data_structures       1
#define JUCE_MODULE_AVAILABLE_juce_dmx                   1
#define JUCE_MODULE_AVAILABLE_juce_dsp                   1
#define JUCE_MODULE_AVAILABLE_juce_events                1
#define JUCE_MODULE_AVAILABLE_juce_graphics              1
#define JUCE_MODULE_AVAILABLE_juce_gui_basics            1
#define JUCE_MODULE_AVAILABLE_juce_gui_extra             1
#define JUCE_MODULE_AVAILABLE_juce_opengl                1
#define JUCE_MODULE_AVAILABLE_juce_organicui             1
#define JUCE_MODULE_AVAILABLE_juce_osc                   1
#define JUCE_MODULE_AVAILABLE_juce_serial                1
#define JUCE_MODULE_AVAILABLE_juce_sharedtexture         1
#define JUCE_MODULE_AVAILABLE_juce_simpleweb             1
#define JUCE_MODULE_AVAILABLE_juce_timeline              1

#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1

//==============================================================================
// juce_audio_devices flags:

#ifndef    JUCE_USE_WINRT_MIDI
 #define   JUCE_USE_WINRT_MIDI 0
#endif

#ifndef    JUCE_ASIO
 #define   JUCE_ASIO 1
#endif

#ifndef    JUCE_WASAPI
 #define   JUCE_WASAPI 1
#endif

#ifndef    JUCE_DIRECTSOUND
 //#define JUCE_DIRECTSOUND 1
#endif

#ifndef    JUCE_ALSA
 //#define JUCE_ALSA 1
#endif

#ifndef    JUCE_JACK
 #define   JUCE_JACK 1
#endif

#ifndef    JUCE_BELA
 //#define JUCE_BELA 0
#endif

#ifndef    JUCE_USE_ANDROID_OBOE
 //#define JUCE_USE_ANDROID_OBOE 1
#endif

#ifndef    JUCE_USE_OBOE_STABILIZED_CALLBACK
 //#define JUCE_USE_OBOE_STABILIZED_CALLBACK 0
#endif

#ifndef    JUCE_USE_ANDROID_OPENSLES
 //#define JUCE_USE_ANDROID_OPENSLES 0
#endif

#ifndef    JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS
 //#define JUCE_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS 0
#endif

//==============================================================================
// juce_audio_formats flags:

#ifndef    JUCE_USE_FLAC
 //#define JUCE_USE_FLAC 1
#endif

#ifndef    JUCE_USE_OGGVORBIS
 //#define JUCE_USE_OGGVORBIS 1
#endif

#ifndef    JUCE_USE_MP3AUDIOFORMAT
 #define   JUCE_USE_MP3AUDIOFORMAT 1
#endif

#ifndef    JUCE_USE_LAME_AUDIO_FORMAT
 #define   JUCE_USE_LAME_AUDIO_FORMAT 1
#endif

#ifndef    JUCE_USE_WINDOWS_MEDIA_FORMAT
 #define   JUCE_USE_WINDOWS_MEDIA_FORMAT 1
#endif

//==============================================================================
// juce_audio_processors flags:

#ifndef    JUCE_PLUGINHOST_VST
 //#define JUCE_PLUGINHOST_VST 0
#endif

#ifndef    JUCE_PLUGINHOST_VST3
 //#define JUCE_PLUGINHOST_VST3 0
#endif

#ifndef    JUCE_PLUGINHOST_AU
 //#define JUCE_PLUGINHOST_AU 0
#endif

#ifndef    JUCE_PLUGINHOST_LADSPA
 //#define JUCE_PLUGINHOST_LADSPA 0
#endif

#ifndef    JUCE_PLUGINHOST_LV2
 //#define JUCE_PLUGINHOST_LV2 0
#endif

#ifndef    JUCE_PLUGINHOST_ARA
 //#define JUCE_PLUGINHOST_ARA 0
#endif

#ifndef    JUCE_CUSTOM_VST3_SDK
 //#define JUCE_CUSTOM_VST3_SDK 0
#endif

//==============================================================================
// juce_audio_utils flags:

#ifndef    JUCE_USE_CDREADER
 //#define JUCE_USE_CDREADER 0
#endif

#ifndef    JUCE_USE_CDBURNER
 //#define JUCE_USE_CDBURNER 0
#endif

//==============================================================================
// juce_core flags:

#ifndef    JUCE_FORCE_DEBUG
 //#define JUCE_FORCE_DEBUG 0
#endif

#ifndef    JUCE_LOG_ASSERTIONS
 //#define JUCE_LOG_ASSERTIONS 0
#endif

#ifndef    JUCE_CHECK_MEMORY_LEAKS
 //#define JUCE_CHECK_MEMORY_LEAKS 1
#endif

#ifndef    JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES
 //#define JUCE_DONT_AUTOLINK_TO_WIN32_LIBRARIES 0
#endif

#ifndef    JUCE_INCLUDE_ZLIB_CODE
 //#define JUCE_INCLUDE_ZLIB_CODE 1
#endif

#ifndef    JUCE_USE_CURL
 //#define JUCE_USE_CURL 1
#endif

#ifndef    JUCE_LOAD_CURL_SYMBOLS_LAZILY
 //#define JUCE_LOAD_CURL_SYMBOLS_LAZILY 0
#endif

#ifndef    JUCE_CATCH_UNHANDLED_EXCEPTIONS
 //#define JUCE_CATCH_UNHANDLED_EXCEPTIONS 0
#endif

#ifndef    JUCE_ALLOW_STATIC_NULL_VARIABLES
 //#define JUCE_ALLOW_STATIC_NULL_VARIABLES 0
#endif

#ifndef    JUCE_STRICT_REFCOUNTEDPOINTER
 //#define JUCE_STRICT_REFCOUNTEDPOINTER 0
#endif

#ifndef    JUCE_ENABLE_ALLOCATION_HOOKS
 //#define JUCE_ENABLE_ALLOCATION_HOOKS 0
#endif

//==============================================================================
// juce_dsp flags:

#ifndef    JUCE_ASSERTION_FIRFILTER
 //#define JUCE_ASSERTION_FIRFILTER 1
#endif

#ifndef    JUCE_DSP_USE_INTEL_MKL
 //#define JUCE_DSP_USE_INTEL_MKL 0
#endif

#ifndef    JUCE_DSP_USE_SHARED_FFTW
 //#define JUCE_DSP_USE_SHARED_FFTW 0
#endif

#ifndef    JUCE_DSP_USE_STATIC_FFTW
 //#define JUCE_DSP_USE_STATIC_FFTW 0
#endif

#ifndef    JUCE_DSP_ENABLE_SNAP_TO_ZERO
 //#define JUCE_DSP_ENABLE_SNAP_TO_ZERO 1
#endif

//==============================================================================
// juce_events flags:

#ifndef    JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK
 //#define JUCE_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK 0
#endif

//==============================================================================
// juce_graphics flags:

#ifndef    JUCE_USE_COREIMAGE_LOADER
 //#define JUCE_USE_COREIMAGE_LOADER 1
#endif

#ifndef    JUCE_USE_DIRECTWRITE
 //#define JUCE_USE_DIRECTWRITE 1
#endif

#ifndef    JUCE_DISABLE_COREGRAPHICS_FONT_SMOOTHING
 //#define JUCE_DISABLE_COREGRAPHICS_FONT_SMOOTHING 0
#endif

#ifndef    JUCE_FLOAT_COLOURS
 #define   JUCE_FLOAT_COLOURS 1
#endif

//==============================================================================
// juce_gui_basics flags:

#ifndef    JUCE_ENABLE_REPAINT_DEBUGGING
 //#define JUCE_ENABLE_REPAINT_DEBUGGING 0
#endif

#ifndef    JUCE_USE_XRANDR
 //#define JUCE_USE_XRANDR 1
#endif

#ifndef    JUCE_USE_XINERAMA
 //#define JUCE_USE_XINERAMA 1
#endif

#ifndef    JUCE_USE_XSHM
 //#define JUCE_USE_XSHM 1
#endif

#ifndef    JUCE_USE_XRENDER
 //#define JUCE_USE_XRENDER 0
#endif

#ifndef    JUCE_USE_XCURSOR
 //#define JUCE_USE_XCURSOR 1
#endif

#ifndef    JUCE_WIN_PER_MONITOR_DPI_AWARE
 //#define JUCE_WIN_PER_MONITOR_DPI_AWARE 1
#endif

//==============================================================================
// juce_gui_extra flags:

#ifndef    JUCE_WEB_BROWSER
 //#define JUCE_WEB_BROWSER 1
#endif

#ifndef    JUCE_USE_WIN_WEBVIEW2
 #define   JUCE_USE_WIN_WEBVIEW2 1
#endif

#ifndef    JUCE_ENABLE_LIVE_CONSTANT_EDITOR
 //#define JUCE_ENABLE_LIVE_CONSTANT_EDITOR 0
#endif

//==============================================================================
// juce_organicui flags:

#ifndef    ORGANICUI_USE_SERVUS
 #define   ORGANICUI_USE_SERVUS 1
#endif

#ifndef    ORGANICUI_USE_SHAREDTEXTURE
 #define   ORGANICUI_USE_SHAREDTEXTURE 1
#endif

#ifndef    ORGANICUI_USE_WEBSERVER
 #define   ORGANICUI_USE_WEBSERVER 1
#endif

//==============================================================================
// juce_osc flags:

#ifndef    JUCE_ALLOW_SPECIAL_CHARS_IN_ADDRESS
 #define   JUCE_ALLOW_SPECIAL_CHARS_IN_ADDRESS 1
#endif

#ifndef    JUCE_ENABLE_BROADCAST_BY_DEFAULT
 #define   JUCE_ENABLE_BROADCAST_BY_DEFAULT 1
#endif

#ifndef    JUCE_EXCLUSIVE_BINDING_BY_DEFAULT
 #define   JUCE_EXCLUSIVE_BINDING_BY_DEFAULT 1
#endif

#ifndef    JUCE_IP_AND_PORT_DETECTION
 #define   JUCE_IP_AND_PORT_DETECTION 1
#endif

//==============================================================================
// juce_sharedtexture flags:

#ifndef    SHAREDTEXTURE_USE_CPU_IMAGE
 //#define SHAREDTEXTURE_USE_CPU_IMAGE 1
#endif

//==============================================================================
// juce_timeline flags:

#ifndef    TIMELINE_USE_SEQUENCEMANAGER_SINGLETON
 #define   TIMELINE_USE_SEQUENCEMANAGER_SINGLETON 0
#endif

#ifndef    TIMELINE_ADD_MENU_ITEMS
 //#define TIMELINE_ADD_MENU_ITEMS 1
#endif

//==============================================================================
#ifndef    JUCE_STANDALONE_APPLICATION
 #if defined(JucePlugin_Name) && defined(JucePlugin_Build_Standalone)
  #define  JUCE_STANDALONE_APPLICATION JucePlugin_Build_Standalone
 #else
  #define  JUCE_STANDALONE_APPLICATION 1
 #endif
#endif
