#pragma once
#include <ossia/network/dataspace/dataspace_base.hpp>

namespace ossia
{

template <typename T>
struct distance_ratio;
struct distance_dataspace;

using meter_u = distance_ratio<std::ratio<1>>;
using kilometer_u = distance_ratio<std::kilo>;
using decimeter_u = distance_ratio<std::deci>;
using centimeter_u = distance_ratio<std::centi>;
using millimeter_u = distance_ratio<std::milli>;
using micrometer_u = distance_ratio<std::micro>;
using nanometer_u = distance_ratio<std::nano>;
using picometer_u = distance_ratio<std::pico>;
using inch_u = distance_ratio<std::ratio<254, 10000>>;
using foot_u = distance_ratio<std::ratio<254 * 12, 10000>>;
using mile_u = distance_ratio<std::ratio<254 * 12 * 5280, 10000>>;

template <typename Impl>
struct distance_unit
{
  using is_unit = std::true_type;
  using neutral_unit = meter_u;
  using value_type = float;
  using concrete_type = Impl;
  using dataspace_type = distance_dataspace;
};

template <typename T>
struct distance_ratio : public linear_unit<distance_unit<distance_ratio<T>>, T>
{
  using linear_unit<distance_unit<distance_ratio<T>>, T>::linear_unit;
};

struct OSSIA_EXPORT pixel_u : public distance_unit<pixel_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("px", "pixels");
  }
  using value_type = float;
  double ppm{}; // pixels per meter

  void set_ppcm(double v) { ppm = v * 100.; }
  void set_dpi(double v) { ppm = v / 254.; }
  strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return float(self.dataspace_value / ppm);
  }

  value_type from_neutral(strong_value<neutral_unit> self)
  {
    return float(self.dataspace_value * ppm);
  }
};


template <>
struct unit_traits<meter_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("m", "meters");
  }
};

template <>
struct unit_traits<kilometer_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("km", "kilometers");
  }
};
template <>
struct unit_traits<decimeter_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("dm", "decimeters");
  }
};
template <>
struct unit_traits<centimeter_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("cm", "centimeters");
  }
};
template <>
struct unit_traits<millimeter_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("mm", "millimeters");
  }
};
template <>
struct unit_traits<micrometer_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("um", "micrometers");
  }
};
template <>
struct unit_traits<nanometer_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("nm", "nanometers");
  }
};
template <>
struct unit_traits<picometer_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("pm", "picometers");
  }
};

template <>
struct unit_traits<inch_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("inches", "\"");
  }
};
template <>
struct unit_traits<foot_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("feet", "'");
  }
};
template <>
struct unit_traits<mile_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("miles");
  }
};
}
