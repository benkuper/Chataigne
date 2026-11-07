// JoyShockLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "JoyShockLibrary.h"
#include <bitset>
#ifdef __APPLE__
#include "../../hidapi/mac/hidapi.h"
#else
#include "hidapi.h"
#endif
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <atomic>
#include "GamepadMotion.hpp"
std::atomic<bool> chataigneJslStopping { false };
#include "JoyShock.cpp"
#include "InputHelpers.cpp"

class JslMutex
{
    std::mutex mutex;
public:
    void lock() { mutex.lock(); }
    void unlock() { mutex.unlock(); }
    void lock_shared() { mutex.lock(); }
    void unlock_shared() { mutex.unlock(); }
};
JslMutex _callbackLock;
JslMutex _connectedLock;
void(*_pollCallback)(int, JOY_SHOCK_STATE, JOY_SHOCK_STATE, IMU_STATE, IMU_STATE, float) = nullptr;
void(*_pollTouchCallback)(int, TOUCH_STATE, TOUCH_STATE, float) = nullptr;
void(*_connectCallback)(int) = nullptr;
void(*_disconnectCallback)(int, bool) = nullptr;
std::unordered_map<int, JoyShock*> _joyshocks;
std::unordered_map<std::string, JoyShock*> _byPath;
std::mutex _pathHandleLock;
std::unordered_map<std::string, int> _pathHandle;
// https://stackoverflow.com/questions/41206861/atomic-increment-and-return-counter
static std::atomic<int> _joyshockHandleCounter;

static int GetUniqueHandle(const std::string &path)
{
	_pathHandleLock.lock();
	auto iter = _pathHandle.find(path);

	if (iter != _pathHandle.end())
	{
		_pathHandleLock.unlock();
		return iter->second;
	}
	const int handle = _joyshockHandleCounter++;
	_pathHandle.emplace(path, handle);
	_pathHandleLock.unlock();

	return handle;
}

// https://stackoverflow.com/questions/25144887/map-unordered-map-prefer-find-and-then-at-or-try-at-catch-out-of-range
// not thread-safe -- because you probably want to do something with the object you get out of it, I've left locking to the caller
static JoyShock* GetJoyShockFromHandle(int handle) {
	auto iter = _joyshocks.find(handle);

	if (iter != _joyshocks.end() && !iter->second->finished.load())
	{
		return iter->second;
		// iter is item pair in the map. The value will be accessible as `iter->second`.
	}
	return nullptr;
}

void pollIndividualLoop(JoyShock *jc) {
	if (!jc->handle) { return; }

	hid_set_nonblocking(jc->handle, 0);
	//hid_set_nonblocking(jc->handle, 1); // temporary, to see if it helps. this means we'll have a crazy spin

	int numTimeOuts = 0;
	int numNoIMU = 0;
	bool hasIMU = false;
	
	int noIMULimit;
	switch (jc->controller_type)
	{
	case ControllerType::s_ds4:
		noIMULimit = 250;
		break;
	case ControllerType::s_ds:
		noIMULimit = 250;
		break;
	case ControllerType::n_switch:
	default:
		noIMULimit = 67;
	}
	float wakeupTimer = 0.0f;

	while (!jc->cancel_thread) {
		// get input:
		unsigned char buf[128];
		memset(buf, 0, sizeof(buf));

		// 10 seconds of no signal means forget this controller
		
		int res = hid_read_timeout(jc->handle, buf, sizeof(buf), 100);

        if (res == -1) break;

        if (res == 0)
        {
            if (++numTimeOuts >= 100) break;
        }
		else
		{
			numTimeOuts = 0;
			// we want to be able to do these check-and-calls without fear of interruption by another thread. there could be many threads (as many as connected controllers),
			// and the callback could be time-consuming (up to the user), so we use a readers-writer-lock.
			bool processed = false;
            {
                std::lock_guard<std::mutex> stateLock(jc->state_lock);
                std::lock_guard<std::recursive_mutex> motionLock(jc->modifying_lock);
                processed = handle_input(jc, buf, res, hasIMU);
            }
            if (processed) { // but the user won't necessarily have a callback at all, so we'll skip the lock altogether in that case
				// accumulate gyro
				IMU_STATE imuState = jc->get_transformed_imu_state(jc->imu_state);
				jc->push_cumulative_gyro(imuState.gyroX, imuState.gyroY, imuState.gyroZ);
				{
					std::shared_lock<JslMutex> lock(_callbackLock);
					if (_pollCallback != nullptr) {
						_pollCallback(jc->intHandle, jc->simple_state, jc->last_simple_state, imuState, jc->get_transformed_imu_state(jc->last_imu_state), jc->delta_time);
					}
					// touchpad will have its own callback so that it doesn't change the existing api
					if (jc->controller_type != ControllerType::n_switch && _pollTouchCallback != nullptr) {
						_pollTouchCallback(jc->intHandle, jc->touch_state, jc->last_touch_state, jc->delta_time);
					}
				}
				// count how many have no IMU result. We want to periodically attempt to enable IMU if it's not present
				if (!hasIMU)
				{
					numNoIMU++;
					if (numNoIMU == noIMULimit)
					{
						memset(buf, 0, 64);
						jc->enable_IMU(buf, 64);
						numNoIMU = 0;
					}
				}
				else
				{
					numNoIMU = 0;
				}
				
				// dualshock 4 bluetooth might need waking up
				if (jc->controller_type == ControllerType::s_ds4 && !jc->is_usb)
				{
					wakeupTimer += jc->delta_time;
					if (wakeupTimer > 30.0f)
					{
						jc->init_ds4_bt();
						wakeupTimer = 0.0f;
					}
				}
			}
		}
	}

    jc->finished.store(true);
    std::shared_lock<JslMutex> lock(_callbackLock);
    if (_disconnectCallback != nullptr) _disconnectCallback(jc->intHandle, numTimeOuts >= 100);
}

