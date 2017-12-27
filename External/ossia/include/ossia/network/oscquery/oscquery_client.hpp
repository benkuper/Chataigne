#pragma once
#include <ossia/detail/mutex.hpp>
#include <ossia/detail/string_map.hpp>
#include <ossia/network/oscquery/detail/server.hpp>
#include <ossia/network/oscquery/detail/outbound_visitor_impl.hpp>
#include <ossia/network/common/network_logger.hpp>
#include <ossia/network/osc/detail/sender.hpp>

namespace osc
{
template<typename T>
class sender;
}
namespace ossia
{
namespace net
{
class parameter_base;
}
namespace oscquery
{
struct oscquery_client
{
  websocket_server::connection_handler connection;
  mutex_t listeningMutex;
  string_map<ossia::net::parameter_base*> listening;

  std::string client_ip;
  std::unique_ptr<osc::sender<oscquery::osc_outbound_visitor>> sender;
  int remote_sender_port{};

public:
  oscquery_client() = default;
  oscquery_client(oscquery_client&& other)
      : connection{std::move(other.connection)}
      , listening{std::move(other.listening)}
      , client_ip{std::move(other.client_ip)}
      , sender{std::move(other.sender)}
  {
    // FIXME http://stackoverflow.com/a/29988626/1495627
  }

  oscquery_client& operator=(oscquery_client&& other)
  {
    connection = std::move(other.connection);
    listening = std::move(other.listening);
    client_ip = std::move(other.client_ip);
    sender = std::move(other.sender);
    return *this;
  }

  oscquery_client(websocket_server::connection_handler h)
      : connection{std::move(h)}
  {
  }

  void start_listen(std::string path, ossia::net::parameter_base* addr)
  {
    if (addr)
    {
      listeningMutex.lock();
      listening.insert(std::make_pair(std::move(path), addr));
      listeningMutex.unlock();
    }
  }

  void stop_listen(const std::string& path)
  {
    listeningMutex.lock();
    listening.erase(path);
    listeningMutex.unlock();
  }

  bool operator==(const websocket_server::connection_handler& h) const
  {
    return !connection.expired() && connection.lock() == h.lock();
  }

  void open_osc_sender(const ossia::net::network_logger& l, uint16_t port)
  {
    sender = std::make_unique<osc::sender<oscquery::osc_outbound_visitor>>(
        l, client_ip, port);
  }

};
}
}
