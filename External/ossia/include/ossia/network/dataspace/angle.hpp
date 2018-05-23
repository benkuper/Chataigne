#pragma once
#include <ossia/network/dataspace/dataspace_base.hpp>

namespace ossia
{
struct angle_dataspace;
struct radian_u;
template <typename Impl>
struct angle_unit
{
  using is_unit = std::true_type;
  using neutral_unit = radian_u;
  using value_type = float;
  using concrete_type = Impl;
  using dataspace_type = angle_dataspace;
};

struct OSSIA_EXPORT radian_u : public angle_unit<radian_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("radian", "rad");
  }

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

struct OSSIA_EXPORT degree_u : public angle_unit<degree_u>
{
  static OSSIA_DECL_RELAXED_CONSTEXPR auto text()
  {
    return ossia::make_string_array("degree", "deg");
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR strong_value<neutral_unit>
  to_neutral(strong_value<concrete_type> self)
  {
    return {self.dataspace_value * ossia::deg_to_rad};
  }

  static OSSIA_DECL_RELAXED_CONSTEXPR value_type
  from_neutral(strong_value<neutral_unit> self)
  {
    return self.dataspace_value * ossia::rad_to_deg;
  }
};
}
