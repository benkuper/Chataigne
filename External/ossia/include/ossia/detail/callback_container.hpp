#pragma once
#include <ossia/detail/config.hpp>
#include <list>
#include <mutex>
#include <ossia_export.h>
#include <stdexcept>

/**
 * \file callback_container.hpp
 */

namespace ossia
{
/**
 * @brief The invalid_callback_error class
 *
 * Means that an invalid callback was passed
 */
struct OSSIA_EXPORT invalid_callback_error : public std::logic_error
{
  invalid_callback_error(const char* e) : std::logic_error(e)
  {
  }

  invalid_callback_error() : std::logic_error("Bad callback")
  {
  }
};

template <typename T>
/**
 * @brief The callback_container class
 *
 * Contains callbacks.
 * Classes that have network callbacks may want to derive from this and
 * implement
 * on_first_callback_added, on_removing_last_callback.
 *
 * This allows to cleanly stop listening when there are no callbacks.
 *
 */
class callback_container
{
public:
  callback_container()
  {
  }
  callback_container(const callback_container& other)
  {
    std::lock_guard<std::mutex> lck{other.m_mutx};
    m_callbacks = other.m_callbacks;
  }
  callback_container(callback_container&& other)
  {
    std::lock_guard<std::mutex> lck{other.m_mutx};
    m_callbacks = std::move(other.m_callbacks);
  }
  callback_container& operator=(const callback_container& other)
  {
    std::lock_guard<std::mutex> lck{other.m_mutx};
    m_callbacks = other.m_callbacks;
    return *this;
  }
  callback_container& operator=(callback_container&& other)
  {
    std::lock_guard<std::mutex> lck{other.m_mutx};
    m_callbacks = std::move(other.m_callbacks);
    return *this;
  }

  virtual ~callback_container()
  {
  }

  /**
   * @brief impl How the callbackas are stored.
   * A list is used since iterators to other callbacks
   * must not be invalidated upon removal.
   */
  using impl = typename std::list<T>;
  using iterator = typename impl::const_iterator;

  /**
   * @brief add_callback Add a new callback.
   * @param callback must be a std::function or similar.
   * @return iterator to save in order to be able to remove the callback.
   */
  iterator add_callback(T callback)
  {
    T cb = callback;
    if (cb)
    {
      std::lock_guard<std::mutex> lck{m_mutx};
      auto it = m_callbacks.insert(m_callbacks.begin(), std::move(cb));
      if (m_callbacks.size() == 1)
        on_first_callback_added();
      return it;
    }
    else
    {
      throw invalid_callback_error{};
    }
  }

  /**
   * @brief remove_callback Removes a callback identified by an iterator.
   * @param it Iterator to remove.
   */
  void remove_callback(iterator it)
  {
    std::lock_guard<std::mutex> lck{m_mutx};
    if (m_callbacks.size() == 1)
      on_removing_last_callback();
    m_callbacks.erase(it);
  }

  /**
   * @brief callback_count
   * @return Number of active callbacks.
   */
  std::size_t callback_count() const
  {
    std::lock_guard<std::mutex> lck{m_mutx};
    return m_callbacks.size();
  }

  /**
   * @brief callbacks_empty
   * @return True if there are no callbacks
   */
  bool callbacks_empty() const
  {
    std::lock_guard<std::mutex> lck{m_mutx};
    return m_callbacks.empty();
  }

  /**
   * @brief send Trigger all callbacks
   * @param args Arguments to send to the callbacks.
   */
  template <typename... Args>
  void send(Args&&... args)
  {
    std::lock_guard<std::mutex> lck{m_mutx};
    for (auto& callback : m_callbacks)
    {
      if (callback)
        callback(std::forward<Args>(args)...);
    }
  }

  /**
   * @brief clear Clears callbacks.
   */
  void callbacks_clear()
  {
    std::lock_guard<std::mutex> lck{m_mutx};
    if (!m_callbacks.empty())
      on_removing_last_callback();
    m_callbacks.clear();
  }

protected:
  /**
   * @brief on_first_callback_added
   *
   * These functions can be reimplemented by
   * subclasses wishing to perform special actions.
   * For instance, when the last callback is removed,
   * stop listening somewhere...
   *
   * \see \ref on_removing_last_callback
  */
  virtual void on_first_callback_added()
  {
  }

  /**
   * @brief on_removing_last_callback
   *
   * \see \ref on_first_callback_added
   */
  virtual void on_removing_last_callback()
  {
  }

private:
  impl m_callbacks;
  mutable std::mutex m_mutx;
};
}
