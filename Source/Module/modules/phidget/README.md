# Phidget module

Add **Hardware > Phidget**. The default configuration targets the PhidgetInterfaceKit
1012 (0/16/16): 16 digital inputs and 16 digital outputs, numbered **0–15**.

## Setup

1. Install the official [Phidget22 runtime and drivers](https://www.phidgets.com/docs/Operating_System_Support).
   Use the same architecture as Chataigne (normally 64-bit on Windows).
2. Plug in the 1012. Close any channel test windows in Phidget Control Panel so
   Chataigne can acquire those channels.
3. Set **Serial Number** to the board's serial, or leave **-1** to select the first
   matching board. Automatic selection binds every channel to that one board.
4. For the USB 1012, leave **Hub Port = -1** and **Hub Port Device = false**.
5. **Is Connected** becomes true when all 32 channels attach. **Attached Channels**,
   **Connected Serial**, and **Status** show connection progress and SDK errors.

The module finds the installed native library automatically. If needed, set
**Library Path** to the absolute path of `phidget22.dll` on Windows,
`/Library/Frameworks/Phidget22.framework/Phidget22` on macOS, or `libphidget22.so`
on Linux. Linux also requires the USB permissions described in the official
[Linux setup guide](https://www.phidgets.com/docs/OS_-_Linux).
Click **Reconnect** after installing the runtime or changing the library location.

## Inputs and outputs

- **Values > Digital Inputs > Channel 0–15** provides feedback-only booleans for
  mappings, conditions, and scripts.
- **Set Digital Output** sets the chosen zero-based output channel to on or off.
  Its State parameter is available to mappings.
- **Values > Digital Outputs** reports the SDK's current output states.
- Unplugging updates connection feedback. The SDK automatically reattaches to the
  selected board when it returns. Disabling or removing the module closes its
  channels. Output commands issued while disconnected are rejected; they are
  never queued or replayed after reconnecting.

Readings are polled on the message thread, every **20 ms** by default. **Poll
Interval** changes this period (5–1000 ms). These are current states, not a queue
of hardware edges; pulses shorter than the polling period can be missed. Values
retain their last reading while disconnected; check **Is Connected** before
using them as live measurements.

For other InterfaceKits, adjust the channel counts to match their hardware. The
module also supports Voltage Input (volts), Voltage Ratio Input (raw ratio), and
Voltage Output (volts). Set unsupported channel counts to zero. Voltage and
ratio input modes may compete for the same physical analog input: choose the
mode appropriate for the sensor. **Set Voltage Output** requires a voltage-output
device. **Set Duty Cycle** requires PWM-capable digital outputs; use **Set Digital
Output** for the 1012. Other Phidget channel classes and Network Server discovery
are not implemented.

See the official [1012 channel table](https://www.phidgets.com/?prodid=6) and
[Phidget22 API overview](https://www.phidgets.com/docs/Using_the_Phidget22_API).

## Build and verification

The module loads the Phidget22 C API at runtime, so no Phidget SDK headers,
import libraries, or CI dependency changes are required. All exporter builds use
the module through the existing `ModuleIncludes.cpp` compilation unit.

On Windows, build Chataigne Debug, then run:

```powershell
Tools/tests/run_phidget_integration_test.ps1
```

This builds a simulated Phidget22 DLL and tests 1012 I/O, single-board binding,
runtime/API failures, analog I/O, SDK write errors, unplug/replug, commands,
disable/cleanup, and session persistence. Physical hardware still requires an
end-to-end check with a real 1012.

Optionally pass `-NativeRuntime <absolute path to phidget22.dll>` to also test
channel creation, non-blocking open, polling, and cleanup against the installed
official Windows runtime. This uses an unmatched serial number and never writes
physical outputs.
