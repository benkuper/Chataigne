#pragma once
#include <ossia/editor/expression/expression_fwd.hpp>
#include <memory>
#include <ossia_export.h>

namespace ossia
{
namespace expressions
{
//! This expression allows the users of the library to implement their own
//! behaviour.
struct OSSIA_EXPORT expression_generic_base
{
  virtual ~expression_generic_base();
  virtual void update() = 0;
  virtual bool evaluate() const = 0;
  virtual void on_first_callback_added(expression_generic&) = 0;
  virtual void on_removing_last_callback(expression_generic&) = 0;
};

class OSSIA_EXPORT expression_generic final
    : public expression_callback_container
{
public:
  expression_generic(std::unique_ptr<expression_generic_base> ptr)
      : expr{std::move(ptr)}
  {
  }
  expression_generic(expression_generic_base* ptr) : expr{ptr}
  {
  }
  std::unique_ptr<expression_generic_base> expr;

  void update() const
  {
    expr->update();
  }
  bool evaluate() const
  {
    return expr->evaluate();
  }

private:
  void on_first_callback_added() override
  {
    expr->on_first_callback_added(*this);
  }
  void on_removing_last_callback() override
  {
    expr->on_removing_last_callback(*this);
  }
};

template<typename T, typename... Args>
auto make_expression_generic(Args&&... args)
{
  static_assert(std::is_base_of<expression_generic_base, T>::value,
                "Must inherit from expression_generic_base");

  return std::make_unique<expression_base>(expression_generic{
        std::make_unique<T>(std::forward<Args>(args)...)
        });
}

}
}
