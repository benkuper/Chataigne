#ifndef JOYTIME_CORE_H
#define JOYTIME_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include "joytime_core_EXPORTS.h"

/*
 * This is a header for C clients of the library.
 * If you're using C++, you'll probably want to use "joytime-core.hpp"
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum _Joytime_ControllerType {
  Type_LeftJoycon = 0,
  Type_RightJoycon = 1,
  Type_Pro = 2,
} Joytime_ControllerType;
typedef enum _Joytime_ControllerInputReportMode {
  Mode_IRCamera = 0x00,
  Mode__SpecialIR = 0x02,
  Mode__MCUUpdateState = 0x20,
  Mode_StandardReport = 0x30,
  Mode_NFCAndIR = 0x31,
  Mode_SimpleHID = 0x3f,
} Joytime_ControllerInputReportMode;
typedef enum _Joytime_ControllerLEDState {
  LED_Off = 0,
  LED_On = 1,
  LED_Flashing = 2,
} Joytime_ControllerLEDState;
typedef enum _Joytime_ControllerPowerState {
  Power_Disconnect = 0x00,
  Power_ColdReboot = 0x01,
  Power_PairingReboot = 0x02,
  Power_Off = 0x04,
} Joytime_ControllerPowerState;
typedef enum _Joytime_ControllerReportCode {
  Code_SubcommandReply = 0x21,
  Code_MCUFWUpdate = 0x23,
  Code_Standard = 0x30,
  Code_NFCIR = 0x31,
  Code_StandardOSController = 0x3f,
} Joytime_ControllerReportCode;
typedef enum _Joytime_ControllerBatteryStatus {
  Battery_Full = 0x08,
  Battery_Medium = 0x06,
  Battery_Low = 0x04,
  Battery_Critical = 0x02,
  Battery_Empty = 0x00,
  Battery_Charging = 0x01,
} Joytime_ControllerBatteryStatus;

typedef struct _Joytime_StickCalibrationData {
  uint16_t xCenter;
  uint16_t yCenter;
  uint16_t xMax;
  uint16_t yMax;
  uint16_t xMin;
  uint16_t yMin;
  uint16_t deadZone;
  uint16_t rangeRatio;
} Joytime_StickCalibrationData;

typedef struct _Joytime_SixAxisCalibrationData {
  int16_t originX;
  int16_t originY;
  int16_t originZ;
  int16_t rawCoeffX;
  int16_t rawCoeffY;
  int16_t rawCoeffZ;
  int16_t offsetX;
  int16_t offsetY;
  int16_t offsetZ;
  double coeffX;
  double coeffY;
  double coeffZ;
} Joytime_SixAxisCalibrationData;

typedef struct _Joytime_Buttons {
  bool a;
  bool b;
  bool x;
  bool y;
  bool up;
  bool down;
  bool left;
  bool right;
  bool l;
  bool r;
  bool zl;
  bool zr;
  bool sl;
  bool sr;
  bool plus;
  bool minus;
  bool lStick;
  bool rStick;
  bool home;
  bool capture;
} Joytime_Buttons;

typedef struct _Joytime_Stick {
  int16_t x;
  int16_t y;
} Joytime_Stick;

typedef struct _Joytime_SixAxis {
  double x;
  double y;
  double z;
} Joytime_SixAxis;

typedef struct _Joytime_Rumble Joytime_Rumble;
typedef struct _Joytime_Controller Joytime_Controller;

typedef uint32_t Joytime_UpdateListenerID;
typedef void (Joytime_UpdateListener)(Joytime_Controller*);
typedef void (Joytime_TransmitBufferFunction)(void*, uint8_t*, int);
typedef uint8_t* (Joytime_ReceiveBufferFunction)(void*, int, int*);

JOYTIME_CORE_EXPORT Joytime_Rumble* Joytime_Rumble_newFromFreqAndAmpSame(double frequency, double amplitude);
JOYTIME_CORE_EXPORT Joytime_Rumble* Joytime_Rumble_newFromFreqAndAmpDiff(double highFrequency, double highAmplitude, double lowFrequency, double lowAmplitude);
JOYTIME_CORE_EXPORT Joytime_Rumble* Joytime_Rumble_newFromPreencoded(uint16_t highFrequency, uint8_t highAmplitude, uint8_t lowFrequency, uint16_t lowAmplitude);
JOYTIME_CORE_EXPORT uint8_t* Joytime_Rumble_toBuffer(Joytime_Rumble* rumble);
JOYTIME_CORE_EXPORT uint16_t* Joytime_Rumble_getHighFrequency(Joytime_Rumble* rumble);
JOYTIME_CORE_EXPORT uint8_t* Joytime_Rumble_getHighAmplitude(Joytime_Rumble* rumble);
JOYTIME_CORE_EXPORT uint8_t* Joytime_Rumble_getLowFrequency(Joytime_Rumble* rumble);
JOYTIME_CORE_EXPORT uint16_t* Joytime_Rumble_getLowAmplitude(Joytime_Rumble* rumble);

JOYTIME_CORE_EXPORT uint16_t Joytime_Rumble_frequencyToHF(double frequency);
JOYTIME_CORE_EXPORT uint8_t Joytime_Rumble_frequencyToLF(double frequency);
JOYTIME_CORE_EXPORT uint8_t Joytime_Rumble_amplitudeToHA(double amplitude);
JOYTIME_CORE_EXPORT uint16_t Joytime_Rumble_amplitudeToLA(double amplitude);

JOYTIME_CORE_EXPORT Joytime_Controller* Joytime_Controller_new(Joytime_ControllerType type, void* handle, Joytime_TransmitBufferFunction* transmitBuffer, Joytime_ReceiveBufferFunction* receiveBuffer);
JOYTIME_CORE_EXPORT void Joytime_Controller_free(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT void Joytime_Controller_initialize(Joytime_Controller* controller, bool calibrate);
JOYTIME_CORE_EXPORT void Joytime_Controller_setVibrate(Joytime_Controller* controller, bool vibrate);
JOYTIME_CORE_EXPORT void Joytime_Controller_setSixAxisEnabled(Joytime_Controller* controller, bool enabled);
JOYTIME_CORE_EXPORT void Joytime_Controller_setInputReportMode(Joytime_Controller* controller, Joytime_ControllerInputReportMode mode);
JOYTIME_CORE_EXPORT void Joytime_Controller_rumbleSame(Joytime_Controller* controller, uint8_t timing, Joytime_Rumble* rumble);
JOYTIME_CORE_EXPORT void Joytime_Controller_rumbleEach(Joytime_Controller* controller, uint8_t timing, Joytime_Rumble* rumble1, Joytime_Rumble* rumble2);
JOYTIME_CORE_EXPORT void Joytime_Controller_setLEDs(Joytime_Controller* controller, Joytime_ControllerLEDState led1, Joytime_ControllerLEDState led2, Joytime_ControllerLEDState led3, Joytime_ControllerLEDState led4);
JOYTIME_CORE_EXPORT void Joytime_Controller_setPowerState(Joytime_Controller* controller, Joytime_ControllerPowerState state);
JOYTIME_CORE_EXPORT int Joytime_Controller_readSPIFlash(Joytime_Controller* controller, int32_t address, uint8_t length, uint8_t* buf);
JOYTIME_CORE_EXPORT void Joytime_Controller_update(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_UpdateListenerID Joytime_Controller_registerUpdateListener(Joytime_Controller* controller, Joytime_UpdateListener* listener);
JOYTIME_CORE_EXPORT void Joytime_Controller_removeUpdateListener(Joytime_Controller* controller, Joytime_UpdateListenerID id);
JOYTIME_CORE_EXPORT int* Joytime_Controller_getInterval(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT void** Joytime_Controller_getHandle(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT uint8_t* Joytime_Controller_getType(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT uint8_t* Joytime_Controller_getBattery(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_StickCalibrationData* Joytime_Controller_getLeftStickCalibration(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_StickCalibrationData* Joytime_Controller_getRightStickCalibration(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_SixAxisCalibrationData* Joytime_Controller_getAccelerometerCalibration(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_SixAxisCalibrationData* Joytime_Controller_getGyroscopeCalibration(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_Buttons* Joytime_Controller_getButtons(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_Stick* Joytime_Controller_getLeftStick(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_Stick* Joytime_Controller_getRightStick(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_SixAxis* Joytime_Controller_getAccelerometer(Joytime_Controller* controller);
JOYTIME_CORE_EXPORT Joytime_SixAxis* Joytime_Controller_getGyroscope(Joytime_Controller* controller);

static int Joytime_Controller_defaultInterval = 60;

JOYTIME_CORE_EXPORT extern Joytime_Rumble* Joytime_neutralRumble;
JOYTIME_CORE_EXPORT extern uint8_t* Joytime_neutralRumbleBuffer;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* JOYTIME_CORE_H */
