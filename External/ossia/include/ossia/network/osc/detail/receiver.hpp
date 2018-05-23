#pragma once
#include <ossia/detail/logger.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <oscpack/ip/UdpSocket.h>
#include <oscpack/osc/OscDebug.h>
#include <oscpack/osc/OscPacketListener.h>
#include <sstream>
#include <thread>

namespace oscpack
{

namespace detail
{

template <typename Impl_T>
struct ClearListener : public oscpack::TimerListener
{
  ClearListener(UdpSocket<Impl_T>& s) : socket{s}
  {
  }
  UdpSocket<Impl_T>& socket;

  void TimerExpired() override
  {
    socket.AsynchronousBreak();
  }
};

template <typename Impl_T>
class ReceiveSocket : public UdpSocket<Impl_T>
{
  SocketReceiveMultiplexer<Impl_T> mux_;
  PacketListener* listener_;

public:
  ReceiveSocket(const IpEndpointName& localEndpoint, PacketListener* listener)
      : listener_(listener)
  {
    this->Bind(localEndpoint);
    mux_.AttachSocketListener(&this->impl_, listener_);
  }

  ~ReceiveSocket()
  {
    mux_.DetachSocketListener(&this->impl_, listener_);
  }

  // see SocketReceiveMultiplexer above for the behaviour of these methods...
  void Run()
  {
    mux_.Run();
  }
  void Break()
  {
    ClearListener<Impl_T> l{*this};
    mux_.AttachPeriodicTimerListener(0, &l);
    mux_.Break();
  }
  void AsynchronousBreak()
  {
    ClearListener<Impl_T> l{*this};
    mux_.AttachPeriodicTimerListener(0, &l);
    mux_.AsynchronousBreak();
  }
};
}
using ReceiveSocket
    = detail::UdpListeningReceiveSocket<detail::Implementation>;
}
namespace osc
{

template <typename MessageHandler>
/**
 * @brief The listener class
 *
 * Listens to OSC messages and handles them.
 */
class listener final : public oscpack::OscPacketListener
{
public:
  listener(MessageHandler msg) : m_messageHandler{msg}
  {
  }

protected:
  void ProcessMessage(
      const oscpack::ReceivedMessage& m,
      const oscpack::IpEndpointName& ip) override
  {
    try
    {
      m_messageHandler(m, ip);
    }
    catch (std::exception& e)
    {
      std::stringstream s;
      oscpack::debug(s, m);

      ossia::logger().error(
          "osc::listener::ProcessMessage error: '{}': {}", s.str(), e.what());
    }
    catch (...)
    {
      std::stringstream s;
      oscpack::debug(s, m);
      ossia::logger().error(
          "osc::listener::ProcessMessage error: '{}': {}", s.str());
    }
  }

  void ProcessPacket(
      const char* data, int size,
      const oscpack::IpEndpointName& remoteEndpoint) override
  {
    try
    {
      oscpack::ReceivedPacket p(data, size);
      if (p.IsBundle())
        this->ProcessBundle(oscpack::ReceivedBundle(p), remoteEndpoint);
      else
        this->ProcessMessage(oscpack::ReceivedMessage(p), remoteEndpoint);
    }
    catch (std::exception& e)
    {
      ossia::logger().error(
          "osc::listener::ProcessPacket error: {}", e.what());
    }
    catch (...)
    {
      ossia::logger().error("osc::listener::ProcessPacket error");
    }
  }

private:
  MessageHandler m_messageHandler;
};

/**
 * @brief The receiver class
 *
 * A OSC server.
 * Note : if a port cannot be opened, it will be incremented.
 */
class receiver
{
public:
  template <typename Handler>
  receiver(unsigned int port, Handler msg)
      : m_impl{std::make_unique<listener<Handler>>(msg)}
  {
    setPort(port);
  }

  receiver() = default;
  receiver(receiver&& other)
  {
    other.stop();
    m_impl = std::move(other.m_impl);
    m_socket = std::move(other.m_socket);
    setPort(other.m_port);
  }

  receiver& operator=(receiver&& other)
  {
    stop();

    m_impl = std::move(other.m_impl);
    m_socket = std::move(other.m_socket);

    setPort(other.m_port);

    return *this;
  }

  ~receiver()
  {
    stop();
  }

  void run()
  {
    if (m_runThread.joinable())
      stop();

    m_runThread = std::thread([this]() { m_socket->Run(); });
  }

  void stop()
  {
    if (m_socket)
    {
      if (m_runThread.joinable())
      {
        {
          oscpack::UdpTransmitSocket send_socket(
              oscpack::IpEndpointName("127.0.0.1", port()));
          send_socket.Send("__stop_", 8);
        }

        m_socket->AsynchronousBreak();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        m_runThread.join();
      }

      m_socket.reset();
    }
  }

  unsigned int port() const
  {
    return m_port;
  }

  unsigned int setPort(unsigned int port)
  {
    m_port = port;

    bool ok = false;
    while (!ok)
    {
      try
      {
        m_socket = std::make_unique<oscpack::ReceiveSocket>(
            oscpack::IpEndpointName(
                oscpack::IpEndpointName::ANY_ADDRESS, m_port),
            m_impl.get());
        ok = true;
      }
      catch (std::runtime_error& e)
      {
        m_port++;
      }
    }

    return m_port;
  }

private:
  unsigned int m_port = 0;
  std::unique_ptr<oscpack::OscPacketListener> m_impl;
  std::unique_ptr<oscpack::ReceiveSocket> m_socket;

  std::thread m_runThread;
};
}
