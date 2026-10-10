#include "MainIncludes.h"
#include <iostream>
#include <mutex>
#include <stdexcept>
#undef main

class DMXReceiverTestApplication : public OrganicApplication
{
public:
    DMXReceiverTestApplication() : OrganicApplication("DMX receiver tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

struct InputRecorder : DMXDevice::DMXDeviceListener
{
    std::mutex mutex;
    Array<uint8> levels;
    int received = 0, universe = -1;

    void dmxDataInChanged(DMXDevice*, int net, int subnet, int u, Array<uint8> values, const String&) override
    {
        std::lock_guard<std::mutex> lock(mutex);
        levels = values;
        universe = (net << 8) | (subnet << 4) | u;
        ++received;
    }
    int count() { std::lock_guard<std::mutex> lock(mutex); return received; }
    bool matches(int u, std::initializer_list<int> expected)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (universe != u || levels.size() != 512) return false;
        int i = 0;
        for (int value : expected) if (levels[i++] != value) return false;
        return true;
    }
    void expect(int u, std::initializer_list<int> expected, const char* message)
    {
        const double deadline = Time::getMillisecondCounterHiRes() + 2000;
        while (!matches(u, expected) && Time::getMillisecondCounterHiRes() < deadline) Thread::sleep(10);
        check(matches(u, expected), message);
    }
};

struct ListenerRegistration
{
    DMXDevice& device;
    InputRecorder& input;
    ListenerRegistration(DMXDevice& d, InputRecorder& i) : device(d), input(i) { device.addDMXDeviceListener(&input); }
    ~ListenerRegistration() { device.inputCC->enabled->setValue(false); device.removeDMXDeviceListener(&input); }
};

static void testSACN()
{
    DMXSACNDevice device;
    device.outputCC->enabled->setValue(false);
    device.localPort->setValue(0);
    device.inputCC->enabled->setValue(true);
    check(device.mergeMode->getValueData() == var(DMXInputMerger::HTP), "sACN defaults to HTP");
    InputRecorder input;
    ListenerRegistration registration(device, input);
    DatagramSocket source;
    const int port = device.receiver->getBoundPort();
    auto send = [&](int cid, int universe, int seq, int startCode, std::initializer_list<uint8> values,
                    int priority = 100, int options = 0, int trim = 0)
    {
        e131_packet_t packet {};
        check(e131_pkt_init(&packet, static_cast<uint16>(universe), static_cast<uint16>(values.size())) == 0, "Create packet");
        packet.root.cid[15] = static_cast<uint8>(cid);
        packet.frame.seq_number = static_cast<uint8>(seq);
        packet.frame.priority = static_cast<uint8>(priority);
        packet.frame.options = static_cast<uint8>(options);
        packet.dmp.prop_val[0] = static_cast<uint8>(startCode);
        std::copy(values.begin(), values.end(), packet.dmp.prop_val + 1);
        const int size = 126 + static_cast<int>(values.size()) - trim;
        check(source.write("127.0.0.1", port, &packet, size) == size, "Send sACN");
    };
    send(1, 1, 0, 0xdd, { 100, 200, 0 });
    Thread::sleep(70);
    check(input.count() == 0, "PAP alone never becomes DMX levels");
    send(1, 1, 1, 0, { 200, 10, 240 });
    send(2, 1, 0, 0xdd, { 120, 200, 110 });
    send(2, 1, 1, 0, { 50, 150, 20 });
    send(3, 1, 0, 0xdd, { 115, 115, 115 });
    send(3, 1, 1, 0, { 100, 90, 220 });
    input.expect(1, { 50, 150, 220, 0 }, "Three sACN sources merge by per-slot priority and HTP");

    device.mergeMode->setValueWithKey("LTP");
    send(1, 1, 2, 0, { 200, 80, 240 });
    input.expect(1, { 50, 80, 220 }, "LTP takes the last changed channel among equal priorities");
    send(2, 1, 2, 0, { 50, 150, 20 });
    Thread::sleep(70);
    check(input.matches(1, { 50, 80, 220 }), "Unchanged LTP keepalive does not take ownership");
    device.mergeMode->setValueWithKey("HTP");
    input.expect(1, { 50, 150, 220 }, "Changing merge mode recomputes current sources");

    Thread::sleep(70);
    const int beforeInvalid = input.count();
    send(2, 1, 3, 0x17, { 255, 255, 255 });
    send(2, 1, 3, 0, { 255, 255, 255 }, 100, 0x80); // Preview
    send(2, 1, 3, 0, { 255, 255, 255 }, 100, 0, 1); // Truncated
    send(2, 1, 3, 0xdd, { 201, 201, 201 }); // Invalid PAP
    send(2, 1, 2, 0, { 255, 255, 255 }); // Duplicate sequence
    Thread::sleep(100);
    check(input.count() == beforeInvalid, "Alternate, preview, malformed and duplicate packets are filtered");
    send(2, 1, 3, 0xdd, { 110, 200, 110 });
    input.expect(1, { 100, 150, 220 }, "PAP changes selection without changing stored levels");
    send(3, 1, 1, 0, { 255, 255, 255 }, 100, 0x40);
    input.expect(1, { 50, 150, 20 }, "Stream termination immediately removes its source");

    send(4, 9, 0, 0xdd, { 100 });
    send(4, 9, 1, 0, { 77, 240 });
    input.expect(9, { 77, 0, 0 }, "Short PAP excludes missing slots and levels zero-fill the universe");
    send(5, 10, 0, 0, { 66 });
    input.expect(10, { 66, 0, 0 }, "Null-only sources start after the PAP discovery wait without further packets");

    // Adding multicast subscriptions must retain both the socket and its source history.
    DMXUniverse universe(0, 0, 1);
    auto* receiver = device.receiver.get();
    device.setupMulticast({ &universe }, {});
    check(device.receiver.get() == receiver, "Adding a subscription retains the receiver");
    device.setupMulticast({}, {});

    device.mergeMode->setValueWithKey("LTP");
    const var saved = JSON::parse(JSON::toString(device.getJSONData()));
    device.mergeMode->setValueWithKey("HTP");
    device.loadJSONData(saved);
    check(device.mergeMode->getValueData() == var(DMXInputMerger::LTP), "Saved merge mode is restored");
    device.removeDMXDeviceListener(&input);
    device.inputCC->enabled->setValue(false);
    std::cout << "sACN input merge integration tests passed\n";
}

static void testArtNet()
{
    DMXArtNetDevice device;
    device.outputCC->enabled->setValue(false);
    device.localPort->setValue(0);
    device.inputCC->enabled->setValue(true);
    InputRecorder input;
    ListenerRegistration registration(device, input);
    DatagramSocket source, otherPort;
    const int port = device.receiver->getBoundPort();
    auto send = [&](DatagramSocket& socket, int physical, int seq, int a, int b, int trim = 0, bool badID = false)
    {
        uint8 packet[20] { 'A', 'r', 't', '-', 'N', 'e', 't', 0, 0, 0x50, 0, 14 };
        packet[12] = static_cast<uint8>(seq);
        packet[13] = static_cast<uint8>(physical);
        packet[14] = 0x27;
        packet[15] = 1;
        packet[17] = 2;
        packet[18] = static_cast<uint8>(a);
        packet[19] = static_cast<uint8>(b);
        if (badID) packet[0] = 'X';
        check(socket.write("127.0.0.1", port, packet, 20 - trim) == 20 - trim, "Send ArtDMX");
    };
    send(source, 0, 1, 200, 10);
    send(source, 1, 1, 50, 150);
    send(source, 2, 1, 100, 90);
    input.expect(0x127, { 200, 150, 0 }, "Three physical Art-Net sources merge with HTP");
    device.mergeMode->setValueWithKey("LTP");
    send(source, 0, 2, 60, 10);
    input.expect(0x127, { 60, 90, 0 }, "Art-Net LTP tracks changes per channel");
    send(source, 1, 2, 50, 150);
    Thread::sleep(70);
    check(input.matches(0x127, { 60, 90 }), "Art-Net LTP ignores unchanged refreshes");
    device.mergeMode->setValueWithKey("HTP");
    input.expect(0x127, { 100, 150 }, "Art-Net mode change updates the merge");
    send(otherPort, 0, 3, 30, 10); // Same IP/physical source, new UDP source port.
    send(source, 2, 2, 20, 90);
    input.expect(0x127, { 50, 150 }, "UDP port changes do not create extra Art-Net sources");
    Thread::sleep(70);
    const int beforeInvalid = input.count();
    send(source, 1, 3, 255, 255, 1);
    send(source, 1, 3, 255, 255, 0, true);
    send(source, 1, 2, 255, 255);
    Thread::sleep(70);
    check(input.count() == beforeInvalid, "Art-Net malformed and duplicate packets are filtered");
    device.removeDMXDeviceListener(&input);
    device.inputCC->enabled->setValue(false);
    std::cout << "Art-Net input merge integration tests passed\n";
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    DMXReceiverTestApplication app;
    app.engine.reset(new ChataigneEngine());
    int result = 0;
    try { testSACN(); testArtNet(); }
    catch (const std::exception& e) { std::cerr << e.what() << '\n'; result = 1; }
    app.engine.reset();
    return result;
}
