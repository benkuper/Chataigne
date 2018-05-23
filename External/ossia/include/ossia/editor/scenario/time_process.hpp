#pragma once

#include <ossia/editor/scenario/time_value.hpp>
#include <ossia/editor/state/state_element_fwd.hpp>
#include <memory>
#include <ossia_export.h>
#include <string>

/**
 * \file time_process.hpp
 */
namespace ossia
{
class time_interval;
/**
 * @brief The time_process class
 *
 * \brief #TimeProcess is the parent class used to describe temporal content.
 */
class OSSIA_EXPORT time_process
{
  friend class time_interval;

public:
  /*! destructor */
  virtual ~time_process();

  /*! get a #StateElement from the process on its parent #time_interval
   offset
   \details don't call offset when the parent #time_interval is running
   \param date offset date
   \param pos offset position (in [0;1] relative to parent nominal duration)
   \return state_element */
  virtual state_element offset(ossia::time_value date, double pos);

  /*! get a #StateElement from the process depending on its parent
   #time_interval date
   \details don't call state when the parent #time_interval is not running
   \return state_element */
  virtual state_element state(ossia::time_value date, double pos) = 0;

  /**
  * @brief start
  *
  * Will be called when
  * the parent time interval is started.
  */
  virtual void start(ossia::state& st);

  /**
  * @brief stop
  *
  * Will be called when
  * the parent time interval is stopped.
  */
  virtual void stop();

  /**
  * @brief stop
  *
  * Will be called when
  * the parent time interval is paused.
  */
  virtual void pause();

  /**
  * @brief resume
  *
  * Will be called when
  * the parent time interval is resumed.
  */
  virtual void resume();

  /**
   * @brief mute Mutes or unmute the process.
   *
   * `mute(true)` mutes the process.
   * A muted process executes itself but does not send its state.
   */
  void mute(bool m);

  //! True if the process is not currently muted.
  bool unmuted() const;

  /**
   * @brief Enables or disable the process.
   *
   * Unlike a muted process, a disabled process does not execute at all.
   */
  void enable(bool m);

  //! True if the process is enabled.
  bool enabled() const;


protected:
  //! Reimplement this to have a special behaviour on mute
  virtual void mute_impl(bool);
  ossia::time_value m_lastDate{ossia::Infinite};
  // used to filter multiple state calls at the
  // same time (use date as position can be always
  // 0 in infinite duration case)

private:
  bool m_unmuted = true;
  bool m_enabled = true;
};
}
