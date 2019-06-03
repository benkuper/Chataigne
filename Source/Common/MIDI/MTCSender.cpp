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

void MTCSender::start(double position)
{
	setPosition(position);
	startTimer(1000 / fps / 4);
}

void MTCSender::pause()
{
    if (isTimerRunning())
        stopTimer();
    else
        startTimer(1000 / fps / 4);
}

void MTCSender::stop()
{
    stopTimer();
}

void MTCSender::setPosition(double position, bool fullFrame)
{

	if (device == nullptr) return; 
	
	lock.enter();
	m_frame = fmodf(position, 1)* fps;// static_cast<int>(modf(position, &unused) * 25);
    m_second = (int)floorf(position) % 60;
    m_minute = (int)floorf(position / 60) % 60;
	m_hour = floorf(position / 3600);
    m_piece = Piece::FrameLSB;
	lock.exit();

	if(fullFrame) device->sendFullframeTimecode(m_hour, m_minute, m_second, m_frame, fpsType);
}

void MTCSender::hiResTimerCallback()
{
   if (device == nullptr) return;
   
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
