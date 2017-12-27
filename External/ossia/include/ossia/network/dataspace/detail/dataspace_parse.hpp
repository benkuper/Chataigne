#pragma once
#include <ossia/detail/string_map.hpp>
#include <ossia/detail/string_view.hpp>
#include <ossia/network/dataspace/dataspace.hpp>
#include <ossia/network/dataspace/dataspace_parse.hpp>
#include <ossia/network/dataspace/dataspace_visitors.hpp>
#include <brigand/algorithms/for_each.hpp>
#include <brigand/algorithms/wrap.hpp>
namespace ossia
{
template <typename T>
struct matching_unit_u_list;

using angle_u_list = brigand::list<degree_u, radian_u>;

using color_u_list =
brigand::list<
argb_u, rgba_u, rgb_u, bgr_u, argb8_u, hsv_u, cmy8_u, xyz_u
/*, hsl_u, cmyk8_u, yxy_u, hunter_lab_u, cie_lab_u, cie_luv_u*/>;

using distance_u_list = brigand::
    list<meter_u, kilometer_u, decimeter_u, centimeter_u, millimeter_u, micrometer_u, nanometer_u, picometer_u, inch_u, foot_u, mile_u>;

using gain_u_list
    = brigand::list<linear_u, midigain_u, decibel_u, decibel_raw_u>;

using orientation_u_list = brigand::list<quaternion_u, euler_u, axis_u>;

using position_u_list = brigand::
    list<cartesian_3d_u, cartesian_2d_u, spherical_u, polar_u, opengl_u, cylindrical_u>;

using speed_u_list = brigand::
    list<meter_per_second_u, miles_per_hour_u, kilometer_per_hour_u, knot_u, foot_per_second_u, foot_per_hour_u>;

using timing_u_list = brigand::
    list<second_u, bark_u, bpm_u, cent_u, frequency_u, mel_u, midi_pitch_u, millisecond_u, playback_speed_u>;

using dataspace_u_list = brigand::
    list<distance_u_list, position_u_list, speed_u_list, orientation_u_list, angle_u_list, color_u_list, gain_u_list, timing_u_list>;

template <>
struct matching_unit_u_list<angle_u>
{
  using type = angle_u_list;
};
template <>
struct matching_unit_u_list<color_u>
{
  using type = color_u_list;
};
template <>
struct matching_unit_u_list<distance_u>
{
  using type = distance_u_list;
};
template <>
struct matching_unit_u_list<position_u>
{
  using type = position_u_list;
};
template <>
struct matching_unit_u_list<orientation_u>
{
  using type = orientation_u_list;
};
template <>
struct matching_unit_u_list<gain_u>
{
  using type = gain_u_list;
};
template <>
struct matching_unit_u_list<speed_u>
{
  using type = speed_u_list;
};
template <>
struct matching_unit_u_list<timing_u>
{
  using type = timing_u_list;
};

template <>
struct matching_unit_u_list<angle_u_list>
{
  using type = angle_u;
};
template <>
struct matching_unit_u_list<color_u_list>
{
  using type = color_u;
};
template <>
struct matching_unit_u_list<distance_u_list>
{
  using type = distance_u;
};
template <>
struct matching_unit_u_list<position_u_list>
{
  using type = position_u;
};
template <>
struct matching_unit_u_list<orientation_u_list>
{
  using type = orientation_u;
};
template <>
struct matching_unit_u_list<gain_u_list>
{
  using type = gain_u;
};
template <>
struct matching_unit_u_list<speed_u_list>
{
  using type = speed_u;
};
template <>
struct matching_unit_u_list<timing_u_list>
{
  using type = timing_u;
};

template <typename T>
struct matching_unit_list;

using angle_list = brigand::list<degree, radian>;

using color_list =
brigand::list<
argb, rgba, rgb, bgr, argb8, hsv, cmy8, xyz
/*, hsl, cmyk8, yxy, hunter_lab, cie_lab, cie_luv_u*/>;

using distance_list = brigand::
    list<meter, kilometer, decimeter, centimeter, millimeter, micrometer, nanometer, picometer, inch, foot, mile>;

using gain_list = brigand::list<linear, midigain, decibel, decibel_raw>;

using orientation_list = brigand::list<quaternion, euler, axis>;

using position_list = brigand::
    list<cartesian_3d, cartesian_2d, spherical, polar, opengl, cylindrical>;

using speed_list = brigand::
    list<meter_per_second, miles_per_hour, kilometer_per_hour, knot, foot_per_second, foot_per_hour>;

using time_list = brigand::
    list<second, bark, bpm, cent, frequency, mel, midi_pitch, millisecond, playback_speed>;

using dataspace_list = brigand::
    list<distance_list, position_list, speed_list, orientation_list, angle_list, color_list, gain_list, time_list>;

template <>
struct matching_unit_list<angle>
{
  using type = angle_list;
};
template <>
struct matching_unit_list<color>
{
  using type = color_list;
};
template <>
struct matching_unit_list<distance>
{
  using type = distance_list;
};
template <>
struct matching_unit_list<position>
{
  using type = position_list;
};
template <>
struct matching_unit_list<orientation>
{
  using type = orientation_list;
};
template <>
struct matching_unit_list<gain>
{
  using type = gain_list;
};
template <>
struct matching_unit_list<speed>
{
  using type = speed_list;
};
template <>
struct matching_unit_list<timing>
{
  using type = time_list;
};

template <>
struct matching_unit_list<angle_list>
{
  using type = angle;
};
template <>
struct matching_unit_list<color_list>
{
  using type = color;
};
template <>
struct matching_unit_list<distance_list>
{
  using type = distance;
};
template <>
struct matching_unit_list<position_list>
{
  using type = position;
};
template <>
struct matching_unit_list<orientation_list>
{
  using type = orientation;
};
template <>
struct matching_unit_list<gain_list>
{
  using type = gain;
};
template <>
struct matching_unit_list<speed_list>
{
  using type = speed;
};
template <>
struct matching_unit_list<time_list>
{
  using type = timing;
};

template <>
struct dataspace_traits<angle_u_list>
{
    using neutral_unit = radian_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("angle");
  }
};

