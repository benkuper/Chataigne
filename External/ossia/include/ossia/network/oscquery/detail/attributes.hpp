#pragma once
#include <ossia/network/base/parameter.hpp>
#include <ossia/network/base/node.hpp>
#include <ossia/network/base/node_attributes.hpp>
#include <ossia/network/common/parameter_properties.hpp>
#include <ossia/network/oscquery/detail/typetag.hpp>

#include <ossia/detail/string_view.hpp>
#include <brigand/sequences/list.hpp>

/**
 * \file ossia/network/oscquery/detail/attributes.hpp
 *
 * This file contains mapping from the ossia attributes to the OSCQuery
 * attributes.
 */
namespace ossia
{
namespace oscquery
{
namespace detail
{

// Attributes
constexpr auto osc_port()
{
  return "OSC_PORT";
}
constexpr auto attribute_full_path()
{
  return "FULL_PATH";
}

constexpr auto attribute_typetag()
{
  return "TYPE";
}
constexpr auto contents()
{
  return "CONTENTS";
}

constexpr auto attribute_value()
{
  return "VALUE";
}
constexpr auto attribute_range()
{
  return "RANGE";
}
constexpr auto attribute_clipmode()
{
  return "CLIPMODE";
}
constexpr auto attribute_accessmode()
{
  return "ACCESS";
}
constexpr auto attribute_description()
{
  return "DESCRIPTION";
}
constexpr auto attribute_tags()
{
  return "TAGS";
}

// Not part of the OSCQuery spec :
constexpr auto attribute_unit()
{
  return "UNIT";
}
constexpr auto attribute_refresh_rate()
{
  return "REFRESH_RATE";
}
constexpr auto attribute_priority()
{
  return "PRIORITY";
}
constexpr auto attribute_step_size()
{
  return "STEP_SIZE";
}
constexpr auto attribute_instance_bounds()
{
  return "INSTANCE_BOUNDS";
}
constexpr auto attribute_critical()
{
  return "CRITICAL";
}
constexpr auto attribute_hidden()
{
  return "HIDDEN";
}
constexpr auto attribute_disabled()
{
  return "DISABLED";
}
constexpr auto attribute_extended_type()
{
  return "EXTENDED_TYPE";
}
constexpr auto attribute_repetition_filter()
{
  return "REPETITION_FILTER";
}
constexpr auto attribute_default_value()
{
  return "DEFAULT_VALUE";
}
constexpr auto attribute_app_name()
{
  return "APP_NAME";
}
constexpr auto attribute_app_version()
{
  return "APP_VERSION";
}
constexpr auto attribute_app_creator()
{
  return "APP_CREATOR";
}

// Commands
constexpr auto command()
{
  return "COMMAND";
}
constexpr auto data()
{
  return "DATA";
}

constexpr auto path_added()
{
  return "PATH_ADDED";
}
constexpr auto path_removed()
{
  return "PATH_REMOVED";
}
constexpr auto path_changed()
{
  return "PATH_CHANGED";
}
constexpr auto attributes_changed()
{
  return "ATTRIBUTES_CHANGED";
}

// Queries
constexpr auto add_node()
{
  return make_string_view("ADD_NODE");
}
constexpr auto remove_node()
{
  return make_string_view("REMOVE_NODE");
}
constexpr auto node_name()
{
  return make_string_view("NAME");
}
constexpr auto set_port()
{
  return make_string_view("SET_PORT");
}
constexpr auto local_port()
{
  return make_string_view("LOCAL_PORT");
}
constexpr auto listen()
{
  return make_string_view("LISTEN");
}
constexpr auto text_true()
{
  return make_string_view("TRUE");
}
constexpr auto text_false()
{
  return make_string_view("FALSE");
}
constexpr auto query_listen_true()
{
  return make_string_view("?LISTEN=TRUE");
}
constexpr auto query_listen_false()
{
  return make_string_view("?LISTEN=FALSE");
}
constexpr auto query_value()
{
  return make_string_view("?VALUE");
}

struct OSSIA_EXPORT full_path_attribute
{
  using type = std::string;
  static auto text()
  {
    return detail::attribute_full_path();
  }
  static auto getter(const ossia::net::node_base& n)
  {
    return ossia::net::osc_parameter_string(n);
  }
};

struct OSSIA_EXPORT typetag_attribute
{
  using type = std::string;
  static auto text()
  {
    return ossia::net::text_value_type();
  }
  static auto getter(const ossia::net::node_base& n)
  {
    return oscquery::get_osc_typetag(n);
  }
  static auto setter(ossia::net::node_base& n, const std::string& t)
  {
    return oscquery::set_osc_typetag(n, t);
  }
};

template <typename Attr>
struct metadata;

// oscquery-specific
template <>
struct metadata<full_path_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_full_path();
  }
};
template <>
struct metadata<typetag_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_typetag();
  }
};

