#pragma once
#include <ossia/detail/config.hpp>
#include <ossia/detail/logger.hpp>
#include <asio.hpp>
#include <asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <utility>

namespace ossia
{
namespace oscquery
{
using tcp = asio::ip::tcp;
class http_get_request
{
  fmt::MemoryWriter m_request;

public:
  http_get_request(
      std::function<void(http_get_request*, const std::string&)> f,
      std::function<void(http_get_request*)> err, asio::io_context& ctx,
      const std::string& server, const std::string& port,
      const std::string& path)
      : m_resolver(ctx)
      , m_socket(ctx)
      , m_fun{std::move(f)}
      , m_err{std::move(err)}
  {
    m_request << "GET " << path << " HTTP/1.1\r\n";
    m_request << "Host: " << server << "\r\n";
    m_request << "Accept: */*\r\n";
    m_request << "Connection: close\r\n\r\n";

    m_resolver.async_resolve(
        server, port, std::bind(
                          &http_get_request::handle_resolve, this,
                          std::placeholders::_1, std::placeholders::_2));
  }

  void close()
  {
    m_socket.close();
  }

private:
  void handle_resolve(
      const asio::error_code& err,
      const tcp::resolver::results_type& endpoints)
  {
    if (!err)
    {
      // Attempt a connection to each endpoint in the list until we
      // successfully establish a connection.
      asio::async_connect(
          m_socket, endpoints,
          std::bind(
              &http_get_request::handle_connect, this, std::placeholders::_1));
    }
    else
    {
      ossia::logger().error("HTTP Error: {}", err.message());
      m_err(this);
    }
  }

  void handle_connect(const asio::error_code& err)
  {
    if (!err)
    {
      asio::const_buffer request(m_request.data(), m_request.size());
      // The connection was successful. Send the request.
      asio::async_write(
          m_socket, request, std::bind(
                                 &http_get_request::handle_write_request, this,
                                 std::placeholders::_1));
    }
    else
    {
      ossia::logger().error("HTTP Error: {}", err.message());
      m_err(this);
    }
  }

  void handle_write_request(const asio::error_code& err)
  {
    if (!err)
    {
      // Read the response status line. The response_ streambuf will
      // automatically grow to accommodate the entire line. The growth may be
      // limited by passing a maximum size to the streambuf constructor.
      asio::async_read_until(
          m_socket, m_response, "\r\n",
          std::bind(
              &http_get_request::handle_read_status_line, this,
              std::placeholders::_1));
    }
    else
    {
      ossia::logger().error("HTTP Error: {}", err.message());
      m_err(this);
    }
  }

  void handle_read_status_line(const asio::error_code& err)
  {
    if (!err)
    {
      // Check that response is OK.
      std::istream response_stream(&m_response);
      std::string http_version;
      response_stream >> http_version;
      unsigned int status_code;
      response_stream >> status_code;
      std::string status_message;
      std::getline(response_stream, status_message);
      if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      {
        ossia::logger().error("HTTP Error: Invalid response");
        return;
      }
      if (status_code != 200)
      {
        ossia::logger().error("HTTP Error: status code {}", status_code);
        return;
      }

      // Read the response headers, which are terminated by a blank line.
      asio::async_read_until(
          m_socket, m_response, "\r\n\r\n",
          std::bind(
              &http_get_request::handle_read_headers, this,
              std::placeholders::_1));
    }
    else
    {
      ossia::logger().error("HTTP Error: {}", err.message());
      m_err(this);
    }
  }

  void handle_read_headers(const asio::error_code& err)
  {
    if (!err)
    {
      // Process the response headers.
      std::istream response_stream(&m_response);
      std::string header;
      while (std::getline(response_stream, header) && header != "\r")
        ;

      // Start reading remaining data until EOF.
      asio::async_read(
          m_socket, m_response, asio::transfer_at_least(1),
          std::bind(
              &http_get_request::handle_read_content, this,
              std::placeholders::_1));
    }
    else
    {
      ossia::logger().error("HTTP Error: {}", err.message());
      m_err(this);
    }
  }

  void handle_read_content(const asio::error_code& err)
  {
    if (!err)
    {
      m_stream << &m_response;
      std::string s = m_stream.str();
      m_fun(this, s);

      // Continue reading remaining data until EOF.
      asio::async_read(
          m_socket, m_response, asio::transfer_at_least(1),
          std::bind(
              &http_get_request::handle_read_content, this,
              std::placeholders::_1));
    }
    else if (err != asio::error::eof)
    {
      ossia::logger().error("HTTP Error: {}", err.message());
      m_err(this);
    }
    else if (err == asio::error::eof)
    {
      // Write all of the data that has been read so far.
      const auto& dat = m_response.data();
      std::string str{asio::buffers_begin(dat), asio::buffers_end(dat)};
      m_fun(this, str);
    }
  }

  tcp::resolver m_resolver;
  tcp::socket m_socket;
  asio::streambuf m_response;
  std::stringstream m_stream;
  std::function<void(http_get_request*, const std::string&)> m_fun;
  std::function<void(http_get_request*)> m_err;
};
}
}
