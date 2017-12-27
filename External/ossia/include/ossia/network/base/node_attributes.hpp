#pragma once
#include <ossia/detail/any_map.hpp>
#include <ossia/detail/optional.hpp>
#include <ossia/network/value/value.hpp>
#include <ossia/network/base/parameter.hpp>
#include <ossia/network/base/node_functions.hpp>
#include <ossia/network/common/parameter_properties.hpp>
#include <ossia/network/common/extended_types.hpp>
#include <ossia_export.h>

#include <boost/any.hpp>
#include <hopscotch_map.h>
#include <string>
#include <vector>

/**
 * \file node_attributes.hpp
 *
 * This file defines default types and accessors for common extended
 * attributes.
 *
 * The attributes are defined as accessors to the
 * extended attributes container.
 *
 * The implementation of the functions is at the end of \ref node.cpp
 */
namespace ossia
{
namespace net
{
class node_base;
struct parameter_data;

//! How many instances a node can have
struct OSSIA_EXPORT instance_bounds
{
  instance_bounds() = default;
  instance_bounds(const instance_bounds&) = default;
  instance_bounds(instance_bounds&&) = default;
  instance_bounds& operator=(const instance_bounds&) = default;
  instance_bounds& operator=(instance_bounds&&) = default;

  instance_bounds(int32_t min, int32_t max)
      : min_instances{min}, max_instances{max}
  {
  }

  int32_t min_instances = 0;
  int32_t max_instances = std::numeric_limits<int32_t>::max();

  friend bool operator==(const instance_bounds& lhs, const instance_bounds& rhs)
  {
    return lhs.min_instances == rhs.min_instances
        && lhs.max_instances == rhs.max_instances;
  }
  friend bool operator!=(const instance_bounds& lhs, const instance_bounds& rhs)
  {
    return !(lhs == rhs);
  }