// Called only by the hardware owner. Reader threads never delete themselves.
static void DisposeJoyShock(JoyShock* jc)
{
    jc->cancel_thread.store(true);
    if (jc->thread != nullptr) {
        jc->thread->join();
        delete jc->thread;
        jc->thread = nullptr;
    }
    delete jc;
}

int JslConnectDevices()
{
	// for writing to console:
	//freopen("CONOUT$", "w", stdout);

	// most of the joycon and pro controller stuff here is thanks to mfosse's vjoy feeder
	// Enumerate and print the HID devices on the system
	struct hid_device_info *devs, *cur_dev;

	std::vector<int> createdIds;

	std::unique_lock<JslMutex> connections(_connectedLock);

    for (auto it = _joyshocks.begin(); it != _joyshocks.end();) {
        JoyShock* jc = it->second;
        if (!jc->finished.load()) { ++it; continue; }
        _byPath.erase(jc->path);
        it = _joyshocks.erase(it);
        connections.unlock();
        DisposeJoyShock(jc);
        connections.lock();
    }
    int res = hid_init();

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;
	while (cur_dev && !chataigneJslStopping.load()) {
		bool isSupported = false;
		bool isSwitch = false;
		switch (cur_dev->vendor_id)
		{
		case JOYCON_VENDOR:
			isSupported = cur_dev->product_id == JOYCON_L_BT ||
				cur_dev->product_id == JOYCON_R_BT ||
				cur_dev->product_id == PRO_CONTROLLER ||
				cur_dev->product_id == JOYCON_CHARGING_GRIP;
			isSwitch = true;
			break;
		case DS_VENDOR:
			isSupported = cur_dev->product_id == DS4_USB ||
				cur_dev->product_id == DS4_USB_V2 ||
				cur_dev->product_id == DS4_USB_DONGLE ||
				cur_dev->product_id == DS4_BT ||
				cur_dev->product_id == DS_USB;
			break;
		case BROOK_DS4_VENDOR:
			isSupported = cur_dev->product_id == BROOK_DS4_USB;
			break;
		default:
			break;
		}
		if (!isSupported)
		{
			cur_dev = cur_dev->next;
			continue;
		}

		const std::string path = std::string(cur_dev->path);
        if (_byPath.find(path) != _byPath.end()) {
            cur_dev = cur_dev->next;
            continue;
        }
        hid_device* handle = hid_open_path(cur_dev->path);
        if (handle != nullptr) {
            JoyShock* jc = new JoyShock(cur_dev, handle, GetUniqueHandle(path), path);
            _joyshocks.emplace(jc->intHandle, jc);
            _byPath.emplace(path, jc);
            createdIds.push_back(jc->intHandle);
        }

		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);

	// init joyshocks:
	for (std::pair<int, JoyShock*> pair : _joyshocks)
	{
		JoyShock* jc = pair.second;
		
		if (jc->initialised || jc->thread != nullptr || chataigneJslStopping.load())
		{
			continue;
		}

		if (jc->controller_type == ControllerType::s_ds4) {
			if (!jc->is_usb) {
				jc->init_ds4_bt();
			}
			else {
				jc->init_ds4_usb();
			}
		} // dualsense
		else if (jc->controller_type == ControllerType::s_ds)
		{
			jc->initialised = true;
		} // charging grip
		else if (jc->is_usb) {
			//printf("USB\n");
			jc->initialised = jc->init_usb();
		}
		else {
			//printf("BT\n");
			jc->initialised = jc->init_bt();
		}
		if (!jc->initialised) jc->finished.store(true);
		// all get time now for polling
		jc->last_polled = std::chrono::steady_clock::now();
		jc->delta_time = 0.0;

		jc->deviceNumber = 0; // left
	}

	unsigned char buf[64];

	// set lights:
	//printf("setting LEDs...\n");
	int switchIndex = 1;
	int dualSenseIndex = 1;
	for (std::pair<int, JoyShock*> pair : _joyshocks)
	{
		JoyShock *jc = pair.second;
        if (jc->thread != nullptr || jc->finished.load() || chataigneJslStopping.load()) continue;

		// restore colours if we have them set for this controller
		switch (jc->controller_type)
		{
		case ControllerType::s_ds4:
			jc->set_ds4_rumble_light(0, 0, jc->led_r, jc->led_g, jc->led_b);
			break;
		case ControllerType::s_ds:
			jc->set_ds5_rumble_light(0, 0, jc->led_r, jc->led_g, jc->led_b, dualSenseIndex++);
			break;
		case ControllerType::n_switch:
			jc->player_number = switchIndex++;
			memset(buf, 0x00, 0x40);
			buf[0] = (unsigned char)jc->player_number;
			jc->send_subcommand(0x01, 0x30, buf, 1);
			break;
		}

		// threads for polling
		if (jc->thread == nullptr)
		{
			printf("\tstarting new thread\n");
			jc->thread = new std::thread(pollIndividualLoop, jc);
		}
	}

	const int totalDevices = (int)_joyshocks.size();

	connections.unlock();

	// notify that we created the new object (now that we're not in a lock that might prevent reading data)
	{
		std::shared_lock<JslMutex> lock(_callbackLock);
		if (_connectCallback != nullptr)
		{
			for (int newConnectionHandle : createdIds)
			{
				_connectCallback(newConnectionHandle);
			}
		}
	}

	return totalDevices;
}

