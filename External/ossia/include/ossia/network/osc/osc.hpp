#pragma once

#include <string>

#include <ossia/detail/mutex.hpp>
#include <ossia/network/base/listening.hpp>
#include <ossia/network/base/protocol.hpp>
#include <ossia/network/osc/detail/osc_fwd.hpp>
#include <atomic>
#include <hopscotch_map.h>
#include <ossia/network/zeroconf/zeroconf.hpp>

namespace oscpack
{
class ReceivedMessage;
class IpEndpointName;
}
namespace osc
{
template <typename T>
class sender;
class receiver;
}
namespace ossia
{
namespace net
{
class OSSIA_EXPORT osc_protocol final : public ossia::net::protocol_base
{
public:
  osc_protocol(std::string ip,
               uint16_t remote_port,
               uint16_t local_port,
               ossia::optional<std::string> expose_name = ossia::none);

  osc_protocol(const osc_protocol&) = delete;
  osc_protocol(osc_protocol&&) = delete;
  osc_protocol& operator=(const osc_protocol&) = delete;
  osc_protocol& operator=(osc_protocol&&) = delete;

  ~osc_protocol();

  const std::string& get_ip() const;
  osc_protocol& set_ip(std::string);

  uint16_t get_remote_port() const;
  osc_protocol& set_remote_port(uint16_t);

  uint16_t get_local_port() const;
  osc_protocol& set_local_port(uint16_t);

  bool learning() const;
  osc_protocol& set_learning(bool);

  bool update(ossia::net::node_base& node_base) override;

  bool pull(ossia::net::parameter_base& parameter_base) override;

  bool push(const ossia::net::parameter_base& parameter_base) override;
  bool push_raw(const ossia::net::full_parameter_data& parameter_base) override;
  bool push_bundle(const std::vector<const ossia::net::parameter_base*>&) override;
  bool push_raw_bundle(const std::vector<full_parameter_data>&) override;

  bool observe(ossia::net::parameter_base& parameter_base, bool enable) override;

private:
  void on_received_message(
      const oscpack::ReceivedMessage& m, const oscpack::IpEndpointName& ip);
  void on_learn(const oscpack::ReceivedMessage& m);
  void set_device(ossia::net::device_base& dev) override;

  void update_sender();
  void update_receiver();
  void update_zeroconf();

  listened_parameters m_listening;

  std::unique_ptr<osc::sender<osc_outbound_visitor>> m_sender;
  std::unique_ptr<osc::receiver> m_receiver;

  net::zeroconf_server m_zeroconfServer;

  ossia::net::device_base* m_device{};
  std::string m_ip;

  uint16_t m_remote_port{}; /// the port that a remote device opens
  uint16_t
      m_local_port{}; /// the port where a remote device sends OSC messages
                      /// to (opened in this library)
  std::atomic_bool
      m_learning{}; /// if the device is currently learning from inbound
                    /// messages.
  optional<std::string> m_expose{};
};
}
}
