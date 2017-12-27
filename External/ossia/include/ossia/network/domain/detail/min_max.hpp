#pragma once
#include <ossia/network/domain/domain_base.hpp>
#include <chobo/small_vector.hpp>

namespace ossia
{
namespace detail
{
/// Min ///

template <typename T>
struct domain_min_visitor_helper
{
  OSSIA_INLINE ossia::value operator()(const T& value) const
  {
    if (value.min)
      return value::make<typename T::value_type>(*value.min);
    else
      return ossia::value{};
  }
};

template <>
struct domain_min_visitor_helper<domain_base<impulse>>
{
  OSSIA_INLINE ossia::value operator()(const domain_base<impulse>& value) const
  {
    return impulse{};
  }
};

template <>
struct domain_min_visitor_helper<domain_base<std::string>>
{
  OSSIA_INLINE ossia::value
  operator()(const domain_base<std::string>& value) const
  {
    return {};
  }
};

template <>
struct domain_min_visitor_helper<domain_base<ossia::value>>
{
  OSSIA_INLINE ossia::value
  operator()(const domain_base<ossia::value>& value) const
  {
    // TODO for this case, it would maybe be better to
    // use the empty state of value instead of a boost::optional ?
    if (value.min)
      return *value.min;
    else
      return ossia::value{};
  }
};

/// Max ///
template <typename T>
struct domain_max_visitor_helper
{
  OSSIA_INLINE ossia::value operator()(const T& value) const
  {
    if (value.max)
      return value::make<typename T::value_type>(*value.max);
    else
      return ossia::value{};
  }
};

template <>
struct domain_max_visitor_helper<domain_base<impulse>>
{
  OSSIA_INLINE ossia::value operator()(const domain_base<impulse>& value) const
  {
    return impulse{};
  }
};

template <>
struct domain_max_visitor_helper<domain_base<std::string>>
{
  OSSIA_INLINE ossia::value
  operator()(const domain_base<std::string>& value) const
  {
    return {};
  }
};

template <>
struct domain_max_visitor_helper<domain_base<ossia::value>>
{
  OSSIA_INLINE ossia::value
  operator()(const domain_base<ossia::value>& value) const
  {
    // TODO for this case, it would maybe be better to
    // use the empty state of value instead of a boost::optional ?
    if (value.max)
      return *value.max;
    else
      return ossia::value{};
  }
};
}

struct domain_min_visitor
{
  template <typename T>
  OSSIA_INLINE ossia::value operator()(const domain_base<T>& value) const
  {
    return detail::domain_min_visitor_helper<domain_base<T>>{}(value);
  }

  template <typename... T>
  OSSIA_INLINE ossia::value operator()(const T&...) const
  {
    return ossia::value{};
  }
};

struct domain_max_visitor
{
  template <typename T>
  OSSIA_INLINE ossia::value operator()(const domain_base<T>& value) const
  {
    return detail::domain_max_visitor_helper<domain_base<T>>{}(value);
  }

  template <typename... T>
  OSSIA_INLINE ossia::value operator()(const T&...) const
  {
    return ossia::value{};
  }
};

struct domain_set_min_visitor
{
  OSSIA_INLINE void operator()(domain_base<int32_t>& domain, int32_t incoming)
  {
    domain.min = incoming;
  }
  OSSIA_INLINE void operator()(domain_base<float>& domain, float incoming)
  {
    domain.min = incoming;
  }
  OSSIA_INLINE void operator()(domain_base<char>& domain, char incoming)
  {
    domain.min = incoming;
  }
  OSSIA_INLINE void operator()(domain_base<bool>& domain, bool incoming)
  {
    domain.min = incoming;
  }
  OSSIA_INLINE void
  operator()(vector_domain& domain, const std::vector<ossia::value>& incoming)
  {
    domain.min = incoming;
  }

  template <std::size_t N>
  OSSIA_INLINE void
  operator()(vecf_domain<N>& domain, const std::array<float, N>& incoming)
  {
    for (std::size_t i = 0; i < N; i++)
      domain.min[i] = incoming[i];
  }

  template <typename T>
  OSSIA_INLINE void
  operator()(domain_base<ossia::value>& domain, const T& incoming)
  {
    domain.min = ossia::value{incoming};
  }

