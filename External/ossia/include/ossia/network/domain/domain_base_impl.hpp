#pragma once
#include <ossia/detail/optional.hpp>
#include <ossia/network/value/value_conversion.hpp>
#include <ossia/network/value/value_traits.hpp>
#include <boost/container/flat_set.hpp>
#include <type_traits>

namespace ossia
{

OSSIA_EXPORT ossia::value clamp(
    const ossia::value& val, const ossia::value& min, const ossia::value& max);
OSSIA_EXPORT ossia::value wrap(
    const ossia::value& val, const ossia::value& min, const ossia::value& max);
OSSIA_EXPORT ossia::value fold(
    const ossia::value& val, const ossia::value& min, const ossia::value& max);
OSSIA_EXPORT ossia::value
clamp_min(const ossia::value& val, const ossia::value& min);
OSSIA_EXPORT ossia::value
clamp_max(const ossia::value& val, const ossia::value& max);

OSSIA_EXPORT ossia::value
clamp(ossia::value&& val, const ossia::value& min, const ossia::value& max);
OSSIA_EXPORT ossia::value
wrap(ossia::value&& val, const ossia::value& min, const ossia::value& max);
OSSIA_EXPORT ossia::value
fold(ossia::value&& val, const ossia::value& min, const ossia::value& max);
OSSIA_EXPORT ossia::value
clamp_min(ossia::value&& val, const ossia::value& min);
OSSIA_EXPORT ossia::value
clamp_max(ossia::value&& val, const ossia::value& max);

template <typename T>
struct OSSIA_EXPORT domain_base
{
  using value_type = typename value_trait<T>::value_type;
  ossia::optional<value_type> min;
  ossia::optional<value_type> max;
  boost::container::flat_set<value_type> values;

  domain_base() noexcept
  {
  }
  domain_base(const domain_base& other) noexcept
      : min{other.min}, max{other.max}, values{other.values}
  {
  }

  domain_base(domain_base&& other) noexcept
      : min{std::move(other.min)}
      , max{std::move(other.max)}
      , values{std::move(other.values)}
  {
  }

  domain_base& operator=(const domain_base& other)
  {
    min = other.min;
    max = other.max;
    values = other.values;
    return *this;
  }

  domain_base& operator=(domain_base&& other) noexcept
  {
    min = std::move(other.min);
    max = std::move(other.max);
    values = std::move(other.values);
    return *this;
  }

  friend bool operator==(const domain_base<T>& lhs, const domain_base<T>& rhs)
  {
    return lhs.min == rhs.min && lhs.max == rhs.max
           && lhs.values == rhs.values;
  }
  friend bool operator!=(const domain_base<T>& lhs, const domain_base<T>& rhs)
  {
    return lhs.min != rhs.min || lhs.max != rhs.max
           || lhs.values != rhs.values;
  }

  domain_base(value_type v1, value_type v2) : min{v1}, max{v2}
  {
  }
  domain_base(
      value_type v1, value_type v2,
      const boost::container::flat_set<value_type>& vals)
      : min{v1}, max{v2}, values{vals}
  {
  }
  domain_base(
      value_type v1, value_type v2,
      boost::container::flat_set<value_type>&& vals)
      : min{v1}, max{v2}, values{std::move(vals)}
  {
  }
};

template <>
struct OSSIA_EXPORT domain_base<impulse>
{
  friend bool
  operator==(const domain_base<impulse>& lhs, const domain_base<impulse>& rhs)
  {
    return true;
  }
  friend bool
  operator!=(const domain_base<impulse>& lhs, const domain_base<impulse>& rhs)
  {
    return false;
  }
};

template <>
struct OSSIA_EXPORT domain_base<std::string>
{
  boost::container::flat_set<std::string> values;
  friend bool operator==(
      const domain_base<std::string>& lhs, const domain_base<std::string>& rhs)
  {
    return lhs.values == rhs.values;
  }
  friend bool operator!=(
      const domain_base<std::string>& lhs, const domain_base<std::string>& rhs)
  {
    return lhs.values != rhs.values;
  }
};

struct OSSIA_EXPORT vector_domain
{
  // If a value does not have a min / max the value won't be valid
  using value_type = std::vector<ossia::value>;
  value_type min;
  value_type max;
  std::vector<boost::container::flat_set<ossia::value>> values;

  vector_domain() noexcept
  {
  }
  vector_domain(const vector_domain& other) noexcept
      : min(other.min)
      , max(other.max)
      , values(other.values)
  {
  }

  vector_domain(vector_domain&& other) noexcept
      : min(std::move(other.min))
      , max(std::move(other.max))
      , values(std::move(other.values))
  {
  }

  vector_domain& operator=(const vector_domain& other)
  {
    min = other.min;
    max = other.max;
    values = other.values;
    return *this;
  }

  vector_domain& operator=(vector_domain&& other) noexcept
  {
    min = std::move(other.min);
    max = std::move(other.max);
    values = std::move(other.values);
    return *this;
  }

  vector_domain(ossia::none_t, ossia::none_t)
  {
  }

