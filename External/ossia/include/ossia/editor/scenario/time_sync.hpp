#pragma once

#include <memory>
#include <string>

#include <ossia/detail/ptr_container.hpp>
#include <ossia/editor/expression/expression.hpp>
#include <ossia/editor/scenario/time_event.hpp>
#include <ossia/editor/scenario/time_value.hpp>
#include <hopscotch_set.h>
#include <nano_signal_slot.hpp>
#include <ossia_export.h>

/**
 * \file time_sync.hpp
 */
namespace ossia
{

class expression_base;
class state;
class time_event;
class scenario;
/**
 * \brief #time_sync is use to describe temporal structure to synchronize each
 * attached #time_event evaluation.
 *
 * \details #time_sync is also a #time_event container.
 * \todo the shared_from_this is used at a single point, maybe it should be
 * removed ?
 * or replaced with intrusive_ptr ?
 */
class OSSIA_EXPORT time_sync final
    : public std::enable_shared_from_this<time_sync>
{
  friend class ossia::scenario;

public:
  using iterator = ptr_container<time_event>::iterator;
  using const_iterator = ptr_container<time_event>::const_iterator;

  time_sync();

  ~time_sync();

  /*! evaluate all #time_event's to make them to happen or to dispose them
 \return boolean true if the operation succeeded */
  bool trigger(ossia::state& st);
  std::atomic_bool trigger_request{};

  /*! get the date
 \details the date is the sum of its previous #time_interval durations
 \details a #time_sync with na previous #time_intervals have a date equals to
 0.
 \return #TimeValue the date */
  time_value get_date() const;

  /*! get the expression of the #time_sync */
  const expression& get_expression() const;

  /*! set the expression of the #time_sync
 \details setting the expression to ExpressionTrue will defer the evaluation
 on #time_event's expression
 \details setting the expression to ExpressionFalse will mute TimeSync
 execution
 \param expression_ptr
 \return #time_sync the time_sync */
  time_sync& set_expression(expression_ptr);

  /*! create and store a #time_event
 \param #Container<#time_event>::const_iterator where to store the #time_event
 \param #time_event::ExecutionCallback to get #time_event's status back
 \param expression_ptr an optional expression to apply to the
 #time_event
 \return std::shared_ptr<#time_event> */
  iterator emplace(
      const_iterator, time_event::exec_callback,
      expression_ptr = expressions::make_expression_true());
  iterator insert(const_iterator, std::shared_ptr<time_event>);
  void remove(const std::shared_ptr<time_event>&);

  /*! get the #time_events of the #time_sync
 \return #Container<#time_event> */
  ptr_container<time_event>& get_time_events()
  {
    return m_timeEvents;
  }

  /*! get the #time_events of the #time_sync
 \return #Container<#time_event> */
  const ptr_container<time_event>& get_time_events() const
  {
    return m_timeEvents;
  }

  // Interface to be used for set-up by other time processes
  /* process all TimeEvents to propagate execution */
  void process(std::vector<time_event*>& statusChangedEvents, ossia::state& st);

  void process_this(std::vector<time_event*>& statusChangedEvents);

  /* is the TimeSync observing its Expression ? */
  bool is_observing_expression() const;

  bool is_evaluating() const;

  /* enable observation of the Expression */
  void observe_expression(bool);

  //! Resets the internal state. Necessary when restarting an execution.
  void reset();

  /* To be called before deletion, to break the shared_ptr cycle */
  void cleanup();

  /*! Execution callbacks
   *
   * Used to be notified when the #time_sync is triggered.
   * \todo why no nano-signal-slot ?
   * \details This is not thread-safe
   */
  callback_container<std::function<void()>> triggered;

  //! Called when the time_sync starts evaluating
  callback_container<std::function<void()>> entered_evaluation;

  //! Called if the time_sync stops evaluating due to a changing duration
  callback_container<std::function<void()>> left_evaluation;

  //! Boolean : true if the evaluation was finished due to the max bound
  callback_container<std::function<void(bool)>> finished_evaluation;

  enum status
  {
    NOT_DONE,
    DONE_TRIGGERED,
    DONE_MAX_REACHED
  };
  status get_status() const
  {
    return m_status;
  }

private:
  ossia::expression_ptr m_expression;

  ptr_container<time_event> m_timeEvents;
  std::vector<time_event*> m_pending;

  optional<expressions::expression_callback_iterator> m_callback;

  status m_status = NOT_DONE;
  bool m_observe = false;
  bool m_evaluating = false;
};
}
