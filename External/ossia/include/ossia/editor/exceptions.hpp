#pragma once
#include <ossia/network/exceptions.hpp>

/**
 * \file exceptions.hpp
 */
namespace ossia
{

/**
 * @brief The execution_error class
 *
 * Means that there was a problem
 * during the execution of a score
 */
struct OSSIA_EXPORT execution_error : public std::runtime_error
{
  execution_error(const char* e) : std::runtime_error(e)
  {
  }
};

/**
 * @brief The invalid_access_error struct
 *
 * Means that a destination_index could not
 * be applied to a value properly.
 */
struct OSSIA_EXPORT invalid_access_error : public std::runtime_error
{
  invalid_access_error(const char* e) : std::runtime_error(e)
  {
  }
};
}
