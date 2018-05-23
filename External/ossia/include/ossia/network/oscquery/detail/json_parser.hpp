#pragma once
#include <ossia/network/oscquery/detail/attributes.hpp>
#include <ossia/network/oscquery/detail/json_reader_detail.hpp>
#include <memory>
namespace ossia
{
namespace oscquery
{

//! Functions to parse replies to OSCQuery json requests
struct OSSIA_EXPORT json_parser
{
  static std::shared_ptr<rapidjson::Document>
  parse(const std::string& message);

  static int get_port(const rapidjson::Value& obj);

  static ossia::oscquery::message_type
  message_type(const rapidjson::Value& obj);

  static void
  parse_namespace(ossia::net::node_base& root, const rapidjson::Value& obj);
  static void
  parse_value(ossia::net::parameter_base& addr, const rapidjson::Value& obj);
  static void parse_parameter_value(
      ossia::net::node_base& root, const rapidjson::Value& obj,
      ossia::net::device_base& dev);
  static void
  parse_path_added(ossia::net::node_base& map, const rapidjson::Value& obj);
  static void
  parse_path_removed(ossia::net::node_base& map, const rapidjson::Value& obj);
  static void
  parse_path_changed(ossia::net::node_base& map, const rapidjson::Value& mess);
  static void parse_attributes_changed(
      ossia::net::node_base& map, const rapidjson::Value& obj);
};
}
}
