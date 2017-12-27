#pragma once

#include <ossia/network/base/device.hpp>
#include <ossia/network/midi/midi_node.hpp>

namespace ossia
{
namespace net
{
namespace midi
{
class OSSIA_EXPORT midi_device final : public ossia::net::device_base,
                                       public midi_node
{
public:
  midi_device(std::unique_ptr<ossia::net::protocol_base> prot);

  node_base& set_name(std::string n) override;

  bool update_namespace();

  const ossia::net::node_base& get_root_node() const override
  {
    return *this;
  }
  ossia::net::node_base& get_root_node() override
  {
    return *this;
  }
};
}
}
}
