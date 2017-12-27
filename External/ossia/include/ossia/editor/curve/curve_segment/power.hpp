#pragma once
#include <cmath>

namespace ossia
{
template <typename Y>
struct curve_segment_power
{
  auto operator()(double power) const
  {
    return [=](double ratio, Y start, Y end) -> Y {
      return start + std::pow(ratio, power) * (end - start);
    };
  }
};
}
