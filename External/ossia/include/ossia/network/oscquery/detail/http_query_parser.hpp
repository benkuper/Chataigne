#pragma once
#include <ossia/detail/string_map.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include<map>
/**
 * \file http_query_parser.hpp
 *
 * Utilities to parse http GET-like requests's query part, e.g.
 * everything after '?' in the following string:
 *
 * `/foo/bar?mrskeltal=dootdoot&hint=1234`
 *
 */
namespace ossia
{
namespace oscquery
{

// See https://stackoverflow.com/questions/45948473/boost-spirit-porting-string-pairs-from-qi-to-x3/
template <typename T = std::string>
auto& query() {
  using namespace boost::spirit::x3;
  static const auto s_pair
      = rule<struct pair_, std::pair<std::string, T> > {"pair"}
      = +~char_("&=") >> -('=' >> *~char_("&"));
  static const auto s_query
      = rule<struct query_, string_map<T>  > {"query"}
      = s_pair % '&';

  return s_query;
}

inline string_map<std::string>
parse_http_methods_encoded(ossia::string_view str)
{
  // TODO a vector would be more efficient.
  string_map<std::string> methods;
  boost::spirit::x3::parse(str.cbegin(), str.cend(), query(), methods);
  return methods;
}

//! url_decode taken from boost
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
// Distributed under the Boost Software License, Version 1.0.
inline bool url_decode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  const int N = in.size();
  for (int i = 0; i < N; ++i)
  {
    switch (in[i])
    {
      case '%':
      {
        if (i + 3 <= N)
        {
          int value = 0;
          std::istringstream is(in.substr(i + 1, 2));
          if (is >> std::hex >> value)
          {
            out += static_cast<char>(value);
            i += 2;
          }
          else
          {
            return false;
          }
        }
        else
        {
          return false;
        }
        break;
      }
      case '+':
        out += ' ';
        break;
      case '\0':
        return true;
      default:
        out += in[i];
        break;
    }
  }
  return true;
}
}
}
