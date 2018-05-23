#pragma once
#include <ossia/network/domain/domain_base_impl.hpp>
/**
 * \file domain_base.hpp
 */
namespace ossia
{
#include <ossia/network/domain/domain_variant_impl.hpp>
/**
 * \brief domain A domain of values
 *
 * The practical domain may be different according to the value type.
 * For instance, \ref domain_base<Impulse> has no min nor max.
 *
 * Domains for most types have a min, a max and a set of values.
 * Domain for String has a set of values.
 *
 * It is used to restrict a value to the domain if available.
 *
 * \todo same order than value
 */
struct OSSIA_EXPORT domain
{
  domain_base_variant v;

  domain() noexcept
  {
  }

  template <typename T>
  domain(const ossia::domain_base<T>& arg) : v(arg)
  {
  }
  template <typename T>
  domain(ossia::domain_base<T>&& arg) : v(std::move(arg))
  {
  }
  domain(const vector_domain& arg) : v(arg)
  {
  }
  domain(vector_domain&& arg) : v(std::move(arg))
  {
  }
  template <std::size_t N>
  domain(const vecf_domain<N>& arg) : v(arg)
  {
  }
  template <std::size_t N>
  domain(vecf_domain<N>&& arg) : v(std::move(arg))
  {
  }
  domain(const domain& d) : v{d.v}
  {
  }
  domain(domain&& d) : v{std::move(d.v)}
  {
  }
  domain& operator=(const domain& d)
  {
    v = d.v;
    return *this;
  }
  domain& operator=(domain&& d)
  {
    v = std::move(d.v);
    return *this;
  }

  value get_min() const;
  value get_max() const;

  operator bool() const
  {
    return bool(v);
  }
  auto which() const
  {
    return v.which();
  }

  template <typename T>
  OSSIA_INLINE T get_min() const
  {
    return get_min().get<T>();
  }
  template <typename T>
  OSSIA_INLINE T get_max() const
  {
    return get_max().get<T>();
  }

  template <typename T>
  ossia::optional<T> maybe_min() const
  {
    auto v = get_min();
    auto u = v.target<T>();
    if (u)
      return *u;
    else
      return {};
  }

  template <typename T>
  ossia::optional<T> maybe_max() const
  {
    auto v = get_max();
    auto u = v.target<T>();
    if (u)
      return *u;
    else
      return {};
  }

  template <typename T>
  OSSIA_INLINE T convert_min() const
  {
    return ossia::convert<T>(get_min());
  }
  template <typename T>
  OSSIA_INLINE T convert_max() const
  {
    return ossia::convert<T>(get_max());
  }

  void set_min(const ossia::value& val);
  void set_max(const ossia::value& val);

  value apply(bounding_mode b, const ossia::value& val) const;
  value apply(bounding_mode b, ossia::value&& val) const;

  std::string to_pretty_string() const;

  friend bool operator==(const ossia::domain& d, const ossia::domain& other)
  {
    return d.v == other.v;
  }

  friend bool operator!=(const ossia::domain& d, const ossia::domain& other)
  {
    return d.v != other.v;
  }

  template <typename T>
  friend bool operator==(const ossia::domain& d, const T& other)
  {
    return d.v == other;
  }
  template <typename T>
  friend bool operator!=(const ossia::domain& d, const T& other)
  {
    return d.v != other;
  }
};

template <typename Functor>
auto apply(Functor&& functor, const domain& var) -> decltype(auto)
{
  return ossia::apply(std::forward<Functor>(functor), var.v);
}
template <typename Functor>
auto apply(Functor&& functor, domain& var) -> decltype(auto)
{
  return ossia::apply(std::forward<Functor>(functor), var.v);
}
template <typename Functor>
auto apply(Functor&& functor, domain&& var) -> decltype(auto)
{
  return ossia::apply(std::forward<Functor>(functor), std::move(var.v));
}
template <typename Functor>
auto apply_nonnull(Functor&& functor, const domain& var) -> decltype(auto)
{
  return ossia::apply_nonnull(std::forward<Functor>(functor), var.v);
}
template <typename Functor>
auto apply_nonnull(Functor&& functor, domain& var) -> decltype(auto)
{
  return ossia::apply_nonnull(std::forward<Functor>(functor), var.v);
}
template <typename Functor>
auto apply_nonnull(Functor&& functor, domain&& var) -> decltype(auto)
{
  return ossia::apply_nonnull(
      std::forward<Functor>(functor), std::move(var.v));
}
}
