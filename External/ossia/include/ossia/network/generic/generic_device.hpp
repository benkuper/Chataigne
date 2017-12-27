#pragma once

#include <ossia/detail/string_view.hpp>
#include <ossia/network/base/device.hpp>
#include <ossia/network/generic/generic_node.hpp>
namespace ossia
{
namespace net
{
class OSSIA_EXPORT generic_device final : public ossia::net::device_base,
                                          public generic_node
{
public:
  generic_device();
  generic_device(const generic_device&) = delete;
  generic_device(generic_device&&) = delete;
  generic_device& operator=(const generic_device&) = delete;
  generic_device& operator=(generic_device&&) = delete;

  generic_device(
      std::unique_ptr<ossia::net::protocol_base> protocol_base,
      std::string name);

  const ossia::net::node_base& get_root_node() const override
  {
    return *this;
  }
  ossia::net::node_base& get_root_node() override
  {
    return *this;
  }

  using generic_node::get_name;
  using generic_node::set_name;

  ~generic_device();
};
}
}
