#pragma once
#include <ossia_export.h>
#include <stdexcept>

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
