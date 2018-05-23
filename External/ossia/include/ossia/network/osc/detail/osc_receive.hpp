#pragma once
#include <oscpack/osc/OscReceivedElements.h>
#include <ossia/network/base/device.hpp>
#include <ossia/network/base/parameter.hpp>
#include <ossia/network/base/listening.hpp>
#include <ossia/network/osc/detail/osc.hpp>

namespace ossia
{
namespace net
{
template<bool SilentUpdate>
inline void handle_osc_message(
    const oscpack::ReceivedMessage& m,
    const ossia::net::listened_parameters& listening,
    ossia::net::device_base& dev)
{
  auto addr_txt = m.AddressPattern();
  auto addr = listening.find(addr_txt);

  if (addr && *addr)
  {
    auto& the_addr = **addr;
    if(net::update_value(the_addr, m))
      dev.on_message( the_addr);
  }
  else
  {
    // We still want to save the value even if it is not listened to.
    if (auto n = find_node(dev.get_root_node(), addr_txt))
    {
      if (auto base_addr = n->get_parameter())
      {
        if(SilentUpdate)
        {
          if(update_value_quiet(*base_addr, m))
            dev.on_message(*base_addr);
        }
        else
        {
          if(update_value(*base_addr, m))
            dev.on_message(*base_addr);
        }
      }
    }
    else
    {
      // Try to handle pattern matching
      auto nodes = find_nodes(dev.get_root_node(), addr_txt);
      for(auto n : nodes)
      {
        if (auto addr = n->get_parameter())
        {
          if(!SilentUpdate || listening.find(net::osc_parameter_string(*n)))
          {
            if(net::update_value(*addr, m))
              dev.on_message(*addr);
          }
          else
          {
            if(net::update_value_quiet(*addr, m))
              dev.on_message(*addr);
          }
        }
      }

      if(nodes.empty())
      {
        dev.on_unhandled_message(
              addr_txt,
              net::osc_utilities::create_any(
                m.ArgumentsBegin(),
                m.ArgumentCount()));
      }
    }
  }
}

}
}
