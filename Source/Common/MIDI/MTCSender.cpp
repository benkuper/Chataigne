#include "../JuceLibraryCode/JuceHeader.h"
#include "MTCSender.h"

#include <math.h>

MTCSender::MTCSender(MIDIOutputDevice* device) :
	device(device)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
	if (device != nullptr) device->open();
}

MTCSender::~MTCSender()
{
}

void MTCSender::setDevice(MIDIOutputDevice * newDevice)
{
	if (newDevice == device) return;
	
	if (device != nullptr) device->close();

	device = newDevice;

	if (device != nullptr) device->open();
}

void MTCSender::start()
{
    m_frame = 0;
    m_second = 0;
    m_minute = 0;
    m_hour = 0;

    device->sendFullframeTimecode(m_hour, m_minute, m_second, m_frame, MidiMessage::SmpteTimecodeType::fps25);

    startTimer(1000 / 25 / 4);
}

void MTCSender::pause()
{
    if (isTimerRunning())
        stopTimer();
    else
        startTimer(1000 / 25 / 4);
}

void MTCSender::stop()
{
    stopTimer();
}

void MTCSender::setPosition(double position)
{
    std::unique_lock<std::mutex> lock_guard(m_mutex);
    double unused;

    m_frame = static_cast<int>(modf(position, &unused) * 25);
    m_second = static_cast<int>(position) % 60;
    m_minute = (static_cast<int>(position) / 60) % 60;
    m_hour = (static_cast<int>(position) / 60 / 60) % 60;
    m_piece = Piece::FrameLSB;

	device->sendFullframeTimecode(m_hour, m_minute, m_second, m_frame, MidiMessage::SmpteTimecodeType::fps25);

}

void MTCSender::hiResTimerCallback()
{
   if (device == nullptr) return;

    std::unique_lock<std::mutex> lock_guard(m_mutex);

    const int value = getValue(m_piece);
    device->sendQuarterframe(static_cast<int>(m_piece), value);

    m_piece = static_cast<Piece>((static_cast<int>(m_piece) + 1) % 8);

    if (++m_quarter >= 4)
    {
        m_quarter = 0;
        if (++m_frame >= 25)
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
        return ((m_hour >> 4) & 0b0001) | (0b01 << 1);
    }

    std::terminate();
}
