#pragma once
#include <ossia/network/domain/domain_base.hpp>
namespace ossia
{

/**
 * Applying a domain value by value to arrays
 */
struct list_clamp
{
  const vector_domain& domain;

  ossia::value
  operator()(bounding_mode b, const std::vector<ossia::value>& val) const;
  ossia::value
  operator()(bounding_mode b, std::vector<ossia::value>&& val) const;
  // TODO numeric_list_clamp that will be used instead
  // of the loops in domain_clamp_visitor
};

template <std::size_t N>
struct vec_clamp
{
  const vecf_domain<N>& domain;

  ossia::value operator()(bounding_mode b, std::array<float, N> val) const;
};
}
