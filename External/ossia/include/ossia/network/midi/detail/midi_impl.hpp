#pragma once
#include <ossia/editor/state/message.hpp>
#include <ossia/network/value/value.hpp>
#include <ossia/network/domain/domain.hpp>
#include <ossia/network/midi/detail/channel.hpp>

#include <ossia/network/midi/midi_parameter.hpp>
#include <ossia/network/midi/midi_device.hpp>
#include <ossia/network/midi/midi_node.hpp>

#include <iostream>

namespace ossia
{
namespace net
{
namespace midi
{
const std::string& midi_node_name(midi_size_t i);

class OSSIA_EXPORT note_on_N_node final : public midi_node, public midi_parameter
{
public:
  note_on_N_node(
      midi_size_t channel, midi_size_t note, midi_device& aDevice,
      ossia::net::node_base& aParent)
      : midi_node{aDevice, aParent}
      , midi_parameter{address_info{channel, address_info::Type::NoteOn_N, note},
                     *this}
  {
    m_name = midi_node_name(note);
    m_parameter.reset(this);
  }

  ~note_on_N_node()
  {
    about_to_be_deleted(*this);
    m_parameter.release();
  }
};

class OSSIA_EXPORT note_off_N_node final : public midi_node,
                                           public midi_parameter
{
public:
  note_off_N_node(
      midi_size_t channel, midi_size_t note, midi_device& aDevice,
      ossia::net::node_base& aParent)
      : midi_node{aDevice, aParent}
      , midi_parameter{
            address_info{channel, address_info::Type::NoteOff_N, note}, *this}
  {
    m_name = midi_node_name(note);
    m_parameter.reset(this);
  }

  ~note_off_N_node()
  {
    about_to_be_deleted(*this);
    m_parameter.release();
  }
};

class OSSIA_EXPORT control_N_node final : public midi_node, public midi_parameter
{
public:
  control_N_node(
      midi_size_t channel, midi_size_t param, midi_device& aDevice,
      ossia::net::node_base& aParent)
      : midi_node{aDevice, aParent}
      , midi_parameter{address_info{channel, address_info::Type::CC_N, param},
                     *this}
  {
    m_name = midi_node_name(param);
    m_parameter.reset(this);
  }

  ~control_N_node()
  {
    about_to_be_deleted(*this);
    m_parameter.release();
  }
};

class OSSIA_EXPORT program_N_node final : public midi_node, public midi_parameter
{
public:
  program_N_node(
      midi_size_t channel, midi_size_t param, midi_device& aDevice, 
      ossia::net::node_base& aParent)
      : midi_node{aDevice, aParent}
      , midi_parameter{address_info{channel, address_info::Type::PC_N, param},
                     *this}
  {
    m_name = midi_node_name(param);
    m_parameter.reset(this);
  }

  ~program_N_node()
  {
    about_to_be_deleted(*this);
    m_parameter.release();
  }
};

class OSSIA_EXPORT program_node final : public midi_node, public midi_parameter
{
public:
  program_node(midi_size_t channel, midi_device& aDevice, 
    ossia::net::node_base& aParent)
      : midi_node(aDevice, aParent)
      , midi_parameter{address_info{channel, address_info::Type::PC, 0}, *this}
  {
    using namespace std::literals;
    m_name = "program"s;
    m_parameter.reset(this);
    m_children.reserve(128);
    for (int i = 0; i < 128; i++)
    {
      auto ptr = std::make_unique<program_N_node>(channel, i, m_device, *this);
      m_children.push_back(std::move(ptr));
    }
  }

  ~program_node()
  {
    about_to_be_deleted(*this);
    m_parameter.release();
  }
};

class OSSIA_EXPORT note_on_node final : public midi_node, public midi_parameter
{
public:
  note_on_node(midi_size_t channel, midi_device& aDevice, 
    ossia::net::node_base& aParent)
      : midi_node(aDevice, aParent)
      , midi_parameter{address_info{channel, address_info::Type::NoteOn, 0},
                     *this}
  {
    using namespace std::literals;
    m_name = "on"s;
    m_parameter.reset(this);
    m_children.reserve(128);
    for (int i = 0; i < 128; i++)
    {
      auto ptr = std::make_unique<note_on_N_node>(channel, i, m_device, *this);
      m_children.push_back(std::move(ptr));
    }
  }

