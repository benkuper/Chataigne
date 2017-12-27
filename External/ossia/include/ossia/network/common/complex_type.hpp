#pragma once
#include <ossia/network/common/parameter_properties.hpp>
#include <eggs/variant.hpp>
#include <ossia/detail/string_view.hpp>
#include <ossia_export.h>
namespace ossia
{
struct unit_t;
using extended_type = std::string;

namespace net
{
class node_base;
class parameter_base;
}

/**
 * @typedef Used when an external information can be mapped to multiple levels
 * of typing.
 *
 * For instance OSC typetags can provide information such as one int, three
 * floats, RGB color...
 *
 * TODO maybe replace val_type by small_vector<val_type> ?
 */
using complex_type
    = eggs::variant<ossia::val_type, ossia::unit_t, ossia::extended_type>;

//! Get the implementation type of a high level type. If unknown will be
//! ossia::impulse.
OSSIA_EXPORT
ossia::val_type underlying_type(const complex_type& t);

//! Creates a parameter in a node according to a complex type.
OSSIA_EXPORT
ossia::net::parameter_base*
setup_parameter(const complex_type& t, ossia::net::node_base& node);
//! Creates a parameter in a node according to heuristics
OSSIA_EXPORT
ossia::net::parameter_base*
try_setup_parameter(std::string t, ossia::net::node_base& node);

//! Change the type of a parameter according to a complex type.
OSSIA_EXPORT
void update_parameter_type(
    const complex_type& t, ossia::net::parameter_base& node);
}
