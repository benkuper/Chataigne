#pragma once
#include <functional>
#include <ossia_export.h>

/**
 * \file custom_state.hpp
 */
namespace ossia
{
/**
 * @brief The custom_state class
 *
 * A custom state allows to register a custom action
 * that will be executed when a state is launched.
 *
 * \see \ref state_element
 */
class OSSIA_EXPORT custom_state
{
public:
  std::function<void()> func;
  void launch() const
  {
    if (func)
      func();
  }

  friend bool operator==(const custom_state& lhs, const custom_state& rhs)
  {
    return false;
  }
  friend bool operator!=(const custom_state& lhs, const custom_state& rhs)
  {
    return true;
  }
};
}
