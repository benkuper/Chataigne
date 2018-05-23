#pragma once
#include <ossia/network/oscquery/detail/outbound_visitor.hpp>

namespace ossia
{
namespace oscquery
{
inline void osc_outbound_visitor::operator()(impulse) const
{
}

inline void osc_outbound_visitor::operator()(int32_t i) const
{
  p << int32_t(i);
}

inline void osc_outbound_visitor::operator()(float f) const
{
  p << float(f);
}

inline void osc_outbound_visitor::operator()(bool b) const
{
  p << bool(b);
}

inline void osc_outbound_visitor::operator()(char c) const
{
  p << char(c);
}

inline void osc_outbound_visitor::operator()(const std::string& str) const
{
  p << (ossia::string_view)str;
}

inline void osc_outbound_visitor::operator()(vec2f vec) const
{
  p << vec[0] << vec[1];
}

inline void osc_outbound_visitor::operator()(vec3f vec) const
{
  p << vec[0] << vec[1] << vec[2];
}

inline void osc_outbound_visitor::operator()(vec4f vec) const
{
  p << vec[0] << vec[1] << vec[2] << vec[3];
}

inline void osc_outbound_visitor::operator()(const std::vector<value>& t) const
{
  for (const auto& val : t)
  {
    val.apply(*this);
  }
}

inline void osc_outbound_visitor::operator()() const
{
}

}
}
