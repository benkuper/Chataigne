#pragma once
#include <ossia/network/dataspace/dataspace_visitors.hpp>
#include <ossia/network/base/node_attributes.hpp>
#include <ossia/network/minuit/detail/minuit_common.hpp>
#include <ossia/network/minuit/minuit.hpp>
#include <ossia/network/osc/detail/osc.hpp>
#include <ossia/network/osc/detail/sender.hpp>
#include <chobo/small_vector.hpp>
#include <oscpack/osc/OscPrintReceivedElements.h>
#include <oscpack/osc/OscReceivedElements.h>

namespace ossia
{
namespace minuit
{
template <minuit_command Req, minuit_operation Op>
struct minuit_behavior
{
  void operator()(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      const oscpack::ReceivedMessage& mess);
};

template <minuit_operation Op>
struct minuit_behavior<minuit_command::Error, Op>
{
  void operator()(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      const oscpack::ReceivedMessage& mess)
  {
  }
};

template <>
struct minuit_behavior<minuit_command::Error, minuit_operation::Get>
{
  void operator()(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      const oscpack::ReceivedMessage& mess)
  {
    // Do nothing
    if (mess.ArgumentCount() > 0)
    {
      auto beg = mess.ArgumentsBegin();
      if (beg->IsString())
      {
        proto.get_refreshed(beg->AsStringUnchecked());
      }
    }
  }
};

// Get
template <>
struct minuit_behavior<minuit_command::Request, minuit_operation::Get>
{
  auto operator()(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      const oscpack::ReceivedMessage& mess)
  {
    ossia::string_view full_address{mess.ArgumentsBegin()->AsString()};
    auto idx = full_address.find_first_of(":");

    if (idx == std::string::npos)
    {
      // Value
      auto node = ossia::net::find_node(dev.get_root_node(), full_address);
      if (!node)
        return;
      auto addr = node->get_parameter();
      if (!addr)
        return;

      proto.sender().send(
          proto.name_table.get_action(minuit_action::GetReply), full_address,
          addr->value());
    }
    else
    {
      ossia::string_view address{full_address.data(), idx};

      // Note : bug if address == "foo:"
      auto attr = get_attribute(ossia::string_view(
          address.data() + idx + 1, full_address.size() - idx - 1));

      auto node = ossia::net::find_node(dev.get_root_node(), address);
      if (!node)
        return;
      auto addr = node->get_parameter();
      if (!addr)
        return;

      switch (attr)
      {
        case minuit_attribute::Value:
          proto.sender().send(
              proto.name_table.get_action(minuit_action::GetReply),
              full_address, addr->value());
          break;
        case minuit_attribute::Type:
          proto.sender().send(
              proto.name_table.get_action(minuit_action::GetReply),
              full_address, to_minuit_type_text(addr->get_value_type()));
          break;
        case minuit_attribute::RangeBounds:
          proto.sender().send(
              proto.name_table.get_action(minuit_action::GetReply),
              full_address, addr->get_domain());
          break;
        case minuit_attribute::RangeClipMode:
          proto.sender().send(
              proto.name_table.get_action(minuit_action::GetReply),
              full_address, to_minuit_bounding_text(addr->get_bounding()));
          break;
        case minuit_attribute::Dataspace:
          proto.sender().send(
              proto.name_table.get_action(minuit_action::GetReply),
              full_address, ossia::get_dataspace_text(addr->get_unit()));
          break;
        case minuit_attribute::DataspaceUnit:
          proto.sender().send(
              proto.name_table.get_action(minuit_action::GetReply),
              full_address, ossia::get_unit_text(addr->get_unit()));
          break;
        case minuit_attribute::RepetitionFilter:
          proto.sender().send(
              proto.name_table.get_action(minuit_action::GetReply),
              full_address, (int32_t)addr->get_repetition_filter());
          break;
        case minuit_attribute::Service:
          proto.sender().send(
              proto.name_table.get_action(minuit_action::GetReply),
              full_address, to_minuit_service_text(addr->get_access()));
          break;
        case minuit_attribute::Description:
          if (const auto& desc = ossia::net::get_description(*node))
            proto.sender().send(
                proto.name_table.get_action(minuit_action::GetReply),
                full_address, *desc);
          break;
        case minuit_attribute::Priority:
          if (const auto& priority = ossia::net::get_priority(*node))
            proto.sender().send(
                proto.name_table.get_action(minuit_action::GetReply),
                full_address, *priority);
          else
            proto.sender().send(
                proto.name_table.get_action(minuit_action::GetReply),
                full_address, 0);
          break;
        case minuit_attribute::ValueStepSize:
          if (const auto& ss = ossia::net::get_value_step_size(*node))
            proto.sender().send(
                proto.name_table.get_action(minuit_action::GetReply),
                full_address, *ss);
          break;
        // TODO some are missing :refresh rate, tags, instance bounds...
        default:
          break;
      }
    }
  }
};

// Listen
template <>
struct minuit_behavior<minuit_command::Request, minuit_operation::Listen>
{
  auto operator()(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      const oscpack::ReceivedMessage& mess)
  {
    // TODO FIXME Add the address to the listeners
  }
};

// Namespace
template <>
struct minuit_behavior<minuit_command::Request, minuit_operation::Namespace>
{
  void handle_root(
      ossia::net::minuit_protocol& proto, const std::vector<std::string>& c)
  {
    proto.sender().send(
        proto.name_table.get_action(minuit_action::NamespaceReply), "/",
        "Application", "nodes={", c, "}", "attributes={", "}");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  void handle_container(
      ossia::net::minuit_protocol& proto, ossia::string_view address,
      const std::vector<std::string>& c)
  {
    proto.sender().send(
        proto.name_table.get_action(minuit_action::NamespaceReply), address,
        "Container", "nodes={", c, "}", "attributes={", "}");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  void
  handle_data(ossia::net::minuit_protocol& proto, ossia::string_view address)
  {
    proto.sender().send(
        proto.name_table.get_action(minuit_action::NamespaceReply), address,
        "Data", "attributes={", "type", "dataspace", "repetitionsFilter",
        "service", "priority", "value", "rangeBounds", "rangeClipmode",
        "dataspaceUnit", "}");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  void handle_data_container(
      ossia::net::minuit_protocol& proto, ossia::string_view address,
      const std::vector<std::string>& c)
  {
    proto.sender().send(
        proto.name_table.get_action(minuit_action::NamespaceReply), address,
        "Data", "attributes={", "type", "dataspace", "repetitionsFilter",
        "service", "priority", "value", "rangeBounds", "rangeClipmode",
        "dataspaceUnit", "}");
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  std::vector<std::string>
  get_children_names(ossia::net::device_base& dev, ossia::string_view address)
  {
    auto node = ossia::net::find_node(dev.get_root_node(), address);
    if (!node)
      return {};

    return node->children_names();
  }

  auto operator()(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      const oscpack::ReceivedMessage& mess)
  {
    ossia::string_view address{mess.ArgumentsBegin()->AsString()};

    if (address == ossia::string_view("/"))
    {
      handle_root(proto, get_children_names(dev, address));
    }
    else
    {
      auto node = ossia::net::find_node(dev.get_root_node(), address);
      if (!node)
        return;

      if (node->children().empty())
      {
        handle_data(proto, address);
      }
      else
      {
        if (node->get_parameter())
        {
          handle_data_container(
              proto, address, get_children_names(dev, address));
        }
        else
        {
          handle_container(proto, address, get_children_names(dev, address));
        }
      }
    }
  }
};

inline ossia::domain get_domain(
    ossia::net::parameter_base& addr,
    oscpack::ReceivedMessageArgumentIterator beg_it,
    oscpack::ReceivedMessageArgumentIterator end_it)
{
  chobo::small_vector<ossia::value, 2> val;
  const auto cur = addr.value();

  // We read all the values one by one
  while (beg_it != end_it)
  {
    auto cur_it = beg_it;
    ++beg_it;
    val.push_back(ossia::net::to_numeric_value(cur, cur_it, beg_it));
  }

  return ossia::make_domain(val, cur);
}

inline optional<double> AsNumber(oscpack::ReceivedMessageArgumentIterator& it)
{
  switch (it->TypeTag())
  {
    case oscpack::TypeTagValues::INT32_TYPE_TAG:
      return (double)it->AsInt32Unchecked();
    case oscpack::TypeTagValues::INT64_TYPE_TAG:
      return (double)it->AsInt64Unchecked();
    case oscpack::TypeTagValues::FLOAT_TYPE_TAG:
      return (double)it->AsFloatUnchecked();
    case oscpack::TypeTagValues::DOUBLE_TYPE_TAG:
      return it->AsDoubleUnchecked();
    default:
      return ossia::none;
  }
}

// Listen
template <>
struct minuit_behavior<minuit_command::Answer, minuit_operation::Listen>
{
  auto operator()(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      const oscpack::ReceivedMessage& mess)
  {
    ossia::string_view full_address;
    auto mess_it = mess.ArgumentsBegin();
    full_address = mess_it->AsStringUnchecked();
    auto idx = full_address.find_first_of(":");

    if (idx == std::string::npos)
    {
      // The OSC message is a standard OSC one, carrying a value.
      auto node = ossia::net::find_node(dev.get_root_node(), full_address);
      if (node)
      {
        if (auto addr = node->get_parameter())
        {
          ossia::net::update_value(
              *addr, ++mess_it, mess.ArgumentsEnd(), mess.ArgumentCount() - 1);
        }
      }
    }
    else
    {
      // The OSC message is a Minuit one.
      // address contains the "sanitized" OSC-like address.
      ossia::string_view address{full_address.data(), idx};

      // Note : bug if address == "foo:"
      auto attr = get_attribute(ossia::string_view(
          address.data() + idx + 1, full_address.size() - idx - 1));

      ++mess_it;
      // mess_it is now at the first argument after the address:attribute

      if (auto node = ossia::net::find_node(dev.get_root_node(), address))
        if (auto addr = node->get_parameter())
          switch (attr)
          {
            case minuit_attribute::Value:
            {
              ossia::net::update_value(
                  *addr, mess_it, mess.ArgumentsEnd(),
                  mess.ArgumentCount() - 1);
              break;
            }
            case minuit_attribute::Type:
            {
              addr->set_value_type(ossia::minuit::type_from_minuit_type_text(
                  mess_it->AsStringUnchecked()));

              break;
            }
            case minuit_attribute::RangeBounds:
            {
              addr->set_domain(
                  get_domain(*addr, mess_it, mess.ArgumentsEnd()));
              break;
            }
            case minuit_attribute::RangeClipMode:
            {
              addr->set_bounding(
                  from_minuit_bounding_text(mess_it->AsStringUnchecked()));
              break;
            }
            case minuit_attribute::RepetitionFilter:
            {
              addr->set_repetition_filter(
                  static_cast<repetition_filter>(mess_it->AsInt32Unchecked()));
              break;
            }
            case minuit_attribute::Dataspace:
            {
              addr->set_unit(
                  ossia::parse_dataspace(mess_it->AsStringUnchecked()));
              break;
            }
            case minuit_attribute::DataspaceUnit:
            {
              addr->set_unit(ossia::parse_unit(
                  mess_it->AsStringUnchecked(), addr->get_unit()));
              break;
            }
            case minuit_attribute::Service:
            {
              addr->set_access(
                  from_minuit_service_text(mess_it->AsStringUnchecked()));
              break;
            }
            case minuit_attribute::Description:
            {
              ossia::net::set_description(
                  *node, std::string(mess_it->AsStringUnchecked()));
              break;
            }
            case minuit_attribute::Priority:
            {
              ossia::net::set_priority(*node, mess_it->AsInt32());
              break;
            }
            case minuit_attribute::ValueStepSize:
            {
              ossia::net::set_value_step_size(*node, AsNumber(mess_it));
              break;
            }
            default:
              break;
          }
    }
    return full_address;
  }
};

// Get
template <>
struct minuit_behavior<minuit_command::Answer, minuit_operation::Get>
{

  void operator()(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      const oscpack::ReceivedMessage& mess)
  {
    auto addr
        = minuit_behavior<minuit_command::Answer, minuit_operation::Listen>{}(
            proto, dev, mess);
    proto.get_refreshed(addr);
  }
};

template <typename It, typename Fun>
auto find_if(It begin, It end, Fun f)
{
  for (auto it = begin; it != end; ++it)
  {
    if (f(*it))
      return it;
  }
  return end;
}

// Namespace
template <>
struct minuit_behavior<minuit_command::Answer, minuit_operation::Namespace>
{
  template <typename Str>
  static auto get_container(
      Str s, oscpack::ReceivedMessageArgumentIterator beg_it,
      oscpack::ReceivedMessageArgumentIterator end_it)
  {
    std::vector<ossia::string_view> elements;
    auto nodes_beg_it = find_if(beg_it, end_it, [=](const auto& mess) {
      return mess.IsString()
             && ossia::string_view(mess.AsStringUnchecked()) == s;
    });

    ++nodes_beg_it; // It will point on the first past "nodes={".
    if (nodes_beg_it != end_it)
    {
      auto nodes_end_it = find_if(nodes_beg_it, end_it, [](const auto& mess) {
        return mess.IsString() && mess.AsStringUnchecked()[0] == '}'
               && mess.AsStringUnchecked()[1] == '\0';
      });

      if (nodes_end_it != end_it)
      {
        for (auto it = nodes_beg_it; it != nodes_end_it; ++it)
        {
          elements.push_back(it->AsStringUnchecked());
        }
      }
    }

    return elements;
  }

  static auto get_nodes(
      oscpack::ReceivedMessageArgumentIterator beg_it,
      oscpack::ReceivedMessageArgumentIterator end_it)
  {
    return get_container("nodes={", beg_it, end_it);
  }

  static auto get_attributes(
      oscpack::ReceivedMessageArgumentIterator beg_it,
      oscpack::ReceivedMessageArgumentIterator end_it)
  {
    return get_container("attributes={", beg_it, end_it);
  }

  static auto handle_container(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      ossia::string_view address,
      oscpack::ReceivedMessageArgumentIterator beg_it,
      oscpack::ReceivedMessageArgumentIterator end_it)
  {
    using namespace oscpack;
    auto sub_request
        = proto.name_table.get_action(minuit_action::NamespaceRequest);

    // Get the sub-nodes
    std::string child_address{address};
    if (child_address.back() != '/')
      child_address += '/';
    const auto child_parameter_size = child_address.size();

    for (auto child : get_nodes(beg_it, end_it))
    {
      child_address.resize(child_parameter_size);

      // Address of the node to create
      child_address.append(child.begin(), child.end());

      // Create the actual node
      ossia::net::find_or_create_node(dev.get_root_node(), address);

      // request children
      proto.namespace_refresh(sub_request, child_address);
    }
  }

  static auto handle_data(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      ossia::string_view address,
      oscpack::ReceivedMessageArgumentIterator beg_it,
      oscpack::ReceivedMessageArgumentIterator end_it)
  {
    using namespace oscpack;

    // Find or create the node
    auto& n = ossia::net::find_or_create_node(dev.get_root_node(), address);
    n.create_parameter(ossia::val_type::IMPULSE);

    // A data can also have child nodes :
    handle_container(proto, dev, address, beg_it, end_it);

    auto sub_request = proto.name_table.get_action(minuit_action::GetRequest);

    // Request all the attributes provided by the node
    // type and dataspace have to be queried before the others since
    // value and dataspace_unit correct parsing depends on correct values
    // prior.
    // note : we should really use TCP for this since there could be a
    // reordering...
    // or a queue of things to query.

    auto attribs = get_attributes(beg_it, end_it);
    for (auto it = attribs.begin(); it != attribs.end();)
    {
      switch (get_attribute(*it))
      {
        case minuit_attribute::Type:
        case minuit_attribute::Dataspace:
        {
          // name?get address:attribute
          auto str = std::string(address);
          str += ':';
          str.append(it->begin(), it->end());
          proto.get_refresh(sub_request, str);

          it = attribs.erase(it);
        }
        break;
        default:
          ++it;
          break;
      }
    }

    for (auto attrib : attribs)
    {
      switch (get_attribute(attrib))
      {
        case minuit_attribute::Value:
        case minuit_attribute::RangeBounds:
        case minuit_attribute::RangeClipMode:
        case minuit_attribute::RepetitionFilter:
        case minuit_attribute::DataspaceUnit:
        case minuit_attribute::Service:
        case minuit_attribute::Priority:
        case minuit_attribute::ValueStepSize:
        case minuit_attribute::Description:
        {
          // name?get address:attribute
          auto str = std::string(address);
          str += ':';
          str.append(attrib.begin(), attrib.end());
          proto.get_refresh(sub_request, str);
        }
        default:
          break;
      }
    }
  }

  static auto handle_minuit(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      ossia::string_view address, minuit_type type,
      oscpack::ReceivedMessageArgumentIterator beg_it,
      oscpack::ReceivedMessageArgumentIterator end_it)
  {
    switch (type)
    {
      case minuit_type::Application:
      case minuit_type::Container:
      {
        handle_container(proto, dev, address, beg_it, end_it);
        break;
      }
      case minuit_type::Data:
      {
        handle_data(proto, dev, address, beg_it, end_it);
        break;
      }
      case minuit_type::ModelInfo:
      case minuit_type::UiInfo:
      case minuit_type::PresetManager:
      case minuit_type::None:
        handle_container(proto, dev, address, beg_it, end_it);
        break;
    }
  }

  auto operator()(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      const oscpack::ReceivedMessage& mess)
  {
    auto it = mess.ArgumentsBegin();
    ossia::string_view address = it->AsString();
    auto type = get_type((++it)->AsString()[0]);

    handle_minuit(proto, dev, address, type, it, mess.ArgumentsEnd());

    proto.namespace_refreshed(address);
  }
};

// Namespace request :
// app?namespace addr

// Namespace answer :
// app:namespace addr minuit_type nodes={ n1 n2 n3 } attributes={ foo bar baz }

class minuit_message_handler
{
public:
  static void handleMinuitMessage(
      ossia::net::minuit_protocol& proto, ossia::net::device_base& dev,
      ossia::string_view address, const oscpack::ReceivedMessage& m)
  {
    // Look for either ':' or '?'
    auto idx = address.find_first_of(":?!");

    if (idx != std::string::npos)
    {
      auto req = get_command(address[idx]);
      auto op = get_operation(*(address.data() + idx + 1));
      switch (req)
      {
        case minuit_command::Answer: // Receiving an answer
        {
          switch (op)
          {
            case minuit_operation::Listen:
              minuit_behavior<minuit_command::Answer, minuit_operation::Listen>{}(
                  proto, dev, m);
              break;
            case minuit_operation::Get:
              minuit_behavior<minuit_command::Answer, minuit_operation::Get>{}(
                  proto, dev, m);
              break;
            case minuit_operation::Namespace:
              minuit_behavior<minuit_command::Answer, minuit_operation::Namespace>{}(
                  proto, dev, m);
              break;
            default:
              break;
          }
          break;
        }
        case minuit_command::Request: // Receiving a request
        {
          switch (op)
          {
            case minuit_operation::Listen:
              minuit_behavior<minuit_command::Request, minuit_operation::Listen>{}(
                  proto, dev, m);
              break;
            case minuit_operation::Get:
              minuit_behavior<minuit_command::Request, minuit_operation::Get>{}(
                  proto, dev, m);
              break;
            case minuit_operation::Namespace:
              minuit_behavior<minuit_command::Request, minuit_operation::Namespace>{}(
                  proto, dev, m);
              break;
            default:
              break;
          }
          break;
        }
        case minuit_command::Error: // Receiving an error
        {
          switch (op)
          {
            case minuit_operation::Listen:
              minuit_behavior<minuit_command::Error, minuit_operation::Listen>{}(
                  proto, dev, m);
              break;
            case minuit_operation::Get:
              minuit_behavior<minuit_command::Error, minuit_operation::Get>{}(
                  proto, dev, m);
              break;
            case minuit_operation::Namespace:
              minuit_behavior<minuit_command::Error, minuit_operation::Namespace>{}(
                  proto, dev, m);
              break;
            default:
              break;
          }
          break;
        }
        default:
          break;
      }
    }
  }
};
}
}