int JslGetConnectedDeviceHandles(int* deviceHandleArray, int size)
{
	int i = 0;
	_connectedLock.lock_shared();
	for (std::pair<int, JoyShock*> pair : _joyshocks)
	{
		if (i >= size) {
			break;
		}
		if (pair.second->finished.load()) continue;
		deviceHandleArray[i] = pair.first;
		i++;
	}
	_connectedLock.unlock_shared();
	return i; // return num actually found
}

void JslDisconnectAndDisposeAll()
{
    JslSetCallback(nullptr);
    JslSetTouchCallback(nullptr);
    JslSetConnectCallback(nullptr);
    JslSetDisconnectCallback(nullptr);
    std::vector<JoyShock*> devices;
    {
        std::unique_lock<JslMutex> lock(_connectedLock);
        for (auto& pair : _joyshocks) {
            pair.second->cancel_thread.store(true);
            devices.push_back(pair.second);
        }
        _joyshocks.clear();
        _byPath.clear();
    }
    for (auto* jc : devices) DisposeJoyShock(jc);
    // HIDAPI is shared with StreamDeck/Ajazz. Its process-global state must
    // remain alive while those modules may still own devices.
}

bool JslStillConnected(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	return GetJoyShockFromHandle(deviceId) != nullptr;
}

// get buttons as bits in the following order, using North South East West to name face buttons to avoid ambiguity between Xbox and Nintendo layouts:
// 0x00001: up
// 0x00002: down
// 0x00004: left
// 0x00008: right
// 0x00010: plus
// 0x00020: minus
// 0x00040: left stick click
// 0x00080: right stick click
// 0x00100: L
// 0x00200: R
// ZL and ZR are reported as analogue inputs (GetLeftTrigger, GetRightTrigger), because DS4 and XBox controllers use analogue triggers, but we also have them as raw buttons
// 0x00400: ZL
// 0x00800: ZR
// 0x01000: S
// 0x02000: E
// 0x04000: W
// 0x08000: N
// 0x10000: home / PS
// 0x20000: capture / touchpad-click
// 0x40000: SL
// 0x80000: SR
// if you want the whole state, this is the best way to do it
JOY_SHOCK_STATE JslGetSimpleState(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		std::lock_guard<std::mutex> stateLock(jc->state_lock);
		return jc->simple_state;
	}
	return {};
}
IMU_STATE JslGetIMUState(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->get_transformed_imu_state(jc->imu_state);
	}
	return {};
}
MOTION_STATE JslGetMotionState(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->get_motion_state();
	}
	return {};
}
TOUCH_STATE JslGetTouchState(int deviceId, bool previous)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return previous ? jc->last_touch_state : jc->touch_state;
	}
	return {};
}

