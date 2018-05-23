#pragma once
#include <ossia/network/common/parameter_properties.hpp>
#include <fmt/format.h>
#include <ossia_export.h>

/**
 * \file ossia/network/common/debug.hpp
 *
 * Various utilities to debug ossia types. Uses fmtlib.
 *
 * Example of usage :
 *
 * \code
    ossia::net::node_base& child;
    fmt::MemoryWriter w;
    w << child;
    std::cerr << w.str() << std::endl;
   \endcode
 */
namespace ossia
{
struct domain;
namespace net
{
class node_base;

OSSIA_EXPORT
fmt::BasicWriter<char>&
operator<<(fmt::BasicWriter<char>&, ossia::access_mode);
OSSIA_EXPORT
fmt::BasicWriter<char>&
operator<<(fmt::BasicWriter<char>&, const ossia::domain&);
OSSIA_EXPORT
fmt::BasicWriter<char>&
operator<<(fmt::BasicWriter<char>&, ossia::bounding_mode);
OSSIA_EXPORT
fmt::BasicWriter<char>&
operator<<(fmt::BasicWriter<char>&, const ossia::net::node_base&);
OSSIA_EXPORT
void debug_recursively(fmt::MemoryWriter&, const ossia::net::node_base&);
}
}
