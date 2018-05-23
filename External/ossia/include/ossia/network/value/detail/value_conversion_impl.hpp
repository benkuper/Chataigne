#pragma once
#include <ossia/network/value/value.hpp>
#include <ossia/network/value/value_conversion.hpp>
#include <ossia/network/value/value_traits.hpp>
#include <boost/type_traits/function_traits.hpp>
#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE
#include <boost/lexical_cast.hpp>
namespace ossia
{

namespace detail
{
template <typename T>
struct array_size;
template <typename T, std::size_t N>
struct array_size<std::array<T, N>>
    : public std::integral_constant<std::size_t, N>
{
};

template <typename Target, typename = void>
struct value_converter
{
};

template <>
struct value_converter<ossia::impulse>
{
  template <typename U>
  ossia::impulse operator()(const U&)
  {
    return {};
  }

  ossia::impulse operator()()
  {
    return {};
  }
};

template <typename T>
struct numeric_value_converter
{
  T operator()(impulse) const
  {
    return T{};
  }
  T operator()(int32_t v)
  {
    return v;
  }
  T operator()(float v)
  {
    return v;
  }
  T operator()(bool v)
  {
    return v;
  }
  T operator()(char v)
  {
    return v;
  }
  T operator()() const
  {
    return T{};
  }

  T operator()(const std::string& v) const
  {
    try
    {
      return boost::lexical_cast<T>(v);
    }
    catch (...)
    {
      return T{};
    }
  }
  T operator()(const vec2f& v) const
  {
    return v[0];
  }
  T operator()(const vec3f& v) const
  {
    return v[0];
  }
  T operator()(const vec4f& v) const
  {
    return v[0];
  }

  T operator()(const std::vector<ossia::value>& v) const
  {
    return !v.empty() ? convert<T>(v[0]) : T{};
  }
};

template <>
struct value_converter<int> : public numeric_value_converter<int>
{
};
template <>
struct value_converter<float> : public numeric_value_converter<float>
{
};
template <>
struct value_converter<double> : public numeric_value_converter<double>
{
};
template <>
struct value_converter<bool> : public numeric_value_converter<bool>
{
};
template <>
struct value_converter<char> : public numeric_value_converter<char>
{
};

template <>
struct value_converter<std::string>
{
  using T = std::string;
  T operator()(impulse) const
  {
    return T{};
  }
  T operator()(int32_t v)
  {
    return boost::lexical_cast<std::string>(v);
  }
  T operator()(float v)
  {
    return boost::lexical_cast<std::string>(v);
  }
  T operator()(bool v)
  {
    return boost::lexical_cast<std::string>(v);
  }
  T operator()(char v)
  {
    return boost::lexical_cast<std::string>(v);
  }
  T operator()(const std::string& v) const
  {
    return v;
  }

  T operator()() const
  {
    return T{};
  }

  template <std::size_t N>
  T operator()(std::array<float, N> v) const
  {
    std::string s = "[";
    s += boost::lexical_cast<std::string>(v[0]);
    for (std::size_t i = 1; i < N; i++)
      s += ", " + boost::lexical_cast<std::string>(v[i]);
    s += "]";
    return s;
  }

  T operator()(const std::vector<ossia::value>& v) const
  {
    using namespace std::literals;
    std::string s = "["s;
    const auto n = v.size();
    if (n > 0)
    {
      s += convert<std::string>(v[0]);

      for (std::size_t i = 1; i < n; i++)
        s += ", "s + convert<std::string>(v[i]);
    }
    s += "]"s;
    return s;
  }
};

template <>
struct value_converter<std::vector<ossia::value>>
{
  template <typename U>
  std::vector<ossia::value> operator()(const U& u)
  {
    return {u};
  }

  template <std::size_t N>
  std::vector<ossia::value> operator()(const std::array<float, N>& u)
  {
    std::vector<ossia::value> v;
    for (std::size_t i = 0; i < N; i++)
    {
      v.push_back(float{u[i]});
    }
    return v;
  }

  std::vector<ossia::value> operator()(const std::vector<ossia::value>& t)
  {
    return t;
  }
  std::vector<ossia::value> operator()(std::vector<ossia::value>&& t)
  {
    return std::move(t);
  }

  std::vector<ossia::value> operator()()
  {
    return {};
  }
};

template <std::size_t N>
struct value_converter<std::array<float, N>>
{
  template <typename U>
  std::array<float, N> operator()(const U&)
  {
    return {};
  }

  std::array<float, N> operator()(std::array<float, N> v)
  {
    return v;
  }

  std::array<float, N> operator()(float f)
  {
    std::array<float, N> a;
    a.fill(f);
    return a;
  }

  std::array<float, N> operator()(int32_t f)
  {
    std::array<float, N> a;
    a.fill(f);
    return a;
  }

  std::array<float, N> operator()(char f)
  {
    std::array<float, N> a;
    a.fill(f);
    return a;
  }

  std::array<float, N> operator()(bool f)
  {
    std::array<float, N> a;
    a.fill(f ? 1. : 0.);
    return a;
  }

  std::array<float, N> operator()(const std::vector<ossia::value>& t)
  {
    return convert<std::array<float, N>>(t);
  }

  std::array<float, N> operator()()
  {
    return {};
  }
};
}

template <typename T>
T convert(const ossia::value& val)
{
  return val.apply(detail::value_converter<T>{});
}

// Used to convert List in Vec2f, Vec3f, Vec4f...
template <typename T>
T convert(const std::vector<ossia::value>& val)
{
  // TODO should we have an error if the List does not
  // have the correct number of arguments ? Or just silently fill
  // with zeros ?

  T res{};
  const auto N = std::min(val.size(), detail::array_size<T>::value);
  for (std::size_t i = 0; i < N; i++)
  {
    res[i] = convert<float>(val[i]);
  }
  return res;
}

// MOVEME
template <typename Fun, typename... Args>
auto lift(ossia::val_type type, Fun f, Args&&... args)
{
  switch (type)
  {
    case val_type::IMPULSE:
      return f(ossia::value_trait<impulse>{}, std::forward<Args>(args)...);
    case val_type::BOOL:
      return f(ossia::value_trait<bool>{}, std::forward<Args>(args)...);
    case val_type::INT:
      return f(ossia::value_trait<int32_t>{}, std::forward<Args>(args)...);
    case val_type::FLOAT:
      return f(ossia::value_trait<float>{}, std::forward<Args>(args)...);
    case val_type::CHAR:
      return f(ossia::value_trait<char>{}, std::forward<Args>(args)...);
    case val_type::STRING:
      return f(ossia::value_trait<std::string>{}, std::forward<Args>(args)...);
    case val_type::LIST:
      return f(
          ossia::value_trait<std::vector<ossia::value>>{},
          std::forward<Args>(args)...);
    case val_type::VEC2F:
      return f(ossia::value_trait<vec2f>{}, std::forward<Args>(args)...);
    case val_type::VEC3F:
      return f(ossia::value_trait<vec3f>{}, std::forward<Args>(args)...);
    case val_type::VEC4F:
      return f(ossia::value_trait<vec4f>{}, std::forward<Args>(args)...);
    case val_type::NONE:
      break;
  }

  throw invalid_value_type_error("lift: Invalid type");
  return decltype(
      f(ossia::value_trait<impulse>{}, std::forward<Args>(args)...)){};
}
}
