#pragma once

class MTCSender :
	Thread
{
public:
	MTCSender(MIDIOutputDevice* device = nullptr);
	~MTCSender();


	void setDevice(MIDIOutputDevice* newDevice);
	void start(double position = 0);
	void pause(bool resumeIfAlreadyPaused = true);
	void stop();
	void setPosition(double position, bool fullFrame = true);
	void setSpeedFactor(float speed);
	void setFPS(MidiMessage::SmpteTimecodeType val);

	MIDIOutputDevice* device;

private:
	enum class Piece {
		FrameLSB = 0,
		FrameMSB,
		SecondLSB,
		SecondMSB,
		MinuteLSB,
		MinuteMSB,
		HourLSB,
		RateAndHourMSB
	};

	void run() override;
	int getValue(Piece piece);

	CriticalSection lock;

	float speedFactor;

	double fps;
	int nominalFPS;
	MidiMessage::SmpteTimecodeType fpsType;

	Piece m_piece{ Piece::FrameLSB };
	int m_quarter{ 0 };
	int m_frame{ 0 };
	int m_second{ 0 };
	int m_minute{ 0 };
	int m_hour{ 0 };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MTCSender)
};
