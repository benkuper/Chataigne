#include "MainIncludes.h"
#include <iostream>
#include <stdexcept>
#if ! JUCE_WINDOWS
#include <arpa/inet.h>
#endif
#undef main

class ArtNetTestApplication : public OrganicApplication
{
public:
    ArtNetTestApplication() : OrganicApplication("ArtNet interface tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

static void checkSenderInterface(DMXArtNetDevice& device, const String& ip)
{
    check(device.sender != nullptr, "Sender configured");
    sockaddr_in address {};
#if JUCE_WINDOWS
    int size = sizeof(address);
#else
    socklen_t size = sizeof(address);
#endif
    check(getsockname(device.sender->getRawSocketHandle(), reinterpret_cast<sockaddr*>(&address), &size) == 0,
        "Read bound sender address");
    check(address.sin_addr.s_addr == (ip.isEmpty() ? htonl(INADDR_ANY) : inet_addr(ip.toRawUTF8())),
        "Sender bound to selected adapter");
}

struct InputRecorder : DMXDevice::DMXDeviceListener
{
    std::atomic<int> received { 0 };
    void dmxDataInChanged(DMXDevice*, int net, int subnet, int universe, Array<uint8> values, const String&) override
    {
        if (net == 1 && subnet == 2 && universe == 7 && values[0] == 123) ++received;
    }
};

static void testInterface(DMXArtNetDevice& device, const String& ip)
{
    if (!device.networkInterface->setValueWithData(ip))
    {
        check(ip == "127.0.0.1", "Requested adapter exists in selector");
        device.networkInterface->addOption("Test loopback", ip);
        check(device.networkInterface->setValueWithData(ip), "Select loopback");
    }
    checkSenderInterface(device, ip);
    check(device.receiver != nullptr, "Receiver configured");

    DatagramSocket monitor;
#if JUCE_WINDOWS
    check(monitor.bindToPort(0, ip), "Bind packet monitor");
#else
    check(monitor.bindToPort(0), "Bind packet monitor");
#endif
    uint8 values[512] {};
    values[0] = 123;
    device.remotePort->setValue(monitor.getBoundPort());
    auto checkOutput = [&](const String& destination)
    {
        device.remoteHost->setValue(destination);
        device.sendDMXValuesInternal(1, 2, 7, values, 512);
        check(monitor.waitUntilReady(true, 2000) == 1, "ArtNet packet delivered");
        uint8 packet[530] {};
        String source;
        int port = 0;
        check(monitor.read(packet, sizeof(packet), false, source, port) == sizeof(packet), "Read ArtDMX packet");
        check(source == ip, "Packet source matches selected adapter");
        check(memcmp(packet, "Art-Net", 8) == 0 && packet[9] == 0x50 && packet[14] == 0x27
            && packet[15] == 1 && packet[18] == 123, "Packet retains ArtDMX header and values");
    };
    checkOutput(ip);
    if (ip != "127.0.0.1") checkOutput("255.255.255.255");

    InputRecorder input;
    device.addDMXDeviceListener(&input);
    device.remotePort->setValue(device.receiver->getBoundPort());
    auto checkInput = [&](const String& destination)
    {
        device.remoteHost->setValue(destination);
        input.received = 0;
        device.sendDMXValuesInternal(1, 2, 7, values, 512);
        const auto deadline = Time::getMillisecondCounter() + 2000;
        while (input.received == 0 && Time::getMillisecondCounter() < deadline) Thread::sleep(10);
        return input.received > 0;
    };
    const bool unicastReceived = checkInput(ip);
    const bool broadcastReceived = ip == "127.0.0.1" || checkInput("255.255.255.255");
    device.removeDMXDeviceListener(&input);
    check(unicastReceived, "Device receives unicast on selected adapter");
    check(broadcastReceived, "Device receives broadcasts on selected adapter");
    std::cout << "ArtNet send and receive passed on " << ip << '\n';
}

int main(int argc, char** argv)
{
    ScopedJuceInitialiser_GUI gui;
    ArtNetTestApplication app;
    app.engine.reset(new ChataigneEngine());
    int result = 0;
    try
    {
        DMXArtNetDevice device;
        device.sendRate->setEnabled(false);
        check(device.networkInterface->getValueKey() == "Auto", "Existing projects default to Auto");
        checkSenderInterface(device, "");
        for (auto* option : device.networkInterface->enumValues)
            check(!option->value.toString().containsChar(':'), "Selector only offers IPv4");
        device.localPort->setValue(0);
        device.inputCC->enabled->setValue(true);
        testInterface(device, "127.0.0.1");
        for (int i = 1; i < argc; ++i) testInterface(device, argv[i]);

        const String selectedIP = device.networkInterface->getIP();
        const var saved = device.getJSONData();
        device.networkInterface->setValueWithKey("Auto");
        checkSenderInterface(device, "");
        device.loadJSONData(saved);
        check(device.networkInterface->getIP() == selectedIP, "Saved adapter restored");
        testInterface(device, selectedIP);

        device.networkInterface->addOption("Unavailable test adapter", "192.0.2.1");
        device.networkInterface->setValueWithData("192.0.2.1");
        check(device.sender == nullptr && device.receiver == nullptr, "Unavailable adapter does not fall back");
        device.networkInterface->setValueWithData(selectedIP);
        checkSenderInterface(device, selectedIP);
        device.outputCC->enabled->setValue(false);
        check(device.sender == nullptr, "Disabling output closes sender");
        device.outputCC->enabled->setValue(true);
        checkSenderInterface(device, selectedIP);
        device.setEnabled(false);
        check(device.sender == nullptr && device.receiver == nullptr, "Disabling device closes sockets");
        device.setEnabled(true);
        checkSenderInterface(device, selectedIP);
        check(device.receiver != nullptr, "Enabling device restores input");

        DMXArtNetDevice outputOnly(false);
        outputOnly.sendRate->setEnabled(false);
        outputOnly.networkInterface->setValueWithData(selectedIP);
        check(outputOnly.receiver == nullptr, "Output-only device needs no input container");
        checkSenderInterface(outputOnly, selectedIP);
        std::cout << "ArtNet network interface regression tests passed\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "ArtNet interface regression failed: " << e.what() << '\n';
        result = 1;
    }
    app.engine.reset();
    return result;
}
