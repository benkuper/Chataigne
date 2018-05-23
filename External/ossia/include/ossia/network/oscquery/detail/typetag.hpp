#pragma once
#include <ossia/network/dataspace/dataspace_visitors.hpp>
#include <ossia/network/value/value.hpp>
#include <ossia/network/base/parameter.hpp>
#include <ossia/network/base/parameter_data.hpp>
#include <ossia/network/base/node.hpp>
#include <ossia/network/base/node_attributes.hpp>
#include <ossia/network/common/extended_types.hpp>
#include <oscpack/osc/OscTypes.h>
namespace ossia
{
namespace oscquery
{
//! Returns the OSC Typetag associated to an ossia node, if there is a value
//! associated with it.
OSSIA_EXPORT optional<std::string>
get_osc_typetag(const ossia::net::node_base& n);

//! Returns an object that tries to describe in the best way possible a typetag
OSSIA_EXPORT complex_type get_type_from_osc_typetag(ossia::string_view str);

//! Sets up the node according to a given typetag
OSSIA_EXPORT void
set_osc_typetag(ossia::net::node_base& n, ossia::string_view tag);
}
}
