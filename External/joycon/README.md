JoyShockLibrary is built from source for every exporter. The old static archives
in lib/ are no longer linked.

Upstream: https://github.com/JibbSmart/JoyShockLibrary
Version: v3.0, commit 21aa18bc71c6dab07af35e3d42260e6ae2196b5d
License: LICENSE.md (MIT, including upstream attributions).

Local changes:
- One hardware owner shared by all JoyCon modules (JoyConDeviceSession).
- Owner joins readers before freeing handles; readers never delete themselves.
- Atomic cancellation and completion, bounded SPI retries, null serial handling.
- Refresh preserves live handles and avoids commands through their readers.
- Protect simple-state copies; reject incomplete input reports.
- Keep process-wide HIDAPI alive for other controller modules.
- Plain read/write lock adapter supports older Intel macOS deployment targets.

macOS builds HIDAPI 0.15.0 from ../hidapi/mac/ instead of static archives.
Other platforms retain their existing system/bundled HIDAPI backend.

Regression tests:
```
cmake -S Tools/tests/controllers -B Binaries/controller-tests
cmake --build Binaries/controller-tests --config Debug
ctest --test-dir Binaries/controller-tests -C Debug --output-on-failure
```
