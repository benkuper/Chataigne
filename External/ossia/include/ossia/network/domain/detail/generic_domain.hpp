#pragma once
#include <ossia/network/domain/domain_base.hpp>
namespace ossia
{
/**
 * Applying a domain without caring about the underlying type
 */
struct generic_clamp
{
  const domain_base<ossia::value>& domain;

  ossia::value operator()(bounding_mode b, const ossia::value& v) const;
  ossia::value operator()(bounding_mode b, ossia::value&& v) const;

  // TODO should we have the same optimization for Vec?
  // This will clamp the List component by component with the given value.
  value
  operator()(bounding_mode b, const std::vector<ossia::value>& val) const;
  value operator()(bounding_mode b, std::vector<ossia::value>&& val) const;
};
}
