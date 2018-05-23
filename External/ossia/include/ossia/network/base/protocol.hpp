#pragma once
#include <ossia/detail/config.hpp>
#include <ossia/network/common/network_logger.hpp>
#include <vector>
#include <future>
#include <memory>

#include <ossia_export.h>

namespace ossia
{
class state;

namespace net
{
class parameter_base;
class node_base;
class device_base;
struct full_parameter_data;

/**
 * @brief The protocol_base class
 *
 * A protocol is used to implement remote communication.
 *
 * \see local_protocol
 * \see osc_protocol
 * \see minuit_protocol
 * \see oscquery_protocol
 * \see midi_protocol
 */
class OSSIA_EXPORT protocol_base
{
public:
  virtual ~protocol_base();

  /**
   * @brief Pulls a value from the server synchronously.
   * @return True if the value was correctly retrieved.
   */
  virtual bool pull(parameter_base&) = 0;

  /**
   * @brief Pulls a value from the server asynchronously.
   * @return A future that will be set when the value is received.
   */
  virtual std::future<void> pull_async(parameter_base&);

  /**
   * @brief Request an update on a value.
   */
  virtual void request(parameter_base&);

  /**
   * @brief Send a value to the network.
   */
  virtual bool push(const parameter_base&) = 0;

  /**
   * @brief Send many values in one go if the protocol supports it
   */
  virtual bool push_bundle(const std::vector<const ossia::net::parameter_base*>&);

   /**
    * @brief Send a value to the network.
    */
  virtual bool push_raw(const full_parameter_data&) = 0;

   /**
    * @brief Send a value to the network.
    */
   virtual bool push_raw_bundle(const std::vector<full_parameter_data>&);

  /**
   * @brief Notify the network that a parameter should be listened to.
   *
   * In some protocols (Minuit, OSCQuery), this may send a message to the other
   * client
   * so that it regularly sends values of this parameter to the local computer.
   *
   * If a parameter is listened to, when a message is received,
   * parameter_base::setValue will be called,
   * which will notify the local signals.
   * Else, parameter_base::setValueQuiet will be called : the value will be
   * updated but the signals
   * won't be notified.
   */
  virtual bool observe(parameter_base&, bool) = 0;

  /**
   * @brief Begin observation without notifying the other computers.
   */
  virtual bool observe_quietly(parameter_base&, bool)
  {
    return false;
  }

  /**
   * @brief If the protocol supports it, request the namespace corresponding
   * to this node.
   */
  virtual bool update(node_base& node_base) = 0;

  /**
   * @brief It is mandatory to call this function from device implementations,
   * when
   * the protocol is set.
   */
  virtual void set_device(ossia::net::device_base& dev)
  {
  }

  //! Replace the loggers used
  void set_logger(const network_logger& l)
  {
    m_logger = l;
  }
  const network_logger& get_logger() const
  {
    return m_logger;
  }

  virtual void stop() { }

protected:
  network_logger m_logger;
};
}
}
