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

The Joy-Con 2 test compiles the real BLE backend and shared sessions against
the SimpleBLE API with simulated radios. It checks unnamed Nintendo
advertisements, left/right buttons (including C), sticks, IMU decoding,
truncated reports, startup commands and acknowledgements, mixed controller
generations, shared ownership, reconnect, cancellation and late notifications.
It also simulates a scan response erasing manufacturer data after a valid
advertisement; both controllers must still be discovered from the callbacks.

Joy-Con 2 on Windows 10/11
-------------------------

For a live connection/input trace without starting the app, close Chataigne and
run this manual probe (it is excluded from automated tests):
```
cmake --build Binaries/controller-tests --config Debug --target joycon2_live_test
Binaries/controller-tests/Debug/joycon2_live_test.exe
```
It scans for two minutes and prints connection changes, buttons, sticks, motion
and raw packet traces. Pass a duration in seconds (for example, `300`) to allow
more time. Hold SYNC, then exercise each input while it is running.

1. Enable Bluetooth, add a JoyCon module and hold the SYNC button on each
   Joy-Con 2 until its player LEDs cycle. Chataigne discovers and connects
   directly over BLE; do not pair these through Windows Bluetooth settings.
2. Use Reconnect Controllers to scan again if needed. Discovery also retries
   automatically while either side is missing. Check Connected under each
   controller, then its buttons, stick, acceleration and orientation. The right
   controller exposes the new C button. Original Joy-Cons still use OS pairing.
3. Test one original Joy-Con with a Joy-Con 2 on the other side, two modules,
   power loss, reconnection and closing a project during discovery. Lost input
   must clear buttons and C; deleting one module must keep the other receiving.

The portable transport uses the existing SimpleBLE dependency and is enabled
with JOYCON2_BLE_SUPPORT=1 in the modern Windows exporters. Windows 7 and the
macOS/Linux exporters leave it disabled. To enable those platforms later, link
a compatible SimpleBLE build, enable the define and validate their BLE stack
and permissions with real controllers. No WinRT code is embedded in the module.
BLE connection/service calls run on the worker and retain SimpleBLE's native
OS timeouts; the scan and protocol acknowledgement waits are cancellable.
The initial implementation uses nominal stick/sensor calibration and does not
expose optical mouse input or rumble.

The macOS test compiles the actual WiiUse transport and substitutes Bluetooth
objects. It checks original/Plus name filtering, excess discovery results,
missing hardware, inquiry completion after a device-found callback, cancellation,
a missing completion callback, delayed pairing authorization, a failed interrupt
channel followed by a retry of both channels, cancellation during pairing, malformed input,
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
   its SYNC button, then use Refresh Wiimotes. On macOS, accept the pairing
   notification while the connection retries (a 30-second retry window per remote);
   keep the remote awake. Check that the same Refresh reaches Connected after
   authorization, and that closing Chataigne during the retry wait exits promptly.
   If it fails, capture stderr: L2CAP errors include the PSM and IOReturn code.
   Check Connected, buttons, battery,
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
