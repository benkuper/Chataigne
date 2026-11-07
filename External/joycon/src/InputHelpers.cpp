#include "JoyShockLibrary.h"
#include "JoyShock.cpp"

#include <cmath>

bool handle_input(JoyShock *jc, uint8_t *packet, int len, bool &hasIMU) {
	hasIMU = true;
    if (!packet || len < 1) return false;
    if (jc->controller_type == ControllerType::n_switch &&
        (packet[0] != 0x30 || len < 49)) return false;
    if (jc->controller_type == ControllerType::s_ds4 && len < (jc->is_usb ? 64 : 78)) return false;
    if (jc->controller_type == ControllerType::s_ds && len < 64) return false;
	if (packet[0] == 0) return false; // ignore non-responses
									  // remember last input

	//printf("%d: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
	//	jc->left_right,
	//	packet[0], packet[1], packet[2], packet[3], packet[4], packet[5], packet[6], packet[7], packet[8], packet[9],
	//	packet[10], packet[11], packet[12], packet[13], packet[14], packet[15], packet[16], packet[17], packet[18], packet[19], packet[20]);

	jc->last_simple_state = jc->simple_state;
	jc->simple_state.buttons = 0;
	jc->last_imu_state = jc->imu_state;
	IMU_STATE imu_state;
	// delta time
	auto time_now = std::chrono::steady_clock::now();
	jc->delta_time = (float)(std::chrono::duration_cast<std::chrono::microseconds>(time_now - jc->last_polled).count() / 1000000.0);
	jc->last_polled = time_now;
	if (jc->cue_motion_reset)
	{
		//printf("RESET motion\n");
		jc->cue_motion_reset = false;
		jc->motion.Reset();
	}
	if (jc->motion.GetCalibrationMode() == GamepadMotionHelpers::CalibrationMode::Manual)
	{
		if (jc->use_continuous_calibration)
		{
			jc->motion.StartContinuousCalibration();
		}
		else
		{
			jc->motion.PauseContinuousCalibration();
		}
	}
	// ds4
	if (jc->controller_type == ControllerType::s_ds4) {
		int indexOffset = 0;
		bool isValid = true;
		if (!jc->is_usb) {
			isValid = packet[0] == 0x11;
			indexOffset = 2;
		}
		else {
			isValid = packet[0] == 0x01;
			if (isValid && (packet[31] & 0x04) == 0x04)
				return false; // ignore packets from Dongle with no connected controller
		}
		if (isValid) {
			// Gyroscope:
			// Gyroscope data is relative (degrees/s)
			int16_t gyroSampleX = uint16_to_int16(packet[indexOffset+13] | (packet[indexOffset+14] << 8) & 0xFF00);
			int16_t gyroSampleY = uint16_to_int16(packet[indexOffset+15] | (packet[indexOffset+16] << 8) & 0xFF00);
			int16_t gyroSampleZ = uint16_to_int16(packet[indexOffset+17] | (packet[indexOffset+18] << 8) & 0xFF00);
			int16_t accelSampleX = uint16_to_int16(packet[indexOffset+19] | (packet[indexOffset+20] << 8) & 0xFF00);
			int16_t accelSampleY = uint16_to_int16(packet[indexOffset+21] | (packet[indexOffset+22] << 8) & 0xFF00);
			int16_t accelSampleZ = uint16_to_int16(packet[indexOffset+23] | (packet[indexOffset+24] << 8) & 0xFF00);

			if ((gyroSampleX | gyroSampleY | gyroSampleZ | accelSampleX | accelSampleY | accelSampleZ) == 0)
			{
				// all zero?
				hasIMU = false;
			}

			// convert to real units
			imu_state.gyroX = (float)(gyroSampleX) * (2000.0f / 32767.0f);
			imu_state.gyroY = (float)(gyroSampleY) * (2000.0f / 32767.0f);
			imu_state.gyroZ = (float)(gyroSampleZ) * (2000.0f / 32767.0f);

			imu_state.accelX = (float)(accelSampleX) / 8192.0f;
			imu_state.accelY = (float)(accelSampleY) / 8192.0f;
			imu_state.accelZ = (float)(accelSampleZ) / 8192.0f;

			//printf("DS4 accel: %.4f, %.4f, %.4f\n", imu_state.accelX, imu_state.accelY, imu_state.accelZ);

			//printf("%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d\n",
			//	jc->gyro.yaw, jc->gyro.pitch, jc->gyro.roll, jc->accel.x, jc->accel.y, jc->accel.z, universal_counter++);

			// Touchpad:
			jc->last_touch_state = jc->touch_state;

			jc->touch_state.t0Id = (int)(packet[indexOffset+35] & 0x7F);
			jc->touch_state.t1Id = (int)(packet[indexOffset+39] & 0x7F);
			jc->touch_state.t0Down = (packet[indexOffset+35] & 0x80) == 0;
			jc->touch_state.t1Down = (packet[indexOffset+39] & 0x80) == 0;

			jc->touch_state.t0X = (packet[indexOffset+36] | (packet[indexOffset+37] & 0x0F) << 8) / 1920.0f;
			jc->touch_state.t0Y = ((packet[indexOffset+37] & 0xF0) >> 4 | packet[indexOffset+38] << 4) / 943.0f;
			jc->touch_state.t1X = (packet[indexOffset+40] | (packet[indexOffset+41] & 0x0F) << 8) / 1920.0f;
			jc->touch_state.t1Y = ((packet[indexOffset+41] & 0xF0) >> 4 | packet[indexOffset+42] << 4) / 943.0f;

			//printf("DS4 touch: %d, %d, %d, %d, %.4f, %.4f, %.4f, %.4f\n",
			//	jc->touch_state.t0Id, jc->touch_state.t1Id, jc->touch_state.t0Down, jc->touch_state.t1Down,
			//	jc->touch_state.t0X, jc->touch_state.t0Y, jc->touch_state.t1X, jc->touch_state.t1Y);

			// DS4 dpad is a hat...  0x08 is released, 0=N, 1=NE, 2=E, 3=SE, 4=S, 5=SW, 6=W, 7=NW
			// http://eleccelerator.com/wiki/index.php?title=DualShock_4
			uint8_t hat = packet[indexOffset+5] & 0x0f;

			if ((hat > 2) & (hat < 6)) jc->simple_state.buttons |= JSMASK_DOWN; // down = SE | S | SW
			if ((hat == 7) | (hat < 2)) jc->simple_state.buttons |= JSMASK_UP; // up = N | NE | NW
			if ((hat > 0) & (hat < 4)) jc->simple_state.buttons |= JSMASK_RIGHT; // right = NE | E | SE
			if ((hat > 4) & (hat < 8)) jc->simple_state.buttons |= JSMASK_LEFT; // left = SW | W | NW

			jc->simple_state.buttons |= ((int)(packet[indexOffset+5] >> 4) << JSOFFSET_W) & JSMASK_W;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+5] >> 7) << JSOFFSET_N) & JSMASK_N;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+5] >> 5) << JSOFFSET_S) & JSMASK_S;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+5] >> 6) << JSOFFSET_E) & JSMASK_E;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+6] >> 6) << JSOFFSET_LCLICK) & JSMASK_LCLICK;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+6] >> 7) << JSOFFSET_RCLICK) & JSMASK_RCLICK;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+6] >> 5) << JSOFFSET_OPTIONS) & JSMASK_OPTIONS;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+6] >> 4) << JSOFFSET_SHARE) & JSMASK_SHARE;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+6] >> 1) << JSOFFSET_R) & JSMASK_R;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+6]) << JSOFFSET_L) & JSMASK_L;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+7]) << JSOFFSET_PS) & JSMASK_PS;
			jc->simple_state.buttons |= ((int)(packet[indexOffset+7] >> 1) << JSOFFSET_TOUCHPAD_CLICK) & JSMASK_TOUCHPAD_CLICK;
			//jc->btns.zr = (packet[indexOffset+6] >> 3) & 1;
			//jc->btns.zl = (packet[indexOffset+6] >> 2) & 1;
			jc->simple_state.rTrigger = packet[indexOffset+9] / 255.0f;
			jc->simple_state.lTrigger = packet[indexOffset+8] / 255.0f;

			if (jc->simple_state.rTrigger > 0.0) jc->simple_state.buttons |= JSMASK_ZR;
			if (jc->simple_state.lTrigger > 0.0) jc->simple_state.buttons |= JSMASK_ZL;

			uint16_t stick_x = packet[indexOffset+1];
			uint16_t stick_y = packet[indexOffset+2];
			stick_y = 255 - stick_y;

			uint16_t stick2_x = packet[indexOffset+3];
			uint16_t stick2_y = packet[indexOffset+4];
			stick2_y = 255 - stick2_y;

			jc->simple_state.stickLX = (std::fmin)(1.0f, (stick_x - 127.0f) / 127.0f);
			jc->simple_state.stickLY = (std::fmin)(1.0f, (stick_y - 127.0f) / 127.0f);
			jc->simple_state.stickRX = (std::fmin)(1.0f, (stick2_x - 127.0f) / 127.0f);
			jc->simple_state.stickRY = (std::fmin)(1.0f, (stick2_y - 127.0f) / 127.0f);

			jc->modifying_lock.lock();
			jc->push_sensor_samples(imu_state.gyroX, imu_state.gyroY, imu_state.gyroZ,
					imu_state.accelX, imu_state.accelY, imu_state.accelZ, jc->delta_time);

			jc->get_calibrated_gyro(imu_state.gyroX, imu_state.gyroY, imu_state.gyroZ);
			jc->modifying_lock.unlock();

			jc->imu_state = imu_state;
		}

		//printf("Buttons: %d LX: %.5f LY: %.5f RX: %.5f RY: %.5f GX: %.4f GY: %.4f GZ: %.4f\n", \
				//	jc->simple_state.buttons, (jc->simple_state.stickLX + 1), (jc->simple_state.stickLY + 1), (jc->simple_state.stickRX + 1), (jc->simple_state.stickRY + 1), jc->imu_state.gyroX, jc->imu_state.gyroY, jc->imu_state.gyroZ);


		return true;
	}

	if (jc->controller_type == ControllerType::s_ds) {
        //printf("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
        //	packet[0], packet[1], packet[2], packet[3], packet[4], packet[5], packet[6], packet[7], packet[8], packet[9],
        //	packet[10], packet[11], packet[12], packet[13], packet[14], packet[15], packet[16], packet[17], packet[18], packet[19], packet[20],
        //	packet[21], packet[22], packet[23], packet[24], packet[25], packet[26], packet[27], packet[28], packet[29], packet[30],
        //	packet[31], packet[32], packet[33], packet[34], packet[35], packet[36], packet[37], packet[38], packet[39], packet[40],
        //	packet[41], packet[42], packet[43], packet[44], packet[45], packet[46], packet[47], packet[48], packet[49], packet[50]);
        int indexOffset = 1;
        if(!jc->is_usb) {
            indexOffset = 2;
        }

        // Gyroscope:
        // Gyroscope data is relative (degrees/s)
        int16_t gyroSampleX = uint16_to_int16(packet[indexOffset + 15] | (packet[indexOffset + 16] << 8) & 0xFF00);
        int16_t gyroSampleY = uint16_to_int16(packet[indexOffset + 17] | (packet[indexOffset + 18] << 8) & 0xFF00);
        int16_t gyroSampleZ = uint16_to_int16(packet[indexOffset + 19] | (packet[indexOffset + 20] << 8) & 0xFF00);
        int16_t accelSampleX = uint16_to_int16(packet[indexOffset + 21] | (packet[indexOffset + 22] << 8) & 0xFF00);
        int16_t accelSampleY = uint16_to_int16(packet[indexOffset + 23] | (packet[indexOffset + 24] << 8) & 0xFF00);
        int16_t accelSampleZ = uint16_to_int16(packet[indexOffset + 25] | (packet[indexOffset + 26] << 8) & 0xFF00);

        if ((gyroSampleX | gyroSampleY | gyroSampleZ | accelSampleX | accelSampleY | accelSampleZ) == 0) {
            // all zero?
            hasIMU = false;
        }

        // convert to real units
        imu_state.gyroX = (float) (gyroSampleX) * (2000.0f / 32767.0f);
        imu_state.gyroY = (float) (gyroSampleY) * (2000.0f / 32767.0f);
        imu_state.gyroZ = (float) (gyroSampleZ) * (2000.0f / 32767.0f);

        imu_state.accelX = (float) (accelSampleX) / 8192.0f;
        imu_state.accelY = (float) (accelSampleY) / 8192.0f;
        imu_state.accelZ = (float) (accelSampleZ) / 8192.0f;

        //printf("DS accel: %.4f, %.4f, %.4f\n", imu_state.accelX, imu_state.accelY, imu_state.accelZ);

        //printf("%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%d\n",
        //	jc->gyro.yaw, jc->gyro.pitch, jc->gyro.roll, jc->accel.x, jc->accel.y, jc->accel.z, universal_counter++);

        // Touchpad:
        jc->last_touch_state = jc->touch_state;

        jc->touch_state.t0Id = (int) (packet[indexOffset + 32] & 0x7F);
        jc->touch_state.t1Id = (int) (packet[indexOffset + 36] & 0x7F);
        jc->touch_state.t0Down = (packet[indexOffset + 32] & 0x80) == 0;
        jc->touch_state.t1Down = (packet[indexOffset + 36] & 0x80) == 0;

        jc->touch_state.t0X = (packet[indexOffset + 33] | (packet[indexOffset + 34] & 0x0F) << 8) / 1920.0f;
        jc->touch_state.t0Y = ((packet[indexOffset + 34] & 0xF0) >> 4 | packet[indexOffset + 35] << 4) / 943.0f;
        jc->touch_state.t1X = (packet[indexOffset + 37] | (packet[indexOffset + 38] & 0x0F) << 8) / 1920.0f;
        jc->touch_state.t1Y = ((packet[indexOffset + 38] & 0xF0) >> 4 | packet[indexOffset + 39] << 4) / 943.0f;

        //printf("DS touch: %d, %d, %d, %d, %.4f, %.4f, %.4f, %.4f\n",
        //	jc->touch_state.t0Id, jc->touch_state.t1Id, jc->touch_state.t0Down, jc->touch_state.t1Down,
        //	jc->touch_state.t0X, jc->touch_state.t0Y, jc->touch_state.t1X, jc->touch_state.t1Y);

        // DS dpad is a hat...  0x08 is released, 0=N, 1=NE, 2=E, 3=SE, 4=S, 5=SW, 6=W, 7=NW
        // http://eleccelerator.com/wiki/index.php?title=DualShock_4
        uint8_t hat = packet[indexOffset + 7] & 0x0f;

        if ((hat > 2) & (hat < 6)) jc->simple_state.buttons |= JSMASK_DOWN; // down = SE | S | SW
        if ((hat == 7) | (hat < 2)) jc->simple_state.buttons |= JSMASK_UP; // up = N | NE | NW
        if ((hat > 0) & (hat < 4)) jc->simple_state.buttons |= JSMASK_RIGHT; // right = NE | E | SE
        if ((hat > 4) & (hat < 8)) jc->simple_state.buttons |= JSMASK_LEFT; // left = SW | W | NW

        jc->simple_state.buttons |= ((int) (packet[indexOffset + 7] >> 4) << JSOFFSET_W) & JSMASK_W;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 7] >> 7) << JSOFFSET_N) & JSMASK_N;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 7] >> 5) << JSOFFSET_S) & JSMASK_S;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 7] >> 6) << JSOFFSET_E) & JSMASK_E;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 8] >> 6) << JSOFFSET_LCLICK) & JSMASK_LCLICK;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 8] >> 7) << JSOFFSET_RCLICK) & JSMASK_RCLICK;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 8] >> 5) << JSOFFSET_OPTIONS) & JSMASK_OPTIONS;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 8] >> 4) << JSOFFSET_SHARE) & JSMASK_SHARE;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 8] >> 1) << JSOFFSET_R) & JSMASK_R;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 8]) << JSOFFSET_L) & JSMASK_L;
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 9]) << JSOFFSET_PS) & JSMASK_PS;
        // The DS5 has a mute button that is normally ignored on PC. We can use this.
        jc->simple_state.buttons |= ((int) (packet[indexOffset + 9] >> 2) << JSOFFSET_MIC) & JSMASK_MIC;
        jc->simple_state.buttons |=
                ((int) (packet[indexOffset + 9] >> 1) << JSOFFSET_TOUCHPAD_CLICK) & JSMASK_TOUCHPAD_CLICK;
        //jc->btns.zr = (packet[indexOffset+6] >> 3) & 1;
        //jc->btns.zl = (packet[indexOffset+6] >> 2) & 1;
        jc->simple_state.rTrigger = packet[indexOffset + 5] / 255.0f;
        jc->simple_state.lTrigger = packet[indexOffset + 4] / 255.0f;

        if (jc->simple_state.rTrigger > 0.0) jc->simple_state.buttons |= JSMASK_ZR;
        if (jc->simple_state.lTrigger > 0.0) jc->simple_state.buttons |= JSMASK_ZL;

            uint16_t stick_x = packet[indexOffset + 0];
            uint16_t stick_y = packet[indexOffset + 1];

		stick_y = 255 - stick_y;

		uint16_t stick2_x = packet[indexOffset + 2];
		uint16_t stick2_y = packet[indexOffset + 3];
		stick2_y = 255 - stick2_y;

		jc->simple_state.stickLX = (std::fmin)(1.0f, (stick_x - 127.0f) / 127.0f);
		jc->simple_state.stickLY = (std::fmin)(1.0f, (stick_y - 127.0f) / 127.0f);
		jc->simple_state.stickRX = (std::fmin)(1.0f, (stick2_x - 127.0f) / 127.0f);
		jc->simple_state.stickRY = (std::fmin)(1.0f, (stick2_y - 127.0f) / 127.0f);

		jc->modifying_lock.lock();
		jc->push_sensor_samples(imu_state.gyroX, imu_state.gyroY, imu_state.gyroZ,
				imu_state.accelX, imu_state.accelY, imu_state.accelZ, jc->delta_time);

		jc->get_calibrated_gyro(imu_state.gyroX, imu_state.gyroY, imu_state.gyroZ);
		jc->modifying_lock.unlock();

		jc->imu_state = imu_state;

		return true;
	}

	// most of this JoyCon and Pro Controller stuff is adapted from MFosse's Joycon driver.

	// bluetooth button pressed packet:
	if (packet[0] == 0x3F) {

		//uint16_t old_buttons = jc->buttons;
		//int8_t old_dstick = jc->dstick;

		jc->dstick = packet[3];
		// todo: get button states here aswell:
	}

	int buttons_pressed = 0;

	// input update packet:
	// 0x21 is just buttons, 0x30 includes gyro, 0x31 includes NFC (large packet size)
	if (packet[0] == 0x21 || packet[0] == 0x30 || packet[0] == 0x31) {

		// offset for usb or bluetooth data:
		/*int offset = settings.usingBluetooth ? 0 : 10;*/
		int offset = 0;
		//int offset = !jc->is_usb ? 0 : 10;

		uint8_t *btn_data = packet + offset + 3;

		// get button states:
		{
			uint16_t states = 0;
			uint16_t states2 = 0;

			// Left JoyCon:
			if (jc->left_right == 1) {
				states = (btn_data[1] << 8) | (btn_data[2] & 0xFF);
				// Right JoyCon:
			}
			else if (jc->left_right == 2) {
				states = (btn_data[1] << 8) | (btn_data[0] & 0xFF);
				// Pro Controller:
			}
			else if (jc->left_right == 3) {
				states = (btn_data[1] << 8) | (btn_data[2] & 0xFF);
				states2 = (btn_data[1] << 8) | (btn_data[0] & 0xFF);
			}

			buttons_pressed = states;
			// Pro Controller:
			if (jc->left_right == 3) {
				buttons_pressed |= states2 << 16;

				// fix some non-sense the Pro Controller does
				// clear nth bit
				//num &= ~(1UL << n);
				buttons_pressed &= ~(1L << 9);
				buttons_pressed &= ~(1L << 12);
				buttons_pressed &= ~(1L << 14);

				buttons_pressed &= ~(1UL << (8 + 16));
				buttons_pressed &= ~(1UL << (11 + 16));
				buttons_pressed &= ~(1UL << (13 + 16));
			}
			else if (jc->left_right == 2) {
				buttons_pressed = buttons_pressed << 16;
			}
		}

		// get stick data:
		uint8_t *stick_data = packet + offset;
		if (jc->left_right == 1) {
			stick_data += 6;
		}
		else if (jc->left_right == 2) {
			stick_data += 9;
		}

		uint16_t stick_x = stick_data[0] | ((stick_data[1] & 0xF) << 8);
		uint16_t stick_y = (stick_data[1] >> 4) | (stick_data[2] << 4);

		// use calibration data:
		if (jc->left_right == 1) {
			jc->CalcAnalogStick2(jc->simple_state.stickLX, jc->simple_state.stickLY,
				stick_x,
				stick_y,
				jc->stick_cal_x_l,
				jc->stick_cal_y_l);
		}
		else if (jc->left_right == 2) {
			jc->CalcAnalogStick2(jc->simple_state.stickRX, jc->simple_state.stickRY,
				stick_x,
				stick_y,
				jc->stick_cal_x_r,
				jc->stick_cal_y_r);
		}
		else if (jc->left_right == 3) {
			// pro controller
			stick_data += 6;
			//printf("%d, %d\n",
			//	jc->stick_cal_x_l,
			//	jc->stick_cal_y_l);
			uint16_t stick_x = stick_data[0] | ((stick_data[1] & 0xF) << 8);
			uint16_t stick_y = (stick_data[1] >> 4) | (stick_data[2] << 4);
			jc->CalcAnalogStick2(jc->simple_state.stickLX, jc->simple_state.stickLY,
				stick_x,
				stick_y,
				jc->stick_cal_x_l,
				jc->stick_cal_y_l);
			stick_data += 3;
			uint16_t stick_x2 = stick_data[0] | ((stick_data[1] & 0xF) << 8);
			uint16_t stick_y2 = (stick_data[1] >> 4) | (stick_data[2] << 4);
			jc->CalcAnalogStick2(jc->simple_state.stickRX, jc->simple_state.stickRY,
				stick_x2,
				stick_y2,
				jc->stick_cal_x_r,
				jc->stick_cal_y_r);
		}

		jc->battery = (stick_data[1] & 0xF0) >> 4;
		//printf("JoyCon battery: %d\n", jc->battery);

		// Accelerometer:
		// Accelerometer data is absolute
		{
			// get accelerometer X:
			float accelSampleZ = (float)uint16_to_int16(packet[13] | (packet[14] << 8) & 0xFF00) * jc->acc_cal_coeff[0];
			float accelSampleX = (float)uint16_to_int16(packet[15] | (packet[16] << 8) & 0xFF00) * jc->acc_cal_coeff[1];
			float accelSampleY = (float)uint16_to_int16(packet[17] | (packet[18] << 8) & 0xFF00) * jc->acc_cal_coeff[2];
			float gyroSampleX = (float)uint16_to_int16(packet[19] | (packet[20] << 8) & 0xFF00) * jc->gyro_cal_coeff[0];
			float gyroSampleY = (float)uint16_to_int16(packet[21] | (packet[22] << 8) & 0xFF00) * jc->gyro_cal_coeff[1];
			float gyroSampleZ = (float)uint16_to_int16(packet[23] | (packet[24] << 8) & 0xFF00) * jc->gyro_cal_coeff[2];

			if (gyroSampleX == 0.f && gyroSampleY == 0.f && gyroSampleZ == 0.f && accelSampleX == 0.f && accelSampleY == 0.f && accelSampleZ == 0.f)
			{
				// all zero?
				hasIMU = false;
			}

			//jc->push_sensor_samples(accelSampleX, accelSampleY, accelSampleZ, gyroSampleX, gyroSampleY, gyroSampleZ);
			float accelX = accelSampleX;
			float accelY = accelSampleY;
			float accelZ = accelSampleZ;
			float totalGyroX = gyroSampleX - jc->sensor_cal[1][0];
			float totalGyroY = gyroSampleY - jc->sensor_cal[1][1];
			float totalGyroZ = gyroSampleZ - jc->sensor_cal[1][2];
			// each packet actually has 3 samples worth of data, so collect sample 2
			accelSampleZ = (float)uint16_to_int16(packet[25] | (packet[26] << 8) & 0xFF00) * jc->acc_cal_coeff[0];
			accelSampleX = (float)uint16_to_int16(packet[27] | (packet[28] << 8) & 0xFF00) * jc->acc_cal_coeff[1];
			accelSampleY = (float)uint16_to_int16(packet[29] | (packet[30] << 8) & 0xFF00) * jc->acc_cal_coeff[2];
			gyroSampleX = (float)uint16_to_int16(packet[31] | (packet[32] << 8) & 0xFF00) * jc->gyro_cal_coeff[0];
			gyroSampleY = (float)uint16_to_int16(packet[33] | (packet[34] << 8) & 0xFF00) * jc->gyro_cal_coeff[1];
			gyroSampleZ = (float)uint16_to_int16(packet[35] | (packet[36] << 8) & 0xFF00) * jc->gyro_cal_coeff[2];
			//jc->push_sensor_samples(accelSampleX, accelSampleY, accelSampleZ, gyroSampleX, gyroSampleY, gyroSampleZ);
			accelX += accelSampleX;
			accelY += accelSampleY;
			accelZ += accelSampleZ;
			totalGyroX += gyroSampleX - jc->sensor_cal[1][0];
			totalGyroY += gyroSampleY - jc->sensor_cal[1][1];
			totalGyroZ += gyroSampleZ - jc->sensor_cal[1][2];
			// ... and sample 3
			accelSampleZ = (float)uint16_to_int16(packet[37] | (packet[38] << 8) & 0xFF00) * jc->acc_cal_coeff[0];
			accelSampleX = (float)uint16_to_int16(packet[39] | (packet[40] << 8) & 0xFF00) * jc->acc_cal_coeff[1];
			accelSampleY = (float)uint16_to_int16(packet[41] | (packet[42] << 8) & 0xFF00) * jc->acc_cal_coeff[2];
			gyroSampleX = (float)uint16_to_int16(packet[43] | (packet[44] << 8) & 0xFF00) * jc->gyro_cal_coeff[0];
			gyroSampleY = (float)uint16_to_int16(packet[45] | (packet[46] << 8) & 0xFF00) * jc->gyro_cal_coeff[1];
			gyroSampleZ = (float)uint16_to_int16(packet[47] | (packet[48] << 8) & 0xFF00) * jc->gyro_cal_coeff[2];
			//jc->push_sensor_samples(accelSampleX, accelSampleY, accelSampleZ, gyroSampleX, gyroSampleY, gyroSampleZ);
			accelX += accelSampleX;
			accelY += accelSampleY;
			accelZ += accelSampleZ;
			totalGyroX += gyroSampleX - jc->sensor_cal[1][0];
			totalGyroY += gyroSampleY - jc->sensor_cal[1][1];
			totalGyroZ += gyroSampleZ - jc->sensor_cal[1][2];
			// average the 3 samples
			accelX /= 3;
			accelY /= 3;
			accelZ /= 3;
			totalGyroX /= 3;
			totalGyroY /= 3;
			totalGyroZ /= 3;
			imu_state.accelX = -accelX;
			imu_state.accelY = accelY;
			imu_state.accelZ = -accelZ;
			imu_state.gyroX = -totalGyroY;
			imu_state.gyroY = totalGyroZ;
			imu_state.gyroZ = totalGyroX;

			//printf("Switch accel: %.4f, %.4f, %.4f\n", imu_state.accelX, imu_state.accelY, imu_state.accelZ);
		}

	}

	// handle buttons
	{
		// left:
		if (jc->left_right == 1) {
			jc->simple_state.buttons |= ((buttons_pressed >> 1) << JSOFFSET_UP) & JSMASK_UP;
			jc->simple_state.buttons |= ((buttons_pressed) << JSOFFSET_DOWN) & JSMASK_DOWN;
			jc->simple_state.buttons |= ((buttons_pressed >> 3) << JSOFFSET_LEFT) & JSMASK_LEFT;
			jc->simple_state.buttons |= ((buttons_pressed >> 2) << JSOFFSET_RIGHT) & JSMASK_RIGHT;
			jc->simple_state.buttons |= ((buttons_pressed >> 11) << JSOFFSET_LCLICK) & JSMASK_LCLICK;
			jc->simple_state.buttons |= ((buttons_pressed >> 8) << JSOFFSET_MINUS) & JSMASK_MINUS;
			jc->simple_state.buttons |= ((buttons_pressed >> 6) << JSOFFSET_L) & JSMASK_L;
			jc->simple_state.buttons |= ((buttons_pressed >> 13) << JSOFFSET_CAPTURE) & JSMASK_CAPTURE;
			jc->simple_state.lTrigger = (float)((buttons_pressed >> 7) & 1);
			jc->simple_state.buttons |= ((int)(jc->simple_state.lTrigger) << JSOFFSET_ZL) & JSMASK_ZL;
			jc->simple_state.buttons |= ((buttons_pressed >> 5) << JSOFFSET_SL) & JSMASK_SL;
			jc->simple_state.buttons |= ((buttons_pressed >> 4) << JSOFFSET_SR) & JSMASK_SR;

			// just need to negate gyroZ
			imu_state.gyroZ = -imu_state.gyroZ;
		}

		// right:
		if (jc->left_right == 2) {
			jc->simple_state.buttons |= ((buttons_pressed >> 16) << JSOFFSET_W) & JSMASK_W;
			jc->simple_state.buttons |= ((buttons_pressed >> 17) << JSOFFSET_N) & JSMASK_N;
			jc->simple_state.buttons |= ((buttons_pressed >> 18) << JSOFFSET_S) & JSMASK_S;
			jc->simple_state.buttons |= ((buttons_pressed >> 19) << JSOFFSET_E) & JSMASK_E;
			jc->simple_state.buttons |= ((buttons_pressed >> 26) << JSOFFSET_RCLICK) & JSMASK_RCLICK;
			jc->simple_state.buttons |= ((buttons_pressed >> 25) << JSOFFSET_PLUS) & JSMASK_PLUS;
			jc->simple_state.buttons |= ((buttons_pressed >> 22) << JSOFFSET_R) & JSMASK_R;
			jc->simple_state.buttons |= ((buttons_pressed >> 28) << JSOFFSET_HOME) & JSMASK_HOME;
			jc->simple_state.rTrigger = (float)((buttons_pressed >> 23) & 1);
			jc->simple_state.buttons |= ((int)(jc->simple_state.rTrigger) << JSOFFSET_ZR) & JSMASK_ZR;
			jc->simple_state.buttons |= ((buttons_pressed >> 21) << JSOFFSET_SL) & JSMASK_SL;
			jc->simple_state.buttons |= ((buttons_pressed >> 20) << JSOFFSET_SR) & JSMASK_SR;

			// for some reason we need to negate x and y, and z on the right joycon
			imu_state.gyroX = -imu_state.gyroX;
			imu_state.gyroY = -imu_state.gyroY;
			imu_state.gyroZ = -imu_state.gyroZ;

			imu_state.accelX = -imu_state.accelX;
			imu_state.accelY = -imu_state.accelY;

		}

		// pro controller:
		if (jc->left_right == 3) {
			jc->simple_state.buttons |= ((buttons_pressed >> 1) << JSOFFSET_UP) & JSMASK_UP;
			jc->simple_state.buttons |= ((buttons_pressed) << JSOFFSET_DOWN) & JSMASK_DOWN;
			jc->simple_state.buttons |= ((buttons_pressed >> 3) << JSOFFSET_LEFT) & JSMASK_LEFT;
			jc->simple_state.buttons |= ((buttons_pressed >> 2) << JSOFFSET_RIGHT) & JSMASK_RIGHT;
			jc->simple_state.buttons |= ((buttons_pressed >> 16) << JSOFFSET_W) & JSMASK_W;
			jc->simple_state.buttons |= ((buttons_pressed >> 17) << JSOFFSET_N) & JSMASK_N;
			jc->simple_state.buttons |= ((buttons_pressed >> 18) << JSOFFSET_S) & JSMASK_S;
			jc->simple_state.buttons |= ((buttons_pressed >> 19) << JSOFFSET_E) & JSMASK_E;
			jc->simple_state.buttons |= ((buttons_pressed >> 11) << JSOFFSET_LCLICK) & JSMASK_LCLICK;
			jc->simple_state.buttons |= ((buttons_pressed >> 26) << JSOFFSET_RCLICK) & JSMASK_RCLICK;
			jc->simple_state.buttons |= ((buttons_pressed >> 25) << JSOFFSET_PLUS) & JSMASK_PLUS;
			jc->simple_state.buttons |= ((buttons_pressed >> 8) << JSOFFSET_MINUS) & JSMASK_MINUS;
			jc->simple_state.buttons |= ((buttons_pressed >> 22) << JSOFFSET_R) & JSMASK_R;
			jc->simple_state.buttons |= ((buttons_pressed >> 6) << JSOFFSET_L) & JSMASK_L;
			jc->simple_state.buttons |= ((buttons_pressed >> 28) << JSOFFSET_HOME) & JSMASK_HOME;
			jc->simple_state.buttons |= ((buttons_pressed >> 13) << JSOFFSET_CAPTURE) & JSMASK_CAPTURE;
			jc->simple_state.rTrigger = (float)((buttons_pressed >> 23) & 1);
			jc->simple_state.lTrigger = (float)((buttons_pressed >> 7) & 1);
			jc->simple_state.buttons |= ((int)(jc->simple_state.lTrigger) << JSOFFSET_ZL) & JSMASK_ZL;
			jc->simple_state.buttons |= ((int)(jc->simple_state.rTrigger) << JSOFFSET_ZR) & JSMASK_ZR;

			// just need to negate gyroZ
			imu_state.gyroZ = -imu_state.gyroZ;
		}
	}

	jc->modifying_lock.lock();
	jc->push_sensor_samples(imu_state.gyroX, imu_state.gyroY, imu_state.gyroZ,
		imu_state.accelX, imu_state.accelY, imu_state.accelZ, jc->delta_time);

	jc->get_calibrated_gyro(imu_state.gyroX, imu_state.gyroY, imu_state.gyroZ);
	jc->modifying_lock.unlock();

	jc->imu_state = imu_state;

	return true;
}
