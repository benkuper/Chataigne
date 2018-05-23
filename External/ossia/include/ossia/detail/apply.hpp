#pragma once
#include <eggs/variant.hpp>

/**
 * \file apply.hpp
 */

namespace ossia
{
/**
 * \brief ossia::apply : helper function to apply a visitor to a variant
 * without throwing in the empty variant case.
 *
 * By default, a eggs::variant throws bad_variant.
 * In this case, the operator()() without arguments will be called.
 * This allows a simpler handling of the default case.
 */
template <typename Visitor, typename Variant>
auto apply(Visitor&& v, Variant&& var) -> decltype(std::forward<Visitor>(v)())
{
  // Thanks K-Ballo (eggs-cpp/variant#21)
  if (var)
    return eggs::variants::apply(
        std::forward<Visitor>(v), std::forward<Variant>(var));
  else
    return std::forward<Visitor>(v)();
}
}
