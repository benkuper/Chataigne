#pragma once
#include <ossia/detail/algorithms.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <ossia_export.h>
#include <string>
#include <vector>
namespace ossia
{
namespace net
{
//! The definition is in node.cpp
OSSIA_EXPORT std::string& sanitize_device_name(std::string& name);
OSSIA_EXPORT std::string& sanitize_name(std::string& name);
OSSIA_EXPORT std::string sanitize_name(const std::string& s);
OSSIA_EXPORT std::string sanitize_name(std::string&& s);
OSSIA_EXPORT std::string sanitize_name(const char* s);
OSSIA_EXPORT std::string
sanitize_name(std::string name_base, const std::vector<std::string>& brethren);

/**
 * @brief Checks that a character is fit to be part of an address.
 */
template <typename Char_T>
bool is_valid_character_for_device(Char_T c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
         || (c >= '0' && c <= '9') || (c == '.') || (c == '~') || (c == '_')
         || (c == '(') || (c == ')') || (c == '-');
}
/**
 * @brief Checks that a character is fit to be part of an address.
 */
template <typename Char_T>
bool is_valid_character_for_name(Char_T c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
         || (c >= '0' && c <= '9') || (c == '.') || (c == '~') || (c == '_')
         || (c == '(') || (c == ')') || (c == '-') || (c == ':');
}

/**
 * @brief Characters valid in an OSSIA device part.
 */
inline ossia::string_view device_characters() noexcept
{
  return make_string_view("a-zA-Z0-9_~().-");
}

/**
 * @brief Characters valid in an OSSIA address part.
 */
inline ossia::string_view name_characters() noexcept
{
  return make_string_view("a-zA-Z0-9_~():.-");
}

/**
 * @brief Characters valid in an OSSIA address path.
 */
inline ossia::string_view path_characters() noexcept
{
  return make_string_view("/a-zA-Z0-9_~():.-");
}

inline ossia::string_view name_characters_no_instance() noexcept
{
  return make_string_view("a-zA-Z0-9_~():-");
}
/**
 * @brief Characters valid in an OSSIA pattern-matching part.
 *
 * Matches the OSC pattern-matching specification.
 *
 * @see \ref ossia::traversal
 */
inline ossia::string_view pattern_match_characters() noexcept
{
  return make_string_view("a-zA-Z0-9_~():.*?,{}\\[\\]-");
}

/**
 * @brief Checks if a given string has braces to expand
 *
 * e.g. foo[0-9]/{bar,b{aa,bb,cc}z}
 *
 * \see ossia::net::create_nodes
 */
OSSIA_EXPORT
bool is_brace_expansion(ossia::string_view);

/**
 * @brief address_parts Split an address
 *
 * Given foo:/bar/baz, returns {"foo:", "bar", "baz"}.
 * Given /bar/baz, returns {"bar, baz"}.
 * Given bar/baz, returns {"bar, baz"}.
 */
OSSIA_EXPORT std::vector<std::string>
address_parts(ossia::string_view src);
}
}