  ~note_on_node()
  {
    about_to_be_deleted(*this);
    m_parameter.release();
  }
};

class OSSIA_EXPORT note_off_node final : public midi_node, public midi_parameter
{
public:
  note_off_node(midi_size_t channel, midi_device& aDevice, 
    ossia::net::node_base& aParent)
      : midi_node(aDevice, aParent)
      , midi_parameter{address_info{channel, address_info::Type::NoteOff, 0},
                     *this}
  {
    using namespace std::literals;
    m_name = "off"s;
    m_parameter.reset(this);

    m_children.reserve(128);
    for (int i = 0; i < 128; i++)
    {
      auto ptr
          = std::make_unique<note_off_N_node>(channel, i, m_device, *this);
      m_children.push_back(std::move(ptr));
    }
  }

  ~note_off_node()
  {
    about_to_be_deleted(*this);
    m_parameter.release();
  }
};

class OSSIA_EXPORT control_node final : public midi_node, public midi_parameter
{
public:
  control_node(midi_size_t channel, midi_device& aDevice, 
    ossia::net::node_base& aParent)
      : midi_node(aDevice, aParent)
      , midi_parameter{address_info{channel, address_info::Type::CC, 0}, *this}
  {
    using namespace std::literals;
    m_name = "control"s;
    m_parameter.reset(this);

    m_children.reserve(128);
    for (int i = 0; i < 128; i++)
    {
      auto ptr = std::make_unique<control_N_node>(channel, i, m_device, *this);
      m_children.push_back(std::move(ptr));
    }
  }

  ~control_node()
  {
    about_to_be_deleted(*this);
    m_parameter.release();
  }
};

class OSSIA_EXPORT pitch_bend_node final : public midi_node, public midi_parameter
{
public:
  pitch_bend_node(midi_size_t channel, midi_device& aDevice, ossia::net::node_base& aParent)
      : midi_node(aDevice, aParent)
      , midi_parameter{address_info{channel, address_info::Type::PB, 0}, *this}
  {
    using namespace std::literals;
    m_name = "pitchbend"s;
    m_parameter.reset(this);
  }

  ~pitch_bend_node()
  {
    about_to_be_deleted(*this);
    m_parameter.release();
  }
};

class OSSIA_EXPORT channel_node final : public midi_node
{
  const midi_size_t m_channel;

public:
  channel_node(midi_size_t channel, midi_device& aDevice, 
    ossia::net::node_base& aParent)
      : midi_node(aDevice, aParent), m_channel{channel}
  {
    m_name = midi_node_name(channel);
    m_children.reserve(4);

    m_children.push_back(
          std::make_unique<note_on_node>(m_channel, m_device, *this));

    m_children.push_back(
          std::make_unique<note_off_node>(m_channel, m_device, *this));

    m_children.push_back(
          std::make_unique<control_node>(m_channel, m_device, *this));

    m_children.push_back(
          std::make_unique<program_node>(m_channel, m_device, *this));

    m_children.push_back(
          std::make_unique<pitch_bend_node>(m_channel, m_device, *this));
  }

  ~channel_node()
  {
    about_to_be_deleted(*this);
  }

  std::array<ossia::message, 2> note_on(midi_size_t note, midi_size_t vel)
  {
    const auto& c = children();
    return {{ossia::message{
                 *c[0]->get_parameter(),
                 std::vector<ossia::value>{int32_t{note}, int32_t{vel}}},
             ossia::message{*c[0]->children()[note]->get_parameter(),
                            int32_t{vel}}}};
  }

  std::array<ossia::message, 2> note_off(midi_size_t note, midi_size_t vel)
  {
    const auto& c = children();
    return {{ossia::message{
                 *c[1]->get_parameter(),
                 std::vector<ossia::value>{int32_t{note}, int32_t{vel}}},
             ossia::message{*c[1]->children()[note]->get_parameter(),
                            int32_t{vel}}}};
  }
};
}
}
}
