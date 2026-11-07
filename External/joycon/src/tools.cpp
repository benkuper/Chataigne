#pragma once

#include <chrono>
#include <thread>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

//#include <curl/curl.h>

#pragma warning(disable: 4996)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

int16_t unsignedToSigned16(uint16_t n) {
	uint16_t A = n;
	uint16_t B = 0xFFFF - A;
	if (A < B) {
		return (int16_t)A;
	} else {
		return (int16_t)(-1 * B);
	}
}

int16_t uint16_to_int16(uint16_t a) {
	int16_t b;
	char* aPointer = (char*)&a, *bPointer = (char*)&b;
	memcpy(bPointer, aPointer, sizeof(a));
	return b;
}

uint16_t combine_uint8_t(uint8_t a, uint8_t b) {
	uint16_t c = ((uint16_t)a << 8) | b;
	return c;
}

int16_t combine_gyro_data(uint8_t a, uint8_t b) {
	uint16_t c = combine_uint8_t(a, b);
	int16_t d = uint16_to_int16(c);
	return d;
}

float clamp(float a, float min, float max) {
	if (a < min) {
		return min;
	} else if (a > max) {
		return max;
	} else {
		return a;
	}
}

uint16_t clamp(uint16_t a, uint16_t min, uint16_t max) {
	if (a < min) {
		return min;
	}
	else if (a > max) {
		return max;
	}
	else {
		return a;
	}
}

unsigned createMask(unsigned a, unsigned b) {
	unsigned r = 0;
	for (unsigned i = a; i <= b; i++)
		r |= 1 << i;

	return r;
}

void hex_dump(unsigned char *buf, int len) {
	for (int i = 0; i < len; i++) {
		printf("%02x ", buf[i]);
	}
	printf("\n");
}

void hex_dump2(unsigned char *buf, int len) {
	for (int i = 0; i < len; i++) {
		printf("%02x ", buf[i]);
	}
}

void hex_dump_0(unsigned char *buf, int len) {
	for (int i = 0; i < len; i++) {
		if (buf[i] != 0) {
			printf("%02x ", buf[i]);
		}
	}
}

void int_dump(unsigned char *buf, int len) {
	for (int i = 0; i < len; i++) {
		printf("%i ", buf[i]);
	}
	printf("\n");
}
