#pragma once
#include <ossia/network/domain/domain.hpp>

namespace ossia
{
template <typename U>
struct domain_conversion
{

  template <typename T>
  OSSIA_INLINE domain operator()(const T&)
  {
    return U{};
  }

  domain operator()(const domain_base<int32_t>& t)
  {
    U f;
    if (t.min)
      f.min = *t.min;
    if (t.max)
      f.max = *t.max;
    if (!t.values.empty())
      for (auto val : t.values)
        f.values.insert(val);
    return f;
  }

  domain operator()(const domain_base<float>& t)
  {
    U f;
    if (t.min)
      f.min = *t.min;
    if (t.max)
      f.max = *t.max;
    if (!t.values.empty())
      for (auto val : t.values)
        f.values.insert(val);
    return f;
  }

  domain operator()(const domain_base<bool>& t)
  {
    U f;
    if (t.min)
      f.min = *t.min;
    if (t.max)
      f.max = *t.max;
    if (!t.values.empty())
      for (auto val : t.values)
        f.values.insert(val);
    return f;
  }

  domain operator()(const domain_base<char>& t)
  {
    U f;
    if (t.min)
      f.min = *t.min;
    if (t.max)
      f.max = *t.max;
    if (!t.values.empty())
      for (auto val : t.values)
        f.values.insert(val);
    return f;
  }
};

template <>
struct domain_conversion<domain_base<impulse>>
{
  template <typename T>
  OSSIA_INLINE domain operator()(const T&)
  {
    return domain_base<impulse>{};
  }
};

template <>
struct domain_conversion<domain_base<bool>>
{
  template <typename T>
  OSSIA_INLINE domain operator()(const T&)
  {
    return domain_base<bool>{false, true};
  }
  OSSIA_INLINE domain operator()(const domain_base<impulse>&)
  {
    return domain_base<bool>{false, true};
  }
  OSSIA_INLINE domain operator()(const domain_base<std::string>&)
  {
    return domain_base<bool>{false, true};
  }
  template<typename T>
  domain operator()(const domain_base<T>& t)
  {
    domain_base<bool> f;
    if (t.min)
      f.min = bool(t.min);
    if (t.max)
      f.max = bool(t.max);
    return f;
  }
};
template <>
struct domain_conversion<vector_domain>
{
  OSSIA_INLINE domain operator()(const vector_domain& src)
  {
    return src;
  }

  template <typename T>
  OSSIA_INLINE domain operator()(const T&)
  {
    return vector_domain();
  }
};

template <std::size_t N>
struct domain_conversion<vecf_domain<N>>
{
  OSSIA_INLINE domain operator()(const vecf_domain<N>& src)
  {
    return src;
  }
  vecf_domain<N> list_func(const vector_domain& t)
  {
    vecf_domain<N> dom;
    const std::size_t min_size = std::min(N, t.min.size());
    for (std::size_t i = 0; i < min_size; i++)
    {
      dom.min[i] = ossia::convert<float>(t.min[i]);
    }

    const std::size_t max_size = std::min(N, t.max.size());
    for (std::size_t i = 0; i < max_size; i++)
    {
      dom.max[i] = ossia::convert<float>(t.max[i]);
    }

    const std::size_t vals_size = std::min(N, t.values.size());
    for (std::size_t i = 0; i < vals_size; i++)
    {
      dom.values[i].clear();
      for (auto& val : t.values[i])
        dom.values[i].insert(ossia::convert<float>(val));
    }

    return dom;
  }

  OSSIA_INLINE domain operator()(const vector_domain& t)
  {
    return list_func(t);
  }

  template <typename T>
  OSSIA_INLINE domain operator()(const T&)
  {
    return vecf_domain<N>();
  }
};

template <>
struct domain_conversion<domain_base<std::string>>
{
  OSSIA_INLINE domain operator()(const domain_base<std::string>& src)
  {
    return src;
  }

  template <typename T>
  OSSIA_INLINE domain operator()(const T&)
  {
    return domain_base<std::string>();
  }
};

// TODO handle the ossia::value case

inline domain convert_domain(const domain& dom, ossia::val_type newtype)
{
  switch (newtype)
  {
    case val_type::IMPULSE:
      return ossia::apply_nonnull(
          domain_conversion<domain_base<impulse>>{}, dom);
    case val_type::INT:
      return ossia::apply_nonnull(
          domain_conversion<domain_base<int32_t>>{}, dom);
    case val_type::FLOAT:
      return ossia::apply_nonnull(
          domain_conversion<domain_base<float>>{}, dom);
    case val_type::BOOL:
      return ossia::apply_nonnull(domain_conversion<domain_base<bool>>{}, dom);
    case val_type::CHAR:
      return ossia::apply_nonnull(domain_conversion<domain_base<char>>{}, dom);
    case val_type::STRING:
      return ossia::apply_nonnull(
          domain_conversion<domain_base<std::string>>{}, dom);
    case val_type::LIST:
      return ossia::apply_nonnull(domain_conversion<vector_domain>{}, dom);
    case val_type::VEC2F:
      return ossia::apply_nonnull(domain_conversion<vecf_domain<2>>{}, dom);
    case val_type::VEC3F:
      return ossia::apply_nonnull(domain_conversion<vecf_domain<3>>{}, dom);
    case val_type::VEC4F:
      return ossia::apply_nonnull(domain_conversion<vecf_domain<4>>{}, dom);
    default:
      return domain{};
  }
}

inline void convert_compatible_domain(domain& dom, ossia::val_type newtype)
{
  constexpr const auto list_index = 6;
  // eggs::variants::detail::checked_index_of<vector_domain,
  // domain_base_variant>::count;

  // Converts domains but keeps compatible different domains.
  // e.g. a float domain works for vec4f or list.
  //! \note check this if the order in domain_base_variant changes.
  if (dom.which() < list_index)
  {
    dom = convert_domain(dom, newtype);
  }
}
}
