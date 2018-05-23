#pragma once
#include <ossia/detail/math.hpp>
#include <ossia/network/domain/domain_base.hpp>

namespace ossia
{

template <typename T>
struct domain_base;

template <typename Domain>
struct numeric_clamp
{
  const Domain& domain;

  template <typename U>
  ossia::value operator()(bounding_mode b, U&& val) const;

  /**
   * Fast algorithm for applying a numeric domain to a Vec<float>
   */
  template <std::size_t N>
  ossia::value operator()(bounding_mode b, std::array<float, N> val) const;
};
}
