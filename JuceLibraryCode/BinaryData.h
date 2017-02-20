/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_123017474_INCLUDED
#define BINARYDATA_H_123017474_INCLUDED

namespace BinaryData
{
    extern const char*   add_png;
    const int            add_pngSize = 16103;

    extern const char*   cancel_png;
    const int            cancel_pngSize = 14817;

    extern const char*   check_png;
    const int            check_pngSize = 15786;

    extern const char*   edit_png;
    const int            edit_pngSize = 16216;

    extern const char*   file_png;
    const int            file_pngSize = 16044;

    extern const char*   icon_png;
    const int            icon_pngSize = 42153;

    extern const char*   in_png;
    const int            in_pngSize = 20744;

    extern const char*   out_png;
    const int            out_pngSize = 20427;

    extern const char*   power_png;
    const int            power_pngSize = 15366;

    extern const char*   reload_png;
    const int            reload_pngSize = 16316;

    extern const char*   settings_png;
    const int            settings_pngSize = 15919;

    extern const char*   target_png;
    const int            target_pngSize = 15244;

    extern const char*   uncheck_png;
    const int            uncheck_pngSize = 15619;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 13;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
