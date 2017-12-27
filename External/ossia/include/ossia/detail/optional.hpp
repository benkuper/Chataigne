#pragma once
#if defined(__native_client__) || defined(__ANDROID_API__) \
    || defined(__ANDROID__) || defined(ANDROID)
#define OSSIA_USE_BOOST_OPTIONAL 1
#elif defined(_MSC_VER)
#define OSSIA_USE_BOOST_OPTIONAL 1

#else

#ifdef __has_include
#if __has_include(<optional>) && __cplusplus > 201402L
#define OSSIA_USE_STD_OPTIONAL 1
#elif __has_include(<experimental/optional>)
#define OSSIA_USE_STD_EXPERIMENTAL_OPTIONAL 1
#else
#define OSSIA_USE_BOOST_OPTIONAL 1
#endif
#else
#define OSSIA_USE_BOOST_OPTIONAL 1
#endif
#endif

#if defined(OSSIA_USE_BOOST_OPTIONAL)
#include <boost/optional.hpp>
namespace ossia
{
template <typename... Args>
using optional = boost::optional<Args...>;
const auto none = boost::none;

template <typename Opt, typename Arg>
auto get_value_or(const Opt& opt, Arg&& arg)
{
  return opt.get_value_or(std::forward<Arg>(arg));
}
}
#elif defined(OSSIA_USE_STD_OPTIONAL)
#include <optional>
namespace ossia
{
template <typename... Args>
using optional = std::optional<Args...>;
const constexpr auto none = std::nullopt;

template <typename Opt, typename Arg>
auto get_value_or(const Opt& opt, Arg&& arg)
{
  return opt.value_or(std::forward<Arg>(arg));
}
}
#elif defined(OSSIA_USE_STD_EXPERIMENTAL_OPTIONAL)
#include <experimental/optional>
namespace ossia
{
template <typename... Args>
using optional = std::experimental::optional<Args...>;
const constexpr auto none = std::experimental::nullopt;

template <typename Opt, typename Arg>
auto get_value_or(const Opt& opt, Arg&& arg)
{
  return opt.value_or(std::forward<Arg>(arg));
}
}
#endif

namespace ossia
{
using none_t = decltype(ossia::none);
}
