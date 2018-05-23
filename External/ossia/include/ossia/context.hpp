#pragma once
#include <memory>
#include <ossia_export.h>
#include <vector>

/**
 * \file context.hpp
 */
namespace spdlog
{
namespace sinks
{
class sink;
}
using sink_ptr = std::shared_ptr<sinks::sink>;
}

namespace ossia
{
/**
 * @brief If using the library, you should create this class at some point.
 *
 * It is not mandatory but some features won't work.
 */
struct OSSIA_EXPORT context
{
  //! Most common case
  context();

  //! Use this constructor to provide custom logging sinks.
  context(const std::vector<spdlog::sink_ptr>& sinks);

  ~context();
};
}
