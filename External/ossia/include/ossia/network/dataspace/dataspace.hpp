#pragma once
#include <ossia/network/dataspace/angle.hpp>
#include <ossia/network/dataspace/color.hpp>
#include <ossia/network/dataspace/dataspace_fwd.hpp>
#include <ossia/network/dataspace/distance.hpp>
#include <ossia/network/dataspace/gain.hpp>
#include <ossia/network/dataspace/orientation.hpp>
#include <ossia/network/dataspace/position.hpp>
#include <ossia/network/dataspace/speed.hpp>
#include <ossia/network/dataspace/temperature.hpp>
#include <ossia/network/dataspace/time.hpp>
#include <brigand/algorithms/fold.hpp>
#include <functional>
namespace ossia
{
#include <ossia/network/dataspace/dataspace_base_variants.hpp>
//! A unit
struct OSSIA_EXPORT unit_t final
{
  unit_variant v;

  unit_t() = default;
  unit_t(const unit_t&) = default;
  unit_t(unit_t&&) = default;
  unit_t& operator=(const unit_t&) = default;
  unit_t& operator=(unit_t&&) = default;

  unit_t(angle_u t) : v{t}
  {
  }
  unit_t(degree_u t) : v{angle_u{t}}
  {
  }
  unit_t(radian_u t) : v{angle_u{t}}
  {
  }

  unit_t(color_u t) : v{t}
  {
  }
  unit_t(argb_u t) : v{color_u{t}}
  {
  }
  unit_t(rgba_u t) : v{color_u{t}}
  {
  }
  unit_t(rgb_u t) : v{color_u{t}}
  {
  }
  unit_t(bgr_u t) : v{color_u{t}}
  {
  }
  unit_t(argb8_u t) : v{color_u{t}}
  {
  }
  unit_t(hsv_u t) : v{color_u{t}}
  {
  }
  unit_t(cmy8_u t) : v{color_u{t}}
  {
  }
  unit_t(xyz_u t) : v{color_u{t}}
  {
  }

  unit_t(distance_u t) : v{t}
  {
  }
  unit_t(meter_u t) : v{distance_u{t}}
  {
  }
  unit_t(kilometer_u t) : v{distance_u{t}}
  {
  }
  unit_t(decimeter_u t) : v{distance_u{t}}
  {
  }
  unit_t(centimeter_u t) : v{distance_u{t}}
  {
  }
  unit_t(millimeter_u t) : v{distance_u{t}}
  {
  }
  unit_t(micrometer_u t) : v{distance_u{t}}
  {
  }
  unit_t(nanometer_u t) : v{distance_u{t}}
  {
  }
  unit_t(picometer_u t) : v{distance_u{t}}
  {
  }
  unit_t(inch_u t) : v{distance_u{t}}
  {
  }
  unit_t(foot_u t) : v{distance_u{t}}
  {
  }
  unit_t(mile_u t) : v{distance_u{t}}
  {
  }

  unit_t(gain_u t) : v{t}
  {
  }
  unit_t(linear_u t) : v{gain_u{t}}
  {
  }
  unit_t(midigain_u t) : v{gain_u{t}}
  {
  }
  unit_t(decibel_u t) : v{gain_u{t}}
  {
  }
  unit_t(decibel_raw_u t) : v{gain_u{t}}
  {
  }

  unit_t(orientation_u t) : v{t}
  {
  }
  unit_t(quaternion_u t) : v{orientation_u{t}}
  {
  }
  unit_t(euler_u t) : v{orientation_u{t}}
  {
  }
  unit_t(axis_u t) : v{orientation_u{t}}
  {
  }

  unit_t(position_u t) : v{t}
  {
  }
  unit_t(cartesian_3d_u t) : v{position_u{t}}
  {
  }
  unit_t(cartesian_2d_u t) : v{position_u{t}}
  {
  }
  unit_t(spherical_u t) : v{position_u{t}}
  {
  }
  unit_t(polar_u t) : v{position_u{t}}
  {
  }
  unit_t(opengl_u t) : v{position_u{t}}
  {
  }
  unit_t(cylindrical_u t) : v{position_u{t}}
  {
  }

