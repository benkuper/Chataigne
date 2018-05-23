#pragma once
#include <ossia/detail/destination_index.hpp>
#include <ossia/network/dataspace/dataspace.hpp>

namespace ossia
{
struct OSSIA_EXPORT destination_qualifiers
{
  ossia::destination_index accessors;
  ossia::unit_t unit;

  bool operator==(const destination_qualifiers& a) const
  {
    return accessors == a.accessors && unit == a.unit;
  }

  bool operator!=(const destination_qualifiers& a) const
  {
    return accessors != a.accessors || unit != a.unit;
  }
};
}
