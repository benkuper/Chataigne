#pragma once
#include <ossia/detail/json.hpp>
#include <ossia/network/oscquery/detail/client.hpp>
#include <atomic>
#include <eggs/variant.hpp>
#include <readerwriterqueue.h>
#include <spdlog/spdlog.h>

namespace ossia
{
struct websocket_threaded_connection
{
  websocket_threaded_connection(const std::string& ip)
      : socket([](auto&&...) {})
  {
    running = true;
    thread = std::thread([=] {
      auto log = spdlog::get("websocket");
      if(!log)
        log = spdlog::stderr_logger_mt("websocket");
      try
      {
        while (running)
        {
          socket.connect(ip);
          if (running)
          {
            // Try to reconnect
            log->critical("Logger could not connect to {}.", ip);
            std::this_thread::sleep_for(std::chrono::seconds(1));
          }
        }
        log->critical("Logger stopping.");
      }
      catch (const websocketpp::exception& e)
      {
        log->critical("Logger error: ", e.what());
      }
      catch (const std::exception& e)
      {
        log->critical("Logger error: ", e.what());
      }
      catch (...)
      {
        log->critical("Logger error");
      }
    });
  }

  ~websocket_threaded_connection()
  {
    running = false;
    if (!socket.after_connect())
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    socket.stop();
    if (thread.joinable())
      thread.join();
  }

  ossia::oscquery::websocket_client socket;
  std::atomic_bool running{};
  std::thread thread;
};

//! A sink to use with spdlog, that will send its log messages over websockets.
struct websocket_log_sink final : public spdlog::sinks::sink,
                                  public Nano::Observer
{
  websocket_log_sink(
      std::shared_ptr<websocket_threaded_connection> s, std::string send)
      : socket{std::move(s)}, sender{std::move(send)}
  {
    socket->socket.onOpen
        .connect<websocket_log_sink, &websocket_log_sink::open_fun>(this);
  }

  void open_fun()
  {
    std::string m;
    while (logs.try_dequeue(m))
    {
      socket->socket.send_message(m);
    }
  }

  ~websocket_log_sink()
  {
    socket->socket.onOpen
        .disconnect<websocket_log_sink, &websocket_log_sink::open_fun>(this);
  }

  void make_message(const spdlog::details::log_msg& msg)
  {
    buffer.Clear();

    rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};

    writer.StartObject();

    writer.Key("operation");
    writer.String("log");

    writer.Key("level");
    switch (msg.level)
    {
      case spdlog::level::trace:
        writer.String("trace");
        break;
      case spdlog::level::debug:
        writer.String("debug");
        break;
      case spdlog::level::info:
        writer.String("info");
        break;
      case spdlog::level::warn:
        writer.String("warn");
        break;
      case spdlog::level::err:
        writer.String("error");
        break;
      case spdlog::level::critical:
        writer.String("critical");
        break;
      case spdlog::level::off:
        writer.String("off");
        break;
    }

    writer.Key("sender");
    writer.String(sender.data(), sender.size());

    writer.Key("message");
    writer.String(msg.raw.data(), msg.raw.size());

    writer.EndObject();
  }

  void send_message(const spdlog::details::log_msg& msg)
  {
    make_message(msg);
    socket->socket.send_message(buffer);
  }

  void log(const spdlog::details::log_msg& msg) override
  {
    if (!socket->socket.connected())
    {
      make_message(msg);
      logs.enqueue(std::string{buffer.GetString(), buffer.GetSize()});
      return;
    }
    else
    {
      send_message(msg);
    }
  }

  void flush() override
  {
  }

private:
  rapidjson::StringBuffer buffer;
  std::shared_ptr<websocket_threaded_connection> socket;
  std::string sender;

  moodycamel::ReaderWriterQueue<std::string> logs;
};

//! Sends websocket "alive" messages at regular intervals.
struct websocket_heartbeat : public Nano::Observer
{
public:
  websocket_heartbeat(
      std::shared_ptr<websocket_threaded_connection> t, std::string s,
      std::chrono::seconds dur)
      : interval{dur}, sender{s}, conn{t}
  {
    thread = std::thread([=] {
      while (running)
      {
        if (init && t->socket.connected())
        {
          buffer.Clear();
          rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
          writer.StartObject();

          writer.Key("operation");
          writer.String("alive");

          writer.Key("sender");
          writer.String(sender.data(), sender.size());

          writer.EndObject();

          t->socket.send_message(buffer);
        }

        for (int i = 0; i < 100; i++)
        {
          std::this_thread::sleep_for(
              std::chrono::duration_cast<std::chrono::milliseconds>(dur)
              / 100.0);
          if (!running)
            return;
        }
      }
    });

    t->socket.onOpen
        .connect<websocket_heartbeat, &websocket_heartbeat::open_fun>(*this);
  }

  void open_fun()
  {
    conn->socket.send_message(init_msg);
    init = true;
  }

  ~websocket_heartbeat()
  {
    conn->socket.onOpen
        .disconnect<websocket_heartbeat, &websocket_heartbeat::open_fun>(
            *this);
    running = false;
    if (thread.joinable())
      thread.join();
  }

  void
  send_init(const std::map<std::string, eggs::variant<std::string, int>>& map)
  {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
    writer.StartObject();

    writer.Key("operation");
    writer.String("initWatchdog");

    writer.Key("sender");
    writer.String(sender.data(), sender.size());

    writer.Key("aliveTime");
    writer.Int(interval.count());

    struct
    {
      rapidjson::Writer<rapidjson::StringBuffer>& writer;
      void operator()(const std::string& s)
      {
        writer.String(s);
      }

      void operator()(int s)
      {
        writer.Int(s);
      }
    } sw{writer};

    for (const auto& pair : map)
    {
      writer.Key(pair.first);
      if (pair.second)
        eggs::variants::apply(sw, pair.second);
    }

    writer.EndObject();

    if (conn->socket.connected())
    {
      conn->socket.send_message(buffer);
      init = true;
    }
    else
    {
      init_msg = std::string(buffer.GetString(), buffer.GetSize());
    }
  }

private:
  rapidjson::StringBuffer buffer;
  std::thread thread;
  std::chrono::seconds interval;
  std::string sender;
  std::string init_msg;
  std::shared_ptr<websocket_threaded_connection> conn;
  std::atomic_bool running{true};
  std::atomic_bool init{false};
};
}
