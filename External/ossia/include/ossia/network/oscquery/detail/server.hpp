#pragma once
#include <ossia/detail/config.hpp>
#include <ossia/detail/json.hpp>
#include <ossia/detail/logger.hpp>
#include <ossia/network/exceptions.hpp>
#include <boost/config.hpp>
#include <boost/version.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/http/request.hpp>
#include <websocketpp/server.hpp>
#if defined(OSSIA_BENCHMARK)
#include <chrono>
#endif
namespace ossia
{
namespace oscquery
{

//! Low-level websocket & http server for oscquery
class websocket_server
{
public:
  using server_t = websocketpp::server<websocketpp::config::asio>;
  using connection_handler = websocketpp::connection_hdl;

  websocket_server()
  {
    m_server.init_asio();
    m_server.set_reuse_addr(true);
    m_server.clear_access_channels(websocketpp::log::alevel::all);
    m_server.set_socket_init_handler(
        [](websocketpp::connection_hdl, asio::ip::tcp::socket& s) {
          asio::ip::tcp::no_delay option(true);
          s.set_option(option);
        });
  }

  template <typename Handler>
  void set_open_handler(Handler h)
  {
    m_server.set_open_handler(h);
  }

  template <typename Handler>
  void set_close_handler(Handler h)
  {
    m_server.set_close_handler(h);
  }

  template <typename Handler>
  void set_message_handler(Handler h)
  {
    m_server.set_message_handler(
        [=](connection_handler hdl, server_t::message_ptr msg) {
#if defined OSSIA_BENCHMARK
          auto t1 = std::chrono::high_resolution_clock::now();
#endif
          try
          {
            rapidjson::StringBuffer res = h(hdl, msg->get_raw_payload());
            if (res.GetSize() > 0)
            {
              send_message(hdl, std::move(res));
            }
          }
          catch (const ossia::node_not_found_error& e)
          {
            auto con = m_server.get_con_from_hdl(hdl);
            ossia::logger().error(
                "Node not found: {} ==> {}", con->get_uri()->get_resource(),
                e.what());
          }
          catch (const ossia::bad_request_error& e)
          {
            auto con = m_server.get_con_from_hdl(hdl);
            ossia::logger().error(
                "Error in request: {} ==> {}", con->get_uri()->get_resource(),
                e.what());
          }
          catch (const std::exception& e)
          {
            auto con = m_server.get_con_from_hdl(hdl);
            ossia::logger().error("Error in request: {}", e.what());
          }
          catch (...)
          {
            auto con = m_server.get_con_from_hdl(hdl);
            ossia::logger().error("Error in request");
          }

#if defined OSSIA_BENCHMARK
          auto t2 = std::chrono::high_resolution_clock::now();
          ossia::logger().info(
              "Time taken: {}",
              std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1)
                  .count());
#endif
        });

    m_server.set_http_handler([=](connection_handler hdl) {
      auto con = m_server.get_con_from_hdl(hdl);

      con->replace_header("Content-Type", "application/json; charset=utf-8");
      con->replace_header("Connection", "close");
      try
      {
        rapidjson::StringBuffer base_str
            = h(hdl, con->get_uri()->get_resource());
        std::string str{base_str.GetString(), base_str.GetSize()};
        str += "\0";
        con->set_body(std::move(str));
        con->set_status(websocketpp::http::status_code::ok);
      }
      catch (const ossia::node_not_found_error& e)
      {
        con->set_status(websocketpp::http::status_code::not_found);
      }
      catch (const ossia::bad_request_error& e)
      {
        ossia::logger().error(
            "Error in request: {} ==> {}", con->get_uri()->get_resource(),
            e.what());
        con->set_status(websocketpp::http::status_code::bad_request);
      }
      catch (const std::exception& e)
      {
        ossia::logger().error("Error in request: {}", e.what());
      }
      catch (...)
      {
        ossia::logger().error("Error in request");
      }
    });
  }

  void listen(uint16_t port = 9002)
  {
    m_server.listen(asio::ip::tcp::v4(), port);
    m_server.start_accept();
  }

  void run()
  {
    m_server.run();
  }

  void stop()
  {
      // (temporarily?) changed to stop_listening()
      // "Straight up stop forcibly stops a bunch of things
      // in a way that bypasses most, if not all, of the cleanup routines"

      //m_server.stop();
      m_server.stop_listening();
  }

  void close(connection_handler hdl)
  {
    auto con = m_server.get_con_from_hdl(hdl);
    con->close(websocketpp::close::status::going_away, "Server shutdown");
  }

  void send_message(connection_handler hdl, const std::string& message)
  {
    auto con = m_server.get_con_from_hdl(hdl);
    con->send(message);
  }

  void
  send_message(connection_handler hdl, const rapidjson::StringBuffer& message)
  {
    auto con = m_server.get_con_from_hdl(hdl);
    con->send(
        message.GetString(), message.GetSize(),
        websocketpp::frame::opcode::text);
  }

  server_t& impl()
  {
    return m_server;
  }

private:
  server_t m_server;
};
}
}
