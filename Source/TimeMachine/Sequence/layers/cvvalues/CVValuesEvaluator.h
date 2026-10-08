#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <utility>
#include <vector>

// Independent of JUCE and target containers: evaluation never reads live values.
namespace CVValuesEvaluation
{
enum class GapMode { Base, Interpolate, Hold };
enum class Mode { Interpolate, Start, End };
enum class Source { Inherit, Value, NoWrite };

template<class T> struct Sample
{
    T value{};
    bool write = true;
    Mode mode = Mode::Interpolate;
};

template<class T> struct Channel
{
    Source source = Source::Inherit;
    Sample<T> constant;
    std::function<T(double)> animation;
};

template<class T> struct Block
{
    double start = 0, length = 1;
    std::vector<Channel<T>> channels;
    double end() const { return start + length; }
};

struct Range { double start, end; };

// Sorted blocks must have strictly increasing starts and ends, and at most two
// simultaneous contributors. Endpoints are half-open, so adjacency is legal.
inline bool validRanges(const std::vector<Range>& ranges)
{
    for (size_t i = 0; i < ranges.size(); ++i)
    {
        const auto& r = ranges[i];
        if (!std::isfinite(r.start) || !std::isfinite(r.end) || r.start < 0 || r.end <= r.start) return false;
        if (i > 0 && (r.start <= ranges[i - 1].start || r.end <= ranges[i - 1].end)) return false;
        if (i > 1 && r.start < ranges[i - 2].end) return false;
    }
    return true;
}

template<class T> struct Timeline
{
    std::vector<T> base;
    std::vector<Block<T>> blocks;
    GapMode gapMode = GapMode::Base;
    double fadeIn = 1, fadeOut = 1;
    std::function<double(double)> curve;
    std::function<T(size_t, const T&, const T&, double)> interpolate;

    double weight(double w) const
    {
        w = std::clamp(w, 0.0, 1.0);
        return w == 0 || w == 1 ? w : std::clamp(curve ? curve(w) : w, 0.0, 1.0);
    }

    // Shared by evaluation and the timeline's fade-region display.
    std::pair<double, double> edgeFades(size_t index) const
    {
        const auto& b = blocks[index];
        const auto* p = index > 0 ? &blocks[index - 1] : nullptr;
        const auto* n = index + 1 < blocks.size() ? &blocks[index + 1] : nullptr;
        double in = p && (p->end() > b.start || gapMode == GapMode::Interpolate) ? 0 : std::max(0.0, fadeIn);
        double out = n && (n->start < b.end() || gapMode != GapMode::Base) ? 0 : std::max(0.0, fadeOut);
        const double available = std::max(0.0, b.length - (p ? std::max(0.0, p->end() - b.start) : 0)
            - (n ? std::max(0.0, b.end() - n->start) : 0));
        if (in + out > available && in + out > 0)
        { const double scale = available / (in + out); in *= scale; out *= scale; }
        return { in, out };
    }

    Sample<T> blend(size_t channel, Sample<T> a, Sample<T> b, double w) const
    {
        if (!a.write) return b;
        if (!b.write) return a;
        w = weight(w);
        if (b.mode == Mode::Start) return b;
        if (w <= 0) return a;
        if (w >= 1) return b;
        if (b.mode == Mode::End) return a;
        return { interpolate(channel, a.value, b.value, w), true, b.mode };
    }

    Sample<T> resolve(size_t index, size_t channel, double time) const
    {
        const auto& b = blocks[index];
        const auto& c = b.channels[channel];
        if (c.source == Source::NoWrite) return { {}, false };
        if (c.source == Source::Value)
        {
            auto result = c.constant;
            if (c.animation) result.value = c.animation(std::clamp(time - b.start, 0.0, b.length));
            return result;
        }
        // Walk authored history, including gaps crossed through no-write blocks.
        for (size_t i = index; i > 0; --i)
        {
            if (gapMode == GapMode::Base && blocks[i - 1].end() < blocks[i].start) break;
            const auto& prev = blocks[i - 1].channels[channel];
            if (prev.source == Source::Value) return resolve(i - 1, channel, time);
        }
        return { base[channel] };
    }

    std::vector<Sample<T>> evaluate(double time) const
    {
        std::vector<Sample<T>> result;
        result.reserve(base.size());
        int first = -1, second = -1, previous = -1, next = -1;
        for (size_t i = 0; i < blocks.size(); ++i)
        {
            const auto& b = blocks[i];
            if (b.start <= time && time < b.end())
            {
                if (first < 0) first = static_cast<int>(i);
                else second = static_cast<int>(i);
            }
            if (b.end() <= time) previous = static_cast<int>(i);
            if (next < 0 && b.start > time) next = static_cast<int>(i);
        }
        for (size_t ch = 0; ch < base.size(); ++ch)
        {
            Sample<T> background{ base[ch] };
            if (second >= 0)
            {
                const auto& a = blocks[first];
                const auto& b = blocks[second];
                result.push_back(blend(ch, resolve(first, ch, time), resolve(second, ch, time),
                    (time - b.start) / (a.end() - b.start)));
            }
            else if (first >= 0)
            {
                const auto& b = blocks[first];
                auto value = resolve(first, ch, time);
                if (!value.write) { result.push_back(value); continue; }
                const bool hasPrevious = first > 0;
                const auto* p = hasPrevious ? &blocks[first - 1] : nullptr;
                const auto fades = edgeFades(static_cast<size_t>(first));
                const double in = fades.first, out = fades.second;
                if (in > 0 && time < b.start + in)
                {
                    auto from = p && gapMode == GapMode::Hold ? resolve(first - 1, ch, p->end()) : background;
                    if (!from.write) from = background;
                    value = blend(ch, from, value, (time - b.start) / in);
                }
                if (out > 0 && time > b.end() - out)
                    value = blend(ch, value, background, (time - (b.end() - out)) / out);
                result.push_back(value);
            }
            else if (previous >= 0 && next >= 0 && gapMode != GapMode::Base)
            {
                auto from = resolve(previous, ch, blocks[previous].end());
                if (!from.write) from = background;
                if (gapMode == GapMode::Hold) result.push_back(from);
                else
                {
                    auto to = resolve(next, ch, blocks[next].start);
                    if (!to.write) to = background;
                    result.push_back(blend(ch, from, to,
                        (time - blocks[previous].end()) / (blocks[next].start - blocks[previous].end())));
                }
            }
            else result.push_back(background);
        }
        return result;
    }
};
}
