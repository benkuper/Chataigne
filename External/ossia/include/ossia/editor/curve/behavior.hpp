#pragma once
#include <ossia/detail/config.hpp>
#include <ossia/editor/curve/curve_abstract.hpp>
#include <memory>
#include <ossia_export.h>
#include <vector>

/**
 * \file behavior.hpp
 */
namespace ossia
{

template <typename X, typename Y>
class curve;

struct behavior;
#include <ossia/editor/curve/behavior_variant_impl.hpp>

/**
 * @class Contains either no curve, a single curve, or multiple curves
 *
 * \see \ref curve_abstract
 * \see \ref curve
 */
struct OSSIA_EXPORT behavior final
{
  behavior_variant_type v;

  behavior() = default;
  behavior(const behavior&) = default;
  behavior(behavior&&) = default;
  behavior& operator=(const behavior&) = default;
  behavior& operator=(behavior&&) = default;

  behavior(std::shared_ptr<ossia::curve_abstract> c) : v{std::move(c)}
  {
  }
  template <typename T, typename U>
  behavior(std::shared_ptr<ossia::curve<T, U>> c) : v{curve_ptr{std::move(c)}}
  {
  }
  behavior(std::vector<ossia::behavior> c) : v{std::move(c)}
  {
  }

  /**
   * @brief reset Recursively calls reset on the curves of this behavior.
   */
  void reset();

  operator bool() const
  {
    return bool(v);
  }

  template <typename T>
  const T& get() const
  {
    return v.get<T>();
  }

  template <typename T>
  T& get()
  {
    return v.get<T>();
  }

  template <typename T>
  const T* target() const noexcept
  {
    return v.target<T>();
  }

  template <typename T>
  T* target() noexcept
  {
    return v.target<T>();
  }

  template <typename Visitor>
  auto apply(Visitor&& vis) -> decltype(auto)
  {
    return ossia::apply(std::forward<Visitor>(vis), this->v);
  }

  template <typename Visitor>
  auto apply(Visitor&& vis) const -> decltype(auto)
  {
    return ossia::apply(std::forward<Visitor>(vis), this->v);
  }
};
template <typename Functor>
auto apply(Functor&& functor, const behavior& var) -> decltype(auto)
{
  return ossia::apply(std::forward<Functor>(functor), var.v);
}
template <typename Functor>
auto apply(Functor&& functor, behavior& var) -> decltype(auto)
{
  return ossia::apply(std::forward<Functor>(functor), var.v);
}
template <typename Functor>
auto apply(Functor&& functor, behavior&& var) -> decltype(auto)
{
  return ossia::apply(std::forward<Functor>(functor), std::move(var.v));
}
template <typename Functor>
auto apply_nonnull(Functor&& functor, const behavior& var) -> decltype(auto)
{
  return ossia::apply_nonnull(std::forward<Functor>(functor), var.v);
}
template <typename Functor>
auto apply_nonnull(Functor&& functor, behavior& var) -> decltype(auto)
{
  return ossia::apply_nonnull(std::forward<Functor>(functor), var.v);
}
template <typename Functor>
auto apply_nonnull(Functor&& functor, behavior&& var) -> decltype(auto)
{
  return ossia::apply_nonnull(
      std::forward<Functor>(functor), std::move(var.v));
}
}
