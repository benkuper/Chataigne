#pragma once
#include "JoyShockLibrary.h"
#include <array>

struct JoyConFrame
{
    bool connected = false;
    bool c = false; // Joy-Con 2 right controller
    JOY_SHOCK_STATE state {};
    MOTION_STATE motion {};
};
using JoyConFrames = std::array<JoyConFrame, 2>;

// All modules share one hardware owner. Releasing one session cannot disconnect
// another; the last release joins the owner and every HID reader cooperatively.
class JoyConBackend;
class JoyConDeviceSession
{
public:
    JoyConDeviceSession();
    ~JoyConDeviceSession();
    JoyConFrames getFrames() const;
    void reconnect();
private:
    JoyConBackend* backend;
    JoyConDeviceSession(const JoyConDeviceSession&) = delete;
    JoyConDeviceSession& operator=(const JoyConDeviceSession&) = delete;
};
