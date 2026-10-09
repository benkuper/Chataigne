# NDI and OMT video output

NDI out and OMT out send the same composition and test card as Video monitor out.
Sending runs on worker threads. Fractional frame rates are preserved to 0.001 FPS.
Runtime loading is optional: a missing/incompatible library produces a module warning
and does not prevent Chataigne from starting. Disable/re-enable the module after
installing a missing runtime.

| Platform | NDI | OMT |
| --- | --- | --- |
| Windows x64 | Supplied NDI 5.5.1 runtime | Supplied OMT 1.0.0.19, Windows 10+ |
| Windows 7 x64 | Supplied NDI runtime; requires testing on Windows 7 | Unavailable: NativeAOT requires Windows 10+ |
| macOS Intel / Apple Silicon | Universal NDI 6 runtime, macOS 13+ | Universal OMT, macOS 10.15 Intel / 11 Apple Silicon |
| Linux x64 | NDI 6 runtime | OMT built on Ubuntu 22.04 |
| Linux ARM64 | NDI 6 runtime | OMT cross-compiled on Ubuntu 22.04 |
| Linux ARM32 (Raspberry Pi) | NDI 6 runtime | Unavailable: NativeAOT/VMX do not support ARM32 |

The application's macOS deployment target is unchanged. Older macOS systems can
use an appropriately compatible installed NDI runtime. x64 OMT/VMX needs SSE4.2,
SSSE3 and LZCNT; AVX2 is used by the optimized codec path.

## Runtime locations

Windows DLLs go beside Chataigne.exe. macOS dylibs go in Contents/Frameworks.
Linux shared libraries go in the AppImage's usr/lib, or a system loader directory.
NDI also searches NDI_RUNTIME_DIR_V6 and NDI_RUNTIME_DIR_V5; macOS searches
/usr/local/lib. OMT additionally searches CHATAIGNE_OMT_RUNTIME_DIR. Both loaders
search beside the executable before installed system libraries. Libraries remain
loaded for the process lifetime, as NativeAOT does not support unloading.

OMT uses libomt and libvmx; libomtnet is compiled into libomt (no .NET installation
is required). The original desktop files are preserved; their Windows ARM64 OMT
binaries are retained in External/omt/lib/win/arm64 for future ARM64 builds.

## Rebuild and package

Run `bash External/ndi/fetch-linux.sh` to obtain the official NDI Linux SDK runtimes.
Run `bash External/omt/build-linux.sh x64` or `arm64` on Ubuntu 22.04 with .NET SDK 8,
Clang, zlib development headers and the target C++ toolchain installed. ARM64 also
needs binutils/gcc/g++-aarch64-linux-gnu and zlib1g-dev:arm64. Source revisions are
pinned in the script. Linux builds use invariant globalization to avoid a dynamic ICU dependency. Runtime binaries are committed so release builds need no SDK
downloads. NDI macOS comes from the official libNDI_for_Mac.pkg runtime installer.

Run `python External/update_video_dependencies.py dependencies_zips` to update the
four existing Windows archives and Linux x64 archive, and produce macOS, Linux
ARM64 and ARM32 archives. All unrelated members are preserved byte-for-byte, each
ZIP is checked, and SHA-256 manifests are written beside and inside the archives.
The macOS ZIP contains Frameworks/ plus video-licenses/; Linux ZIPs contain lib/
plus video-licenses/. Re-upload the archives using the existing dependency hosting
process. The CI checkout of JUCE is unchanged; local builds use ../JUCE.

Licenses and third-party notices accompany the binaries. NDI is proprietary; its
SDK headers carry separate MIT terms. OMT and VMX are MIT licensed.
