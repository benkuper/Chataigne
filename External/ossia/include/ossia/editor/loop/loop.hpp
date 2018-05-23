#pragma once

#include <ossia/detail/ptr_container.hpp>
#include <ossia/editor/scenario/time_interval.hpp>
#include <ossia/editor/scenario/time_event.hpp>
#include <ossia/editor/scenario/time_process.hpp>
#include <ossia_export.h>

/**
 * \file loop.hpp
 */
namespace ossia
{
/**
 * @brief The loop class
 *
 * A time process that allows looping around a time_interval.
 * First the start time_sync is checked.
 * Then the main interval executes.
 * Then the end time_sync is checked.
 */
class OSSIA_EXPORT loop final : public time_process
{
public:
  /*! factory
   \param const #TimeValue& duration of the pattern #time_interval
   \param #time_interval::ExecutionCallback to be notified at each step of
   the
   loop
   \param #time_event::ExecutionCallback to get start pattern #time_event's
   status
   back
   \param #time_event::ExecutionCallback to get end pattern #time_event's
   status
   back
   \return a new loop */
  loop(
      time_value, time_interval::exec_callback, time_event::exec_callback,
      time_event::exec_callback);

  /*! destructor */
  ~loop();

  void start(ossia::state& st) override;
  void stop() override;
  void pause() override;
  void resume() override;

  state_element offset(ossia::time_value, double pos) override;
  state_element state(ossia::time_value date, double pos) override;

  /*! get the pattern #time_interval
   \return std::shared_ptr<TimeInterval> */
  const std::shared_ptr<time_interval> get_time_interval() const;

  /*! get the pattern start #time_sync
   \return std::shared_ptr<TimeSync> */
  const std::shared_ptr<time_sync> get_start_timesync() const;

  /*! get the pattern end #time_sync
   \return std::shared_ptr<TimeSync> */
  const std::shared_ptr<time_sync> get_end_timesync() const;

private:
  void interval_callback(double, time_value, const ossia::state_element&);

  void start_event_callback(time_event::status);

  void end_event_callback(time_event::status);

  std::shared_ptr<time_sync> m_startNode;
  time_event::exec_callback m_startCallback;

  std::shared_ptr<time_sync> m_endNode;
  time_event::exec_callback m_endCallback;

  std::shared_ptr<time_interval> m_interval;
  time_interval::exec_callback m_intervalCallback;

  ossia::state m_currentState; // an internal State to return on state call
  ossia::state m_offsetState;  // an internal State built when offset is called
};
}
