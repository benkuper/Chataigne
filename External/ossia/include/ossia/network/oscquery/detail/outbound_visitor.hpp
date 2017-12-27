#pragma once
#include <ossia/network/value/value.hpp>
namespace oscpack
{
class OutboundPacketStream;
}
namespace ossia
{
namespace oscquery
{
struct osc_outbound_visitor
{
public:
  oscpack::OutboundPacketStream& p;
  void operator()(ossia::impulse) const;
  void operator()(int32_t i) const;
  void operator()(float f) const;
  void operator()(bool b) const;
  void operator()(char c) const;
  void operator()(const std::string& str) const;
  void operator()(ossia::vec2f vec) const;
  void operator()(ossia::vec3f vec) const;
  void operator()(ossia::vec4f vec) const;
  void operator()(const std::vector<ossia::value>& t) const;
  void operator()() const;
};
}
}
