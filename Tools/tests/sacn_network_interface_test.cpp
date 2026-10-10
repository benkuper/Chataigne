#include "MainIncludes.h"
#include "Module/ModuleIncludes.h"
#include <iostream>
#include <stdexcept>
#if JUCE_WINDOWS
#include <iphlpapi.h>
#else
#include <arpa/inet.h>
#endif
#undef main

class SACNTestApplication : public OrganicApplication
{
public:
    SACNTestApplication() : OrganicApplication("sACN interface tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

static void pumpUI()
{
#if JUCE_WINDOWS
    const auto deadline = GetTickCount64() + 30;
    do
    {
        MSG message;
        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        MsgWaitForMultipleObjects(0, nullptr, FALSE, 1, QS_ALLINPUT);
    } while (GetTickCount64() < deadline);
#endif
}

static void testProtocolControls()
{
    struct TestModule : DMXModule
    {
        ~TestModule() { clearItem(); }
    } module;
    module.enabled->setValue(false);
    auto* output = module.outputUniverseManager.items[0];
    output->netParam->setValue(3);
    output->subnetParam->setValue(4);
    DMXUniverseItemEditor outputEditor({ output }, true);
    outputEditor.setSize(600, 300);
    check(!outputEditor.netUI->isVisible() && !outputEditor.subnetUI->isVisible(),
        "Serial device hides Net/Subnet in universe header");
    check(output->netParam->hideInEditor && output->subnetParam->hideInEditor,
        "Serial device hides Net/Subnet in universe contents");
    check(module.useMulticast->hideInEditor, "Legacy multicast control is hidden");

    module.dmxType->setValueWithData(DMXDevice::ARTNET);
    pumpUI();
    check(outputEditor.netUI->isVisible() && outputEditor.subnetUI->isVisible(),
        "Switching to Art-Net updates an existing universe header");
    check(!output->netParam->hideInEditor && !output->subnetParam->hideInEditor,
        "Art-Net exposes Net/Subnet in universe contents");
    auto* input = module.inputUniverseManager.addItem(nullptr, var(), false);
    DMXUniverseItemEditor inputEditor({ input }, true);
    inputEditor.setSize(600, 300);
    check(inputEditor.netUI->isVisible() && inputEditor.subnetUI->isVisible(),
        "New Art-Net input universe exposes addressing in its header");

    module.dmxType->setValueWithData(DMXDevice::SACN);
    pumpUI();
    check(!outputEditor.netUI->isVisible() && !inputEditor.netUI->isVisible()
        && !outputEditor.subnetUI->isVisible() && !inputEditor.subnetUI->isVisible(),
        "Switching to sACN hides addressing in both existing headers");
    check(outputEditor.universeUI->isVisible() && inputEditor.universeUI->isVisible(),
        "Universe selector remains visible");
    auto* sacn = dynamic_cast<DMXSACNDevice*>(module.dmxDevice.get());
    check(sacn != nullptr && sacn->useMulticast->parentContainer == sacn,
        "Visible multicast control belongs to sACN settings");
    sacn->remoteHost->setValue("192.0.2.10");
    sacn->useMulticast->setValue(true);
    check(module.useMulticast->boolValue() && !sacn->remoteHost->enabled,
        "Multicast disables Remote Host and synchronizes legacy control");
    check(sacn->outMulticastMap.contains(output->universe), "Multicast toggle configures output destinations");
    sacn->useMulticast->setValue(false);
    check(sacn->remoteHost->enabled && sacn->remoteHost->stringValue() == "192.0.2.10"
        && sacn->outMulticastMap.size() == 0, "Unicast restores retained Remote Host and clears multicast map");
    module.useMulticast->setValue(true);
    check(sacn->useMulticast->boolValue() && !sacn->remoteHost->enabled,
        "Original multicast address still controls sACN");

    auto* added = module.outputUniverseManager.addItem(nullptr, var(), false);
    check(!added->showArtNetAddressing && added->netParam->hideInEditor,
        "New sACN universes inherit hidden addressing");
    auto* copied = new DMXUniverseItem();
    module.outputUniverseManager.addItems({ copied }, var(), false);
    check(!copied->showArtNetAddressing, "Bulk-added sACN universes inherit hidden addressing");

    var saved = JSON::parse(JSON::toString(module.getJSONData()));
    TestModule restored;
    restored.loadJSONData(saved);
    auto* restoredSACN = dynamic_cast<DMXSACNDevice*>(restored.dmxDevice.get());
    check(restoredSACN != nullptr && restoredSACN->useMulticast->boolValue()
        && !restoredSACN->remoteHost->enabled, "Saved project restores multicast and Remote Host state");
    for (auto* u : restored.outputUniverseManager.items)
        check(!u->showArtNetAddressing && u->netParam->hideInEditor, "Loaded sACN universes hide addressing");

    // Simulate an older project with only the module-level multicast parameter.
    for (var deviceData : { saved["device"], saved["params"]["containers"]["sacn"] })
    {
        auto* deviceParameters = deviceData["parameters"].getArray();
        check(deviceParameters != nullptr, "Saved sACN device contains parameters");
        for (int i = deviceParameters->size(); --i >= 0;)
            if ((*deviceParameters)[i]["controlAddress"].toString() == "/useMulticast") deviceParameters->remove(i);
    }
    restoredSACN->useMulticast->setValue(false);
    restored.loadJSONData(saved);
    restoredSACN = dynamic_cast<DMXSACNDevice*>(restored.dmxDevice.get());
    check(restoredSACN != nullptr && restoredSACN->useMulticast->boolValue()
        && !restoredSACN->remoteHost->enabled, "Legacy project restores multicast and Remote Host state");

    module.dmxType->setValueWithData(DMXDevice::ARTNET);
    pumpUI();
    check(outputEditor.netUI->isVisible() && inputEditor.netUI->isVisible()
        && output->netParam->intValue() == 3 && output->subnetParam->intValue() == 4,
        "Returning to Art-Net restores visibility and retained Net/Subnet values");
    module.dmxType->setValueWithData(DMXDevice::SACN);
    sacn = dynamic_cast<DMXSACNDevice*>(module.dmxDevice.get());
    check(sacn->useMulticast->boolValue() && !sacn->remoteHost->enabled,
        "Returning to sACN retains multicast mode");
    pumpUI();
    std::cout << "DMX protocol control regression tests passed\n";
}

static void checkSenderInterface(DMXSACNDevice& device, const String& ip)
{
    check(device.sender != nullptr, "Sender configured");
    in_addr address {};
#if JUCE_WINDOWS
    int size = sizeof(address);
#else
    socklen_t size = sizeof(address);
#endif
    check(getsockopt(device.sender->getRawSocketHandle(), IPPROTO_IP, IP_MULTICAST_IF,
        reinterpret_cast<char*>(&address), &size) == 0, "Read multicast output interface");
#if JUCE_WINDOWS
    // Accept both Winsock's interface index and its legacy address result.
    if (ip.isEmpty())
    {
        check(address.s_addr == 0, "Auto clears explicit multicast adapter");
        return;
    }
    ULONG tableSize = 0;
    GetIpAddrTable(nullptr, &tableSize, false);
    std::vector<char> buffer(tableSize);
    auto* table = reinterpret_cast<MIB_IPADDRTABLE*>(buffer.data());
    check(GetIpAddrTable(table, &tableSize, false) == NO_ERROR, "Read adapter addresses");
    for (DWORD i = 0; i < table->dwNumEntries; ++i)
        if (table->table[i].dwAddr == inet_addr(ip.toRawUTF8()))
        {
            check(address.s_addr == table->table[i].dwIndex || address.s_addr == table->table[i].dwAddr,
                "Multicast output uses selected adapter");
            return;
        }
    check(false, "Selected adapter has a local address");
#else
    check(address.s_addr == (ip.isEmpty() ? htonl(INADDR_ANY) : inet_addr(ip.toRawUTF8())),
        "Multicast output uses selected adapter");
#endif
}

struct InputRecorder : DMXDevice::DMXDeviceListener
{
    std::atomic<int> received { 0 };
    void dmxDataInChanged(DMXDevice*, int, int, int universe, Array<uint8> values, const String&) override
    {
        if (universe == 63900 && values[0] == 123) ++received;
    }
};

static void testInterface(DMXSACNDevice& device, const String& ip)
{
    if (!device.networkInterface->setValueWithData(ip))
    {
        // Loopback is omitted by interface enumeration on some platforms.
        check(ip == "127.0.0.1", "Requested adapter exists in selector");
        device.networkInterface->addOption("Test loopback", ip);
        check(device.networkInterface->setValueWithData(ip), "Select loopback");
    }
    checkSenderInterface(device, ip);

    DatagramSocket monitor;
    check(monitor.bindToPort(0), "Bind multicast monitor");
    ip_mreq membership {};
    membership.imr_multiaddr.s_addr = inet_addr(device.getMulticastIPForUniverse(63900).toRawUTF8());
    membership.imr_interface.s_addr = inet_addr(ip.toRawUTF8());
    check(setsockopt(monitor.getRawSocketHandle(), IPPROTO_IP, IP_ADD_MEMBERSHIP,
        reinterpret_cast<const char*>(&membership), sizeof(membership)) == 0, "Join selected adapter");

    device.remotePort->setValue(monitor.getBoundPort());
    uint8 values[512] {};
    values[0] = 123;
    device.sendDMXValuesInternal(0, 0, 63900, values, 512);
    check(monitor.waitUntilReady(true, 2000) == 1, "Multicast packet arrives on selected adapter");
    e131_packet_t packet {};
    String source;
    int port = 0;
    check(monitor.read(&packet, sizeof(packet), false, source, port) == sizeof(packet), "Read sACN packet");
    check(source == ip, "Packet source matches selected adapter");
    check(e131_pkt_validate(&packet) == E131_ERR_NONE && packet.dmp.prop_val[1] == 123,
        "Multicast packet contains valid DMX data");

    // Use the production receiver, including after an interface change.
    check(device.receiver != nullptr, "Receiver recreated");
    InputRecorder input;
    device.addDMXDeviceListener(&input);
    device.remotePort->setValue(device.receiver->getBoundPort());
    device.sendDMXValuesInternal(0, 0, 63900, values, 512);
    const auto deadline = Time::getMillisecondCounter() + 2000;
    while (input.received == 0 && Time::getMillisecondCounter() < deadline) Thread::sleep(10);
    device.removeDMXDeviceListener(&input);
    check(input.received > 0, "Device receives multicast on selected adapter");

    // A universe absent from the multicast map must still use Remote Host.
    device.remoteHost->setValue(ip);
    device.remotePort->setValue(monitor.getBoundPort());
    device.sendDMXValuesInternal(0, 0, 63901, values, 512);
    check(monitor.waitUntilReady(true, 2000) == 1, "Unicast delivery remains available");
    check(monitor.read(&packet, sizeof(packet), false, source, port) == sizeof(packet)
        && source == ip && ntohs(packet.frame.universe) == 63901, "Unicast uses selected adapter");
    std::cout << "Multicast send and receive passed on " << ip << '\n';
}

int main(int argc, char** argv)
{
    ScopedJuceInitialiser_GUI gui;
    SACNTestApplication app;
    app.engine.reset(new ChataigneEngine());
    int result = 0;
    try
    {
        testProtocolControls();
        DMXSACNDevice device;
        device.sendRate->setEnabled(false);
        check(device.networkInterface->getValueKey() == "Auto", "Existing projects default to Auto");
        checkSenderInterface(device, "");
        for (auto* option : device.networkInterface->enumValues)
            check(!option->value.toString().containsChar(':'), "Selector only offers IPv4");

        DMXUniverse universe(0, 0, 63900);
        device.setupMulticast({ &universe }, { &universe });
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
        checkSenderInterface(device, selectedIP);
        testInterface(device, selectedIP);

        device.networkInterface->addOption("Unavailable test adapter", "192.0.2.1");
        device.networkInterface->setValueWithData("192.0.2.1");
        check(device.sender == nullptr, "Unavailable adapter does not fall back to another NIC");
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
        std::cout << "sACN network interface regression tests passed\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "sACN interface regression failed: " << e.what() << '\n';
        result = 1;
    }
    app.engine.reset();
    return result;
}
