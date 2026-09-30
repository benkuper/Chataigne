#include "../../Modules/juce_organicui/automation/easing/EasingMath.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>

static void expectNear(float actual, float expected)
{
	assert(std::abs(actual - expected) < 1.0e-5f);
}

int main()
{
	for (int frame = 0; frame <= 250; ++frame)
	{
		const double time = static_cast<double>(frame) / 25.0;
		assert(std::abs(EasingMath::linear(0.0, 1.0, time / 10.0) - frame * 0.004) < 1.0e-14);
	}
	assert(std::abs(EasingMath::sine(0.25, 0.0, 1.0, 1.0, 1.0, 0.25) - 0.5) < 1.0e-14);
	for (double duration : { 0.7, 1.0, 1.3, 10.0 })
	{
		for (double frequency : { 0.0, 0.3, 1.0, 3.0 })
		{
			assert(std::abs(EasingMath::sine(0.0, 0.2, 0.9, duration, frequency, 0.25) - 0.2) < 1.0e-14);
			assert(std::abs(EasingMath::sine(1.0, 0.2, 0.9, duration, frequency, 0.25) - 0.9) < 1.0e-14);
		}
		for (double parameter : { 0.0, 0.16224 * duration, 0.3 * duration,
			0.58112 * duration, 0.75 * duration, duration })
		{
			assert(std::abs(EasingMath::elastic(0.0, 0.2, 0.9, duration, parameter) - 0.2) < 1.0e-14);
			assert(std::abs(EasingMath::elastic(1.0, 0.2, 0.9, duration, parameter) - 0.9) < 1.0e-14);
			for (int i = 0; i <= 100; ++i)
			{
				const double value = EasingMath::elastic(i / 100.0, 0.2, 0.9, duration, parameter);
				assert(std::isfinite(value));
				assert(std::abs(value) < 5.0);
			}
		}
		assert(std::abs(EasingMath::elastic(0.5, 0.2, 0.9, duration, duration) - 0.55) < 1.0e-14);
	}

	expectNear(EasingMath::bounceOut(0.0f), 0.0f);
	expectNear(EasingMath::bounceOut(1.0f), 1.0f);
	assert(EasingMath::bounceOut(0.0) == 0.0);
	assert(EasingMath::bounceOut(1.0) == 1.0);
	assert(std::abs(EasingMath::bounceOut(0.123456789) - 7.5625 * 0.123456789 * 0.123456789) < 1.0e-15);

	for (float boundary : { 1.0f / 2.75f, 2.0f / 2.75f, 2.5f / 2.75f })
	{
		const float before = std::nextafter(boundary, 0.0f);
		const float after = std::nextafter(boundary, 1.0f);
		expectNear(EasingMath::bounceOut(before), EasingMath::bounceOut(after));
	}
	for (double boundary : { 1.0 / 2.75, 2.0 / 2.75, 2.5 / 2.75 })
	{
		const double before = std::nextafter(boundary, 0.0);
		const double after = std::nextafter(boundary, 1.0);
		assert(std::abs(EasingMath::bounceOut(before) - EasingMath::bounceOut(after)) < 1.0e-12);
	}

	for (int i = 0; i <= 10000; ++i)
	{
		const float value = EasingMath::bounceOut(i / 10000.0f);
		assert(std::isfinite(value));
		assert(value >= 0.0f && value <= 1.00001f);
	}

	std::cout << "Easing math passed\n";
}