  // TODO, currently only here for ossia::any
  friend std::ostream& operator<<(std::ostream& os, const instance_bounds& c)
  {
    return os;
  }
  friend std::istream& operator>>(std::istream& is, instance_bounds& c)
  {
    return is;
  }
};


//! Tags applied to a node: {"model", "interesting", ...}
using tags = std::vector<std::string>;

//! Human-readable description of a node
using description = std::string;

//! When a node must be sent before other
using priority = float;

//! How often a node is refreshed
using refresh_rate = int32_t;

//! Granularity of the space
using value_step_size = double;

//! The value on which a node should revert by default
using default_value = ossia::value;

//! Means that the node is very important, e.g. a "play" message
using critical = bool;

//! A disabled node
using disabled = bool;

//! Means that the node is not present on the "host" device
using zombie = bool;

//! Means that the node should not be advertised by default
using hidden = bool;

//! Means that the node should not send / receives network messages
using muted = bool;

//! Device attribute : the name of the software managed by this device
using app_name = std::string;

//! Device attribute : the version of the software managed by this device
using app_version = std::string;

//! Device attribute : the creator of the software managed by this device
using app_creator = std::string;

OSSIA_EXPORT ossia::string_view text_instance_bounds();
OSSIA_EXPORT optional<instance_bounds> get_instance_bounds(const extended_attributes& n);
OSSIA_EXPORT void set_instance_bounds(extended_attributes& n, optional<instance_bounds>);
OSSIA_EXPORT void set_instance_bounds(ossia::net::node_base& n, optional<instance_bounds>);

OSSIA_EXPORT ossia::string_view text_tags();
OSSIA_EXPORT optional<tags> get_tags(const extended_attributes& n);
OSSIA_EXPORT void set_tags(extended_attributes& n, optional<tags> v);
OSSIA_EXPORT void set_tags(ossia::net::node_base& n, optional<tags> v);

OSSIA_EXPORT ossia::string_view text_description();
OSSIA_EXPORT optional<description> get_description(const extended_attributes& n);
OSSIA_EXPORT void set_description(extended_attributes& n, optional<description> v);
OSSIA_EXPORT void set_description(extended_attributes& n, const char* v);
OSSIA_EXPORT void set_description(ossia::net::node_base& n, optional<description> v);
OSSIA_EXPORT void set_description(ossia::net::node_base& n, const char* v);

OSSIA_EXPORT ossia::string_view text_priority();
OSSIA_EXPORT optional<priority> get_priority(const extended_attributes& n);
OSSIA_EXPORT void set_priority(extended_attributes& n, optional<priority> v);
OSSIA_EXPORT void set_priority(ossia::net::node_base& n, optional<priority> v);

OSSIA_EXPORT ossia::string_view text_refresh_rate();
OSSIA_EXPORT optional<refresh_rate> get_refresh_rate(const extended_attributes& n);
OSSIA_EXPORT void set_refresh_rate(extended_attributes& n, optional<refresh_rate> v);
OSSIA_EXPORT void set_refresh_rate(ossia::net::node_base& n, optional<refresh_rate> v);

OSSIA_EXPORT ossia::string_view text_value_step_size();
OSSIA_EXPORT optional<value_step_size> get_value_step_size(const extended_attributes& n);
OSSIA_EXPORT void set_value_step_size(extended_attributes& n, optional<value_step_size> v);
OSSIA_EXPORT void set_value_step_size(ossia::net::node_base& n, optional<value_step_size> v);

OSSIA_EXPORT ossia::string_view text_zombie();
OSSIA_EXPORT zombie get_zombie(const extended_attributes& n);
OSSIA_EXPORT void set_zombie(extended_attributes& n, zombie v);
OSSIA_EXPORT void set_zombie(ossia::net::node_base& n, zombie v);

OSSIA_EXPORT ossia::string_view text_hidden();
OSSIA_EXPORT hidden get_hidden(const extended_attributes& n);
OSSIA_EXPORT void set_hidden(extended_attributes& n, hidden v);
OSSIA_EXPORT void set_hidden(ossia::net::node_base& n, hidden v);

OSSIA_EXPORT ossia::string_view text_extended_type();
OSSIA_EXPORT optional<extended_type> get_extended_type(const ossia::net::node_base& n);
OSSIA_EXPORT void set_extended_type(extended_attributes& n, optional<extended_type> v);
OSSIA_EXPORT void set_extended_type(ossia::net::node_base& n, optional<extended_type> v);

OSSIA_EXPORT ossia::string_view text_app_name();
OSSIA_EXPORT optional<app_name> get_app_name(const extended_attributes& n);
OSSIA_EXPORT void set_app_name(extended_attributes& n, optional<app_name> v);
OSSIA_EXPORT void set_app_name(ossia::net::node_base& n, optional<app_name> v);
OSSIA_EXPORT void set_app_name(extended_attributes& n, const char* v); // to please clang
OSSIA_EXPORT void set_app_name(ossia::net::node_base& n, const char* v);

OSSIA_EXPORT ossia::string_view text_app_version();
OSSIA_EXPORT optional<app_version> get_app_version(const extended_attributes& n);
OSSIA_EXPORT void set_app_version(extended_attributes& n, optional<app_version> v);
OSSIA_EXPORT void set_app_version(ossia::net::node_base& n, optional<app_version> v);
OSSIA_EXPORT void set_app_version(extended_attributes& n, const char* v); // to please clang
OSSIA_EXPORT void set_app_version(ossia::net::node_base& n, const char* v);

OSSIA_EXPORT ossia::string_view text_app_creator();
OSSIA_EXPORT optional<app_creator> get_app_creator(const extended_attributes& n);
OSSIA_EXPORT void set_app_creator(extended_attributes& n, optional<app_creator> v);
OSSIA_EXPORT void set_app_creator(ossia::net::node_base& n, optional<app_creator> v);
OSSIA_EXPORT void set_app_creator(extended_attributes& n, const char* v); // to please clang
OSSIA_EXPORT void set_app_creator(ossia::net::node_base& n, const char* v);

OSSIA_EXPORT ossia::string_view text_default_value();
OSSIA_EXPORT optional<ossia::value> get_default_value(const extended_attributes& n);
OSSIA_EXPORT void set_default_value(extended_attributes& n, const ossia::value& v);
OSSIA_EXPORT void set_default_value(extended_attributes& n, ossia::value&& v);
OSSIA_EXPORT void set_default_value(extended_attributes& n, int v);
OSSIA_EXPORT void set_default_value(extended_attributes& n, long v);
OSSIA_EXPORT void set_default_value(extended_attributes& n, bool v);
OSSIA_EXPORT void set_default_value(extended_attributes& n, char v);
OSSIA_EXPORT void set_default_value(extended_attributes& n, float v);
OSSIA_EXPORT void set_default_value(extended_attributes& n, double v);
OSSIA_EXPORT void set_default_value(extended_attributes& n, const char* v);
OSSIA_EXPORT void set_default_value(extended_attributes& n, ossia::none_t v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, const ossia::value& v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, ossia::value&& v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, int v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, long v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, bool v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, char v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, float v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, double v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, const char* v);
OSSIA_EXPORT void set_default_value(ossia::net::node_base& n, ossia::none_t v);

// These attributes require a parameter
OSSIA_EXPORT ossia::string_view text_value();
OSSIA_EXPORT value clone_value(const ossia::net::node_base& n);
OSSIA_EXPORT void set_value(ossia::net::node_base& n, value v);
OSSIA_EXPORT void set_value(ossia::net::parameter_data& n, value v);

OSSIA_EXPORT ossia::string_view text_value_type();
OSSIA_EXPORT optional<val_type> get_value_type(const ossia::net::node_base& n);
OSSIA_EXPORT void set_value_type(ossia::net::node_base& n, val_type v);

OSSIA_EXPORT ossia::string_view text_domain();
OSSIA_EXPORT domain get_domain(const ossia::net::node_base& n);
OSSIA_EXPORT void set_domain(ossia::net::node_base& n, domain v);
OSSIA_EXPORT void set_domain(ossia::net::parameter_data& n, domain v);

OSSIA_EXPORT ossia::string_view text_access_mode();
OSSIA_EXPORT optional<access_mode> get_access_mode(const ossia::net::node_base& n);
OSSIA_EXPORT void set_access_mode(ossia::net::node_base& n, access_mode v);
OSSIA_EXPORT void set_access_mode(ossia::net::parameter_data& n, access_mode v);

OSSIA_EXPORT ossia::string_view text_bounding_mode();
OSSIA_EXPORT optional<bounding_mode> get_bounding_mode(const ossia::net::node_base& n);
OSSIA_EXPORT void set_bounding_mode(ossia::net::node_base& n, bounding_mode v);
OSSIA_EXPORT void set_bounding_mode(ossia::net::parameter_data& n, bounding_mode v);

OSSIA_EXPORT ossia::string_view text_repetition_filter();
OSSIA_EXPORT repetition_filter get_repetition_filter(const ossia::net::node_base& n);
OSSIA_EXPORT void set_repetition_filter(ossia::net::node_base& n, repetition_filter v);
OSSIA_EXPORT void set_repetition_filter(ossia::net::parameter_data& n, repetition_filter v);

OSSIA_EXPORT ossia::string_view text_unit();
OSSIA_EXPORT unit_t get_unit(const ossia::net::node_base& n);
OSSIA_EXPORT void set_unit(ossia::net::node_base& n, unit_t v);
OSSIA_EXPORT void set_unit(ossia::net::parameter_data& n, unit_t v);

OSSIA_EXPORT ossia::string_view text_muted();
OSSIA_EXPORT muted get_muted(const ossia::net::node_base& n);
OSSIA_EXPORT void set_muted(ossia::net::node_base& n, muted v);
OSSIA_EXPORT void set_muted(ossia::net::parameter_data& n, muted v);

OSSIA_EXPORT ossia::string_view text_disabled();
OSSIA_EXPORT disabled get_disabled(const ossia::net::node_base& n);
OSSIA_EXPORT void set_disabled(ossia::net::node_base& n, disabled v);
OSSIA_EXPORT void set_disabled(ossia::net::parameter_data& n, disabled v);

OSSIA_EXPORT ossia::string_view text_critical();
OSSIA_EXPORT critical get_critical(const ossia::net::node_base& n);
OSSIA_EXPORT void set_critical(ossia::net::node_base& n, critical v);
OSSIA_EXPORT void set_critical(ossia::net::parameter_data& n, critical v);

template<typename T>
struct is_parameter_attribute : public std::false_type { };
// Some macros to have minimal reflection facilities...
#define OSSIA_ATTRIBUTE(Type, Name)                               \
  struct OSSIA_EXPORT Name##_attribute                            \
  {                                                               \
    using type = Type;                                            \
    static auto text()                                            \
    {                                                             \
      return ossia::net::text_##Name();                           \
    }                                                             \
    template <typename... Args>                                   \
    static auto getter(Args&&... args)                            \
    {                                                             \
      return ossia::net::get_##Name(std::forward<Args>(args)...); \
    }                                                             \
    template <typename... Args>                                   \
    static auto setter(Args&&... args)                            \
    {                                                             \
      return ossia::net::set_##Name(std::forward<Args>(args)...); \
    }                                                             \
  };

#define OSSIA_PARAM_ATTRIBUTE(Type, Name) \
  OSSIA_ATTRIBUTE(Type, Name) \
template<> \
struct is_parameter_attribute<Name##_attribute> : public std::true_type { };

#define OSSIA_ATTRIBUTE_2(Type, Name, Text, Get, Set) \
  struct OSSIA_EXPORT Name##_attribute                \
  {                                                   \
    using type = Type;                                \
    static auto text()                                \
    {                                                 \
      return Text();                                  \
    }                                                 \
    template <typename... Args>                       \
    static auto getter(Args&&... args)                \
    {                                                 \
      return Get(std::forward<Args>(args)...);        \
    }                                                 \
    template <typename... Args>                       \
    static auto setter(Args&&... args)                \
    {                                                 \
      return Set(std::forward<Args>(args)...);        \
    }                                                 \
  };

// Attributes of a parameter
OSSIA_ATTRIBUTE_2(
    ossia::value, value, ossia::net::text_value, ossia::net::clone_value,
    ossia::net::push_value)

template<> \
struct is_parameter_attribute<value_attribute> : public std::true_type { };

OSSIA_PARAM_ATTRIBUTE(ossia::val_type, value_type)
OSSIA_PARAM_ATTRIBUTE(ossia::domain, domain)
OSSIA_PARAM_ATTRIBUTE(ossia::access_mode, access_mode)
OSSIA_PARAM_ATTRIBUTE(ossia::bounding_mode, bounding_mode)
OSSIA_PARAM_ATTRIBUTE(ossia::unit_t, unit)
OSSIA_PARAM_ATTRIBUTE(ossia::value, default_value)
OSSIA_PARAM_ATTRIBUTE(ossia::net::disabled, disabled)
OSSIA_PARAM_ATTRIBUTE(ossia::net::muted, muted)
OSSIA_PARAM_ATTRIBUTE(ossia::net::critical, critical)

// Metadata attributes
OSSIA_ATTRIBUTE(ossia::net::tags, tags)
OSSIA_ATTRIBUTE(ossia::net::description, description)
OSSIA_ATTRIBUTE(ossia::net::refresh_rate, refresh_rate)
OSSIA_ATTRIBUTE(ossia::net::priority, priority)
OSSIA_ATTRIBUTE(ossia::net::value_step_size, value_step_size)
OSSIA_ATTRIBUTE(ossia::net::instance_bounds, instance_bounds)
OSSIA_ATTRIBUTE(ossia::extended_type, extended_type)
OSSIA_ATTRIBUTE(ossia::repetition_filter, repetition_filter)
OSSIA_ATTRIBUTE(ossia::net::app_name, app_name)
OSSIA_ATTRIBUTE(ossia::net::app_creator, app_creator)
OSSIA_ATTRIBUTE(ossia::net::app_version, app_version)
OSSIA_ATTRIBUTE(ossia::net::hidden, hidden)

template <typename T, typename U>
bool compare_optional(const T& t, const U& u)
{
  return !t || (t && *t != u);
}
inline bool compare_optional(const ossia::value& t, const ossia::value& u)
{
  return t != u;
}
template <typename U>
inline bool compare_optional(const ossia::domain& t, const U& u)
{
  return t != u;
}
template <typename U>
inline bool compare_optional(const ossia::unit_t& t, const U& u)
{
  return t != u;
}
inline bool compare_optional(bool t, bool u)
{
  return t != u;
}
inline bool
compare_optional(ossia::repetition_filter t, ossia::repetition_filter u)
{
  return t != u;
}

inline bool valid(const ossia::value& v)
{
  return v.valid();
}
template <typename T>
inline bool valid(const T& v)
{
  return bool(v);
}
}
}
