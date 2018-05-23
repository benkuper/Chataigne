#pragma once
#include <ossia/detail/json.hpp>

#include <ossia/network/base/node_attributes.hpp>
#include <ossia/network/exceptions.hpp>

namespace ossia
{
namespace oscquery
{
inline void json_assert(bool val)
{
  if (!val)
    throw ossia::bad_request_error{"Bad request"};
}

namespace detail
{

struct json_parser_impl
{
  static bool ReadValue(const rapidjson::Value& val, int32_t& res);
  static bool ReadValue(const rapidjson::Value& val, float& res);
  static bool ReadValue(const rapidjson::Value& val, double& res);
  static bool ReadValue(const rapidjson::Value& val, std::string& res);
  static bool ReadValue(const rapidjson::Value& val, bool& res);
  static bool
  ReadValue(const rapidjson::Value& val, ossia::repetition_filter& res);
  static bool
  ReadValue(const rapidjson::Value& val, ossia::bounding_mode& res);
  static bool ReadValue(const rapidjson::Value& val, ossia::access_mode& am);
  static bool ReadValue(const rapidjson::Value& val, ossia::domain& res);
  static bool ReadValue(const rapidjson::Value& val, ossia::unit_t& res);
  static bool ReadValue(const rapidjson::Value& val, ossia::net::tags& res);
  static bool
  ReadValue(const rapidjson::Value& val, ossia::net::instance_bounds& res);

  static ossia::value ReadValue(const rapidjson::Value& val);

  static void
  readObject(ossia::net::node_base& node, const rapidjson::Value& obj);
};
}

using val_t = rapidjson::Value;
}
}
