#pragma once

#include <memory>

#include <ossia/editor/expression/expression_fwd.hpp>
#include <ossia/editor/expression/operators.hpp>
#include <ossia_export.h>

/**
 * \file expression_composition.hpp
 */
namespace ossia
{
namespace expressions
{
/**
 * @brief expression_composition : logical composition (and, or, xor)
 * @see expression.hpp
 */
class OSSIA_EXPORT expression_composition final
    : public expression_callback_container
{
public:
  expression_composition(
      expression_ptr expr1, binary_operator op, expression_ptr expr2);
  expression_composition(expression_composition&& other) = delete;
  expression_composition& operator=(expression_composition&& other) = delete;

  expression_composition(const expression_composition& other) = delete;
  expression_composition& operator=(const expression_composition& other)
      = delete;

  virtual ~expression_composition();

  bool evaluate() const;

  void update() const;

  expression_base& get_first_operand() const;
  binary_operator get_operator() const;
  expression_base& get_second_operand() const;

private:
  void on_first_callback_added() override;
  void on_removing_last_callback() override;

  bool do_evaluation(bool first, bool second) const;

  void first_callback(bool first_result);
  void second_callback(bool second_result);

  expression_ptr m_first;
  expression_ptr m_second;

  expression_callback_iterator m_firstIndex;
  expression_callback_iterator m_secondIndex;

  binary_operator m_operator{};
};
}
}
