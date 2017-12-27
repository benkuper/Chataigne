#pragma once
#include <ossia/detail/destination_index.hpp>
#include <ossia/network/dataspace/dataspace.hpp>
#include <ossia/network/value/value.hpp>
#include <ossia/network/common/parameter_properties.hpp>
#include <functional>
#include <initializer_list>
#include <memory>
#include <ossia_export.h>
#include <utility>
#include <vector>
/**
 * \file destination.hpp
 */
namespace ossia
{
class value;
namespace net
{
class parameter_base;
}
/**
 * @class Destination destination.hpp ossia/network/value/value.hpp
 *
 * \see \ref node_base
 * \see \ref destination_index
 */
class OSSIA_EXPORT destination final
{
public:
  std::reference_wrapper<net::parameter_base> value;
  destination_index index;
  ossia::unit_t unit;

  ossia::value pull() const;
  ossia::net::parameter_base& address()
  {
    return value.get();
  }
  ossia::net::parameter_base& address() const
  {
    return value.get();
  }

  destination(net::parameter_base& v);
  destination(net::parameter_base& v, destination_index);
  destination(net::parameter_base& v, destination_index, const ossia::unit_t&);
  destination(net::parameter_base& v, const ossia::unit_t&);

  destination(const destination& other);
  destination(destination&& other);
  destination& operator=(const destination&);
  destination& operator=(destination&&);

  bool operator==(const ossia::value&) const;
  bool operator!=(const ossia::value&) const;
  bool operator>(const ossia::value&) const;
  bool operator>=(const ossia::value&) const;
  bool operator<(const ossia::value&) const;
  bool operator<=(const ossia::value&) const;
};

OSSIA_EXPORT bool operator==(const destination&, const destination&);
OSSIA_EXPORT bool operator!=(const destination&, const destination&);
OSSIA_EXPORT bool
operator==(const destination&, const ossia::net::parameter_base&);
OSSIA_EXPORT bool
operator!=(const destination&, const ossia::net::parameter_base&);

OSSIA_EXPORT std::string to_pretty_string(const destination& d);
}
