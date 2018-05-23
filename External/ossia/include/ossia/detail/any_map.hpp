#pragma once
#include <ossia/detail/optional.hpp>
#include <ossia/detail/string_map.hpp>
#include <ossia/detail/any.hpp>
#include <ossia_export.h>

/**
 * \file any_map.hpp
 */

namespace ossia
{
//! A container to store any kind of data indexed by a string
using any_map = string_map<ossia::any>;

using extended_attributes = any_map;
/**
 * @brief get_attribute Get an attribute of an any_map.
 *
 * Example :
 * \code
 * auto desc = get_attribute<description>(e, "description");
 * \endcode
 *
 * The value will be default-constructed if it is not found (e.g. empty string,
 * 0...)
 */
template <typename T>
auto get_attribute(const any_map& e, ossia::string_view name)
{
  auto it = e.find(name);
  if (it != e.cend())
  {
    auto val = ossia::any_cast<T>(&it.value());
    if (val)
      return *val;
  }

  return T{};
}

/**
 * @brief get_optional_attribute Maybe get an attribute of an any_map.
 *
 * Example :
 * \code
 * auto desc = get_optional_attribute<description>(e, "description");
 * if(desc)
 *   std::cerr << "The description is: " << *desc << "\n";
 * \endcode
 *
 * The value will be an empty \ref optional if it is not found.
 */
template <typename T>
optional<T> get_optional_attribute(const any_map& e, ossia::string_view name)
{
  auto it = e.find(name);
  if (it != e.cend())
  {
    auto val = ossia::any_cast<T>(&it.value());
    if (val)
      return *val;
  }

  return ossia::none;
}

struct is_empty_value
{
  template <typename T>
  bool operator()(const T&)
  {
    return false;
  }
  bool operator()(const std::string& v)
  {
    return v.empty();
  }
  bool operator()(const ossia::string_view& v)
  {
    return v.empty();
  }
  template <typename T>
  bool operator()(const std::vector<T>& v)
  {
    return v.empty();
  }
};

//! Remove an attribute
inline void unset_attribute(any_map& e, ossia::string_view str)
{
  e.erase(str);
}

//! Sets an attribute in an any_map
template <typename T>
void set_attribute(any_map& e, ossia::string_view str, const T& val)
{
  if (!is_empty_value{}(val))
  {
    // TODO insert_or_assign
    auto it = e.find(str);
    if (it != e.end())
      it.value() = val;
    else
      e.insert(std::make_pair(std::string(str), ossia::any{val}));
  }
  else
  {
    unset_attribute(e, str);
  }
}

//! Checks if an attribute is present.
inline bool has_attribute(const any_map& e, ossia::string_view str)
{
  return e.find(str) != e.end();
}

//! Sets a bool-like attribute. It should be checked for with has_attribute.
inline void set_attribute(any_map& e, ossia::string_view str)
{
  auto it = e.find(str);
  if (it == e.end())
    e.insert(std::make_pair(std::string(str), ossia::any{}));
}

//! Sets an attribute in an any_map
template <typename T>
void set_attribute(any_map& e, ossia::string_view str, T&& val)
{
  if (!is_empty_value{}(val))
  {
    // TODO insert_or_assign
    auto it = e.find(str);
    if (it != e.end())
      it.value() = std::move(val);
    else
      e.insert(std::make_pair(std::string(str), ossia::any{std::move(val)}));
  }
  else
  {
    unset_attribute(e, str);
  }
}

//! Removes an attribute in an any_map
inline OSSIA_EXPORT void
set_attribute(any_map& e, ossia::string_view str, ossia::none_t)
{
  unset_attribute(e, str);
}

//! Sets an attribute if opt has a value, else remove the attribute
template <typename T>
void set_optional_attribute(
    any_map& e, ossia::string_view str, const optional<T>& opt)
{
  if (opt && !is_empty_value{}(*opt))
    set_attribute(e, str, *opt);
  else
    set_attribute(e, str, ossia::none);
}

template <typename T>
void set_optional_attribute(
    any_map& e, ossia::string_view str, optional<T>&& opt)
{
  if (opt && !is_empty_value{}(*opt))
    set_attribute(e, str, *std::move(opt));
  else
    set_attribute(e, str, ossia::none);
}
}
