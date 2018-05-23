#pragma once
#include <ossia/editor/state/destination_qualifiers.hpp>
#include <ossia/network/value/destination.hpp>
#include <ossia/network/value/value.hpp>
#include <bitset>
#include <memory>
#include <ossia_export.h>
#include <utility>

/**
 * \file message.hpp
 */
namespace ossia
{
namespace net
{
class parameter_base;
}
/**
 * @brief The message struct
 *
 * A message allows to store something to send to the network.
 * For instance : a:/b 234
 *
 * \see \ref state_element
 */
struct OSSIA_EXPORT message
{
  ossia::destination dest;
  ossia::value message_value;

  const ossia::unit_t& get_unit() const
  {
    return dest.unit;
  }
  void launch() const;

  friend bool operator==(const message& lhs, const message& rhs)
  {
    return lhs.dest == rhs.dest
           && lhs.message_value == rhs.message_value;
  }

  friend bool operator!=(const message& lhs, const message& rhs)
  {
    return lhs.dest != rhs.dest
           || lhs.message_value != rhs.message_value;
  }

  message() = delete;
  message(const message&) = default;
  message(message&&) = default;
  message& operator=(const message&) = default;
  message& operator=(message&&) = default;

  message(const destination& d, const ossia::value& v)
      : dest{d}, message_value{v}
  {
  }
  message(const destination& d, ossia::value&& v)
      : dest{d}, message_value{std::move(v)}
  {
  }
  message(const destination& d, const ossia::value& v, const ossia::unit_t& u)
      : dest{d.value, d.index, u}, message_value{v}
  {
  }
  message(const destination& d, ossia::value&& v, const ossia::unit_t& u)
      : dest{d.value, d.index, u}, message_value{std::move(v)}
  {
  }
};

struct OSSIA_EXPORT piecewise_message
{
  std::reference_wrapper<ossia::net::parameter_base> address;
  std::vector<ossia::value> message_value;
  ossia::unit_t unit;

  const ossia::unit_t& get_unit() const
  {
    return unit;
  }
  void launch() const;

  friend bool
  operator==(const piecewise_message& lhs, const piecewise_message& rhs)
  {
    return &lhs.address.get() == &rhs.address.get()
           && lhs.message_value == rhs.message_value && lhs.unit == rhs.unit;
  }

  friend bool
  operator!=(const piecewise_message& lhs, const piecewise_message& rhs)
  {
    return &lhs.address.get() != &rhs.address.get()
           || lhs.message_value != rhs.message_value || lhs.unit != rhs.unit;
  }
};

template <std::size_t N>
struct piecewise_vec_message
{
  std::reference_wrapper<ossia::net::parameter_base> address;
  std::array<float, N> message_value;
  ossia::unit_t unit;
  std::bitset<N> used_values; // True for each value that has been set.

  const ossia::unit_t& get_unit() const
  {
    return unit;
  }
  void launch() const;

  friend bool operator==(
      const piecewise_vec_message& lhs, const piecewise_vec_message& rhs)
  {
    return &lhs.address.get() == &rhs.address.get()
           && lhs.message_value == rhs.message_value && lhs.unit == rhs.unit
           && lhs.used_values == rhs.used_values;
  }

  friend bool operator!=(
      const piecewise_vec_message& lhs, const piecewise_vec_message& rhs)
  {
    return &lhs.address.get() != &rhs.address.get()
           || lhs.message_value != rhs.message_value || lhs.unit != rhs.unit
           || lhs.used_values != rhs.used_values;
  }
};
}
