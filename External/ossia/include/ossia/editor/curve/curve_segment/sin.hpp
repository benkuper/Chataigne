#pragma once
#include <cmath>
namespace ossia
{
template <typename Y>
struct curve_segment_sin
{
  auto operator()(double freq, double phase, double ampl) const
  {
    return [=](double ratio, Y start, Y end) {
      return start
             + ampl * std::sin(phase + two_pi * ratio * freq) * (end - start);
    };
  }
};
}