bool JslGetTouchpadDimension(int deviceId, int &sizeX, int &sizeY)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	// I am assuming a single touchpad (or all touchpads are the same dimension)?
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr)
	{
		switch (jc->controller_type)
		{
		case ControllerType::s_ds4:
		case ControllerType::s_ds:
			sizeX = 1920;
			sizeY = 943;
			break;
		default:
			sizeX = 0;
			sizeY = 0;
			break;
		}
		return true;
	}
	return false;
}

int JslGetButtons(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->simple_state.buttons;
	}
	return 0;
}

// get thumbsticks
float JslGetLeftX(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->simple_state.stickLX;
	}
	return 0.0f;
}
float JslGetLeftY(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->simple_state.stickLY;
	}
	return 0.0f;
}
float JslGetRightX(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->simple_state.stickRX;
	}
	return 0.0f;
}
float JslGetRightY(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->simple_state.stickRY;
	}
	return 0.0f;
}

// get triggers. Switch controllers don't have analogue triggers, but will report 0.0 or 1.0 so they can be used in the same way as others
float JslGetLeftTrigger(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->simple_state.lTrigger;
	}
	return 0.0f;
}
float JslGetRightTrigger(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->simple_state.rTrigger;
	}
	return 0.0f;
}

// get gyro
float JslGetGyroX(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->get_transformed_imu_state(jc->imu_state).gyroX;
	}
	return 0.0f;
}
float JslGetGyroY(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->get_transformed_imu_state(jc->imu_state).gyroY;
	}
	return 0.0f;
}
float JslGetGyroZ(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->get_transformed_imu_state(jc->imu_state).gyroZ;
	}
	return 0.0f;
}

void JslGetAndFlushAccumulatedGyro(int deviceId, float& gyroX, float& gyroY, float& gyroZ)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		jc->get_and_flush_cumulative_gyro(gyroX, gyroY, gyroZ);
		return;
	}
	gyroX = gyroY = gyroZ = 0.f;
}

void JslSetGyroSpace(int deviceId, int gyroSpace)
{
	if (gyroSpace < 0) {
		gyroSpace = 0;
	}
	if (gyroSpace > 2) {
		gyroSpace = 2;
	}
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		jc->modifying_lock.lock();
		jc->gyroSpace = gyroSpace;
		jc->modifying_lock.unlock();
	}
}

// get accelerometor
float JslGetAccelX(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->imu_state.accelX;
	}
	return 0.0f;
}
float JslGetAccelY(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->imu_state.accelY;
	}
	return 0.0f;
}
float JslGetAccelZ(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->imu_state.accelZ;
	}
	return 0.0f;
}

// get touchpad
int JslGetTouchId(int deviceId, bool secondTouch)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		if (!secondTouch) {
			return jc->touch_state.t0Id;
		}
		else {
			return jc->touch_state.t1Id;
		}
	}
	return false;
}
bool JslGetTouchDown(int deviceId, bool secondTouch)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		if (!secondTouch) {
			return jc->touch_state.t0Down;
		}
		else {
			return jc->touch_state.t1Down;
		}
	}
	return false;
}

float JslGetTouchX(int deviceId, bool secondTouch)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		if (!secondTouch) {
			return jc->touch_state.t0X;
		}
		else {
			return jc->touch_state.t1X;
		}
	}
	return 0.0f;
}
float JslGetTouchY(int deviceId, bool secondTouch)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		if (!secondTouch) {
			return jc->touch_state.t0Y;
		}
		else {
			return jc->touch_state.t1Y;
		}
	}
	return 0.0f;
}

