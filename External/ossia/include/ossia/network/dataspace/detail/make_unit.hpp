#pragma once
#include <ossia/network/dataspace/dataspace.hpp>
#include <ossia/network/dataspace/detail/dataspace_parse.hpp>
#include <brigand/algorithms/for_each.hpp>

namespace ossia
{
class make_unit_helper
{
public:
  make_unit_helper()
      : indices(make_dataspace_index_array()), units(make_unit_array())
  {
  }

  ossia::unit_t get_unit(uint64_t dataspace, uint64_t unit) const
  {
    // Position of the dataspace + position of the unit
    // position of the dataspace is the sum of the n first in unit_sizes

    if (dataspace < indices.size())
    {
      auto idx = indices[dataspace];

      if (idx + unit < units.size())
        return units[idx + unit];
    }

    return {};
  }

private:
  /**
   * We have two arrays here :
   * One (units) with all the units sequentially ordered.
   * One (indices) with the starting index of each dataspace
   * i.e:
   * indices[0] == 0
   * indices[1] == position of "first position unit" in the units array
   */
  using indices_array = std::array<uint64_t, ossia::dataspace_count>;
  using units_array = std::array<ossia::unit_t, ossia::unit_count>;

  const indices_array indices;
  const units_array units;

  static indices_array make_dataspace_index_array()
  {
    indices_array arr;

    uint64_t i = 0;
    uint64_t sum = 0;
    brigand::for_each<ossia::dataspace_u_list>([&](auto t) {
      using dataspace_type = typename decltype(t)::type;
      arr[i] = sum;
      sum += brigand::size<dataspace_type>::value;
      i++;
    });

    return arr;
  }

  // Creation of an array where each value is the corresponding unit
  static units_array make_unit_array()
  {
    units_array arr;

    uint64_t i = 0;
    brigand::for_each<ossia::dataspace_u_list>([&](auto t) {
      using dataspace_type = typename decltype(t)::type;
      brigand::for_each<dataspace_type>([&](auto u) {
        using unit_type = typename decltype(u)::type;
        arr[i] = unit_type{};
        i++;
      });
    });
    return arr;
  }
};
}