// common attributes
template <>
struct metadata<net::value_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_value();
  }
};
template <>
struct metadata<net::domain_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_range();
  }
};
template <>
struct metadata<net::access_mode_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_accessmode();
  }
};
template <>
struct metadata<net::bounding_mode_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_clipmode();
  }
};
template <>
struct metadata<net::unit_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_unit();
  }
};
template <>
struct metadata<net::default_value_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_default_value();
  }
};
template <>
struct metadata<net::tags_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_tags();
  }
};
template <>
struct metadata<net::refresh_rate_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_refresh_rate();
  }
};
template <>
struct metadata<net::priority_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_priority();
  }
};
template <>
struct metadata<net::value_step_size_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_step_size();
  }
};
template <>
struct metadata<net::instance_bounds_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_instance_bounds();
  }
};
template <>
struct metadata<net::critical_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_critical();
  }
};
template <>
struct metadata<net::hidden_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_hidden();
  }
};
template <>
struct metadata<net::disabled_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_disabled();
  }
};
template <>
struct metadata<net::extended_type_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_extended_type();
  }
};
template <>
struct metadata<net::description_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_description();
  }
};
template <>
struct metadata<net::repetition_filter_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_repetition_filter();
  }
};
template <>
struct metadata<net::app_version_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_app_version();
  }
};
template <>
struct metadata<net::app_creator_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_app_creator();
  }
};
template <>
struct metadata<net::app_name_attribute>
{
  static constexpr auto key()
  {
    return detail::attribute_app_name();
  }
};

using base_attributes = brigand::
    list<typetag_attribute, net::value_attribute, net::domain_attribute, net::access_mode_attribute, net::bounding_mode_attribute, net::repetition_filter_attribute, net::unit_attribute, net::default_value_attribute>;

using base_attributes_without_type = brigand::
    list<net::value_attribute, net::domain_attribute, net::access_mode_attribute, net::bounding_mode_attribute, net::repetition_filter_attribute, net::unit_attribute, net::default_value_attribute>;

using extended_attributes = brigand::
    list<net::tags_attribute, net::refresh_rate_attribute, net::priority_attribute, net::value_step_size_attribute, net::instance_bounds_attribute, net::critical_attribute, net::hidden_attribute, net::disabled_attribute, net::extended_type_attribute, net::description_attribute, net::app_name_attribute, net::app_creator_attribute, net::app_version_attribute>;

using attributes_when_reading = brigand::list<net::domain_attribute, net::access_mode_attribute, net::bounding_mode_attribute, net::repetition_filter_attribute, net::tags_attribute, net::refresh_rate_attribute, net::priority_attribute, net::value_step_size_attribute, net::instance_bounds_attribute, net::critical_attribute, net::hidden_attribute, net::disabled_attribute, net::description_attribute, net::app_name_attribute, net::app_creator_attribute, net::app_version_attribute>;

using all_attributes = brigand::list<
typetag_attribute,
net::unit_attribute,
net::extended_type_attribute,

net::value_attribute,
net::default_value_attribute,

net::domain_attribute,
net::access_mode_attribute,
net::bounding_mode_attribute,
net::repetition_filter_attribute,
net::tags_attribute,
net::refresh_rate_attribute,
net::priority_attribute,
net::value_step_size_attribute,
net::instance_bounds_attribute,
net::critical_attribute,
net::hidden_attribute,
net::disabled_attribute,
net::description_attribute,
net::app_name_attribute,
net::app_creator_attribute,
net::app_version_attribute
>;
}
enum class message_type
{
  Device,
  Namespace,
  Value,
  PathChanged,
  PathAdded,
  PathRemoved,
  AttributesChanged
};

using key_map_type = string_view_map<ossia::string_view>;

//! Mapping between ossia attributes and oscquery attributes
OSSIA_EXPORT const key_map_type& ossia_to_oscquery_key();

//! Mapping between oscquery attributes and ossia attributes.
OSSIA_EXPORT const key_map_type& oscquery_to_ossia_key();

//! Convert the text of an ossia attribute to the key of an oscquery attribute
OSSIA_EXPORT optional<ossia::string_view>
    ossia_to_oscquery_key(ossia::string_view);

//! Convert the text of an oscquery attribute to the key of an ossia attribute.
OSSIA_EXPORT optional<ossia::string_view>
    oscquery_to_ossia_key(ossia::string_view);
}
}