// analog parameters have different resolutions depending on device
float JslGetStickStep(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		if (jc->controller_type != ControllerType::n_switch) {
			return 1.0f / 128.0;
		}
		else {
			if (jc->left_right == 2) // right joycon has no calibration for left stick
			{
				return 1.0f / (jc->stick_cal_x_r[2] - jc->stick_cal_x_r[1]);
			}
			else {
				return 1.0f / (jc->stick_cal_x_l[2] - jc->stick_cal_x_l[1]);
			}
		}
	}
	return 0.0f;
}
float JslGetTriggerStep(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->controller_type != ControllerType::n_switch ? 1 / 256.0f : 1.0f;
	}
	return 1.0f;
}
float JslGetPollRate(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->controller_type != ControllerType::n_switch ? 250.0f : 66.6667f;
	}
	return 0.0f;
}
float JslGetTimeSinceLastUpdate(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		auto time_now = std::chrono::steady_clock::now();
		return (float)(std::chrono::duration_cast<std::chrono::microseconds>(time_now - jc->last_polled).count() / 1000000.0);
	}
	return 0.0f;
}

// calibration
void JslResetContinuousCalibration(int deviceId) {
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		jc->reset_continuous_calibration();
	}
}
void JslStartContinuousCalibration(int deviceId) {
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		jc->use_continuous_calibration = true;
		jc->cue_motion_reset = true;
	}
}
void JslPauseContinuousCalibration(int deviceId) {
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		jc->use_continuous_calibration = false;
	}
}
void JslSetAutomaticCalibration(int deviceId, bool enabled) {
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		jc->modifying_lock.lock();
		jc->motion.SetCalibrationMode(enabled ? GamepadMotionHelpers::CalibrationMode::SensorFusion | GamepadMotionHelpers::CalibrationMode::Stillness : GamepadMotionHelpers::CalibrationMode::Manual);
		jc->modifying_lock.unlock();
	}
}
void JslGetCalibrationOffset(int deviceId, float& xOffset, float& yOffset, float& zOffset) {
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		// not technically modifying, but also not a simple getter
		jc->modifying_lock.lock();
		jc->motion.GetCalibrationOffset(xOffset, yOffset, zOffset);
		jc->modifying_lock.unlock();
	}
}
void JslSetCalibrationOffset(int deviceId, float xOffset, float yOffset, float zOffset) {
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		jc->modifying_lock.lock();
		jc->motion.SetCalibrationOffset(xOffset, yOffset, zOffset, 1);
		jc->modifying_lock.unlock();
	}
}
JSL_AUTO_CALIBRATION JslGetAutoCalibrationStatus(int deviceId) {
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		JSL_AUTO_CALIBRATION calibration;

		calibration.autoCalibrationEnabled = jc->motion.GetCalibrationMode() != GamepadMotionHelpers::CalibrationMode::Manual;
		calibration.confidence = jc->motion.GetAutoCalibrationConfidence();
		calibration.isSteady = jc->motion.GetAutoCalibrationIsSteady();

		return calibration;
	}

	return {};
}

// this function will get called for each input event from each controller
void JslSetCallback(void(*callback)(int, JOY_SHOCK_STATE, JOY_SHOCK_STATE, IMU_STATE, IMU_STATE, float)) {
	// exclusive lock
	_callbackLock.lock();
	_pollCallback = callback;
	_callbackLock.unlock();
}

// this function will get called for each input event, even if touch data didn't update
void JslSetTouchCallback(void(*callback)(int, TOUCH_STATE, TOUCH_STATE, float)) {
	_callbackLock.lock();
	_pollTouchCallback = callback;
	_callbackLock.unlock();
}

// this function will get called for each device when it is newly connected
void JslSetConnectCallback(void(*callback)(int)) {
	// exclusive lock
	_callbackLock.lock();
	_connectCallback = callback;
	_callbackLock.unlock();
}

// this function will get called for each device when it is disconnected (and whether it was a timeout (true))
void JslSetDisconnectCallback(void(*callback)(int, bool)) {
	// exclusive lock
	_callbackLock.lock();
	_disconnectCallback = callback;
	_callbackLock.unlock();
}

// super-getter for reading a whole lot of state at once
JSL_SETTINGS JslGetControllerInfoAndSettings(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		JSL_SETTINGS settings;

		settings.gyroSpace = jc->gyroSpace;
		settings.playerNumber = jc->player_number;
		settings.splitType = jc->left_right;
		settings.isConnected = true;
		settings.isCalibrating = jc->use_continuous_calibration;
		settings.autoCalibrationEnabled = jc->motion.GetCalibrationMode() != GamepadMotionHelpers::CalibrationMode::Manual;

		switch (jc->controller_type)
		{
		case ControllerType::s_ds4:
			settings.controllerType = JS_TYPE_DS4;
			break;
		case ControllerType::s_ds:
			settings.controllerType = JS_TYPE_DS;
			break;
		default:
		case ControllerType::n_switch:
			settings.controllerType = jc->left_right;
			settings.colour = jc->body_colour;
			break;
		}

		if (jc->controller_type != ControllerType::n_switch)
		{
			// get led colour
			settings.colour = (int)(jc->led_b) | ((int)(jc->led_g) << 8) | ((int)(jc->led_r) << 16);
		}

		return settings;
	}
	return {};
}

