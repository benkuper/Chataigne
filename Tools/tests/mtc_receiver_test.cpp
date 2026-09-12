#include <algorithm>
#include <cassert>
#include <cmath>
#include <mutex>
#include <vector>

struct Time
{
    static double now;
    static double getMillisecondCounterHiRes() { return now; }
};
double Time::now = 0;

struct Timer
{
    virtual ~Timer() = default;
    virtual void timerCallback() = 0;
    void startTimer(int) { running = true; }
    void stopTimer() { running = false; }
    bool running = false;
};

using CriticalSection = std::recursive_mutex;
using ScopedLock = std::lock_guard<CriticalSection>;
#define JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Type)

template <typename Listener> struct ListenerList
{
    void add(Listener* listener) { listeners.push_back(listener); }
    void remove(Listener* listener) { listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end()); }
    template <typename Method, typename... Args> void call(Method method, Args... args)
    {
        for (auto* listener : listeners) (listener->*method)(args...);
    }
    std::vector<Listener*> listeners;
};

struct MidiMessage
{
    enum SmpteTimecodeType { fps24 = 0, fps25 = 1, fps30drop = 2, fps30 = 3 };
    int hours = 0, minutes = 0, seconds = 0, frames = 0;
    int piece = 0, value = 0;
    SmpteTimecodeType rate = fps30;
    static MidiMessage quarterFrame(int piece, int value)
    {
        MidiMessage message;
        message.piece = piece;
        message.value = value;
        return message;
    }
    static MidiMessage fullFrame(int hours, int minutes, int seconds, int frames, SmpteTimecodeType rate)
    {
        MidiMessage message;
        message.hours = hours;
        message.minutes = minutes;
        message.seconds = seconds;
        message.frames = frames;
        message.rate = rate;
        return message;
    }
    int getQuarterFrameSequenceNumber() const { return piece; }
    int getQuarterFrameValue() const { return value; }
    void getFullFrameParameters(int& h, int& m, int& s, int& f, SmpteTimecodeType& r) const
    {
        h = hours; m = minutes; s = seconds; f = frames; r = rate;
    }
};

struct MIDIInputDevice
{
    struct MIDIInputListener
    {
        virtual ~MIDIInputListener() = default;
        virtual void fullFrameTimecodeReceived(const MidiMessage&) {}
        virtual void quarterFrameTimecodeReceived(const MidiMessage&) {}
    };
    void addMIDIInputListener(MIDIInputListener*) {}
    void removeMIDIInputListener(MIDIInputListener*) {}
};

struct MIDIManager
{
    struct Listener
    {
        virtual ~Listener() = default;
        virtual void midiDeviceInRemoved(MIDIInputDevice*) {}
    };
    static MIDIManager* getInstance() { static MIDIManager manager; return &manager; }
    static MIDIManager* getInstanceWithoutCreating() { return getInstance(); }
    void addMIDIManagerListener(Listener*) {}
    void removeMIDIManagerListener(Listener*) {}
};

#include "../../Source/Common/MIDI/MTCReceiver.h"
#include "../../Source/Common/MIDI/MTCReceiver.cpp"

struct TrackingListener : MTCReceiver::MTCListener
{
    int updates = 0, starts = 0, stops = 0;
    void mtcTimeUpdated(bool) override { ++updates; }
    void mtcStarted() override { ++starts; }
    void mtcStopped() override { ++stops; }
};

static void sendCycle(MTCReceiver& receiver, int h, int m, int s, int f, int rate, int skip = -1)
{
    const int values[8] = {
        f & 15, (f >> 4) & 1, s & 15, (s >> 4) & 3,
        m & 15, (m >> 4) & 3, h & 15, ((h >> 4) & 1) | (rate << 1)
    };
    for (int piece = 0; piece < 8; ++piece)
    {
        if (piece == skip) continue;
        Time::now += 8;
        receiver.quarterFrameTimecodeReceived(MidiMessage::quarterFrame(piece, values[piece]));
    }
}

int main()
{
    MIDIInputDevice device;
    MTCReceiver receiver(&device);
    TrackingListener listener;
    receiver.addMTCListener(&listener);

    receiver.quarterFrameTimecodeReceived(MidiMessage::quarterFrame(7, 0));
    assert(listener.updates == 0 && listener.starts == 0);
    receiver.fullFrameTimecodeReceived(MidiMessage::fullFrame(0, 61, 0, 0, MidiMessage::fps30));
    assert(listener.updates == 0);

    for (int rate = 0; rate < 4; ++rate)
    {
        const auto type = static_cast<MidiMessage::SmpteTimecodeType>(rate);
        receiver.fullFrameTimecodeReceived(MidiMessage::fullFrame(1, 23, 45, 12, type));
        assert(receiver.type == type);
        assert(std::abs(receiver.getTime() - TimecodeMath::toSeconds(1, 23, 45, 12, rate)) < 1e-6);
        assert(!receiver.isPlaying);

        const int updatesBefore = listener.updates;
        sendCycle(receiver, 1, 23, 45, 12, rate, 3);
        assert(listener.updates == updatesBefore);
        sendCycle(receiver, 1, 23, 45, 12, rate);
        assert(listener.updates == updatesBefore + 1);
        assert(receiver.type == type && receiver.isPlaying);
        assert(std::abs(receiver.getTime() - (TimecodeMath::toSeconds(1, 23, 45, 12, rate) + 2.0 / TimecodeMath::actualFPS(rate))) < 1e-6);

        Time::now += 100;
        receiver.timerCallback();
        assert(receiver.isPlaying && listener.stops == rate);
        Time::now += 501;
        receiver.timerCallback();
        assert(!receiver.isPlaying && listener.stops == rate + 1);
    }

    assert(listener.starts == 4);
}
