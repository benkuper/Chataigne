#include "../../Modules/juce_timeline/timeline/Sequence/SequencePlaybackClock.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>

static void expectNear(double actual, double expected)
{
	assert(std::abs(actual - expected) < 1.0e-9);
}

int main()
{
	SequencePlaybackClock clock;
	clock.reset(1000.0, 0.0, 250, 1.0);
	assert(clock.frameAt(1000.0) == 0);
	assert(clock.frameAt(1003.9) == 0);
	assert(clock.frameAt(1004.0) == 1);
	expectNear(clock.timeForFrame(1), 0.004);

	// An early wake cannot create another evaluation, and a late wake skips
	// overdue frames without sending them in a burst.
	std::int64_t lastFrame = 0;
	int sends = 0;
	for (double wake : { 1003.9, 1004.3, 1007.8, 1008.1, 1013.2, 1013.4, 1016.1 })
	{
		const auto frame = clock.frameAt(wake);
		if (frame > lastFrame)
		{
			lastFrame = frame;
			++sends;
		}
	}
	assert(sends == 4);
	assert(lastFrame == 4);
	expectNear(clock.timeForFrame(lastFrame), 0.016);
	expectNear(clock.deadlineForFrame(lastFrame + 1), 1020.0);

	clock.reset(1016.1, 2.0, 250, -1.0);
	assert(clock.frameAt(1020.1) == 1);
	expectNear(clock.timeForFrame(1), 1.996);
	clock.reset(1020.1, clock.timeForFrame(1), 250, 0.5);
	expectNear(clock.timeForFrame(1), 1.998);

	clock.reset(0.0, 0.0, 60, 1.0);
	assert(clock.frameAt(clock.deadlineForFrame(216000)) == 216000);
	expectNear(clock.timeForFrame(216000), 3600.0);

	std::cout << "Sequence playback clock passed\n";
}
