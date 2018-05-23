#pragma once
#include <ossia/detail/json.hpp>
#include <ossia/network/base/node_attributes.hpp>

namespace ossia
{
namespace oscquery
{
namespace detail
{
//! Implementation of the JSON serialisation mechanism for oscquery
struct json_writer_impl
{
  using writer_t = rapidjson::Writer<rapidjson::StringBuffer>;
  writer_t& writer;

  void writeKey(ossia::string_view k) const;

  void writeValue(const ossia::value& val) const;
  void writeValue(ossia::bounding_mode b) const;
  void writeValue(ossia::access_mode b) const;
  void writeValue(const ossia::domain& d) const;
  void writeValue(const ossia::unit_t& d) const;
  void writeValue(const ossia::net::tags& tags) const;
  void writeValue(int32_t i) const;
  void writeValue(float i) const;
  void writeValue(double i) const;
  void writeValue(bool i) const;
  void writeValue(const std::string& i) const;
  void writeValue(const ossia::repetition_filter& i) const;
  void writeValue(const ossia::net::instance_bounds& i) const;

  template <typename T>
  void writeValue(const optional<T>& t) const
  {
    if (t)
    {
      writeValue(*t);
    }
    else
    {
      writer.Null();
    }
  }

  //! Writes a single attribute
  void writeAttribute(
      const ossia::net::node_base& n, ossia::string_view method) const;

  //! Writes only the attributes
  void writeNodeAttributes(const ossia::net::node_base& n) const;

  //! Writes a node recursively. Creates a new object.
  void writeNode(const ossia::net::node_base& n);
};
}
}
}
