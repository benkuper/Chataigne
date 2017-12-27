#pragma once
#include <ossia/detail/config.hpp>
#include <array>
#include <ossia_export.h>

namespace ossia
{
class value;

using vec2f = std::array<float, 2ul>;
using vec3f = std::array<float, 3ul>;
using vec4f = std::array<float, 4ul>;

inline vec2f make_vec(float f1, float f2)
{
  return std::array<float, 2>{f1, f2};
}
inline vec3f make_vec(float f1, float f2, float f3)
{
  return std::array<float, 3>{f1, f2, f3};
}
inline vec4f make_vec(float f1, float f2, float f3, float f4)
{
  return std::array<float, 4>{f1, f2, f3, f4};
}

template <std::size_t N>
std::array<float, N> fill_vec(float f)
{
  std::array<float, N> arr;
  arr.fill(f);
  return arr;
}
}
