#pragma once

#include <memory>

#include <ossia/editor/expression/expression_fwd.hpp>
#include <ossia_export.h>

/**
 * \file expression_not.hpp
 */
namespace ossia
{
namespace expressions
{
/**
 * @brief expression_not : negation of an expression
 * @see expression.hpp
 */
class OSSIA_EXPORT expression_not final : public expression_callback_container
{
public:
  expression_not(expression_ptr);
  expression_not(expression_not&& other) = delete;
  expression_not& operator=(expression_not&& other) = delete;

  expression_not(const expression_not& other) = delete;
  expression_not& operator=(const expression_not& other) = delete;

  virtual ~expression_not();

  bool evaluate() const;
  void update() const;

  expression_base& get_expression() const;

private:
  void on_first_callback_added() override;
  void on_removing_last_callback() override;

  void result_callback(bool result);

  expression_ptr m_expression;
  expression_callback_iterator m_callback;
};
}
}
