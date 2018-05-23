#pragma once
#include <ossia/detail/config.hpp>
#include <ratio>

namespace ossia
{
class value;
template <typename Unit>
struct strong_value;

// 2
struct degree_u;
struct radian_u;

using degree = strong_value<degree_u>;
using radian = strong_value<radian_u>;

// 8
struct argb_u;
struct rgba_u;
struct rgb_u;
struct bgr_u;
struct argb8_u;
struct hsv_u;
struct cmy8_u;
struct xyz_u;

// unused:
struct hsl_u;
struct cmyk8_u;
struct xyz_u;
struct yxy_u;
struct hunter_lab_u;
struct cie_lab_u;
struct cie_luv_u;

using argb = strong_value<argb_u>;
using rgba = strong_value<rgba_u>;
using rgb = strong_value<rgb_u>;
using bgr = strong_value<bgr_u>;
using argb8 = strong_value<argb8_u>;
using hsv = strong_value<hsv_u>;
using cmy8 = strong_value<cmy8_u>;

using hsl = strong_value<hsl_u>;
using cmyk8 = strong_value<cmyk8_u>;
using xyz = strong_value<xyz_u>;
using yxy = strong_value<yxy_u>;
using hunter_lab = strong_value<hunter_lab_u>;
using cie_lab = strong_value<cie_lab_u>;
using cie_luv = strong_value<cie_luv_u>;

template <typename T>
struct distance_ratio;

// 11
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

using meter = strong_value<meter_u>;
using kilometer = strong_value<kilometer_u>;
using decimeter = strong_value<decimeter_u>;
using centimeter = strong_value<centimeter_u>;
using millimeter = strong_value<millimeter_u>;
using micrometer = strong_value<micrometer_u>;
using nanometer = strong_value<nanometer_u>;
using picometer = strong_value<picometer_u>;
using inch = strong_value<inch_u>;
using foot = strong_value<foot_u>;
using mile = strong_value<mile_u>;

// 4
struct linear_u;
struct midigain_u;
struct decibel_u;
struct decibel_raw_u;

using linear = strong_value<linear_u>;
using midigain = strong_value<midigain_u>;
using decibel = strong_value<decibel_u>;
using decibel_raw = strong_value<decibel_raw_u>;

// 3
struct quaternion_u;
struct euler_u;
struct axis_u;

using quaternion = strong_value<quaternion_u>;
using euler = strong_value<euler_u>;
using axis = strong_value<axis_u>;

// 6
struct cartesian_3d_u;
struct cartesian_2d_u;
struct spherical_u;
struct polar_u;
struct opengl_u;
struct cylindrical_u;

using cartesian_3d = strong_value<cartesian_3d_u>;
using cartesian_2d = strong_value<cartesian_2d_u>;
using spherical = strong_value<spherical_u>;
using polar = strong_value<polar_u>;
using opengl = strong_value<opengl_u>;
using cylindrical = strong_value<cylindrical_u>;

template <typename T>
struct speed_ratio;

// 6
using meter_per_second_u = speed_ratio<std::ratio<1>>;
using miles_per_hour_u
    = speed_ratio<std::ratio<254 * 12 * 5280, 3600 * 10000>>;
using kilometer_per_hour_u = speed_ratio<std::ratio<1000, 3600>>;
using knot_u = speed_ratio<std::ratio<1852, 3600>>;
using foot_per_hour_u = speed_ratio<std::ratio<254 * 12, 3600 * 10000>>;
using foot_per_second_u = speed_ratio<std::ratio<254 * 12, 10000>>;

using meter_per_second = strong_value<meter_per_second_u>;
using miles_per_hour = strong_value<miles_per_hour_u>;
using kilometer_per_hour = strong_value<kilometer_per_hour_u>;
using knot = strong_value<knot_u>;
using foot_per_second = strong_value<foot_per_second_u>;
using foot_per_hour = strong_value<foot_per_hour_u>;

// 9
struct second_u;
struct bark_u;
struct bpm_u;
struct cent_u;
struct frequency_u;
struct mel_u;
struct midi_pitch_u;
struct millisecond_u;
struct playback_speed_u;

using second = strong_value<second_u>;
using bark = strong_value<bark_u>;
using bpm = strong_value<bpm_u>;
using cent = strong_value<cent_u>;
using frequency = strong_value<frequency_u>;
using mel = strong_value<mel_u>;
using midi_pitch = strong_value<midi_pitch_u>;
using millisecond = strong_value<millisecond_u>;
using playback_speed = strong_value<playback_speed_u>;

/// Dataspaces ///
struct angle_u;
struct color_u;
struct distance_u;
struct orientation_u;
struct gain_u;
struct position_u;
struct speed_u;
struct timing_u;

struct angle;
struct color;
struct distance;
struct orientation;
struct gain;
struct position;
struct speed;
struct timing;

struct unit_t;
struct value_with_unit;

template <typename T>
struct dataspace_traits : public std::false_type
{
};
}
