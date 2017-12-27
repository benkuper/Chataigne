#pragma once
#include <ossia/network/oscquery/detail/json_parser.hpp>
#include <ossia/network/oscquery/detail/json_writer.hpp>

namespace ossia
{
namespace oscquery
{

/**
 * @brief OSCQuery JSON query-answering logic.
 *
 * Used by the server to answer client requests.
 */
struct json_query_answerer
{
  template <typename Protocol>
  auto operator()(
      Protocol& proto, const typename Protocol::connection_handler&,
      const rapidjson::Document& doc)
  {
    auto& dev = proto.get_device();
    auto& root = proto.get_device().get_root_node();

    // [ { "/addr/val" : 123 } ] or { "/addr/val": 123 }
    if (doc.IsArray())
    {
      const auto& arr = doc.GetArray();
      for (const auto& e : arr)
      {
        json_parser::parse_parameter_value(root, e, dev);
      }
    }
    else
    {
      json_parser::parse_parameter_value(root, doc, dev);
    }
    return rapidjson::StringBuffer{};
  }
};
}
}
