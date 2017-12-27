#pragma once
#include <ossia/detail/optional.hpp>
#include <ossia/detail/ptr_container.hpp>
#include <ossia/editor/curve/behavior.hpp>
#include <ossia/editor/scenario/clock.hpp>
#include <ossia/editor/scenario/time_process.hpp>
#include <ossia/editor/state/state.hpp>
#include <ossia/editor/state/state_element.hpp>
#include <ossia/network/base/parameter.hpp>
#include <ossia_export.h>

#include <ossia/detail/mutex.hpp>

/**
 * \file mapper.hpp
 */
namespace ossia
{
namespace net
{
class parameter_base;
}
class value;

/**
 * @brief The mapper class
 *
 * Allows to map a value to another following a transfer function.
 * The driver address is where the input value is taken from;
 * The driven address is where the output value is sent to.
 */
class OSSIA_EXPORT mapper final : public ossia::time_process
{
public:
  mapper();
  mapper(ossia::destination, ossia::destination, ossia::behavior);

  ~mapper();

  void set_driver(ossia::destination);
  void set_driven(ossia::destination);
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
  compute_value(const ossia::value&, const ossia::behavior&);

  void driver_value_callback(ossia::value value);

  optional<ossia::destination> m_driverAddress;
  ossia::behavior m_drive;

  optional<ossia::message> m_lastMessage;
  ossia::value m_valueToMap;
  mutable mutex_t m_valueToMapMutex;
  mutable mutex_t m_driverAddressMutex;

  optional<ossia::net::parameter_base::callback_index> m_callback;

  friend struct mapper_compute_visitor;
};
}
