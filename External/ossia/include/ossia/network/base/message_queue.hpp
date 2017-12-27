#pragma once
#include <ossia/network/base/parameter.hpp>
#include <ossia/network/base/device.hpp>
#include <readerwriterqueue.h>

namespace ossia
{
struct received_value
{
    ossia::net::parameter_base* address{};
    ossia::value value;
};

class message_queue final : public Nano::Observer
{
  public:
    message_queue(ossia::net::device_base& dev)
    {
      dev.on_parameter_removing
          .connect<message_queue, &message_queue::on_param_removed>(*this);
    }

    ~message_queue()
    {
      for(auto reg : m_reg)
      {
        reg.first->remove_callback(reg.second);
      }
    }

    bool try_dequeue(ossia::received_value& v)
    {
      return m_queue.try_dequeue(v);
    }

    void reg(ossia::net::parameter_base& p)
    {
      auto ptr = &p;
      auto it = p.add_callback([=] (const ossia::value& val) {
        m_queue.enqueue({ptr, val});
      });
      m_reg.insert({&p, it});
    }

    void unreg(ossia::net::parameter_base& p)
    {
      auto it = m_reg.find(&p);
      if(it != m_reg.end())
      {
        p.remove_callback(it->second);
        m_reg.erase(it);
      }
    }

  private:
    void on_param_removed(const ossia::net::parameter_base& p)
    {
      auto it = m_reg.find(const_cast<ossia::net::parameter_base*>(&p));
      if(it != m_reg.end())
        m_reg.erase(it);
    }

    moodycamel::ReaderWriterQueue<received_value> m_queue;

    std::unordered_map<
      ossia::net::parameter_base*,
      ossia::net::parameter_base::callback_index> m_reg;
};

class global_message_queue final : public Nano::Observer
{
  public:
    global_message_queue(ossia::net::device_base& dev)
    {
      dev.on_message
         .connect<global_message_queue, &global_message_queue::on_message>(
            *this);
    }

    void on_message(const ossia::net::parameter_base& p)
    {
      m_queue.enqueue({const_cast<ossia::net::parameter_base*>(&p), p.value()});
    }

    bool try_dequeue(ossia::received_value& v)
    {
      return m_queue.try_dequeue(v);
    }

  private:
    moodycamel::ReaderWriterQueue<received_value> m_queue;
};

}
