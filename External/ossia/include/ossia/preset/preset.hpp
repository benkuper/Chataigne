#pragma once

#include <ossia/detail/string_view.hpp>
#include <functional>
#include <map>
#include <memory>
#include <ossia/preset/exception.hpp>
#include <ossia_export.h>
#include <regex>
#include <string>
#include <fstream>

namespace ossia
{
namespace net
{
class device_base;
class node_base;
class parameter_base;
}
class value;
}

namespace ossia
{
namespace presets
{
using preset = std::vector<std::pair<std::string, ossia::value>>;

using instance_functions = std::
    vector<std::
               pair<std::regex, std::function<void(const ossia::net::node_base&)>>>;

using preset_pair = std::pair<std::string, ossia::value>;

OSSIA_EXPORT preset read_json(const std::string&, bool skip_first_level = true);
OSSIA_EXPORT std::string
write_json(const std::string& devicename, const preset&);

OSSIA_EXPORT std::string to_string(const preset&);
OSSIA_EXPORT preset from_string(const ossia::string_view& str);

enum keep_arch_type
{
  keep_arch_on,
  keep_arch_off
};

/*!
 * \brief export device tree to a json tree
 * \param device device to export to json
 * \return a string containing json tree
 */
OSSIA_EXPORT std::string write_json(const ossia::net::device_base& deviceBase);

/**
 * @brief Write a string to a file
 * @param string The string to write
 * @param filename The file name
 */
OSSIA_EXPORT void
write_file(ossia::string_view string, ossia::string_view filename);

OSSIA_EXPORT const std::string
read_file(const std::string& filename);

/**
 * @brief make_json_preset create a JSON preset
 * @details The return JSON contains all node's child parameters
 * with associated values, sorted by priority
 * @param node
 * @return JSON string
 */
OSSIA_EXPORT const std::string
make_json_preset(const ossia::net::node_base& node);

typedef void (*func_t)(ossia::net::node_base* n);

/**
 * @brief apply_json : apply a json preset to a node
 * @param json : preset string
 * @param node : root node to apply preset on
 * @param fn : function to call just after push a value
 * @return true if there is no parse error
 */
OSSIA_EXPORT bool
apply_json(const std::string& json,
           ossia::net::node_base& node,
           func_t fn = nullptr);

/**
 * @brief apply_preset : apply simple preset to a node
 * @details KISS = keep it simple and stupid,
 * one parameter address followed by its value on each line
 * @param kiss : preset string
 * @param node : root node
 * @param fn : function to call just after push a value
 */
OSSIA_EXPORT void apply_preset(
    const std::string& kiss,
    ossia::net::node_base& node,
    func_t fn = nullptr);

// DEPRECATED keep it for backward compatibility
OSSIA_EXPORT void apply_preset(
    ossia::net::node_base&, const presets::preset&,
    keep_arch_type t = keep_arch_on,
    presets::instance_functions = {},
    bool allow_nonterminal = false);

OSSIA_EXPORT presets::preset
make_preset(ossia::net::node_base&);

OSSIA_EXPORT ossia::net::node_base*
get_node(ossia::net::node_base&, const std::string&);

OSSIA_EXPORT std::string
to_string(const ossia::net::device_base& ossiadev);

} // namespace presets
} // namespace ossia
