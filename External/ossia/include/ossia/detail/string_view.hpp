#pragma once
#include <ossia/detail/config.hpp>
#include "boost/version.hpp"

#if defined(__has_include)
#if __has_include(<string_view>) && defined(OSSIA_CXX14) && (!defined(_MSVC_LANG) || (defined(_MSVC_LANG) && _HAS_CXX17))
#define OSSIA_STRING_VIEW 1
#include <string_view>
namespace ossia
{
using string_view = std::string_view;
}
#elif __has_include(<experimental/string_view>)
#define OSSIA_STRING_VIEW 1
#include <experimental/string_view>
namespace ossia
{
using string_view = std::experimental::string_view;
}
#endif
#endif

#if !defined(OSSIA_STRING_VIEW)
#if BOOST_VERSION >= 106100
#define HAS_BOOST_STRING_VIEW
#include <boost/utility/string_view.hpp>
namespace ossia
{
using string_view = boost::string_view;
}
#else
#define HAS_BOOST_STRING_REF
#include <boost/utility/string_ref.hpp>
namespace ossia
{
using string_view = boost::string_ref;
}
#endif
#endif

#if (__cplusplus <= 201402L) || defined(__clang__) || defined(_MSC_VER) \
    || (defined(__GNUC__) && (__GNUC__ < 7))
inline std::string& operator+=(std::string& s, const ossia::string_view& v)
{
  s.append(v.data(), v.size());
  return s;
}
#endif

namespace ossia
{
// See also https://gist.github.com/klmr/2775736
template <std::size_t N>
constexpr ossia::string_view make_string_view(const char (&str)[N]) noexcept
{
  return {str, N - 1};
}
}
