#pragma once
#include <memory>
#include <ossia_export.h>

namespace spdlog
{
// Use it by doing : #include <ossia/detail/logger.hpp>
class logger;
}

namespace ossia
{
namespace net
{
//! Stores custom loggers for the inbound and outbound network messages
class OSSIA_EXPORT network_logger
{
public:
  network_logger();
  ~network_logger();

  network_logger(
      std::shared_ptr<spdlog::logger> i, std::shared_ptr<spdlog::logger> o)
      : inbound_logger{std::move(i)}, outbound_logger{std::move(o)}
  {
  }

  /**
   * @brief inbound_logger Set log function for messages coming from
   * outside.
   */
  std::shared_ptr<spdlog::logger> inbound_logger;

  /**
   * @brief outbound_logger Set log function for messages going outside.
   */
  std::shared_ptr<spdlog::logger> outbound_logger;
};
}
}
