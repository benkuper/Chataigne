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
	void setPosition(double position, bool fullFrame = false);
	void setSpeedFactor(float speed);
	void setFPS(MidiMessage::SmpteTimecodeType val);

	MIDIOutputDevice* device;

	// Used only in separate thread!
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

	SpinLock lock;

	float currentPosition;
	float speedFactor;

	double frameTime;
	double lastFrameSendTime;

	int fps;
	MidiMessage::SmpteTimecodeType fpsType;

	Piece m_piece{ Piece::FrameLSB };
	int m_quarter{ 0 };
	int m_frame{ 0 };
	int m_second{ 0 };
	int m_minute{ 0 };
	int m_hour{ 0 };
	int m_deviceIndex{ -1 };

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MTCSender)
};
