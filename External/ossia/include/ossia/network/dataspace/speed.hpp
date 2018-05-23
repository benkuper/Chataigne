#pragma once
#include <ossia/network/dataspace/dataspace_base.hpp>
#include <ossia/network/dataspace/dataspace_base_defs_fwd.hpp>

namespace ossia
{

template <typename T>
struct speed_ratio;

using meter_per_second_u = speed_ratio<std::ratio<1>>;
using miles_per_hour_u
    = speed_ratio<std::ratio<254 * 12 * 5280, 3600 * 10000>>;
using kilometer_per_hour_u = speed_ratio<std::ratio<1000, 3600>>;
using knot_u = speed_ratio<std::ratio<1852, 3600>>;
using foot_per_hour_u = speed_ratio<std::ratio<254 * 12, 3600 * 10000>>;
using foot_per_second_u = speed_ratio<std::ratio<254 * 12, 10000>>;
struct speed_dataspace;
template <typename Impl>
struct speed_unit
{
  using is_unit = std::true_type;
  using neutral_unit = meter_per_second_u;
  using value_type = float;
  using concrete_type = Impl;
  using dataspace_type = speed_dataspace;
};

template <typename T>
struct speed_ratio : public linear_unit<speed_unit<speed_ratio<T>>, T>
{
  using linear_unit<speed_unit<speed_ratio<T>>, T>::linear_unit;
};

template <>
struct unit_traits<meter_per_second_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("m/s");
  }
};

template <>
struct unit_traits<miles_per_hour_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("mph", "mi/h");
  }
};

template <>
struct unit_traits<kilometer_per_hour_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("km/h", "kmph");
  }
};

template <>
struct unit_traits<knot_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("kn");
  }
};

template <>
struct unit_traits<foot_per_hour_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("ft/h");
  }
};

template <>
struct unit_traits<foot_per_second_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("ft/s");
  }
};
}