template <>
struct dataspace_traits<color_u_list>
{
  using neutral_unit = argb_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("color");
  }
};

template <>
struct dataspace_traits<distance_u_list>
{
  using neutral_unit = meter_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("distance");
  }
};

template <>
struct dataspace_traits<gain_u_list>
{
  using neutral_unit = linear_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("gain");
  }
};

template <>
struct dataspace_traits<orientation_u_list>
{
  using neutral_unit = quaternion_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("orientation");
  }
};
template <>
struct dataspace_traits<position_u_list>
{
  using neutral_unit = cartesian_3d_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("position");
  }
};

template <>
struct dataspace_traits<speed_u_list>
{
  using neutral_unit = meter_per_second_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("speed");
  }
};

template <>
struct dataspace_traits<timing_u_list>
{
  using neutral_unit = second_u;
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("time");
  }
};

namespace detail
{
struct unit_text_visitor
{
  OSSIA_INLINE ossia::string_view operator()(const angle_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const color_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const distance_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const gain_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const position_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const orientation_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const speed_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const timing_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }

  template <typename Unit>
  OSSIA_INLINE ossia::string_view operator()(Unit)
  {
    return ossia::unit_traits<Unit>::text()[0];
  }

  OSSIA_INLINE ossia::string_view operator()(const ossia::value&)
  {
    return {};
  }

  OSSIA_INLINE ossia::string_view operator()()
  {
    return {};
  }
};

struct unit_accessor_visitor
{
  OSSIA_INLINE ossia::string_view operator()(const angle_u& dataspace)
  {
    return {};
  }
  OSSIA_INLINE ossia::string_view operator()(const color_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const distance_u& dataspace)
  {
    return {};
  }
  OSSIA_INLINE ossia::string_view operator()(const gain_u& dataspace)
  {
    return {};
  }
  OSSIA_INLINE ossia::string_view operator()(const position_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const orientation_u& dataspace)
  {
    return ossia::apply(*this, dataspace);
  }
  OSSIA_INLINE ossia::string_view operator()(const speed_u& dataspace)
  {
    return {};
  }
  OSSIA_INLINE ossia::string_view operator()(const timing_u& dataspace)
  {
    return {};
  }

