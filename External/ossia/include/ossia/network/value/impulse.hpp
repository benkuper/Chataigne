#pragma once
#include <ossia/detail/config.hpp>
#include <ossia_export.h>

namespace ossia
{
class value;

/**
 * @class Impulse impulse.hpp ossia/network/value/value.hpp
 *
 * Any value can be converted to an impulse.
 * An impulse generally just means that we want to send a message to the
 * address,
 * and a value is not needed.
 *
 * For instance :
 * \code
 * /audio/player/stop
 * \endcode
 *
 * \see expression_pulse
 */
struct OSSIA_EXPORT impulse
{
  using value_type = impulse;
  OSSIA_DECL_RELAXED_CONSTEXPR impulse() noexcept
  {
  }
  OSSIA_DECL_RELAXED_CONSTEXPR impulse(const impulse&) noexcept
  {
  }
  OSSIA_DECL_RELAXED_CONSTEXPR impulse(impulse&&) noexcept
  {
  }
  OSSIA_DECL_RELAXED_CONSTEXPR impulse& operator=(const impulse&) noexcept
  {
    return *this;
  }
  OSSIA_DECL_RELAXED_CONSTEXPR impulse& operator=(impulse&&) noexcept
  {
    return *this;
  }

  OSSIA_DECL_RELAXED_CONSTEXPR bool operator==(const ossia::impulse&) const
  {
    return true;
  }

  OSSIA_DECL_RELAXED_CONSTEXPR bool operator!=(const ossia::impulse&) const
  {
    return false;
  }

  OSSIA_DECL_RELAXED_CONSTEXPR bool operator>(const ossia::impulse&) const
  {
    return false;
  }

  OSSIA_DECL_RELAXED_CONSTEXPR bool operator>=(const ossia::impulse&) const
  {
    return true;
  }

  OSSIA_DECL_RELAXED_CONSTEXPR bool operator<(const ossia::impulse&) const
  {
    return false;
  }

  OSSIA_DECL_RELAXED_CONSTEXPR bool operator<=(const ossia::impulse&) const
  {
    return true;
  }
};

template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator==(const T&, const ossia::impulse&)
{
  return true;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator!=(const T&, const ossia::impulse&)
{
  return false;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator>(const T&, const ossia::impulse&)
{
  return false;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator>=(const T&, const ossia::impulse&)
{
  return true;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator<(const T&, const ossia::impulse&)
{
  return false;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator<=(const T&, const ossia::impulse&)
{
  return true;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator==(const ossia::impulse&, const T&)
{
  return true;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator!=(const ossia::impulse&, const T&)
{
  return false;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator>(const ossia::impulse&, const T&)
{
  return false;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator>=(const ossia::impulse&, const T&)
{
  return true;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator<(const ossia::impulse&, const T&)
{
  return false;
}
template <typename T>
OSSIA_DECL_RELAXED_CONSTEXPR bool operator<=(const ossia::impulse&, const T&)
{
  return true;
}
}
