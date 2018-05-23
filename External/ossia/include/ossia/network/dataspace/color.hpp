#pragma once
#include <ossia/network/dataspace/dataspace_base.hpp>
// Algorithms taken from http://www.easyrgb.com
namespace ossia
{

struct argb_u;
struct color_dataspace;
template <typename Impl>
struct color_unit
{
  using is_unit = std::true_type;
  using is_multidimensional
      = std::true_type; // number of dimensiosn -> decltype(value)::size_value
  using neutral_unit = argb_u;
  using concrete_type = Impl;
  using dataspace_type = color_dataspace;
};

struct OSSIA_EXPORT argb_u : public color_unit<argb_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("argb");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("argb");
  }

  using value_type = vec4f;

  static OSSIA_DECL_RELAXED_CONSTEXPR strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return self;
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR value_type
  from_neutral(strong_value<neutral_unit> self)
  {
    return self.dataspace_value;
  }
};

struct OSSIA_EXPORT rgba_u : public color_unit<rgba_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("rgba");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("rgba");
  }

  using value_type = vec4f;

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return {self.dataspace_value[3], self.dataspace_value[0],
            self.dataspace_value[1], self.dataspace_value[2]};
  }

  static value_type from_neutral(strong_value<neutral_unit> self)
  {
    return {self.dataspace_value[1], self.dataspace_value[2],
            self.dataspace_value[3], self.dataspace_value[0]};
  }
};

struct OSSIA_EXPORT rgb_u : public color_unit<rgb_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("rgb");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("rgb");
  }

  using value_type = vec3f;

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return {1., self.dataspace_value[0], self.dataspace_value[1],
            self.dataspace_value[2]};
  }

  static value_type from_neutral(strong_value<neutral_unit> self)
  {
    return {self.dataspace_value[1], self.dataspace_value[2],
            self.dataspace_value[3]};
  }
};

struct OSSIA_EXPORT bgr_u : public color_unit<bgr_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("bgr");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("bgr");
  }

  using value_type = vec3f;

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return {1., self.dataspace_value[2], self.dataspace_value[1],
            self.dataspace_value[0]};
  }

  static value_type from_neutral(strong_value<neutral_unit> self)
  {
    return {self.dataspace_value[3], self.dataspace_value[2],
            self.dataspace_value[1]};
  }
};

struct OSSIA_EXPORT argb8_u : public color_unit<argb8_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("argb8");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("argb");
  }

  using value_type = vec4f;

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return make_vec(
        self.dataspace_value[0] / 255., self.dataspace_value[1] / 255.,
        self.dataspace_value[2] / 255., self.dataspace_value[3] / 255.);
  }

  static value_type from_neutral(strong_value<neutral_unit> self)
  {
    return make_vec(
        self.dataspace_value[0] * 255., self.dataspace_value[1] * 255.,
        self.dataspace_value[2] * 255., self.dataspace_value[3] * 255.);
  }
};

struct OSSIA_EXPORT hsv_u : public color_unit<hsv_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("hsv");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("hsv");
  }

  using value_type = vec3f;
  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self);

  static value_type from_neutral(strong_value<neutral_unit> self);
};

struct OSSIA_EXPORT hsl_u : public color_unit<hsl_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("hsl");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("hsl");
  }

  using value_type = vec3f;
};

struct OSSIA_EXPORT cmy8_u : public color_unit<cmy8_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("cmy8");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("cmy");
  }

  using value_type = vec3f;

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return make_vec(
        1., (255. - self.dataspace_value[0]) / 255.,
        (255. - self.dataspace_value[1]) / 255.,
        (255. - self.dataspace_value[2]) / 255.);
  }

  static value_type from_neutral(strong_value<neutral_unit> self)
  {
    return make_vec(
        255. * (1. - self.dataspace_value[1]),
        255. * (1. - self.dataspace_value[2]),
        255. * (1. - self.dataspace_value[3]));
  }
};

struct OSSIA_EXPORT cmyk8_u : public color_unit<cmyk8_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("cmyk8");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("cmyk");
  }

  using value_type = vec4f;
};

struct OSSIA_EXPORT xyz_u : public color_unit<xyz_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("xyz");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("xyz");
  }

  using value_type = vec3f;

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self);

  static value_type from_neutral(strong_value<neutral_unit> self);
};

struct OSSIA_EXPORT yxy_u : public color_unit<yxy_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("Yxy");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("Yxy");
  }

  using value_type = vec3f;
};

struct OSSIA_EXPORT hunter_lab_u : public color_unit<hunter_lab_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("hunter_lab");
  }
  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("lab");
  }
  using value_type = vec3f;

  static strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self);
  static value_type from_neutral(strong_value<neutral_unit> self);
};

struct OSSIA_EXPORT cie_lab_u : public color_unit<cie_lab_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("cie_lab");
  }
  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("lab");
  }
  using value_type = vec3f;
};

struct OSSIA_EXPORT cie_luv_u : public color_unit<cie_luv_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("cie_luv");
  }
  static OSSIA_DECL_RELAXED_CONSTEXPR auto array_parameters()
  {
    return ossia::make_string_view("luv");
  }
  using value_type = vec3f;
};
}