  // Removal cases
  // Here we could maybe allow a cast or something like this...
  // for e.g. int -> float
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<int32_t>& domain, U&&...)
  {
    domain.min = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<float>& domain, U&&...)
  {
    domain.min = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<char>& domain, U&&...)
  {
    domain.min = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<bool>& domain, U&&...)
  {
    domain.min = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(vector_domain& domain, U&&...)
  {
    domain.min.clear();
  }
  template <std::size_t N, typename... U>
  OSSIA_INLINE void operator()(vecf_domain<N>& domain, U&&...)
  {
    for (std::size_t i = 0; i < N; i++)
      domain.min[i] = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<ossia::value>& domain, U&&...)
  {
    domain.min = ossia::none;
  }

  template <typename T, typename... U>
  OSSIA_INLINE void operator()(const domain_base<T>& domain, U&&...)
  {
  }
};

struct domain_set_max_visitor
{
  OSSIA_INLINE void operator()(domain_base<int32_t>& domain, int32_t incoming)
  {
    domain.max = incoming;
  }
  OSSIA_INLINE void operator()(domain_base<float>& domain, float incoming)
  {
    domain.max = incoming;
  }
  OSSIA_INLINE void operator()(domain_base<char>& domain, char incoming)
  {
    domain.max = incoming;
  }
  OSSIA_INLINE void operator()(domain_base<bool>& domain, bool incoming)
  {
    domain.max = incoming;
  }
  OSSIA_INLINE void
  operator()(vector_domain& domain, const std::vector<ossia::value>& incoming)
  {
    domain.max = incoming;
  }

  template <std::size_t N>
  OSSIA_INLINE void
  operator()(vecf_domain<N>& domain, const std::array<float, N>& incoming)
  {
    for (std::size_t i = 0; i < N; i++)
      domain.max[i] = incoming[i];
  }

  template <typename T>
  OSSIA_INLINE void
  operator()(domain_base<ossia::value>& domain, const T& incoming)
  {
    domain.max = ossia::value{incoming};
  }

  // Removal cases
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<int32_t>& domain, U&&...)
  {
    domain.max = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<float>& domain, U&&...)
  {
    domain.max = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<char>& domain, U&&...)
  {
    domain.max = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<bool>& domain, U&&...)
  {
    domain.max = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(vector_domain& domain, U&&...)
  {
    domain.max.clear();
  }
  template <std::size_t N, typename... U>
  OSSIA_INLINE void operator()(vecf_domain<N>& domain, U&&...)
  {
    for (std::size_t i = 0; i < N; i++)
      domain.max[i] = ossia::none;
  }
  template <typename... U>
  OSSIA_INLINE void operator()(domain_base<ossia::value>& domain, U&&...)
  {
    domain.max = ossia::none;
  }

  template <typename T, typename... U>
  OSSIA_INLINE void operator()(const domain_base<T>& domain, U&&...)
  {
  }
};

struct domain_minmax_creation_visitor
{
  template <typename T>
  OSSIA_INLINE domain operator()(T min, T max)
  {
    return domain_base<T>(min, max);
  }

  template <std::size_t N>
  OSSIA_INLINE domain
  operator()(const std::array<float, N>& lhs, const std::array<float, N>& rhs)
  {
    return vecf_domain<N>(lhs, rhs);
  }

  OSSIA_INLINE domain operator()(
      const std::vector<ossia::value>& min,
      const std::vector<ossia::value>& max)
  {
    return vector_domain(min, max);
  }
  OSSIA_INLINE domain
  operator()(std::vector<ossia::value>&& min, std::vector<ossia::value>&& max)
  {
    return vector_domain(std::move(min), std::move(max));
  }

  OSSIA_INLINE domain operator()(impulse, impulse)
  {
    return domain_base<impulse>{};
  }

  OSSIA_INLINE domain operator()(const std::string&, const std::string&)
  {
    return domain_base<std::string>();
  }

  // Cases where there is no possible domain
  template <typename T, typename U>
  OSSIA_INLINE domain operator()(const T&, const U&)
  {
    return domain{};
  }
};

/**
 * @brief The domain_value_set_creation_visitor struct
 *
 * Used to create domains with a set of acceptable values instead of a min /
 * max
 */
struct domain_value_set_creation_visitor
{
  const chobo::small_vector<ossia::value, 2>& values;
  template <typename T>
  domain operator()(const T&)
  {
    // Cases where there is no possible domain
    domain_base<T> dom;
    for (auto& value : values)
    {
      if (auto r = value.target<T>())
        dom.values.insert(*r);
    }
    return dom;
  }

  domain operator()(const std::vector<ossia::value>& orig)
  {
    vector_domain res;
    boost::container::flat_set<ossia::value> vals;

    for (const auto& value : values)
      vals.insert(value);

    res.values = std::vector<boost::container::flat_set<ossia::value>>(
        orig.size(), std::move(vals));

    return res;
  }

  template <std::size_t N>
  domain operator()(const std::array<float, N>& dom)
  {
    vecf_domain<N> res;
    boost::container::flat_set<float> vals;

    for (const auto& value : values)
      if (auto r = value.target<float>())
        vals.insert(*r);

    for (std::size_t i = 0; i < N - 1; i++)
      res.values[i] = vals;
    res.values[N - 1] = std::move(vals);
    return res;
  }

  domain operator()()
  {
    // TODO
    return {};
  }

  domain operator()(const impulse&)
  {
    return domain{};
  }
};

}
