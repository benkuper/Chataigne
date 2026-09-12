#include "Common/CommonIncludes.h"
#include "TimecodeMath.h"

#include <math.h>

MTCSender::MTCSender(MIDIOutputDevice* device) :
	Thread("MTC"),
	device(device),
	speedFactor(1),
	fps(30),
	nominalFPS(30),
	fpsType(MidiMessage::SmpteTimecodeType::fps30)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	if (device != nullptr) device->open();

}

MTCSender::~MTCSender()
{
	stopThread(-1);
	if (device != nullptr) device->close();
}

void MTCSender::setDevice(MIDIOutputDevice* newDevice)
{
	if (newDevice == device) return;
	const bool wasRunning = isThreadRunning();
	stopThread(-1);

	if (device != nullptr) device->close();

	device = newDevice;

	if (device != nullptr) device->open();
	if (wasRunning && device != nullptr) startThread();
}

void MTCSender::start(double position)
{
	setPosition(position);
	startThread();
}

void MTCSender::pause(bool resumeIfAlreadyPaused)
{
	if (isThreadRunning())
	{
		stopThread(-1);
	}
	else if (resumeIfAlreadyPaused)
	{
		if (device != nullptr) startThread();
	}
}

void MTCSender::stop()
{
	stopThread(-1);
}

void MTCSender::setPosition(double position, bool fullFrame)
{
	const ScopedLock scopedLock(lock);
	if (device == nullptr) return;
	TimecodeMath::fromSeconds(position, static_cast<int>(fpsType), m_hour, m_minute, m_second, m_frame);
	m_piece = Piece::FrameLSB;
	m_quarter = 0;

	if (fullFrame)
	{
		device->sendFullframeTimecode(m_hour, m_minute, m_second, m_frame, fpsType);
	}

}

void MTCSender::setSpeedFactor(float speed)
{
	const ScopedLock scopedLock(lock);
	speedFactor = speed;
}

void MTCSender::setFPS(MidiMessage::SmpteTimecodeType val)
{
	const ScopedLock scopedLock(lock);
	fpsType = val;
	nominalFPS = TimecodeMath::nominalFPS(static_cast<int>(fpsType));
	fps = TimecodeMath::actualFPS(static_cast<int>(fpsType));
}

void MTCSender::run()
{
	if (device == nullptr) return;

	double lastFrameTime = Time::getMillisecondCounterHiRes();

	while (!threadShouldExit())
	{
		sleep(1);
		GenericScopedLock _lock(lock);

		if (speedFactor <= 0) continue;
		const double frameTime = (1000.0 / fps / 4) / speedFactor;

		double t = Time::getMillisecondCounterHiRes();
		if (t < lastFrameTime + frameTime) continue;

		lastFrameTime = t - lastFrameTime > frameTime * 2 ? t : lastFrameTime + frameTime;


		const int value = getValue(m_piece);
		device->sendQuarterframe(static_cast<int>(m_piece), value);

		m_piece = static_cast<Piece>((static_cast<int>(m_piece) + 1) % 8);

		if (++m_quarter >= 4)
		{
			m_quarter = 0;
			if (++m_frame >= nominalFPS)
			{
				m_frame = 0;
				if (++m_second >= 60)
				{
					m_second = m_second % 60;
					if (++m_minute >= 60)
					{
						m_minute = m_minute % 60;
						if (++m_hour >= 24)
						{
							m_hour = m_hour % 24;
						}
					}
				}
			}
			if (fpsType == MidiMessage::fps30drop && m_second == 0 && m_minute % 10 != 0 && m_frame == 0)
				m_frame = 2;
		}
	}

}

int MTCSender::getValue(Piece piece)
{
	switch (piece) {
	case Piece::FrameLSB:
		return m_frame & 0b1111;
	case Piece::FrameMSB:
		return (m_frame >> 4) & 0b0001;
	case Piece::SecondLSB:
		return m_second & 0b1111;
	case Piece::SecondMSB:
		return (m_second >> 4) & 0b0011;
	case Piece::MinuteLSB:
		return m_minute & 0b1111;
	case Piece::MinuteMSB:
		return (m_minute >> 4) & 0b0011;
	case Piece::HourLSB:
		return m_hour & 0b1111;
	case Piece::RateAndHourMSB:
		return ((m_hour >> 4) & 0b0001) | ((0b00 | fpsType) << 1);
	}

	std::terminate();
}
