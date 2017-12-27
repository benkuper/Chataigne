#pragma once
#include <ossia/network/domain/detail/array_domain.hpp>
#include <ossia/network/domain/detail/generic_domain.hpp>
#include <ossia/network/domain/detail/numeric_domain.hpp>
#include <ossia/network/domain/detail/value_set_domain.hpp>
#include <ossia/network/domain/domain_base.hpp>
#define FAST_COMPILES // See also clamp_visitors.hpp

#if defined(FAST_COMPILES)
namespace ossia
{
template <typename T>
auto& move(const T& t)
{
  return t;
}
}
#else
namespace ossia
{
template <typename T>
constexpr typename std::remove_reference<T>::type&& move(T&& t) noexcept
{
  return static_cast<typename std::remove_reference<T>::type&&>(t);
}
}
#endif

namespace ossia
{

struct apply_domain_visitor
{
  bounding_mode b;

  // General case with incompatible values
  template <typename T, typename U>
  OSSIA_INLINE ossia::value
  operator()(const T& value, const U& bad_domain) const
  {
    return {};
  }

  // Generic case
  template <typename T>
  OSSIA_INLINE ossia::value
  operator()(T&& value, const domain_base<ossia::value>& domain) const
  {
    return generic_clamp{domain}(b, std::move(value));
  }

  template <typename T>
  OSSIA_INLINE ossia::value
  operator()(const T& value, const domain_base<ossia::value>& domain) const
  {
    return generic_clamp{domain}(b, value);
  }

  // Values without meaningful domains
  OSSIA_INLINE ossia::value
  operator()(impulse value, const domain_base<impulse>& domain) const
  {
    return value;
  }
  // Numeric values
  ossia::value
  operator()(int32_t value, const domain_base<int32_t>& domain) const;
  ossia::value operator()(float value, const domain_base<float>& domain) const;
  ossia::value operator()(char value, const domain_base<char>& domain) const;
  ossia::value operator()(bool value, const domain_base<bool>& domain) const;

  // Strings
  OSSIA_INLINE ossia::value operator()(
      const std::string& value, const domain_base<std::string>& domain) const
  {
    return value_set_clamp<domain_base<std::string>>{domain}(b, value);
  }
  OSSIA_INLINE ossia::value
  operator()(std::string&& value, const domain_base<std::string>& domain) const
  {
    return value_set_clamp<domain_base<std::string>>{domain}(
        b, std::move(value));
  }

  // Lists
  // First case : list with another domain : we try to filter all the values
  // of the list that are filterable by this domain.
  // They are defined outside the class to handle a GCC bug...
  template <typename T>
  ossia::value operator()(
      const std::vector<ossia::value>& value,
      const domain_base<T>& domain) const;
  template <typename T>
  ossia::value operator()(
      std::vector<ossia::value>&& value, const domain_base<T>& domain) const;

  ossia::value operator()(
      const std::vector<ossia::value>& value,
      const domain_base<ossia::value>& domain) const;
  ossia::value operator()(
      std::vector<ossia::value>&& value,
      const domain_base<ossia::value>& domain) const;

  // Second case : we filter a whole list.
  ossia::value operator()(
      const std::vector<ossia::value>& value,
      const vector_domain& domain) const;
  ossia::value operator()(
      std::vector<ossia::value>&& value, const vector_domain& domain) const;

  // Vec : we can either filter each value, or filter the whole shebang
  ossia::value operator()(
      const std::array<float, 2>& value,
      const domain_base<float>& domain) const;
  ossia::value operator()(
      const std::array<float, 2>& value,
      const domain_base<int32_t>& domain) const;
  ossia::value operator()(
      const std::array<float, 2>& value,
      const domain_base<bool>& domain) const;
  ossia::value operator()(
      const std::array<float, 2>& value,
      const domain_base<char>& domain) const;
  ossia::value operator()(
      const std::array<float, 2>& value, const vecf_domain<2>& domain) const;
  ossia::value operator()(
      const std::array<float, 2>& value, const vector_domain& domain) const;

  ossia::value operator()(
      const std::array<float, 3>& value,
      const domain_base<float>& domain) const;
  ossia::value operator()(
      const std::array<float, 3>& value,
      const domain_base<int32_t>& domain) const;
  ossia::value operator()(
      const std::array<float, 3>& value,
      const domain_base<bool>& domain) const;
  ossia::value operator()(
      const std::array<float, 3>& value,
      const domain_base<char>& domain) const;
  ossia::value operator()(
      const std::array<float, 3>& value, const vecf_domain<3>& domain) const;
  ossia::value operator()(
      const std::array<float, 3>& value, const vector_domain& domain) const;

  ossia::value operator()(
      const std::array<float, 4>& value,
      const domain_base<float>& domain) const;
  ossia::value operator()(
      const std::array<float, 4>& value,
      const domain_base<int32_t>& domain) const;
  ossia::value operator()(
      const std::array<float, 4>& value,
      const domain_base<bool>& domain) const;
  ossia::value operator()(
      const std::array<float, 4>& value,
      const domain_base<char>& domain) const;
  ossia::value operator()(
      const std::array<float, 4>& value, const vecf_domain<4>& domain) const;
  ossia::value operator()(
      const std::array<float, 4>& value, const vector_domain& domain) const;
};

template <typename Domain_T>
struct list_apply_domain_helper
{
  const apply_domain_visitor& vis;
  const Domain_T& dom;

  template <typename U>
  OSSIA_INLINE ossia::value operator()(U&& u) const
  {
    return vis(std::forward<U>(u), dom);
  }
};

template <typename T>
ossia::value apply_domain_visitor::operator()(
    const std::vector<ossia::value>& value, const domain_base<T>& domain) const
{
  std::vector<ossia::value> res = value;
  for (auto& val : res)
  {
    if (val.getType() == ossia::value_trait<T>::ossia_enum)
      val = ossia::apply_nonnull(
          list_apply_domain_helper<domain_base<T>>{*this, domain}, val.v);
  }
  return res;
}

template <typename T>
ossia::value apply_domain_visitor::operator()(
    std::vector<ossia::value>&& value, const domain_base<T>& domain) const
{
  for (auto& val : value)
  {
    if (val.getType() == ossia::value_trait<T>::ossia_enum)
      val = ossia::apply_nonnull(
          list_apply_domain_helper<domain_base<T>>{*this, domain},
          std::move(val.v));
  }
  // TODO currently other values (strings, etc...) are ignored; what should we
  // do here ?
  return std::move(value);
}
}
