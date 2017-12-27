#pragma once
#include <ossia/editor/expression/expression_atom.hpp>
#include <ossia/editor/expression/expression_bool.hpp>
#include <ossia/editor/expression/expression_composition.hpp>
#include <ossia/editor/expression/expression_fwd.hpp>
#include <ossia/editor/expression/expression_generic.hpp>
#include <ossia/editor/expression/expression_not.hpp>
#include <ossia/editor/expression/expression_pulse.hpp>

/**
 * @file expression.hpp
 *
 * This file defines the expression API.
 * An expression is a logical construct such as :
 *
 * (a:/b == 1) && (c:/d <= 2.)
 *
 * pulse(a:/b)
 *
 * !(c:/d == e:/f/g)
 *
 * etc...
 *
 * The basic logical operators are expression_not (!) and
 * expression_composition (and, or, xor).
 *
 * expression_atom is used for numeric comparison : <=, ==, >, etc.
 *
 * expression_bool is a simple boolean value.
 *
 * expression_pulse is an expression that will become true
 * if a particular message happened since the last time it has been updated.
 *
 * The expressions are organised as a tree.
 * The expression results are passed by cascading callbacks
 * to allow notification when an expression is listened to.
 *
 * \see
 * \ref expression_bool
 * \ref expression_atom
 * \ref expression_pulse
 * \ref expression_composition
 * \ref expression_not
 * \ref expression_generic
 */

namespace ossia
{
namespace expressions
{
/**
 * @brief evaluate
 * @param e An expression
 * @return The truth value of the expression
 */
OSSIA_EXPORT bool evaluate(const expression_base& e);
OSSIA_EXPORT inline bool evaluate(const expression_ptr& e)
{
  return evaluate(*e);
}

/**
 * @brief update
 * @param e An expression
 *
 * Recursively asks an expression to query the addresses that it
 * refers, in order to check if the value has changed.
 */
OSSIA_EXPORT void update(const expression_base& e);
OSSIA_EXPORT inline void update(const expression_ptr& e)
{
  update(*e);
}

/**
 * @brief operator==(const expression_base& lhs, const expression_base& rhs)
 * Compares two expressions.
 *
 * Two expressions are comparable if they have all the same terms.
 */
OSSIA_EXPORT bool
operator==(const expression_base& lhs, const expression_base& rhs);
OSSIA_EXPORT bool
operator!=(const expression_base& lhs, const expression_base& rhs);

/**
 * @brief add_callback Add a callback to an expression.
 */
OSSIA_EXPORT expression_callback_iterator
add_callback(expression_base&, expression_result_callback);

/**
 * @brief remove_callback Remove a callback to an expression.
 */
OSSIA_EXPORT void
remove_callback(expression_base&, expression_callback_iterator);

/**
 * @brief callback_count
 * @return Number of callbacks in an expression.
 */
OSSIA_EXPORT std::size_t callback_count(expression_base&);

/**
  \brief expression_true Convenience constant expression always evaluating to
  true.
 */
OSSIA_EXPORT const expression_base& expression_true();

/**
  \brief false Convenience constant expression always evaluating to false.
 */
OSSIA_EXPORT const expression_base& expression_false();

/**
 * The following functions are factories used to create
 * expressions.
 */

/**
 * @brief make_expression_true
 * @return A "true" expression_bool.
 * @see expression_bool
 */
OSSIA_EXPORT inline expression_ptr make_expression_true()
{
  return std::make_unique<expression_base>(
      eggs::variants::in_place<expression_bool>, true);
}

/**
 * @brief make_expression_false
 * @return A "false" expression_bool.
 * @see expression_bool
 */
OSSIA_EXPORT inline expression_ptr make_expression_false()
{
  return std::make_unique<expression_base>(
      eggs::variants::in_place<expression_bool>, false);
}

/**
 * @brief make_expression_atom
 * @return An expression_atom
 * @see expression_atom
 */
template <typename... Args>
expression_ptr make_expression_atom(Args&&... args)
{
  return std::make_unique<expression_base>(
      eggs::variants::in_place<expression_atom>, std::forward<Args>(args)...);
}
OSSIA_EXPORT
expression_ptr make_expression_atom(
    const ossia::expressions::expression_atom::val_t& lhs,
    ossia::expressions::comparator c,
    const ossia::expressions::expression_atom::val_t& rhs);
OSSIA_EXPORT
expression_ptr make_expression_atom(
    ossia::expressions::expression_atom::val_t&& lhs,
    ossia::expressions::comparator c,
    ossia::expressions::expression_atom::val_t&& rhs);

/**
 * @brief make_expression_bool
 * @return An expression_bool
 * @see expression_bool
 */
template <typename... Args>
expression_ptr make_expression_bool(Args&&... args)
{
  return std::make_unique<expression_base>(
      eggs::variants::in_place<expression_bool>, std::forward<Args>(args)...);
}

/**
 * @brief make_expression_composition
 * @return An expression_composition
 * @see expression_composition
 */
template <typename... Args>
expression_ptr make_expression_composition(Args&&... args)
{
  return std::make_unique<expression_base>(
      eggs::variants::in_place<expression_composition>,
      std::forward<Args>(args)...);
}

/**
 * @brief make_expression_not
 * @return An expression_not
 * @see expression_not
 */
template <typename... Args>
expression_ptr make_expression_not(Args&&... args)
{
  return std::make_unique<expression_base>(
      eggs::variants::in_place<expression_not>, std::forward<Args>(args)...);
}

/**
 * @brief make_expression_pulse
 * @return An expression_pulse
 * @see expression_pulse
 */
template <typename... Args>
expression_ptr make_expression_pulse(Args&&... args)
{
  return std::make_unique<expression_base>(
      eggs::variants::in_place<expression_pulse>, std::forward<Args>(args)...);
}
}
}
