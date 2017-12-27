#pragma once
#include <ossia/editor/curve/curve_segment/easing.hpp>
namespace ossia
{
template <typename Y>
struct curve_segment_linear
{
  Y operator()(double ratio, Y start, Y end) const
  {
    return ossia::easing::ease{}(start, end, ratio);
  }
};
}