  unit_t(speed_u t) : v{t}
  {
  }
  unit_t(meter_per_second_u t) : v{speed_u{t}}
  {
  }
  unit_t(miles_per_hour_u t) : v{speed_u{t}}
  {
  }
  unit_t(kilometer_per_hour_u t) : v{speed_u{t}}
  {
  }
  unit_t(knot_u t) : v{speed_u{t}}
  {
  }
  unit_t(foot_per_hour_u t) : v{speed_u{t}}
  {
  }
  unit_t(foot_per_second_u t) : v{speed_u{t}}
  {
  }

  unit_t(timing_u t) : v{t}
  {
  }
  unit_t(second_u t) : v{timing_u{t}}
  {
  }
  unit_t(bark_u t) : v{timing_u{t}}
  {
  }
  unit_t(bpm_u t) : v{timing_u{t}}
  {
  }
  unit_t(cent_u t) : v{timing_u{t}}
  {
  }
  unit_t(frequency_u t) : v{timing_u{t}}
  {
  }
  unit_t(mel_u t) : v{timing_u{t}}
  {
  }
  unit_t(midi_pitch_u t) : v{timing_u{t}}
  {
  }
  unit_t(millisecond_u t) : v{timing_u{t}}
  {
  }
  unit_t(playback_speed_u t) : v{timing_u{t}}
  {
  }

  template <typename T>
  unit_t(T unit, typename T::is_unit* = nullptr) : v{unit}
  {
  }

  auto which() const
  {
    return v.which();
  }
  operator bool() const
  {
    return bool(v);
  }

  friend bool operator==(const ossia::unit_t& unit, const ossia::unit_t& other)
  {
    return unit.v == other.v;
  }

  friend bool operator!=(const ossia::unit_t& unit, const ossia::unit_t& other)
  {
    return unit.v != other.v;
  }
};

template <typename T>
struct is_unit
{
  template <typename U>
  static char test(typename U::is_unit* x);

  template <typename U>
  static long test(U* x);

  static const bool value = sizeof(test<T>(0)) == 1;
};

template <typename T, typename U>
typename std::enable_if_t<is_unit<T>::value && is_unit<U>::value, bool>
operator==(T, U)
{
  return false;
}
template <typename T, typename U>
typename std::enable_if_t<is_unit<T>::value && is_unit<U>::value, bool>
operator!=(T, U)
{
  return true;
}

template <typename T>
typename std::enable_if_t<is_unit<T>::value, bool> operator==(T, T)
{
  return true;
}
template <typename T>
typename std::enable_if_t<is_unit<T>::value, bool> operator!=(T, T)
{
  return false;
}

static const constexpr int dataspace_count = 8;
static const constexpr int unit_count = 2 + 8 + 11 + 4 + 3 + 6 + 6 + 9;
}
namespace std
{
template <>
struct hash<ossia::distance_u>
{
  std::size_t operator()(const ossia::distance_u& k) const
  {
    return k.which();
  }
};
template <>
struct hash<ossia::position_u>
{
  std::size_t operator()(const ossia::position_u& k) const
  {
    return k.which();
  }
};
template <>
struct hash<ossia::speed_u>
{
  std::size_t operator()(const ossia::speed_u& k) const
  {
    return k.which();
  }
};
template <>
struct hash<ossia::orientation_u>
{
  std::size_t operator()(const ossia::orientation_u& k) const
  {
    return k.which();
  }
};
template <>
struct hash<ossia::angle_u>
{
  std::size_t operator()(const ossia::angle_u& k) const
  {
    return k.which();
  }
};
template <>
struct hash<ossia::color_u>
{
  std::size_t operator()(const ossia::color_u& k) const
  {
    return k.which();
  }
};
template <>
struct hash<ossia::gain_u>
{
  std::size_t operator()(const ossia::gain_u& k) const
  {
    return k.which();
  }
};
template <>
struct hash<ossia::timing_u>
{
  std::size_t operator()(const ossia::timing_u& k) const
  {
    return k.which();
  }
};
}
