Controller regression checks
===========================

Build the hardware-independent tests with CMake:
```
cmake -S Tools/tests/controllers -B Binaries/controller-tests
cmake --build Binaries/controller-tests --config Debug
ctest --test-dir Binaries/controller-tests -C Debug --output-on-failure
```

Linux/macOS builds enable AddressSanitizer and UndefinedBehaviorSanitizer.
The Joy-Con tests exercise the actual shared session owner and the vendored
JoyShock source with a fake HID transport: 64 handles, simultaneous modules,
refresh floods, concurrent session teardown, null serials, complete/truncated
reports, unplug, malformed calibration replies, failed initialization and
reader joins. No physical controller is required.

The macOS test compiles the actual WiiUse transport and substitutes Bluetooth
objects. It checks original/Plus name filtering, excess discovery results,
missing hardware, cancellation, a missing completion callback, malformed input,
bounded queues, failed writes, lost L2CAP channels, notification ownership and
100 connect/disconnect cycles. It never starts real Bluetooth discovery.

After building Windows Debug, run the application-level lifecycle test:
```
Tools/tests/run_controller_module_lifecycle_test.ps1
```
This constructs the real modules, changes device selection and smoothing,
reconnects Joy-Cons, disables modules, and destroys them during startup. It uses
the machine's real HID/Bluetooth backend; Wii pairing is not requested.

Physical macOS verification (still required for radio/OS compatibility)
--------------------------------------------------------------------

1. Enable Bluetooth and allow Chataigne's Bluetooth permission when requested.
2. Add a Wiimote module. Make the original Wii Remote discoverable with 1+2 or
   its SYNC button, then use Refresh Wiimotes. Check Connected, buttons, battery,
   acceleration, orientation, and Device Select with multiple remotes/modules.
3. Check Nunchuk C/Z, stick and extension insertion/removal. With an IR source,
   check all four points and clearing when the source disappears.
4. Pair left/right Nintendo Joy-Cons in macOS Bluetooth settings. Add two JoyCon
   modules, check both button sets, both sticks, acceleration and orientation.
   Delete one module and verify the other keeps receiving input.
5. Refresh repeatedly, power controllers off/on, disconnect/reconnect Bluetooth,
   sleep/wake, and reload/close a project with both module types. Check that lost
   controllers release buttons and that the application stays responsive.

Record the macOS version, Mac architecture, controller model and any crash
report when reporting a failed physical check. Passing simulated transport
tests does not establish Bluetooth pairing compatibility with every macOS release.
