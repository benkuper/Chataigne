#pragma once
#include <ossia/detail/optional.hpp>
#include <ossia/editor/scenario/time_process.hpp>
#include <ossia/network/value/destination.hpp>
#include <ossia/editor/curve/curve.hpp>

namespace ossia
{
class OSSIA_EXPORT metronome : public ossia::time_process
{
public:
  metronome();

  void set_destination(ossia::destination a);
  void set_curve(std::shared_ptr<curve<double,float>> b);

  ossia::state_element state(ossia::time_value date, double pos) override;

  void start(ossia::state& st) override;
  void stop() override;
  void pause() override;
  void resume() override;

private:
  optional<ossia::destination> m_parameter;
  std::shared_ptr<curve<double,float>> m_curve;
  time_value m_lastTick;
};
}
