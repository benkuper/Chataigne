#pragma once
#include <ossia/network/value/value_base.hpp>
#include <chobo/small_vector.hpp>
#include <vector>
#include <ossia_export.h>
namespace ossia
{
class value;
template <typename T>
struct domain_base;
struct domain;
struct vector_domain;
template <std::size_t N>
struct vecf_domain;

OSSIA_EXPORT value
apply_domain(const domain& dom, bounding_mode b, const ossia::value& val);
OSSIA_EXPORT value
apply_domain(const domain& dom, bounding_mode b, ossia::value&& val);

OSSIA_EXPORT value get_min(const domain& dom);
OSSIA_EXPORT value get_max(const domain& dom);

OSSIA_EXPORT void set_min(domain& dom, const ossia::value& val);
OSSIA_EXPORT void set_max(domain& dom, const ossia::value& val);

OSSIA_EXPORT void
set_values(domain& dom, const std::vector<ossia::value>& val);
OSSIA_EXPORT std::vector<ossia::value>
get_values(const ossia::domain& dom);

OSSIA_EXPORT domain make_domain(
    const ossia::value& min,
    const ossia::value& max);
OSSIA_EXPORT domain make_domain(const std::vector<std::string>& s);
OSSIA_EXPORT domain make_domain(
    const chobo::small_vector<ossia::value, 2>& val, const ossia::value& cur);
OSSIA_EXPORT domain make_domain(
    const ossia::value& min, const ossia::value& max,
    const std::vector<ossia::value>& vals);

OSSIA_EXPORT
ossia::domain make_domain_from_minmax(
    const std::vector<ossia::value>& min,
    const std::vector<ossia::value>& max,
    ossia::val_type v);

OSSIA_EXPORT domain init_domain(ossia::val_type type);

template <typename T>
bool operator!=(const domain_base<T>& lhs, const domain_base<T>& rhs)
{
  return !(lhs == rhs);
}
}
namespace std
{
std::ostream& operator<<(std::ostream&, const ossia::domain&);
}
