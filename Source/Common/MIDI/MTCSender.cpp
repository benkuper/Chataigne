#include "Common/CommonIncludes.h"

#include <math.h>

MTCSender::MTCSender(MIDIOutputDevice* device) :
	Thread("MTC"),
	device(device),
	speedFactor(1),
	fps(30),
	fpsType(MidiMessage::SmpteTimecodeType::fps30)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	if (device != nullptr) device->open();

}

MTCSender::~MTCSender()
{
	stopThread(10);
}

void MTCSender::setDevice(MIDIOutputDevice* newDevice)
{
	if (newDevice == device) return;

	if (device != nullptr) device->close();

	device = newDevice;

	if (device != nullptr) device->open();
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
		stopThread(10);
	}
	else if (resumeIfAlreadyPaused)
		startThread();
}

void MTCSender::stop()
{
	stopThread(10);
}

void MTCSender::setPosition(double position, bool fullFrame)
{
	if (device == nullptr) return;

	double unused;
	lock.enter();
	m_frame = static_cast<int>(modf(position, &unused) * fps);
	m_second = static_cast<int>(position) % 60;
	m_minute = (static_cast<int>(position) / 60) % 60;
	m_hour = (static_cast<int>(position) / 60 / 60) % 60;
	m_piece = Piece::FrameLSB;
	lock.exit();

	if (fullFrame) device->sendFullframeTimecode(m_hour, m_minute, m_second, m_frame, fpsType);
}

void MTCSender::setSpeedFactor(float speed)
{
	speedFactor = speed;
}

void MTCSender::setFPS(MidiMessage::SmpteTimecodeType val)
{
	fpsType = val;
	switch (fpsType)
	{
	case MidiMessage::fps24: fps = 24;
	case MidiMessage::fps25: fps = 25;
	case MidiMessage::fps30: fps = 30;
	case MidiMessage::fps30drop: fps = 29.997;
	}
}

void MTCSender::run()
{
	if (device == nullptr) return;

	double lastFrameTime = Time::getMillisecondCounterHiRes();

	while (!threadShouldExit())
	{
		sleep(1);

		frameTime = (1000.0 / fps / 4) / speedFactor;

		double t = Time::getMillisecondCounterHiRes();
		if (t < lastFrameTime + frameTime) continue;

		lastFrameTime += frameTime;

		lock.enter();

		const int value = getValue(m_piece);
		device->sendQuarterframe(static_cast<int>(m_piece), value);

		m_piece = static_cast<Piece>((static_cast<int>(m_piece) + 1) % 8);

		if (++m_quarter >= 4)
		{
			m_quarter = 0;
			if (++m_frame >= fps)
			{
				m_frame = 0;
				if (++m_second >= 60)
				{
					m_second = 0;
					if (++m_minute >= 60)
					{
						m_minute = 0;
						if (++m_hour >= 24)
						{
							m_hour = 0;
						}
					}
				}
			}
		}

		lock.exit();
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
