#pragma once

#include <functional>
#include <memory>
#include <string>

#include <ossia/detail/ptr_container.hpp>
#include <ossia/editor/expression/expression_fwd.hpp>
#include <ossia/editor/state/state_element.hpp>
#include <cstdint>
#include <ossia_export.h>

/**
 * \file time_event.hpp
 */

namespace ossia
{
class state;
class time_interval;
class time_sync;
class scenario;
/**
 * @brief The time_event class
 *
 * \brief #time_event is use to describe temporal structure to launch the start
 * or the end of each attached #time_interval.
 *
 * \details #time_event has a #State and can also be submitted to an
 * expression.
 *
 */
class OSSIA_EXPORT time_event
{
  friend class ossia::scenario;

public:
  /*! event status */
  enum class status : int8_t
  {
    NONE,
    PENDING,
    HAPPENED,
    DISPOSED
  };

  /**
     * @brief The OffsetBehavior enum
     * Describes what happens when a parent scenario
     * does an offset beyond this event. This is useful to
     * make default cases for the scenario.
     */
  enum class offset_behavior : int8_t
  {
    EXPRESSION_TRUE,  //! The condition is considered True
    EXPRESSION_FALSE, //! The condition is considered False
    EXPRESSION        //! The condition will be evaluated
  };

  /*! to get the event status back
     \param #Status new status */
  using exec_callback = std::function<void(status)>;

public:
  time_event(
      time_event::exec_callback, time_sync& aTimeSync,
      expression_ptr anExpression);

  /*! destructor */
  ~time_event();

  /*! changes the callback in the event
   \param #time_event::ExecutionCallback to get #time_event's status back
   \details this may be unsafe to do during execution */
  void set_callback(time_event::exec_callback);

  /*! make the event happen to propagate the execution to next TimeIntervals
   \details the event have to be in PENDING status to call this method
   otherwise
   it will raise a runtime_error
   \details turning the event' status into HAPPENED will call the callback to
   notify its owner */
  void happen(ossia::state& st);

  /*! dispose the event to not propagate the execution to next TimeIntervals
   \details the event have to be in PENDING status to call this method
   otherwise
   it will raise a runtime_error
   \details turning the event' status into DISPOSED will call the callback to
   notify its owner */
  void dispose();

  /*! add a sub state into the state of the event
   \param std::shared_ptr<#State> to add */
  void add_state(state_element&&);

  /*! remove a sub state from the state of the event
   \param std::shared_ptr<#State> to remove */
  void remove_state(const state_element&);

  /*! get the #time_sync where the event is
   \return std::shared_ptr<#time_sync> */
  time_sync& get_time_sync() const;

  /*! get the state of the event
  \return std::shared_ptr<#State> */
  const state& get_state() const;

  /*! get the expression of the event
  \return std::shared_ptr<expression> */
  const expression& get_expression() const;

  /*! set the expression of the event
   \param std::shared_ptr<expression>
   \return #time_event the event */
  time_event& set_expression(expression_ptr);

  /*! get the status of the event
   \return #Status */
  status get_status() const;

  /**
   * @brief getOffsetValue Returns the value of the condition if
   * we are offseting past this time event.
   */
  offset_behavior get_offset_behavior() const;

  /**
   * @brief setOffsetValue Sets the value of the condition if we are offseting
   * past this time event.
   */
  time_event& set_offset_behavior(offset_behavior);

  /*! get previous time contraints attached to the event
   \return #Container<#time_interval> */
  ptr_container<time_interval>& previous_time_intervals()
  {
    return m_previous_time_intervals;
  }

  /*! get previous time contraints attached to the event
   \return #Container<#TimeProcess> */
  const ptr_container<time_interval>& previous_time_intervals() const
  {
    return m_previous_time_intervals;
  }

  /*! get next time contraints attached to the event
   \return #Container<#time_interval> */
  ptr_container<time_interval>& next_time_intervals()
  {
    return m_next_time_intervals;
  }

  /*! get next time contraints attached to the event
   \return #Container<#TimeProcess> */
  const ptr_container<time_interval>& next_time_intervals() const
  {
    return m_next_time_intervals;
  }

  void set_status(status s);

  void reset();

  /* To be called before deletion, to break the shared_ptr cycle */
  void cleanup();

private:
  time_event::exec_callback m_callback;

  time_sync& m_timesync;
  state m_state;
  status m_status;
  offset_behavior m_offset{offset_behavior::EXPRESSION_TRUE};

  expression_ptr m_expression;

  ptr_container<time_interval> m_previous_time_intervals;
  ptr_container<time_interval> m_next_time_intervals;
};
}
