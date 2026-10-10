#include "../../Modules/juce_dmx/device/DMXInputMerger.h"
#include <cassert>
#include <iostream>

using Merger = DMXInputMerger;

static void testHTPAndLTP()
{
    Merger m(Merger::Protocol::ArtNet);
    uint8_t a[] { 200, 10, 30 }, b[] { 50, 150, 20 }, c[] { 100, 90, 220 };
    m.receive(1, "a", a, 3, 0);
    m.receive(1, "b", b, 3, 1);
    m.receive(1, "c", c, 3, 2);
    auto out = m.poll(2, Merger::HTP);
    assert(out.size() == 1 && out[0].levels[0] == 200 && out[0].levels[1] == 150 && out[0].levels[2] == 220);
    out = m.poll(3, Merger::LTP);
    assert(out.size() == 1 && out[0].levels[0] == 100);
    a[0] = 60;
    m.receive(1, "a", a, 3, 4);
    m.receive(1, "b", b, 3, 5); // A keepalive must not take ownership back.
    out = m.poll(5, Merger::LTP);
    assert(out[0].levels[0] == 60 && out[0].levels[1] == 90 && out[0].levels[2] == 220);
    m.receive(2, "a", b, 3, 6);
    out = m.poll(6, Merger::LTP);
    assert(out.size() == 1 && out[0].universe == 2 && out[0].levels[0] == 50);
    m.receive(1, "b", b, 3, 9999);
    out = m.poll(10005, Merger::LTP);
    assert(out.size() == 1 && out[0].universe == 1 && out[0].levels[0] == 50);
    assert(m.poll(20000, Merger::LTP).empty()); // Hold last look, then reclaim expired state.
    m.receive(1, "b", b, 1, 20001);
    out = m.poll(20001, Merger::HTP);
    assert(out[0].levels[0] == 50 && out[0].levels[1] == 0);
    m.receive(1, "b", b, 3, 20002);
    m.poll(20002, Merger::HTP);
    m.receive(1, "b", b, 1, 20003);
    out = m.poll(20003, Merger::HTP);
    assert(out[0].levels[1] == 0 && out[0].levels[2] == 0); // Shorter frames release the old tail.
    m.clear();
    assert(m.poll(20001, Merger::HTP).empty());
}

static void testPriorityAndPap()
{
    Merger m(Merger::Protocol::SACN);
    uint8_t a[] { 200, 10, 240 }, b[] { 50, 150, 20 }, c[] { 100, 90, 220 };
    uint8_t ap[] { 100, 200, 0 }, bp[] { 120, 200, 110 };
    m.receive(1, "a", a, 3, 0, 100, 0, 0);
    assert(m.poll(0, Merger::HTP).empty()); // Wait for PAP; never publish priority bytes as levels.
    m.receive(1, "a", ap, 3, 1, 100, 0xdd, 1);
    m.receive(1, "b", bp, 3, 2, 100, 0xdd, 0);
    m.receive(1, "b", b, 3, 3, 100, 0, 1);
    m.receive(1, "c", c, 3, 4, 115, 0, 0);
    auto out = m.poll(1504, Merger::HTP);
    assert(out.size() == 1 && out[0].levels[0] == 50 && out[0].levels[1] == 150 && out[0].levels[2] == 220);
    out = m.poll(1505, Merger::LTP);
    assert(out[0].levels[0] == 50 && out[0].levels[1] == 150); // Priority always wins before LTP.
    ap[1] = 199;
    m.receive(1, "a", ap, 3, 1600, 100, 0xdd, 2);
    out = m.poll(1600, Merger::HTP);
    assert(out[0].levels[1] == 150 && out[0].levels[0] != 100); // PAP changes priority only.
    // Keep DMX alive while both PAP streams expire, then fall back to universe priorities.
    m.receive(1, "a", a, 3, 2000, 130, 0, 3);
    m.receive(1, "b", b, 3, 2000, 100, 0, 2);
    m.receive(1, "c", c, 3, 2000, 115, 0, 1);
    m.poll(2000, Merger::HTP);
    out = m.poll(4100, Merger::HTP);
    assert(out.size() == 1 && out[0].levels[0] == 200 && out[0].levels[1] == 10 && out[0].levels[2] == 240);
    m.receive(1, "a", nullptr, 0, 4101, 100, 0, 3, true); // Termination also works with repeated sequence.
    out = m.poll(4101, Merger::HTP);
    assert(out[0].levels[0] == 100);
    assert(m.poll(4500, Merger::HTP).empty());

    Merger shortPap(Merger::Protocol::SACN);
    shortPap.receive(1, "a", ap, 1, 0, 100, 0xdd);
    shortPap.receive(1, "a", a, 3, 1);
    out = shortPap.poll(1, Merger::HTP);
    assert(out[0].levels[0] == 200 && out[0].levels[1] == 0 && out[0].levels[2] == 0);
    Merger zeroPriority(Merger::Protocol::SACN);
    zeroPriority.receive(1, "a", a, 1, 0, 0);
    out = zeroPriority.poll(1500, Merger::HTP);
    assert(out[0].levels[0] == 200); // Universe priority 0 still sources data.
    uint8_t lowestPap[] { 1 }, lowerLevel[] { 20 };
    zeroPriority.receive(1, "b", lowestPap, 1, 1501, 100, 0xdd);
    zeroPriority.receive(1, "b", lowerLevel, 1, 1502);
    out = zeroPriority.poll(1502, Merger::HTP);
    assert(out[0].levels[0] == 200); // Universe zero and PAP one merge at equal priority.
}

static void testFilteringAndSequence()
{
    Merger m(Merger::Protocol::SACN);
    uint8_t a[] { 200 }, b[] { 10 }, invalid[] { 201 };
    assert(!m.receive(1, "a", a, 1, 0, 100, 0x17));
    assert(!m.receive(1, "a", invalid, 1, 0, 100, 0xdd));
    assert(!m.receive(1, "a", a, 513, 0));
    assert(!m.receive(1, "a", a, 1, 0, 201));
    assert(m.poll(2000, Merger::HTP).empty());
    assert(m.receive(1, "a", a, 1, 0, 100, 0, 254));
    assert(!m.receive(1, "a", b, 1, 1, 100, 0, 254));
    assert(!m.receive(1, "a", b, 1, 2, 100, 0xdd, 253));
    assert(m.receive(1, "a", b, 1, 3, 100, 0, 255));
    assert(m.receive(1, "a", a, 1, 4, 100, 0, 0));
    assert(m.receive(2, "a", b, 1, 5, 100, 0, 0));
    assert(m.receive(1, "b", b, 1, 6, 100, 0, 0));
    auto out = m.poll(1506, Merger::HTP);
    assert(out.size() == 2 && out[0].levels[0] == 200 && out[1].levels[0] == 10);
    assert(m.receive(1, "a", b, 1, 3000, 100, 0, 0)); // Expired source restarts its sequence.
    out = m.poll(4500, Merger::HTP);
    assert(out.size() == 1 && out[0].levels[0] == 10);
    Merger artnet(Merger::Protocol::ArtNet);
    assert(artnet.receive(1, "ip:0", a, 1, 0, 100, 0, 255));
    assert(artnet.receive(1, "ip:0", b, 1, 1, 100, 0, 1));
    assert(!artnet.receive(1, "ip:0", a, 1, 2, 100, 0, 255));
    assert(artnet.receive(1, "ip:0", b, 1, 3)); // Sequence zero disables checking.
}

int main()
{
    testHTPAndLTP();
    testPriorityAndPap();
    testFilteringAndSequence();
    std::cout << "DMX input merge regression tests passed\n";
}
