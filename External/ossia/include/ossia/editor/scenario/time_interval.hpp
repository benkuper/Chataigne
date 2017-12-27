#pragma once

#include <functional>
#include <memory>
#include <string>

#include <ossia/detail/ptr_container.hpp>
#include <ossia/editor/scenario/time_value.hpp>
#include <ossia/editor/state/state_element_fwd.hpp>
#include <ossia_export.h>

/**
 * \file time_interval.hpp
 */
namespace ossia
{
class time_event;
class time_process;

/**
 * @brief The time_interval class
 *
 * \brief #time_interval is use to describe temporal structure precising
 * which
 * amount of time (the duration) are between two #time_events.
 *
 * \details The duration can be fixed or between a minimal and a maximal
 * duration. \n
 * #time_interval is a #TimeProcess container.
 */
class OSSIA_EXPORT time_interval
{

public:
  auto get_date() const
  {
    return m_date;
  }
  auto get_position() const
  {
    return m_position;
  }
  auto get_offset() const
  {
    return m_offset;
  }
  auto get_speed() const
  {
    return m_speed;
  }
  void set_offset(ossia::time_value g)
  {
    m_offset = g;
  }
  void set_speed(double g)
  {
    m_speed = g;
  }
  ossia::state_element tick(ossia::time_value count);
  ossia::state_element tick(ossia::time_value count, double ratio);

  /*! to get the interval execution back
   \param const #TimeValue position
   \param const #TimeValue date
   \param std::shared_ptr<#State> */
  using exec_callback = std::function<void(
      double, ossia::time_value, const ossia::state_element&)>;

  /*! constructor
   \details by default a #time_interval has an infinite duration with no
   minimal duration and an infinite maximal duration.
   \param #time_interval::ExecutionCallback to use to be notified at each
   step
   \param #std::shared_ptr<TimeEvent> event where the #time_interval starts
   \param #std::shared_ptr<TimeEvent> event where the #time_interval ends
   \param const #TimeValue& nominal duration of the #time_interval
   \param const #TimeValue& minimal duration of the #time_interval
   \param const #TimeValue& maximal duration of the #time_interval
   \return std::shared_ptr<#time_interval> */
  static std::shared_ptr<time_interval> create(
      time_interval::exec_callback, time_event&, time_event&,
      time_value = Infinite, time_value = Zero, time_value = Infinite);

  time_interval(
      time_interval::exec_callback, time_event&, time_event&,
      time_value = Infinite, time_value = Zero, time_value = Infinite);

  /*! desctructor */
  ~time_interval();

  /*! start #time_interval */
  void start();
  void start(ossia::state&);

  /*! stop #time_interval */
  void stop();

  /*! start #time_interval */
  void pause();

  /*! stop #time_interval */
  void resume();

  /*! set #time_interval's offset and process a state at offset date
   \details the returned #State is made of as many as sub States for each
   TimeProcess the #time_interval manages
   \details don't call offset when the #time_interval is running
   \param const #TimeValue offset date
   \return std::shared_ptr<#State> */
  ossia::state_element offset(ossia::time_value);

  /*! get a #State from the interval depending on its date
   \details the returned #State is made of as many as sub States for each
   TimeProcess the #time_interval manages
   \details don't call state when the #time_interval is not running
   \return std::shared_ptr<#State> */
  ossia::state_element state();

  /*! sets a new callback for the interval
    \param #time_interval::ExecutionCallback to use to be notified at each
    step
    */
  void set_callback(exec_callback);

  //! This callback won't compute the state.
  void set_stateless_callback(exec_callback);

  /*! get the #time_interval nominal duration
   \return const #TimeValue& nominal duration */
  const time_value& get_nominal_duration() const;

  /*! set the #time_interval duration
   \param const #TimeValue& duration
   \return #time_interval the interval */
  time_interval& set_nominal_duration(ossia::time_value);

  /*! get the #time_interval minimal duration
   \return const #TimeValue& minimal duration */
  const time_value& get_min_duration() const;

  /*! set the #time_interval minimal duration
   \param const #TimeValue& minimal duration
   \return #time_interval the interval */
  time_interval& set_min_duration(ossia::time_value);

  /*! get the #time_interval maximal duration
   \return const #TimeValue& maximal duration */
  const time_value& get_max_duration() const;

  /*! set the #time_interval maximal duration
   \param const #TimeValue& maximal duration
   \return #time_interval the interval */
  time_interval& set_max_duration(ossia::time_value);

  /*! get the event from where the #time_interval starts
   \return std::shared_ptr<#time_event> start event */
  time_event& get_start_event() const;

  /*! get the event from where the #time_interval starts
   \return std::shared_ptr<#time_event> start event */
  time_event& get_end_event() const;

  /*! add a #TimeProcess
   \details it also stores the #TimeProcess's start and end #States into the
   #time_interval's start and end #time_events
   \param std::shared_ptr<#TimeProcess> to insert */
  void add_time_process(std::shared_ptr<time_process>);

  /*! remove a #TimeProcess
   \details it also removes the #TimeProcess's start and end #States from the
   #time_interval's start and end #time_events
   \param std::shared_ptr<#TimeProcess> to insert */
  void remove_time_process(time_process*);

  /*! get time processes attached to the #time_interval
   \return #Container<#TimeProcess> */
  const std::vector<std::shared_ptr<time_process>>& get_time_processes() const
  {
    return m_processes;
  }

private:
  ossia::state make_state();

  std::vector<std::shared_ptr<time_process>> m_processes;
  time_interval::exec_callback m_callback;

  time_event& m_start;
  time_event& m_end;

  time_value m_nominal{};
  time_value m_min{};
  time_value m_max{};

  /// the progression of the interval between the beginning
  /// and the nominal duration [0. :: 1.]
  double m_position{};

  time_value m_date{};

  time_value m_offset{}; /// the date the clock will run from
  double m_speed{1.}; /// tick length is multiplied by this
  bool m_running{};
};
}
