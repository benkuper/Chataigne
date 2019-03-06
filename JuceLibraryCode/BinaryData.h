/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   conditionScriptTemplate_js;
    const int            conditionScriptTemplate_jsSize = 5536;

    extern const char*   filterScriptTemplate_js;
    const int            filterScriptTemplate_jsSize = 646;

    extern const char*   genericScriptTemplate_js;
    const int            genericScriptTemplate_jsSize = 4737;

    extern const char*   httpScriptTemplate_js;
    const int            httpScriptTemplate_jsSize = 805;

    extern const char*   midiScriptTemplate_js;
    const int            midiScriptTemplate_jsSize = 1271;

    extern const char*   moduleScriptTemplate_js;
    const int            moduleScriptTemplate_jsSize = 1415;

    extern const char*   oscScriptTemplate_js;
    const int            oscScriptTemplate_jsSize = 798;

    extern const char*   streamingScriptTemplate_js;
    const int            streamingScriptTemplate_jsSize = 1091;

    extern const char*   about_png;
    const int            about_pngSize = 36971;

    extern const char*   add_png;
    const int            add_pngSize = 3269;

    extern const char*   connected_png;
    const int            connected_pngSize = 3998;

    extern const char*   default_chalayout;
    const int            default_chalayoutSize = 3448;

    extern const char*   disconnected_png;
    const int            disconnected_pngSize = 3820;

    extern const char*   icon_png;
    const int            icon_pngSize = 42153;

    extern const char*   in_png;
    const int            in_pngSize = 3754;

    extern const char*   nextcue_png;
    const int            nextcue_pngSize = 2624;

    extern const char*   out_png;
    const int            out_pngSize = 4041;

    extern const char*   padlock_png;
    const int            padlock_pngSize = 584;

    extern const char*   pause_png;
    const int            pause_pngSize = 701;

    extern const char*   play_png;
    const int            play_pngSize = 1849;

    extern const char*   playing_png;
    const int            playing_pngSize = 952;

    extern const char*   playpause_png;
    const int            playpause_pngSize = 16163;

    extern const char*   prevcue_png;
    const int            prevcue_pngSize = 2536;

    extern const char*   smallstripe_png;
    const int            smallstripe_pngSize = 1959;

    extern const char*   snap_png;
    const int            snap_pngSize = 3649;

    extern const char*   stop_png;
    const int            stop_pngSize = 1978;

    extern const char*   stripe_png;
    const int            stripe_pngSize = 2043;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 27;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
