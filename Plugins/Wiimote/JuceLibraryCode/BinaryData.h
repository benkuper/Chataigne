/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_51678538_INCLUDED
#define BINARYDATA_H_51678538_INCLUDED

namespace BinaryData
{
    extern const char*   configBT_png;
    const int            configBT_pngSize = 15364;

    extern const char*   removeBT_png;
    const int            removeBT_pngSize = 58744;

    extern const char*   target_png;
    const int            target_pngSize = 25820;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 3;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
