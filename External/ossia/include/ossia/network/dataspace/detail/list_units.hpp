#pragma once
#include <ossia/network/dataspace/dataspace.hpp>
#include <ossia/network/dataspace/dataspace_base_fwd.hpp>
#include <ossia/network/dataspace/dataspace_visitors.hpp>
#include <ossia/network/dataspace/detail/dataspace_parse.hpp>
#include <ossia/network/base/parameter_data.hpp>
#include <boost/algorithm/string/case_conv.hpp>
namespace ossia
{
namespace detail
{

template<typename InsertFun>
void list_units(InsertFun fun)
{
  // TODO frozen hash-map
  brigand::for_each<dataspace_u_list>([&](auto t) {
    ossia::net::parameter_data p;
    using dataspace_type = typename decltype(t)::type;
    using d_traits = dataspace_traits<dataspace_type>;
    for(auto dn : d_traits::text())
    {
      std::string dataspace_name(dn);
      boost::algorithm::to_lower(dataspace_name);
      using neutral_u = typename d_traits::neutral_unit;

      fun(dataspace_name, neutral_u{});

      brigand::for_each<dataspace_type>([&](auto u) {
        using unit_type = typename decltype(u)::type;

        for (auto un : unit_traits<unit_type>::text())
        {
          // Add the unit in short form and long
          std::string s = boost::algorithm::to_lower_copy(std::string(un));
          fun(dataspace_name + "." + s, unit_type{});
          fun(s, unit_type{});
        }
      });
    }
  });

  fun("complex", cartesian_2d_u{});
  fun("point2d", ossia::cartesian_2d_u{});
  fun("2d", ossia::cartesian_2d_u{});
  fun("cartesian2d", ossia::cartesian_2d_u{});

  fun("pos",   ossia::cartesian_3d_u{});
  fun("point",   ossia::cartesian_3d_u{});
  fun("point3d", ossia::cartesian_3d_u{});
  fun("3d", ossia::cartesian_3d_u{});
  fun("cartesian3d", ossia::cartesian_3d_u{});
  fun("coord", ossia::cartesian_3d_u{});
  fun("coordinate", ossia::cartesian_3d_u{});
  fun("coordinates", ossia::cartesian_3d_u{});
  fun("pvector", ossia::cartesian_3d_u{});
  fun("vertex",  ossia::cartesian_3d_u{});

  fun("gl",  ossia::opengl_u{});
  fun("opengl",  ossia::opengl_u{});
  fun("position.gl",  ossia::opengl_u{});
  fun("position.opengl",  ossia::opengl_u{});

  fun("freq",  ossia::frequency_u{});
  fun("frequence",  ossia::frequency_u{});
  fun("frequency",  ossia::frequency_u{});

  fun("col", ossia::dataspace_traits<color_u>::neutral_unit{});

  fun("rot", ossia::dataspace_traits<angle_u>::neutral_unit{});
  fun("rotation", ossia::dataspace_traits<angle_u>::neutral_unit{});
}


}
}
