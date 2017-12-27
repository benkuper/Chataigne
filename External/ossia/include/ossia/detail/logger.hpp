#pragma once
// Important defines :
#include <ossia/detail/config.hpp>

// They have to be in this order
#define SPDLOG_FINAL final
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/logger.h>

#include <ossia_export.h>

/**
 * \file logger.hpp
 */

namespace ossia
{
/**
 * @brief Where the errors will be logged. Default is stderr.
 *
 * It is also accessible through spdlog::get("ossia");
 *
 * The logger is first created in ossia::context construction.
 */
OSSIA_EXPORT spdlog::logger& logger();
}
