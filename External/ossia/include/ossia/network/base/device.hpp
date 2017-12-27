#pragma once
#include <ossia/network/base/node.hpp>
#include <ossia/network/base/node_attributes.hpp>
#include <nano_signal_slot.hpp>
#include <ossia_export.h>

namespace ossia
{
namespace net
{
struct parameter_data;
class protocol_base;

/**
 * @brief What a device is able to do
 */
struct device_capabilities
{
  /**
   * @brief change_tree : nodes can be added and removed externally.
   */
  bool change_tree = false;
};

/**
 * @brief Root of a device tree
 *
 * A device models a real hardware or software
 * with which communication should be established.
 *
 * It handles transformations of the node tree and allows
 * to add callbacks to be called :
 * - after a node has been created : device_base::on_node_created
 * - after a node has been renamed : device_base::on_node_renamed
 * - before a node is removed : device_base::on_node_removing
 *
 * - after a parameter has been created : device_base::on_parameter_created
 * - before a parameter is being removed : device_base::on_parameter_removing
 *
 * The root node of a device maps to the "/" address.
 *
 * A device is necessarily constructed with a protocol.
 * If the device is meant to mirror a remote application, it should
 * be constructed directly with the remote protocol
 * (e.g. \ref minuit_protocol, \ref osc_protocol).
 *
 * if the device is meant to expose the current application to the network,
 * it should be constructed with the \ref local_protocol.
 *
 * \see generic_device
 * \see node_base
 * \see protocol_base
 */
class OSSIA_EXPORT device_base
{
public:
  device_base(std::unique_ptr<ossia::net::protocol_base> proto);

  device_base() = delete;
  device_base(const device_base&) = delete;
  device_base(device_base&&) = delete;
  device_base& operator=(const device_base&) = delete;
  device_base& operator=(device_base&&) = delete;

  ossia::net::protocol_base& get_protocol() const;

  virtual ~device_base();

  virtual const ossia::net::node_base& get_root_node() const = 0;
  virtual ossia::net::node_base& get_root_node() = 0;

  device_capabilities get_capabilities() const
  {
    return m_capabilities;
  }

  void set_name(const std::string& str)
  {
    get_root_node().set_name(str);
  }
  std::string get_name() const
  {
    return get_root_node().get_name();
  }

  Nano::Signal<void(const node_base&)>
      on_node_created; // The node being created
  Nano::Signal<void(const node_base&)>
      on_node_removing; // The node being removed
  Nano::Signal<void(const node_base&, std::string)>
      on_node_renamed; // Node has the new name, second argument is the old
                       // name
  Nano::Signal<void(const node_base&, ossia::string_view)>
      on_attribute_modified; // Second argument is an identifier
  Nano::Signal<void(const parameter_base&)>
      on_parameter_created; // The parameter being created
  Nano::Signal<void(const parameter_base&)>
      on_parameter_removing; // The node whose parameter was removed
  Nano::Signal<void(const parameter_base&)>
      on_message; // A received value
  Nano::Signal<void(ossia::string_view, const ossia::value& val)>
      on_unhandled_message; // A received value on a non-existing address

  //! Called when a network client requests the creation of an instance.
  //!  First argument is the path to the parent.
  Nano::Signal<void(std::string, const parameter_data&)> on_add_node_requested;

  //! Called when a network client requests the removal of an instance.
  //! Argument is the path of the parent and the node to remove.
  Nano::Signal<void(std::string, std::string)> on_remove_node_requested;

protected:
  std::unique_ptr<ossia::net::protocol_base> m_protocol;
  device_capabilities m_capabilities;
};

template <typename T>
void node_base::set(ossia::string_view str, const T& value)
{
  static_assert(!is_parameter_attribute<T>::value, "No parameter");
  auto opt = ossia::get_optional_attribute<T>(*this, str);
  if ((opt && *opt != value) || !opt)
  {
    ossia::set_attribute((extended_attributes&)*this, str, value);
    get_device().on_attribute_modified(*this, str);
  }
}

template <typename T>
void node_base::set(ossia::string_view str, T&& value)
{
  static_assert(!is_parameter_attribute<T>::value, "No parameter");
  auto opt = ossia::get_optional_attribute<T>(*this, str);
  if ((opt && *opt != value) || !opt)
  {
    ossia::set_attribute((extended_attributes&)*this, str, std::move(value));
    get_device().on_attribute_modified(*this, str);
  }
}

template <typename T>
void node_base::set(ossia::string_view str, const optional<T>& value)
{
  static_assert(!is_parameter_attribute<T>::value, "No parameter");
  auto opt = ossia::get_optional_attribute<T>(*this, str);
  if (opt != value)
  {
    ossia::set_optional_attribute((extended_attributes&)*this, str, value);
    get_device().on_attribute_modified(*this, str);
  }
}

template <typename T>
void node_base::set(ossia::string_view str, optional<T>&& value)
{
  static_assert(!is_parameter_attribute<T>::value, "No parameter");
  auto opt = ossia::get_optional_attribute<T>(*this, str);
  if (opt != value)
  {
    ossia::set_optional_attribute((extended_attributes&)*this, str, std::move(value));
    get_device().on_attribute_modified(*this, str);
  }
}

template <typename Attribute, typename T>
void node_base::set(Attribute a, const T& value)
{
  // We make a copy here to prevent a double conversion
  // for instance from std::vector<> to value. TODO do the same in the other.
  typename Attribute::type val = value;
  a.setter(*this, std::move(val));
}

template <typename Attribute, typename T>
void node_base::set(Attribute a, T& value)
{
  set(a, const_cast<const T&>(value));
}

template <typename Attribute, typename T>
void node_base::set(Attribute a, T&& value)
{
  typename Attribute::type val = std::move(value);
  a.setter(*this, std::move(val));
}

}
}
