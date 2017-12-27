#pragma once
#include <ossia/network/domain/domain_base.hpp>

namespace ossia
{

template <typename T>
struct domain_base;

template <typename Domain>
struct value_set_clamp
{
  const Domain& domain;
  template <typename U>
  value operator()(bounding_mode b, U&& val)
  {
    const auto& values = domain.values;
    if (b == bounding_mode::FREE || values.empty())
    {
      return std::forward<U>(val);
    }
    else
    {
      auto it = values.find(val);
      return (it != values.end()) ? ossia::value{std::forward<U>(val)}
                                  : ossia::value{};
    }
  }
};

struct value_set_get_visitor
{
    template <typename T>
    std::vector<ossia::value> operator()(const ossia::domain_base<T>& dom)
    {
      return std::vector<ossia::value>(dom.values.begin(), dom.values.end());
    }
    std::vector<ossia::value> operator()(const ossia::domain_base<ossia::impulse>& dom)
    {
      return std::vector<ossia::value>{};
    }

    std::vector<ossia::value> operator()(const ossia::vector_domain& dom)
    {
      // TODO
      return std::vector<ossia::value>{};
    }
    template<std::size_t N>
    std::vector<ossia::value> operator()(const ossia::vecf_domain<N>& dom)
    {
      // TODO
      return std::vector<ossia::value>{};
    }
};

struct value_set_update_visitor
{
  const std::vector<ossia::value>& values;
  template <typename T>
  void operator()(ossia::domain_base<T>& dom)
  {
    dom.values.clear();
    for (auto& value : values)
    {
      if (auto r = value.target<T>())
        dom.values.insert(*r);
    }
  }

  void operator()(ossia::vector_domain& dom)
  {
    for (auto& set : dom.values)
    {
      set.clear();
      for (auto& value : values)
      {
        dom.values[0].insert(value);
      }
    }
  }

  void operator()(ossia::domain_base<ossia::value>& dom)
  {
    dom.values.clear();
    for (auto& value : values)
    {
      dom.values.insert(value);
    }
  }

  template <std::size_t N>
  void operator()(ossia::vecf_domain<N>& dom)
  {
    for (std::size_t i = 0; i < N; i++)
      dom.values[i].clear();

    for (auto& value : values)
    {
      if (auto r = value.target<float>())
      {
        for (std::size_t i = 0; i < N; i++)
        {
          dom.values[i].insert(*r);
        }
      }
    }
  }

  void operator()(ossia::domain_base<impulse>&)
  {
  }
};
}
