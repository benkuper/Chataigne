# libmpv binaries

The video engine uses the libmpv C API headers in `include/mpv`.

- Windows 10/11 x64: CI builds a playback-focused `libmpv-2.dll` with `build-windows.sh` and the exact commits in `windows-sources.json`. It uses the same mpv commit as the previous development build. The previous full DLL ([release](https://github.com/mpv-player/mpv/releases/tag/git-release), asset `libmpv-v0.41.0-dev-ga1f50f2c3-36640285359-x86_64-w64-mingw32-lgpl.zip`) has SHA-256 `1e23e98611c99137565b684d7d59b5bf30dc3b81f3c8e606df6d503f0bd6b8a3`. The locally tested playback DLL has SHA-256 `90addb0cb1a8afaa3452048dbe2b6ea86310e54845e5f7db182f96c2e2796c52`.
- Windows 7 x64: `libmpv-2.dll` from the [mpv.net v6.0.4.0 portable release](https://github.com/mpvnet-player/mpv.net/releases/tag/v6.0.4.0-stable). SHA-256 of the DLL: `0a819f1a056f4b1a6ea2eb8e5b41437d58cae15e747c65d9b14575458fd57e4a`.
- macOS Intel and Apple Silicon: the 18 universal dylibs from [media-kit libmpv-darwin-build v0.7.3](https://github.com/media-kit/libmpv-darwin-build/releases/tag/v0.7.3), asset `libmpv-libs_v0.7.3_macos-universal-video-default.tar.gz`. SHA-256 of the source archive: `324d32690882640ce7fde71c43a8bff25066e7f69be740a8ac2c284ef6900d28`.
- Linux x64, Raspberry Pi arm64 and armhf: `libmpv-dev` is installed in CI and the resulting `libmpv.so` and its non-system dependencies are copied to the AppDir.

Windows dependency ZIPs are distributed from `https://benjamin.kuperberg.fr/chataigne/user/data/`. The modern Windows release and debug archives contain the playback runtime and notices. CI also builds the runtime from the pinned sources and uses that build when making the Windows 10/11 installer. The `mpv-win-x64` Actions artifact contains the DLL, license notices, build recipe and corresponding source archive. Video support is still bundled in the installer.

The local `stage_mpv.bat` script searches an explicit `CHATAIGNE_MPV_SRC` file/directory first, then `Binaries/mpv-win-x64`, then `dependencies_zips/Chataigne-win-x64-release-dependencies.zip`. Point `CHATAIGNE_MPV_SRC` at the generated `dist` directory, or copy its contents to `Binaries/mpv-win-x64`. It stages the DLL and its accompanying `mpv-licenses` directory beside the executable.

## Building the Windows playback runtime

Run the build on Linux or WSL, with the build directory on the Linux filesystem for speed. Keep it outside the Chataigne workspace. Prerequisites on Ubuntu:

```sh
sudo apt-get install gcc-mingw-w64-x86-64-posix g++-mingw-w64-x86-64-posix \
    nasm pkg-config cmake ninja-build meson autoconf automake libtool git python3 xz-utils
bash External/mpv/build-windows.sh /absolute/path/to/chataigne-mpv 8
```

The runtime statically links its dependencies and needs only Windows system DLLs. It keeps all enabled built-in FFmpeg decoders, demuxers, parsers and filters, network playback with Windows TLS, OpenGL, D3D11VA/DXVA2/NVDEC hardware decoding, AV1 via dav1d, VP8/VP9 via libvpx, Opus, subtitle rendering and LCMS color management. PNG/MJPEG encoders and the SPDIF muxer remain because mpv uses them internally. Alpha is retained, including ProRes 4444, HAP Alpha and VP9 through the libvpx decoder.

The build removes unused video encoders and muxers, Vulkan, shaderc, SPIRV-Cross, the D3D11 renderer, Lua/JavaScript, optical-disc/archive input, device capture, other audio outputs and unrelated optional external FFmpeg libraries. Formats that require those external libraries, such as JPEG XL, are outside this profile. The patch in `patches/` enables D3D11 decoding helpers independently of the D3D11 renderer in the pinned mpv version.

Outputs are `dist/libmpv-2.dll`, `dist/mpv-licenses` (including SHA-256 and provenance), and `dist/mpv-sources.tar.xz`. Publish the source archive alongside releases; it is kept out of the installer. Source versions are locked, while binary hashes can vary with the compiler/toolchain.

Windows 7 retains its existing compatibility DLL. The stronger installer compression applies to both Windows installers. macOS and Linux continue using their existing runtime builds.

To update local dependency archives from a tested runtime:

```powershell
python External/mpv/update-windows-dependencies.py --runtime Binaries/mpv-win-x64 dependencies_zips/Chataigne-win-x64-release-dependencies.zip dependencies_zips/Chataigne-win-x64-debug-dependencies.zip
```

The updater verifies the runtime's manifest, preserves every other dependency byte-for-byte, and checks the new ZIP's integrity before replacing the archive. Upload both updated ZIPs to the site's `user/data` directory using the site SFTP configuration; keep credentials outside Git. Locally, the release archive decreased from 59.24 MB to 19.75 MB, and the debug archive from 60.33 MB to 20.80 MB.

## Measuring installer size

Both Inno scripts default to solid `lzma2/ultra64`. For an apples-to-apples comparison, keep the executable and other dependencies identical, changing only the libmpv DLL and its notices. `AppSourceDir` selects a staging directory, and `InstallerCompression` overrides the compression setting:

```powershell
ISCC install.iss /DAppSourceDir=D:\staging\original /DInstallerCompression=lzma2 /OD:\packages /Fbaseline
ISCC install.iss /DAppSourceDir=D:\staging\original /OD:\packages /Fcompression-only
ISCC install.iss /DAppSourceDir=D:\staging\trimmed /OD:\packages /Ftrimmed
```

Compression uses more memory and build time; the `ultra64` dictionary needs about 64 MB during installation. Use the same Inno compiler version for all measurements.

Measured locally with Inno Setup 6.7.3, the same 17.44 MB Chataigne executable and other dependencies:

| Windows 10/11 package | Size (decimal MB) |
| --- | ---: |
| Full DLL, previous LZMA2 compression | 49.46 |
| Full DLL, LZMA2 ultra64 | 48.68 |
| Playback DLL plus notices, LZMA2 ultra64 | 20.78 |

The DLL itself decreased from 126.28 MB to 43.27 MB. The resulting installer is 58% smaller. Actual release size depends on the executable, toolchain and other packaged dependencies. Native Windows checks matched the previous DLL for OpenGL playback and seeking of H.264/AAC, HEVC, AV1, ProRes 4444 alpha, HAP alpha and VP9 alpha (with its libvpx decoder). PCM audio output and D3D11VA hardware decoding were also checked. NVIDIA decoding is compiled in but was not exercised on this machine. CI checks Windows loading, all API symbols used by Chataigne and initialization before packaging.

Keep the macOS dylibs and Windows DLLs together with their corresponding source and license notices when redistributing packages. Chataigne is GPLv3; see the root `LICENSE` file.
