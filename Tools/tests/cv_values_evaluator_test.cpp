#include "../../Source/TimeMachine/Sequence/layers/cvvalues/CVValuesEvaluator.h"
#include <cassert>
#include <iostream>
#include <random>
#include <string>

using namespace CVValuesEvaluation;

static Timeline<double> timeline()
{
    Timeline<double> t;
    t.base = { 0, 10 };
    t.interpolate = [](size_t, double a, double b, double w) { return a + (b - a) * w; };
    return t;
}

static Block<double> block(double start, double length, double value)
{
    Block<double> b;
    b.start = start; b.length = length;
    b.channels.resize(2);
    b.channels[0].source = Source::Value;
    b.channels[0].constant.value = value;
    return b;
}

static void expect(const Timeline<double>& t, double time, double value, size_t ch = 0)
{
    const auto sample = t.evaluate(time)[ch];
    assert(sample.write && std::abs(sample.value - value) < 1.0e-8);
}

int main()
{
    assert(validRanges({}));
    assert(validRanges({ {0, 10}, {8, 18}, {18, 28} }));
    assert(!validRanges({ {0, 10}, {8, 18}, {9, 28} }));
    assert(!validRanges({ {0, 10}, {2, 5} }));
    assert(!validRanges({ {0, 10}, {0, 20} }));
    assert(!validRanges({ {0, 0} }));
    assert(!validRanges({ {-1, 10} }));

    auto t = timeline();
    expect(t, 20, 0);
    t.blocks = { block(2, 4, 100), block(10, 4, 200) };
    expect(t, 0, 0); expect(t, 2, 0); expect(t, 2.5, 50);
    expect(t, 3, 100); expect(t, 5.5, 50); expect(t, 6, 0);
    expect(t, 8, 0); expect(t, 14, 0); expect(t, 3, 10, 1);

    t.gapMode = GapMode::Interpolate;
    expect(t, 5.5, 100); expect(t, 6, 100); expect(t, 8, 150);
    expect(t, 10, 200); expect(t, 10.5, 200); expect(t, 14, 0);
    t.gapMode = GapMode::Hold;
    expect(t, 6, 100); expect(t, 8, 100); expect(t, 10, 100);
    expect(t, 10.5, 150); expect(t, 11, 200);

    t.gapMode = GapMode::Base;
    t.fadeIn = t.fadeOut = 4;
    t.blocks = { block(2, 2, 100) };
    expect(t, 2.5, 50); expect(t, 3, 100); expect(t, 3.5, 50);
    t.fadeIn = t.fadeOut = 0;
    expect(t, 2, 100); expect(t, 4, 0);

    t.blocks = { block(0, 10, 100), block(8, 10, 200) };
    expect(t, 8, 100); expect(t, 9, 150); expect(t, 10, 200);
    t.curve = [](double w) { return w * w; };
    expect(t, 9, 125);
    t.curve = {};
    t.blocks[0].channels[0].animation = [](double local) { return local * 10; };
    expect(t, 9, 145);
    t.blocks[1].channels[0].source = Source::Inherit;
    expect(t, 9, 90); expect(t, 11, 100);

    t.blocks[1].channels[0].source = Source::NoWrite;
    expect(t, 9, 90);
    assert(!t.evaluate(10)[0].write);
    expect(t, 18, 0);
    t.blocks[0].channels[0].source = Source::NoWrite;
    assert(!t.evaluate(9)[0].write);
    expect(t, 9, 10, 1);

    t.blocks = { block(0, 4, 100), block(6, 4, 200) };
    t.blocks[0].channels[0].animation = [](double local) { return local * 10; };
    t.blocks[1].channels[0].source = Source::Inherit;
    expect(t, 7, 0); // Base gap resets inheritance.
    t.gapMode = GapMode::Hold;
    expect(t, 7, 40);
    t.gapMode = GapMode::Interpolate;
    expect(t, 5, 40); expect(t, 7, 40);
    t.blocks.insert(t.blocks.begin() + 1, block(3, 2, 999));
    t.blocks[1].channels[0].source = Source::NoWrite;
    expect(t, 7, 40); // No-write blocks do not become authored history.
    t.gapMode = GapMode::Base;
    expect(t, 7, 0);

    t = timeline(); t.fadeIn = t.fadeOut = 0;
    t.blocks = { block(0, 10, 100), block(8, 10, 200) };
    t.blocks[1].channels[0].constant.mode = Mode::End;
    expect(t, 9, 100); expect(t, 10, 200);
    t.blocks[1].channels[0].constant.mode = Mode::Start;
    expect(t, 8, 200); expect(t, 8.001, 200);

    // Missing Hold endpoints fade from base; no-write inside an active block
    // stays authoritative even when a background fade would otherwise write.
    auto sparse = timeline(); sparse.gapMode = GapMode::Hold;
    sparse.blocks = { block(0, 2, 100), block(4, 2, 200) };
    sparse.blocks[0].channels[0].source = Source::NoWrite;
    expect(sparse, 3, 0); expect(sparse, 4, 0); expect(sparse, 4.5, 100);
    assert(!sparse.evaluate(.5)[0].write);
    sparse.blocks[1].channels[0].source = Source::NoWrite;
    assert(!sparse.evaluate(4.5)[0].write);

    // Fades reserve overlap spans, use proportional shortening, and the
    // display queries exactly the same effective durations as evaluation.
    auto edges = timeline(); edges.fadeIn = edges.fadeOut = 10;
    edges.blocks = { block(0, 4, 100), block(3, 4, 200), block(6, 4, 300) };
    assert(edges.edgeFades(0) == std::make_pair(3.0, 0.0));
    assert(edges.edgeFades(1) == std::make_pair(0.0, 0.0));
    assert(edges.edgeFades(2) == std::make_pair(0.0, 3.0));
    expect(edges, 1.5, 50); expect(edges, 3.5, 150); expect(edges, 8.5, 150);

    // Per-block controls replace automatic fades, including an explicit zero.
    auto manual = timeline(); manual.blocks = { block(2, 4, 100) };
    manual.blocks[0].manualFadeIn = true; manual.blocks[0].fadeIn = 0;
    expect(manual, 2, 100);
    manual.blocks[0].fadeIn = 2; expect(manual, 3, 50);
    manual.blocks[0].manualFadeOut = true; manual.blocks[0].fadeOut = 6;
    assert(manual.edgeFades(0) == std::make_pair(1.0, 3.0));
    expect(manual, 2.5, 50); expect(manual, 4.5, 50);
    manual.blocks[0].manualFadeIn = manual.blocks[0].manualFadeOut = false;
    expect(manual, 2, 0); expect(manual, 3, 100);
    manual.fadeIn = manual.fadeOut = 0;
    manual.blocks = { block(0, 10, 100), block(8, 10, 200) };
    manual.blocks[1].manualFadeIn = true; manual.blocks[1].fadeIn = 1;
    expect(manual, 8, 100); expect(manual, 8.5, 140); expect(manual, 9, 500.0 / 3);
    manual.blocks[1].channels[0].source = Source::NoWrite; expect(manual, 9, 100);
    manual.blocks[0].channels[0].source = Source::NoWrite; assert(!manual.evaluate(9)[0].write);
    manual.blocks = { block(0, 4, 100), block(1, 5, 200) };
    manual.blocks[0].manualFadeIn = manual.blocks[0].manualFadeOut = true;
    manual.blocks[0].fadeIn = manual.blocks[0].fadeOut = 3;
    expect(manual, 1, 50); expect(manual, 2, 125);
    manual.blocks[0].manualFadeOut = false;
    assert(manual.edgeFades(0).first == 1 && manual.displayFades(0).out == 3);
    expect(manual, 1, 100);
    assert(BlockTransitions::overlap(0, 10, 8, 18) == 2);
    assert(BlockTransitions::gain(.5, 4, { 1, 1 }) == .5);
    assert(BlockTransitions::withReservedOverlaps({ 0, 20 }, 10, { 2, 0 }).out == 8);

    // Evaluation order, playback direction, and visit history cannot affect output.
    t.blocks[1].channels[0].constant.mode = Mode::Interpolate;
    t.blocks[0].channels[0].animation = [](double local) { return 20 + local * local; };
    std::vector<std::pair<double, std::vector<Sample<double>>>> reference;
    for (int i = 0; i <= 220; ++i) reference.emplace_back(i * .1, t.evaluate(i * .1));
    std::mt19937 random(42);
    std::shuffle(reference.begin(), reference.end(), random);
    for (const auto& visit : reference)
    {
        auto actual = t.evaluate(visit.first);
        for (size_t ch = 0; ch < actual.size(); ++ch)
        { assert(actual[ch].write == visit.second[ch].write); assert(actual[ch].value == visit.second[ch].value); }
    }

    Timeline<std::string> text;
    text.base = { "base" };
    text.interpolate = [](size_t, const std::string& a, const std::string& b, double w) { return w < 1 ? a : b; };
    Block<std::string> label;
    label.start = 2; label.length = 4; label.channels.resize(1);
    label.channels[0].source = Source::Value;
    label.channels[0].constant = { "hello", true, Mode::End };
    text.blocks = { label };
    assert(text.evaluate(2.5)[0].value == "base");
    assert(text.evaluate(3)[0].value == "hello");
    std::cout << "CV Values evaluator passed\n";
}
