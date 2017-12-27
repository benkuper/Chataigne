#pragma once
#include <ossia/detail/optional.hpp>
#include <ossia/detail/ptr_container.hpp>
#include <ossia/editor/curve/behavior.hpp>
#include <ossia/editor/scenario/time_process.hpp>
#include <ossia/editor/state/state_element.hpp>
#include <ossia/network/value/destination.hpp>
#include <ossia_export.h>

/**
 * \file automation.hpp
 */
namespace ossia
{
class value;
namespace net
{
class parameter_base;
}

/**
 * \brief The automation class
 *
 * An automation is a time process that represents a curve of one
 * or multiple parameters.
 *
 * It drives (i.e. sends messages) to a parameter, according to a driving value.
 * Its driven \ref net::parameter_base can be either of numeric type (\ref
 * val_type::INT, \ref val_type::FLOAT...)
 * or of \ref val_type::LIST, if the list only has numeric elements (e.g.
 * List{Float, Float}).
 *
 * The driving \ref value can either be a single \ref Behavior or a \ref List
 * of \ref Behavior,
 * in accordance to the type of the driven \ref net::parameter_base.
 *
 * The automation has a "source" domain, i.e. the data space in which the
 * transformation
 * happens. The target domain is taken from the driven parameter_base.
 * The unit is stored in m_lastMessage.unit.
 *
 *
 * \see \ref Behavior \ref curve \ref curve_segment
 */
class OSSIA_EXPORT automation final : public ossia::time_process
{
public:
  automation();
  automation(destination, const ossia::behavior&);
  automation(destination, ossia::behavior&&);

  ~automation();

  void set_destination(destination d);
  void set_behavior(ossia::behavior b);

  void clean();

private:
  ossia::state_element offset(ossia::time_value, double pos) override;
  ossia::state_element state(ossia::time_value date, double pos) override;

  void start(ossia::state& st) override;
  void stop() override;
  void pause() override;
  void resume() override;

  static ossia::value
  compute_value(double, ossia::val_type drivenType, const ossia::behavior&);

  void update_message(double t);

  ossia::behavior m_drive;
  optional<ossia::message> m_lastMessage;
  ossia::val_type m_drivenType{};
};
}
