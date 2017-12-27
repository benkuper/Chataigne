#pragma once
#include <ossia/network/base/parameter.hpp>
#include <ossia/network/domain/domain.hpp>
#include <ossia/network/midi/detail/channel.hpp>

#define BOOST_LEXICAL_CAST_ASSUME_C_LOCALE
#include <boost/lexical_cast.hpp>
namespace ossia
{
namespace net
{
namespace midi
{
class midi_protocol;
struct address_info
{
  enum class Type : midi_size_t
  {
    NoteOn,    // /12/note/on 64 127 -> C3 at 127 velocity
    NoteOn_N,  // /12/note/on/64 127 -> C3 at 127 velocity
    NoteOff,   // /12/note/off 64 127
    NoteOff_N, // /12/note/off/64 127
    CC,        // /12/CC 64 123
    CC_N,      // /12/CC/64 123,
    PC,        // /12/PC 32
    PC_N,      // /12/PC/32 Impulse
    PB         // /12/PB -8192 -> 8191
  };

  ossia::val_type matchingType()
  {
    switch (type)
    {
      case Type::NoteOn:
      case Type::NoteOff:
      case Type::CC:
        return ossia::val_type::LIST;
      case Type::NoteOn_N:
      case Type::NoteOff_N:
      case Type::CC_N:
      case Type::PC:
      case Type::PB:
        return ossia::val_type::INT;
      case Type::PC_N:
        return ossia::val_type::IMPULSE;
    }
    return {};
  }

  std::string address()
  {
    switch (type)
    {
      case Type::NoteOn:
        return "/" + boost::lexical_cast<std::string>(channel) + "/note/on";
      case Type::NoteOff:
        return "/" + boost::lexical_cast<std::string>(channel) + "/note/off";
      case Type::CC:
        return "/" + boost::lexical_cast<std::string>(channel) + "/CC";
      case Type::NoteOn_N:
        return "/" + boost::lexical_cast<std::string>(channel) + "/note/on/"
               + boost::lexical_cast<std::string>(note);
      case Type::NoteOff_N:
        return "/" + boost::lexical_cast<std::string>(channel) + "/note/off/"
               + boost::lexical_cast<std::string>(note);
      case Type::CC_N:
        return "/" + boost::lexical_cast<std::string>(channel) + "/CC/"
               + boost::lexical_cast<std::string>(note);
      case Type::PC:
        return "/" + boost::lexical_cast<std::string>(channel) + "/PC";
      case Type::PC_N:
        return "/" + boost::lexical_cast<std::string>(channel) + "/PC/"
               + boost::lexical_cast<std::string>(note);
      case Type::PB:
        return "/" + boost::lexical_cast<std::string>(channel) + "/PB";
    }
    return {};
  }

  ossia::value defaultValue(int32_t val)
  {
    switch (type)
    {
      case Type::NoteOn:
      case Type::NoteOff:
      case Type::CC:
        return std::vector<ossia::value>{int32_t{val}, int32_t{val}};
      case Type::NoteOn_N:
      case Type::NoteOff_N:
      case Type::CC_N:
      case Type::PC:
      case Type::PB:
        return int32_t{val};
      case Type::PC_N:
        return ossia::impulse{};
    }
    return {};
  }

  ossia::domain defaultDomain()
  {
    if(type != Type::PB)
      return ossia::make_domain(defaultValue(0), defaultValue(127));
    return ossia::make_domain(-8192, 8191);
  }

  address_info(Type t) : type{t}
  {
  }

  address_info(Type t, midi_size_t n) : type{t}, note{n}
  {
  }

  address_info(midi_size_t chan, Type t, midi_size_t n)
      : channel{chan}, type{t}, note{n}
  {
  }
  midi_size_t channel{};
  Type type{};
  midi_size_t note{};
};

class midi_parameter : public ossia::net::parameter_base
{
  address_info m_info;
  midi_protocol& m_protocol;
  ossia::domain m_domain;

  ossia::val_type m_type = ossia::val_type::INT;
  ossia::value m_value;

public:
  midi_parameter(address_info info, ossia::net::node_base& parent);

  const address_info& info() const;

  void pull_value() final override;
  parameter_base& push_value(const ossia::value& val) final override;
  parameter_base& push_value(ossia::value&& val) final override;
  parameter_base& push_value() final override;
  const ossia::value& getValue() const;

  ossia::value value() const final override;
  parameter_base& set_value(const ossia::value& v) final override;
  parameter_base& set_value(ossia::value&& v) final override;

  ossia::val_type get_value_type() const final override;
  parameter_base& set_value_type(ossia::val_type) final override;

  ossia::access_mode get_access() const final override;
  parameter_base& set_access(ossia::access_mode) final override;

  const ossia::domain& get_domain() const final override;
  parameter_base& set_domain(const ossia::domain&) final override;

  ossia::bounding_mode get_bounding() const final override;
  parameter_base& set_bounding(ossia::bounding_mode) final override;

  void on_first_callback_added() final override;
  void on_removing_last_callback() final override;

  void value_callback(const ossia::value& val);
};
}
}
}