// what split type of controller is this?
int JslGetControllerType(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		switch (jc->controller_type)
		{
		case ControllerType::s_ds4:
			return JS_TYPE_DS4;
		case ControllerType::s_ds:
			return JS_TYPE_DS;
		default:
		case ControllerType::n_switch:
			return jc->left_right;
		}
	}
	return 0;
}
// what split type of controller is this?
int JslGetControllerSplitType(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->left_right;
	}
	return 0;
}
// what colour is the controller (not all controllers support this; those that don't will report white)
int JslGetControllerColour(int deviceId)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	// this just reports body colour. Switch controllers also give buttons colour, and in Pro's case, left and right grips
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr) {
		return jc->body_colour;
	}
	return 0xFFFFFF;
}
// set controller light colour (not all controllers have a light whose colour can be set, but that just means nothing will be done when this is called -- no harm)
void JslSetLightColour(int deviceId, int colour)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr && jc->controller_type == ControllerType::s_ds4) {
		jc->modifying_lock.lock();
		jc->led_r = (colour >> 16) & 0xff;
		jc->led_g = (colour >> 8) & 0xff;
		jc->led_b = colour & 0xff;
		jc->set_ds4_rumble_light(
			jc->small_rumble,
			jc->big_rumble,
			jc->led_r,
			jc->led_g,
			jc->led_b);
		jc->modifying_lock.unlock();
	}
	else if(jc != nullptr && jc->controller_type == ControllerType::s_ds) {
		jc->modifying_lock.lock();
        jc->led_r = (colour >> 16) & 0xff;
        jc->led_g = (colour >> 8) & 0xff;
        jc->led_b = colour & 0xff;
        jc->set_ds5_rumble_light(
                jc->small_rumble,
                jc->big_rumble,
                jc->led_r,
                jc->led_g,
                jc->led_b,
                jc->player_number);
		jc->modifying_lock.unlock();
	}
}
// set controller rumble
void JslSetRumble(int deviceId, int smallRumble, int bigRumble)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr && jc->controller_type == ControllerType::s_ds4) {
		jc->modifying_lock.lock();
		jc->small_rumble = smallRumble;
		jc->big_rumble = bigRumble;
		jc->set_ds4_rumble_light(
			jc->small_rumble,
			jc->big_rumble,
			jc->led_r,
			jc->led_g,
			jc->led_b);
		jc->modifying_lock.unlock();
	}
    else if (jc != nullptr && jc->controller_type == ControllerType::s_ds) {
		jc->modifying_lock.lock();
        jc->small_rumble = smallRumble;
        jc->big_rumble = bigRumble;
        jc->set_ds5_rumble_light(
                jc->small_rumble,
                jc->big_rumble,
                jc->led_r,
                jc->led_g,
                jc->led_b,
                jc->player_number);
		jc->modifying_lock.unlock();
    }
}
// set controller player number indicator (not all controllers have a number indicator which can be set, but that just means nothing will be done when this is called -- no harm)
void JslSetPlayerNumber(int deviceId, int number)
{
	std::shared_lock<JslMutex> lock(_connectedLock);
	JoyShock* jc = GetJoyShockFromHandle(deviceId);
	if (jc != nullptr && jc->controller_type == ControllerType::n_switch) {
		jc->modifying_lock.lock();
		jc->player_number = number;
		unsigned char buf[64];
		memset(buf, 0x00, 0x40);
		buf[0] = (unsigned char)number;
		jc->send_subcommand(0x01, 0x30, buf, 1);
		jc->modifying_lock.unlock();
	}
	else if(jc != nullptr && jc->controller_type == ControllerType::s_ds) {
		jc->modifying_lock.lock();
	    jc->player_number = number;
        jc->set_ds5_rumble_light(
                jc->small_rumble,
                jc->big_rumble,
                jc->led_r,
                jc->led_g,
                jc->led_b,
                jc->player_number);
		jc->modifying_lock.unlock();
	}
}

void JslSetCancellation(bool cancelled) { chataigneJslStopping.store(cancelled); }
