#pragma once
#include <ossia/detail/callback_container.hpp>
#include <ossia/detail/config.hpp>

#include <eggs/variant.hpp>
#include <functional>
#include <ossia_export.h>

/**
 * \file expression_fwd.hpp
 */
namespace ossia
{
namespace expressions
{
/**
 * A callback for expressions.
 */
using expression_result_callback = std::function<void(bool)>;

using expression_callback_container
    = callback_container<expression_result_callback>;
using expression_callback_iterator =
    typename expression_callback_container::iterator;
class expression_atom;
class expression_bool;
class expression_composition;
class expression_not;
class expression_pulse;
class expression_generic;
using expression_base = eggs::
    variant<expression_atom, expression_bool, expression_composition, expression_not, expression_pulse, expression_generic>;

// concept expression_base :
//    public callback_container<expression_result_callback>
//{
// public:

//  /*! evaluate the expression
//   \return bool result of the evaluation */
//  bool evaluate() const;

//  /*! pull the value of any #Destination operand */
//  void update() const;

//};
}
using expression_ptr = std::unique_ptr<expressions::expression_base>;

/**
 * \typedef expression
 *
 * An expression is a formula that checks truth values
 * according to values in a network tree.
 *
 * For instance :
 *
 * \code
 * (a:/b == 50) && !(c:/d < e:/f)
 * \endcode
 *
 * Expressions are built with factories found in \ref expression.hpp :
 * * \ref make_expression_true : a constant "true" node.
 * * \ref make_expression_false : a constant "false" node.
 * * \ref make_expression_atom : a relational comparison node (==, <=, !=,
 * etc...)
 *                               between two \ref ossia::value.
 * * \ref make_expression_pulse : a node that becomes true
 *                                if a message is received opn a given address
 * since the last time
 *                                update() has been called on it.
 * * \ref make_expression_not : standard logical not operator.
 * * \ref make_expression_composition : standard logical and, or, xor,
 * operators
 *                                      between two expressions..
 *
 * \see expression_atom
 * \see expression_bool
 * \see expression_composition
 * \see expression_not
 * \see expression_pulse
 */
using expression = expressions::expression_base;
}
