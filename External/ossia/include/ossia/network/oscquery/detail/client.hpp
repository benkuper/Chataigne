#pragma once
#include <ossia/detail/config.hpp>
#include <ossia/detail/json.hpp>
#include <ossia/detail/logger.hpp>
#include <boost/config.hpp>
#include <boost/version.hpp>
#include <nano_signal_slot.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

namespace ossia
{
namespace oscquery
{

//! Low-level Websocket client for OSCQuery
class websocket_client
{
public:
  using connection_handler = websocketpp::connection_hdl;
  Nano::Signal<void()> onOpen;
  std::function<void()> onClose;
  std::function<void()> onFail;

  //! \tparam Function that will be called when the client receives a server
  //! message.
  template <typename MessageHandler>
  websocket_client(MessageHandler&& onMessage) : m_open{false}
  {
    m_client.clear_access_channels(websocketpp::log::alevel::all);
    m_client.clear_error_channels(websocketpp::log::elevel::all);
    m_client.init_asio();

    m_client.set_open_handler([=](connection_handler hdl) {
      scoped_lock guard(m_lock);
      m_open = true;
      onOpen();
    });

    m_client.set_message_handler([handler = std::move(onMessage)](
        connection_handler hdl, client_t::message_ptr msg) {
      handler(hdl, msg->get_raw_payload());
    });

    m_client.set_close_handler([=](connection_handler hdl) {
      {
        scoped_lock guard(m_lock);
        m_open = false;
      }
      if (onClose)
        onClose();
    });

    m_client.set_fail_handler([=](connection_handler hdl) {
      {
        scoped_lock guard(m_lock);
        m_open = false;
      }
      if (onFail)
        onFail();
    });
  }

  ~websocket_client()
  {
    if (m_open)
      stop();
  }

  bool connected() const
  {
    return m_open;
  }

  void stop()
  {
    if (!connected())
      return;

    scoped_lock guard(m_lock);
    m_client.close(m_hdl, websocketpp::close::status::normal, "");
    m_open = false;
  }

  void close()
  {
    stop();
  }

  auto& client()
  {
    return m_client;
  }
  auto& handle()
  {
    return m_hdl;
  }
  auto after_connect()
  {
    return m_connected;
  }

  // This function returns if the connection is stopped / fails.
  void connect(const std::string& uri)
  {
    websocketpp::lib::error_code ec;

    auto con = m_client.get_connection(uri, ec);
    if (ec)
    {
      m_client.get_alog().write(
          websocketpp::log::alevel::app,
          "Get Connection Error: " + ec.message());
      return;
    }

    m_hdl = con->get_handle();
    m_client.connect(con);
    m_connected = true;

    m_client.run();

    m_connected = false;
    m_client.reset(); // In order to be able to reconnect afterwards.
  }

  void send_message(const std::string& request)
  {
    if (!m_open)
      return;

    websocketpp::lib::error_code ec;

    m_client.send(m_hdl, request, websocketpp::frame::opcode::text, ec);

    if (ec)
    {
      m_client.get_alog().write(
          websocketpp::log::alevel::app, "Send Error: " + ec.message());
    }
  }

  void send_message(const rapidjson::StringBuffer& request)
  {
    if (!m_open)
      return;

    websocketpp::lib::error_code ec;

    m_client.send(
        m_hdl, request.GetString(), request.GetSize(),
        websocketpp::frame::opcode::text, ec);

    if (ec)
    {
      m_client.get_alog().write(
          websocketpp::log::alevel::app, "Send Error: " + ec.message());
    }
  }

private:
  using client_t = websocketpp::client<websocketpp::config::asio_client>;
  using scoped_lock = websocketpp::lib::lock_guard<websocketpp::lib::mutex>;

  client_t m_client;
  connection_handler m_hdl;
  websocketpp::lib::mutex m_lock;
  bool m_open;
  bool m_connected{false};
};
}
}
