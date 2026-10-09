# Linux Avahi failure handling

Servus 1.6.0 retries `avahi_simple_poll_iterate()` after it returns a nonzero
status. A return of 1 means the poll has quit; a second iteration asserts in
`avahi_simple_poll_prepare()` and aborts Chataigne. Daemon disconnections and
browser/resolver failures can reach this path. The bundled Linux x64 library
contains this retry loop (the call returns at `browse()+0x75`).

`patches/avahi-poll-failure.patch` latches terminal poll failures, prevents
subsequent browse/announce calls from reusing the poll, and handles Avahi's
registering event without throwing out of its C callback. Discovery stops with
a warning; restarting Chataigne after Avahi recovers restores discovery.

Linux and Raspberry Pi release jobs rebuild Servus from the pinned
`benkuper/Servus` revision `a44cf1345f9d72aab6942e165b3e89d5feaff7f7` and apply
the patch before linking and packaging. `source/` contains the Linux library
sources from that revision and the three generated-header templates from its
pinned Eyescale/CMake revision `89c330d50b978276dcea3af700258e554be8dfd7`.
Licenses and attribution are included. The build uses these local sources;
CI dependency checkout configuration is unchanged.
The existing checked-in binaries have not been replaced: **local Linux builds
must rebuild Servus too**. From the repository root:

```sh
sudo apt-get install cmake pkg-config libavahi-client-dev
bash Tools/build_servus_linux.sh
bash Tools/tests/run_servus_avahi_failure_test.sh
```

The script keeps dependency sources/builds in `/tmp` by default. Set
`SERVUS_BUILD_DIR` to change that location and pass an output directory as its
first argument to avoid replacing the local bundled library. Cross builds can
set `CXX`, `CXXFLAGS`, `PKG_CONFIG_LIBDIR`, and `CMAKE_TOOLCHAIN_FILE`.
Build release libraries on the supported baseline distribution (Ubuntu 22.04)
so newer host libc/libstdc++ requirements are not introduced into releases.

The regression test injects poll quits/errors, daemon failure, registration,
browser/resolver failures and announcement failure. It uses real Avahi polling
without connecting to a daemon. The original library aborts on the quit test;
the patched library passes all scenarios, including continued healthy polling.
