#pragma once
#include <ossia/detail/config.hpp>
#include <boost/version.hpp>
#include <boost/utility/string_view.hpp>
namespace ossia
{
using string_view = boost::string_view;
inline std::string& operator+=(std::string& str, string_view s)
{
  str.append(s.data(), s.size());
  return str;
}

// See also https://gist.github.com/klmr/2775736
template <std::size_t N>
constexpr ossia::string_view make_string_view(const char (&str)[N]) noexcept
{
  return {str, N - 1};
}
}
