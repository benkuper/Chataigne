#pragma once
#include <ossia/detail/algorithms.hpp>
#include <ossia/network/dataspace/dataspace_base_defs_fwd.hpp>

namespace ossia
{
template <>
struct dataspace_traits<angle_u>
{
  using neutral_unit = radian_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("angle");
  }
};

template <>
struct dataspace_traits<color_u>
{
  using neutral_unit = argb_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("color");
  }
};

template <>
struct dataspace_traits<distance_u>
{
  using neutral_unit = meter_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("distance");
  }
};

template <>
struct dataspace_traits<gain_u>
{
  using neutral_unit = linear_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("gain");
  }
};

template <>
struct dataspace_traits<orientation_u>
{
  using neutral_unit = quaternion_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("orientation");
  }
};
template <>
struct dataspace_traits<position_u>
{
  using neutral_unit = cartesian_3d_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("position");
  }
};

template <>
struct dataspace_traits<speed_u>
{
  using neutral_unit = meter_per_second_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("speed");
  }
};

template <>
struct dataspace_traits<timing_u>
{
  using neutral_unit = second_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("time");
  }
};
}
