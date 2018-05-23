#pragma once
#include <ossia/network/value/value.hpp>
#include <ossia/network/base/parameter.hpp>
#include <ossia/network/base/device.hpp>
#include <ossia/network/base/protocol.hpp>
#include <ossia/network/common/parameter_properties.hpp>
#include <ossia/network/domain/domain.hpp>
#include <ossia/network/midi/detail/midi_impl.hpp>
#include <array>
#include <cassert>

#include <atomic>
namespace mm
{
class MidiInput;
class MidiOutput;
}
namespace ossia
{
namespace net
{
namespace midi
{
struct OSSIA_EXPORT midi_info
{
    enum class Type
    {
      RemoteInput,
      RemoteOutput
    };

    midi_info() = default;
    midi_info(Type t, std::string d, int p)
      : type{t}, device{std::move(d)}, port{p}
    {
    }

    Type type{};
    std::string device{};
    int port{};
};

class OSSIA_EXPORT midi_protocol final : public ossia::net::protocol_base
{
  public:
    midi_protocol();
    midi_protocol(midi_info);
    ~midi_protocol();

    bool set_info(midi_info);
    midi_info get_info() const;

    std::vector<midi_info> scan();

  private:
    std::unique_ptr<mm::MidiInput> m_input;
    std::unique_ptr<mm::MidiOutput> m_output;

    std::array<midi_channel, 16> m_channels;

    midi_info m_info;
    midi_device* m_dev{};

    friend class midi_device;
    friend class midi_parameter;
    bool pull(ossia::net::parameter_base&) override;
    bool push(const ossia::net::parameter_base&) override;
    bool push_raw(const ossia::net::full_parameter_data& parameter_base) override;
    bool observe(ossia::net::parameter_base&, bool) override;
    bool update(ossia::net::node_base& node_base) override;
    void set_device(ossia::net::device_base& dev) override;

    void value_callback(ossia::net::parameter_base& param, const ossia::value& val);

};
}
}
}