  vector_domain(const value_type& v1, const value_type& v2) : min(v1), max(v2)
  {
  }
  vector_domain(value_type&& v1, value_type&& v2)
      : min(std::move(v1)), max(std::move(v2))
  {
  }
  vector_domain(
      const value_type& v1, const value_type& v2,
      const std::vector<boost::container::flat_set<ossia::value>>& vals)
      : min(v1), max(v2), values(vals)
  {
  }
  vector_domain(
      value_type&& v1, value_type&& v2,
      std::vector<boost::container::flat_set<ossia::value>>&& vals)
      : min(std::move(v1)), max(std::move(v2)), values(std::move(vals))
  {
  }

  friend bool operator==(const vector_domain& lhs, const vector_domain& rhs)
  {
    return lhs.min == rhs.min && lhs.max == rhs.max
           && lhs.values == rhs.values;
  }
  friend bool operator!=(const vector_domain& lhs, const vector_domain& rhs)
  {
    return lhs.min != rhs.min || lhs.max != rhs.max
           || lhs.values != rhs.values;
  }
};

template <std::size_t N>
struct OSSIA_EXPORT vecf_domain
{
  using value_type = std::array<float, N>;
  std::array<optional<float>, N> min;
  std::array<optional<float>, N> max;
  std::array<boost::container::flat_set<float>, N> values;

  vecf_domain() noexcept
  {
  }
  vecf_domain(const vecf_domain& other) noexcept
      : min{std::move(other.min)}
      , max{std::move(other.max)}
      , values{std::move(other.values)}
  {
  }

  vecf_domain(vecf_domain&& other) noexcept
      : min{std::move(other.min)}
      , max{std::move(other.max)}
      , values{std::move(other.values)}
  {
  }

  vecf_domain& operator=(const vecf_domain& other)
  {
    min = other.min;
    max = other.max;
    values = other.values;
    return *this;
  }

  vecf_domain& operator=(vecf_domain&& other) noexcept
  {
    min = std::move(other.min);
    max = std::move(other.max);
    values = std::move(other.values);
    return *this;
  }

  vecf_domain(
      const std::array<optional<float>, N>& v1,
      const std::array<optional<float>, N>& v2)
      : min{v1}, max{v2}
  {
  }

  vecf_domain(const std::array<float, N>& v1, const std::array<float, N>& v2)
  {
    for (std::size_t i = 0; i < N; i++)
    {
      min[i] = v1[i];
      max[i] = v2[i];
    }
  }
  vecf_domain(
      const std::array<optional<float>, N>& v1,
      const std::array<optional<float>, N>& v2,
      const std::array<boost::container::flat_set<float>, N>& vals)
      : min{v1}, max{v2}, values{vals}
  {
  }
  vecf_domain(
      const std::array<optional<float>, N>& v1,
      const std::array<optional<float>, N>& v2,
      std::array<boost::container::flat_set<float>, N>&& vals)
      : min{v1}, max{v2}, values{std::move(vals)}
  {
  }

  friend bool operator==(const vecf_domain& lhs, const vecf_domain& rhs)
  {
    return lhs.min == rhs.min && lhs.max == rhs.max
           && lhs.values == rhs.values;
  }
  friend bool operator!=(const vecf_domain& lhs, const vecf_domain& rhs)
  {
    return lhs.min != rhs.min || lhs.max != rhs.max
           || lhs.values != rhs.values;
  }
};

template <>
struct OSSIA_EXPORT domain_base<ossia::value>
{
  using value_type = ossia::value;
  ossia::optional<value_type> min;
  ossia::optional<value_type> max;
  boost::container::flat_set<value_type> values;

  domain_base() noexcept
  {
  }
  domain_base(const domain_base<value_type>& other) noexcept
      : min{std::move(other.min)}
      , max{std::move(other.max)}
      , values{std::move(other.values)}
  {
  }

  domain_base(domain_base<value_type>&& other) noexcept
      : min{std::move(other.min)}
      , max{std::move(other.max)}
      , values{std::move(other.values)}
  {
  }

  domain_base& operator=(const domain_base& other)
  {
    min = other.min;
    max = other.max;
    values = other.values;
    return *this;
  }

  domain_base& operator=(domain_base&& other) noexcept
  {
    min = std::move(other.min);
    max = std::move(other.max);
    values = std::move(other.values);
    return *this;
  }

  domain_base<value_type>(const value_type& v1, const value_type& v2)
      : min{v1}, max{v2}
  {
  }
  domain_base<value_type>(value_type&& v1, value_type&& v2)
      : min{std::move(v1)}, max{std::move(v2)}
  {
  }
  domain_base(
      const value_type& v1, const value_type& v2,
      const boost::container::flat_set<value_type>& vals)
      : min{v1}, max{v2}, values{vals}
  {
  }
  domain_base(
      value_type&& v1, value_type&& v2,
      boost::container::flat_set<value_type>&& vals)
      : min{std::move(v1)}, max{std::move(v2)}, values{std::move(vals)}
  {
  }

  friend bool operator==(
      const domain_base<ossia::value>& lhs,
      const domain_base<ossia::value>& rhs)
  {
    return lhs.min == rhs.min && lhs.max == rhs.max
           && lhs.values == rhs.values;
  }
  friend bool operator!=(
      const domain_base<ossia::value>& lhs,
      const domain_base<ossia::value>& rhs)
  {
    return lhs.min != rhs.min || lhs.max != rhs.max
           || lhs.values != rhs.values;
  }
};
}