  template <typename Unit>
  OSSIA_INLINE ossia::string_view operator()(Unit)
  {
    return Unit::array_parameters();
  }

  OSSIA_INLINE ossia::string_view operator()()
  {
    return {};
  }
};

struct dataspace_text_visitor
{
  template <typename Dataspace>
  OSSIA_INLINE ossia::string_view operator()(const Dataspace& dataspace)
  {
    return ossia::dataspace_traits<Dataspace>::text()[0];
  }

  OSSIA_INLINE ossia::string_view operator()()
  {
    return {};
  }
};

using unit_map = string_view_map<ossia::unit_t>;

template <typename Arg, typename... Args>
struct unit_map_factory
{
  void operator()(unit_map& m)
  {
    for (ossia::string_view v : ossia::unit_traits<Arg>::text())
      m.emplace(v, ossia::unit_t{Arg{}});
    unit_map_factory<Args...>{}(m);
  }
};

template <typename Arg>
struct unit_map_factory<Arg>
{
  void operator()(unit_map& m)
  {
    for (ossia::string_view v : ossia::unit_traits<Arg>::text())
      m.emplace(v, ossia::unit_t{Arg{}});
  }
};

template <typename... Args>
struct make_unit_map
{
  unit_map operator()()
  {
    unit_map map;
    unit_map_factory<Args...>{}(map);
    return map;
  }
};

struct unit_factory_visitor
{
  ossia::string_view text;

  template <typename Dataspace_T>
  ossia::unit_t operator()(Dataspace_T arg)
  {
    static const auto units = brigand::wrap<
        typename matching_unit_u_list<Dataspace_T>::type, make_unit_map>{}();
    auto it = units.find(text);
    return it != units.end() ? it->second : ossia::unit_t{};
  }

  OSSIA_INLINE ossia::unit_t operator()()
  {
    return {};
  }
};

template <typename Unit>
using enable_if_multidimensional
    = std::enable_if_t<Unit::is_multidimensional::value>;

template <typename Dataspace, typename Unit, typename = void>
struct make_unit_symbols_sub_helper
{
  void operator()(unit_parse_symbols_t& map)
  {
    using unit_type = Unit;

    std::string res;
    res.reserve(20);

    for (auto ds : dataspace_traits<Dataspace>::text())
    {
      // For each unit :
      for (auto un : unit_traits<unit_type>::text())
      {
        res.clear();

        res += ds;  // color
        res += '.'; // color.

        res += un; // color.rgb

        // Add the unit in long form
        map.add(res, {{}, unit_type{}});
      }
    }
  }
};

template <typename Dataspace, typename Unit>
struct
    make_unit_symbols_sub_helper<Dataspace, Unit, enable_if_multidimensional<Unit>>
{
  void operator()(unit_parse_symbols_t& map)
  {
    using unit_type = Unit;

    std::string res;
    res.reserve(20);

    for (auto ds : dataspace_traits<Dataspace>::text())
    {
      // For each unit :
      for (auto un : unit_traits<unit_type>::text())
      {
        res.clear();

        res += ds;  // color
        res += '.'; // color.

        res += un; // color.rgb

        // Add the unit in long form
        map.add(res, {{}, unit_type{}});

        // Add all the accessors
        res += "._"; // color.rgb._

        const auto& params = unit_type::array_parameters();
        const auto n = params.size();
        for (std::size_t i = 0; i < n; i++)
        {
          // replace the last char with the one in the array parameter
          res[res.size() - 1] = params[i]; // color.rgb.r
          map.add(res, {{(uint8_t)i}, unit_type{}});
        }
      }
    }
  }
};

struct make_unit_symbols_helper
{
  unit_parse_symbols_t map;

  make_unit_symbols_helper()
  {
    brigand::for_each<dataspace_u_list>([&](auto t) {
      using dataspace_type = typename decltype(t)::type;
      brigand::for_each<dataspace_type>([&](auto u) {
        using unit_type = typename decltype(u)::type;
        make_unit_symbols_sub_helper<dataspace_type, unit_type>{}(map);
      });
    });
  }
};
}
}
