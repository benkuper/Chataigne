# libmpv binaries

The video engine uses the libmpv C API headers in `include/mpv`.

- Windows 10/11 x64: `libmpv-2.dll` from the mpv project's LGPL x86_64 development build `libmpv-v0.41.0-dev-ga1f50f2c3-36640285359-x86_64-w64-mingw32-lgpl.zip` ([release](https://github.com/mpv-player/mpv/releases/tag/git-release)). SHA-256 of the DLL: `1e23e98611c99137565b684d7d59b5bf30dc3b81f3c8e606df6d503f0bd6b8a3`.
- Windows 7 x64: `libmpv-2.dll` from the [mpv.net v6.0.4.0 portable release](https://github.com/mpvnet-player/mpv.net/releases/tag/v6.0.4.0-stable). SHA-256 of the DLL: `0a819f1a056f4b1a6ea2eb8e5b41437d58cae15e747c65d9b14575458fd57e4a`.
- macOS Intel and Apple Silicon: the 18 universal dylibs from [media-kit libmpv-darwin-build v0.7.3](https://github.com/media-kit/libmpv-darwin-build/releases/tag/v0.7.3), asset `libmpv-libs_v0.7.3_macos-universal-video-default.tar.gz`. SHA-256 of the source archive: `324d32690882640ce7fde71c43a8bff25066e7f69be740a8ac2c284ef6900d28`.
- Linux x64, Raspberry Pi arm64 and armhf: `libmpv-dev` is installed in CI and the resulting `libmpv.so` and its non-system dependencies are copied to the AppDir.

Windows runtime DLLs live in the platform-specific dependency ZIPs distributed from `https://benjamin.kuperberg.fr/chataigne/user/data/`. The local `stage_mpv.bat` script can use `dependencies_zips/Chataigne-win-x64-release-dependencies.zip` or an explicit `CHATAIGNE_MPV_SRC` path.

Keep the macOS dylibs and Windows DLLs together with their corresponding source and license notices when redistributing packages. Chataigne is GPLv3; see the root `LICENSE` file.
