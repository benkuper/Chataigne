/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   about_png;
    const int            about_pngSize = 36971;

    extern const char*   add_png;
    const int            add_pngSize = 16103;

    extern const char*   default_chalayout;
    const int            default_chalayoutSize = 3448;

    extern const char*   icon_png;
    const int            icon_pngSize = 42153;

    extern const char*   in_png;
    const int            in_pngSize = 3358;

    extern const char*   nextcue_png;
    const int            nextcue_pngSize = 877;

    extern const char*   out_png;
    const int            out_pngSize = 3368;

    extern const char*   padlock_png;
    const int            padlock_pngSize = 584;

    extern const char*   pause_png;
    const int            pause_pngSize = 701;

    extern const char*   play_png;
    const int            play_pngSize = 768;

    extern const char*   playing_png;
    const int            playing_pngSize = 952;

    extern const char*   playpause_png;
    const int            playpause_pngSize = 16163;

    extern const char*   prevcue_png;
    const int            prevcue_pngSize = 1043;

    extern const char*   smallstripe_png;
    const int            smallstripe_pngSize = 1959;

    extern const char*   snap_png;
    const int            snap_pngSize = 3649;

    extern const char*   stop_png;
    const int            stop_pngSize = 813;

    extern const char*   stripe_png;
    const int            stripe_pngSize = 2043;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 17;

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
