#ifndef JOYTIME_CORE_HPP
#define JOYTIME_CORE_HPP

#include <cstdint>
#include <vector>
#include "EventEmitter.hpp"
#include "joytime_core_EXPORTS.h"

/*
 * This is a header for C++ clients of the library.
 * If you're using C, you'll probably want to use "joytime-core.h"
 */

namespace Joytime {
  enum class ControllerType: uint8_t {
    LeftJoycon = 0,
    RightJoycon = 1,
    Pro = 2,
  };
  enum class ControllerCommand: uint8_t {
    RumbleAndSubcommand = 0x01,
    SendRumble = 0x10,
  };
  enum class ControllerSubcommand: uint8_t {
    GetOnlyControllerState = 0x00,
    GetDeviceInfo = 0x02,
    SetInputReportMode = 0x03,
    SetPowerState = 0x06,
    ReadSPIFlash = 0x10,
    SetPlayerLights = 0x30,
    SetSixAxisSensor = 0x40,
    SetVibration = 0x48,
  };
  enum class ControllerInputReportMode: uint8_t {
    IRCamera = 0x00,
    _SpecialIR = 0x02,
    _MCUUpdateState = 0x20,
    StandardReport = 0x30,
    NFCAndIR = 0x31,
    SimpleHID = 0x3f,
  };
  enum class ControllerLEDState: uint8_t {
    Off = 0,
    On = 1,
    Flashing = 2,
  };
  enum class ControllerPowerState: uint8_t {
    Disconnect = 0x00,
    ColdReboot = 0x01,
    PairingReboot = 0x02,
    PowerOff = 0x04,
  };
  enum class ControllerReportCode: uint8_t {
    SubcommandReply = 0x21,
    MCUFWUpdate = 0x23,
    Standard = 0x30,
    NFCIR = 0x31,
    StandardOSController = 0x3f,
  };
  enum class ControllerBatteryStatus: uint8_t {
    Full = 0x08,
    Medium = 0x06,
    Low = 0x04,
    Critical = 0x02,
    Empty = 0x00,
    Charging = 0x01,
  };
  struct StickCalibrationData {
    uint16_t xCenter = 0;
    uint16_t yCenter = 0;
    uint16_t xMax = 0;
    uint16_t yMax = 0;
    uint16_t xMin = 0;
    uint16_t yMin = 0;
    uint16_t deadZone = 0;
    uint16_t rangeRatio = 0;
  };
  struct SixAxisCalibrationData {
    int16_t originX = 0;
    int16_t originY = 0;
    int16_t originZ = 0;
    int16_t rawCoeffX = 0;
    int16_t rawCoeffY = 0;
    int16_t rawCoeffZ = 0;
    int16_t offsetX = 0;
    int16_t offsetY = 0;
    int16_t offsetZ = 0;
    double coeffX = 0;
    double coeffY = 0;
    double coeffZ = 0;
  };
  struct Buttons {
    bool a = false;
    bool b = false;
    bool x = false;
    bool y = false;
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool l = false;
    bool r = false;
    bool zl = false;
    bool zr = false;
    bool sl = false;
    bool sr = false;
    bool plus = false;
    bool minus = false;
    bool lStick = false;
    bool rStick = false;
    bool home = false;
    bool capture = false;
  };
  struct Stick {
    int16_t x = 0;
    int16_t y = 0;
  };
  struct SixAxis {
    double x = 0;
    double y = 0;
    double z = 0;
  };
  typedef void (TransmitBufferFunction)(void*, std::vector<uint8_t>);
  typedef std::vector<uint8_t> (ReceiveBufferFunction)(void*, int);
  typedef void (CTransmitBufferFunction)(void*, uint8_t*, int);
  typedef uint8_t* (CReceiveBufferFunction)(void*, int, int*);
  class JOYTIME_CORE_EXPORT Rumble {
    public:
      uint16_t highFrequency;
      uint8_t highAmplitude;
      uint8_t lowFrequency;
      uint16_t lowAmplitude;
      Rumble(double frequency, double amplitude);
      Rumble(double highFrequency, double highAmplitude, double lowFrequency, double lowAmplitude);
      Rumble(uint16_t highFrequency, uint8_t highAmplitude, uint8_t lowFrequency, uint16_t lowAmplitude);
      uint8_t* toBuffer();
      std::vector<uint8_t> toVector();
      static uint16_t frequencyToHF(double frequency);
      static uint8_t frequencyToLF(double frequency);
      static uint8_t amplitudeToHA(double amplitude);
      static uint16_t amplitudeToLA(double amplitude);
  };
  class JOYTIME_CORE_EXPORT Controller {
    private:
      TransmitBufferFunction* transmitBuffer = nullptr;
      ReceiveBufferFunction* receiveBuffer = nullptr;
      CTransmitBufferFunction* transmitBufferC = nullptr;
      CReceiveBufferFunction* receiveBufferC = nullptr;
      bool usable = false;
      bool initializable = true;

      // global packet counter for subcommands,
      // loops in 0x0 through 0xf
      uint8_t counter = 0;

      void performUsabilityCheck();
      void update(std::vector<uint8_t> buf);
      void _transmitBuffer(std::vector<uint8_t> buffer);
      std::vector<uint8_t> _receiveResponse();
      std::vector<uint8_t> sendCommand(Joytime::ControllerCommand command, std::vector<uint8_t> buf);
      std::vector<uint8_t> sendSubcommand(Joytime::ControllerCommand command, Joytime::ControllerSubcommand subcommand, std::vector<uint8_t> buf);
    public:
      // suggested update interval, in milliseconds
      int interval = 60;
      void* handle;
      ControllerType type;
      ControllerBatteryStatus battery;
      StickCalibrationData leftStickCalibration;
      StickCalibrationData rightStickCalibration;
      SixAxisCalibrationData accelerometerCalibration;
      SixAxisCalibrationData gyroscopeCalibration;
      Buttons buttons;
      Stick leftStick;
      Stick rightStick;
      SixAxis accelerometer;
      SixAxis gyroscope;
      EventEmitter<Controller*> updated;

      Controller();
      Controller(const Controller&);
      Controller(ControllerType type, void* handle, TransmitBufferFunction* sendBuffer, ReceiveBufferFunction* receiveBuffer);
      Controller(ControllerType type, void* handle, CTransmitBufferFunction* sendBufferC, CReceiveBufferFunction* receiveBufferC);
      void initialize(bool calibrate = false);

      void setVibration(bool vibrate);
      void setSixAxisEnabled(bool enabled);
      void setInputReportMode(ControllerInputReportMode mode);
      void rumble(uint8_t timing, Rumble* rumble);
      void rumble(uint8_t timing, Rumble* leftRumble, Rumble* rightRumble);
      void setLEDs(ControllerLEDState led1, ControllerLEDState led2, ControllerLEDState led3, ControllerLEDState led4);
      void setPowerState(ControllerPowerState powerState);
      std::vector<uint8_t> readSPIFlash(int32_t address, uint8_t size);

      void update();

      // default suggested update interval, in milliseconds
      static const int defaultInterval = 60;
  };
  JOYTIME_CORE_EXPORT extern Joytime::Rumble neutralRumble;
  JOYTIME_CORE_EXPORT extern uint8_t* neutralRumbleBuffer;
  JOYTIME_CORE_EXPORT extern std::vector<uint8_t> neutralRumbleVector;
}

#endif /* JOYTIME_CORE_HPP */
