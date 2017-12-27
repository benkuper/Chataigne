#pragma once

namespace ossia
{
template <typename Y>
struct curve_segment_empty
{
  Y operator()(double ratio, Y start, Y end) const
  {
    return start;
  }
};
}
