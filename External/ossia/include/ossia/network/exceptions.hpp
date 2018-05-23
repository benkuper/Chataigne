#pragma once
#include <ossia_export.h>
#include <stdexcept>
#include <string>

namespace ossia
{

/**
 * @brief The parse_error class
 *
 * Means that there was a problem
 * while parsing a network message.
 */
struct OSSIA_EXPORT parse_error : public std::runtime_error
{
  parse_error(std::string e) : std::runtime_error(std::move(e))
  {
  }
};

/**
 * @brief The connection_error class
 *
 * Means that a connection could not be established.
 */
struct OSSIA_EXPORT connection_error : public std::runtime_error
{
  connection_error(std::string e) : std::runtime_error(std::move(e))
  {
  }
};

/**
 * @brief The invalid_node_error class
 *
 * Means that there an incorrect node
 * was passed.
 * For instance a node without a parameter
 * when a parameter was required.
 */
struct OSSIA_EXPORT invalid_node_error : public std::logic_error
{
  invalid_node_error(std::string e) : std::logic_error(std::move(e))
  {
  }
};

/**
 * @brief Used when a requested node could not be found.
 */
struct OSSIA_EXPORT node_not_found_error : public std::runtime_error
{
  node_not_found_error(std::string e) : std::runtime_error(std::move(e))
  {
  }
};

/**
 * @brief Used when a bad network request is done on a local server.
 */
struct OSSIA_EXPORT bad_request_error : public std::runtime_error
{
  bad_request_error(std::string e) : std::runtime_error(std::move(e))
  {
  }
};

/**
 * @brief The invalid_value_type_error class
 *
 * Means that an invalid value was passed.
 * For instance a List where an Impulse is required.
 */
struct OSSIA_EXPORT invalid_value_type_error : public std::logic_error
{
  invalid_value_type_error(const char* e) : std::logic_error(e)
  {
  }
};
}
