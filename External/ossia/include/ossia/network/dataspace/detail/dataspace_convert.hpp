#pragma once
#include <ossia/network/dataspace/dataspace_variant_visitors.hpp>
#include <ossia/network/dataspace/dataspace_visitors.hpp>
#include <ossia/network/dataspace/value_with_unit.hpp>
#include <ossia/network/value/value_traits.hpp>
namespace ossia
{
namespace detail
{

template <typename T, typename U>
struct convert_unit_helper
{
  OSSIA_INLINE ossia::value_with_unit
  operator()(const strong_value<T>& value, const U& unit)
  {
    return strong_value<U>(value);
  }
};

struct convert_unit_visitor
{
  template <typename T, typename U>
  OSSIA_INLINE ossia::value_with_unit
  operator()(const strong_value<T>& value, const U& unit)
  {
    return convert_unit_helper<T, U>{}(value, unit);
  }

  template <typename T, typename U>
  OSSIA_INLINE ossia::value_with_unit operator()(const T& d1, const U& d2)
  {
    return {};
  }

  OSSIA_INLINE ossia::value_with_unit
  operator()(const angle& d1, const angle_u& d2)
  {
    return ossia::apply(*this, d1, d2);
  }
  OSSIA_INLINE ossia::value_with_unit
  operator()(const color& d1, const color_u& d2)
  {
    return ossia::apply(*this, d1, d2);
  }
  OSSIA_INLINE ossia::value_with_unit
  operator()(const distance& d1, const distance_u& d2)
  {
    return ossia::apply(*this, d1, d2);
  }
  OSSIA_INLINE ossia::value_with_unit
  operator()(const orientation& d1, const orientation_u& d2)
  {
    return ossia::apply(*this, d1, d2);
  }
  OSSIA_INLINE ossia::value_with_unit
  operator()(const gain& d1, const gain_u& d2)
  {
    return ossia::apply(*this, d1, d2);
  }
  OSSIA_INLINE ossia::value_with_unit
  operator()(const position& d1, const position_u& d2)
  {
    return ossia::apply(*this, d1, d2);
  }
  OSSIA_INLINE ossia::value_with_unit
  operator()(const speed& d1, const speed_u& d2)
  {
    return ossia::apply(*this, d1, d2);
  }
  OSSIA_INLINE ossia::value_with_unit
  operator()(const timing& d1, const timing_u& d2)
  {
    return ossia::apply(*this, d1, d2);
  }
};

struct convert_to_value_visitor
{
  OSSIA_INLINE ossia::value operator()(const angle& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::value operator()(const color& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::value operator()(const distance& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::value operator()(const orientation& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::value operator()(const gain& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::value operator()(const position& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::value operator()(const speed& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::value operator()(const timing& d1)
  {
    return ossia::apply(*this, d1);
  }

  template <typename T>
  OSSIA_INLINE ossia::value operator()(const strong_value<T>& val)
  {
    return val.dataspace_value;
  }

  OSSIA_INLINE ossia::value operator()(ossia::value val)
  {
    return val;
  }

  OSSIA_INLINE ossia::value operator()()
  {
    return {};
  }
};

struct convert_to_unit_visitor
{
  OSSIA_INLINE ossia::unit_t operator()(const angle& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::unit_t operator()(const color& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::unit_t operator()(const distance& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::unit_t operator()(const orientation& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::unit_t operator()(const gain& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::unit_t operator()(const position& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::unit_t operator()(const speed& d1)
  {
    return ossia::apply(*this, d1);
  }
  OSSIA_INLINE ossia::unit_t operator()(const timing& d1)
  {
    return ossia::apply(*this, d1);
  }

  template <typename T>
  OSSIA_INLINE ossia::unit_t operator()(const T&)
  {
    return typename T::unit_type{};
  }

  OSSIA_INLINE ossia::unit_t operator()(ossia::value val)
  {
    return {};
  }

  OSSIA_INLINE ossia::unit_t operator()()
  {
    return {};
  }
};
}
}
