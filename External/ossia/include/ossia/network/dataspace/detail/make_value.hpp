#pragma once
#include <ossia/network/dataspace/dataspace.hpp>
#include <ossia/network/value/value_conversion.hpp>

namespace ossia
{

template <typename U, typename V>
struct make_value_helper;

template <typename U>
struct make_value_helper<U, float>
{

  template <typename T>
  OSSIA_INLINE ossia::value_with_unit operator()(const T& t)
  {
    // TODO maybe return first value for list or vecNf ?
    // throw std::runtime_error("mismatching value type and unit");
    return {};
  }

  OSSIA_INLINE ossia::value_with_unit operator()(int32_t t)
  {
    return strong_value<U>(float(t));
  }
  OSSIA_INLINE ossia::value_with_unit operator()(float t)
  {
    return strong_value<U>(float(t));
  }
  OSSIA_INLINE ossia::value_with_unit operator()(char t)
  {
    return strong_value<U>(float(t));
  }
  OSSIA_INLINE ossia::value_with_unit operator()(bool t)
  {
    return strong_value<U>(float(t));
  }
};

template <typename U>
struct make_value_helper<U, ossia::vec2f>
{

  template <typename T>
  OSSIA_INLINE ossia::value_with_unit operator()(const T& t)
  {
    // throw std::runtime_error("mismatching value type and unit");
    return {};
  }

  OSSIA_INLINE ossia::value_with_unit
  operator()(const std::vector<ossia::value>& t)
  {
    return strong_value<U>{ossia::convert<std::array<float, 2>>(t)};
  }
  
  OSSIA_INLINE ossia::value_with_unit operator()(ossia::vec2f t)
  {
    return strong_value<U>{t};
  }
  OSSIA_INLINE ossia::value_with_unit operator()(ossia::vec3f t)
  {
    return strong_value<U>{ossia::make_vec(t[0], t[1])};
  }
  OSSIA_INLINE ossia::value_with_unit operator()(ossia::vec4f t)
  {
    return strong_value<U>{ossia::make_vec(t[0], t[1])};
  }
};

template <typename U>
struct make_value_helper<U, ossia::vec3f>
{

  template <typename T>
  OSSIA_INLINE ossia::value_with_unit operator()(const T& t)
  {
    // throw std::runtime_error("mismatching value type and unit");
    return {};
  }

  OSSIA_INLINE ossia::value_with_unit
  operator()(const std::vector<ossia::value>& t)
  {
    return strong_value<U>{ossia::convert<std::array<float, 3>>(t)};
  }
  
  OSSIA_INLINE ossia::value_with_unit operator()(ossia::vec2f t)
  {
    return strong_value<U>{ossia::make_vec(t[0], t[1], 0.)};
  }
  OSSIA_INLINE ossia::value_with_unit operator()(ossia::vec3f t)
  {
    return strong_value<U>{t};
  }
  OSSIA_INLINE ossia::value_with_unit operator()(ossia::vec4f t)
  {
    return strong_value<U>{ossia::make_vec(t[0], t[1], t[2])};
  }
};

template <typename U>
struct make_value_helper<U, ossia::vec4f>
{

  template <typename T>
  OSSIA_INLINE ossia::value_with_unit operator()(const T& t)
  {
    // throw std::runtime_error("mismatching value type and unit");
    return {};
  }

  OSSIA_INLINE ossia::value_with_unit
  operator()(const std::vector<ossia::value>& t)
  {
    return strong_value<U>{ossia::convert<std::array<float, 4>>(t)};
  }
  
  OSSIA_INLINE ossia::value_with_unit operator()(ossia::vec2f t)
  {
    return strong_value<U>{ossia::make_vec(t[0], t[1], 0., 0.)};
  }
  OSSIA_INLINE ossia::value_with_unit operator()(ossia::vec3f t)
  {
    return strong_value<U>{ossia::make_vec(t[0], t[1], t[2], 0.)};
  }
  OSSIA_INLINE ossia::value_with_unit operator()(ossia::vec4f t)
  {
    return strong_value<U>{t};
  }
};

struct make_value_with_unit_visitor
{
  template <typename Val, typename Unit>
  OSSIA_INLINE ossia::value_with_unit operator()(const Val& v, const Unit& u)
  {
    return make_value_helper<Unit, typename Unit::value_type>{}(v);
  }
};
}
