#pragma once
#include <ossia/dataflow/dataflow_fwd.hpp>
#include <ossia/network/common/complex_type.hpp>
#include <chobo/small_vector.hpp>
namespace ossia
{
template <typename T>
using value_vector = chobo::small_vector<T, 4>;

using audio_vector = std::vector<chobo::small_vector<double, 64>>;

struct audio_port
{
  audio_vector samples;
};

#if defined(OSSIA_PROTOCOL_MIDI)
struct midi_port
{
  value_vector<mm::MidiMessage> messages;
};
#endif

struct value_port
{
  ossia::complex_type type;
  value_vector<ossia::value> data;
};

struct audio_delay_line
{
  std::vector<audio_vector> samples;
};

#if defined(OSSIA_PROTOCOL_MIDI)
struct midi_delay_line
{
  std::vector<value_vector<mm::MidiMessage>> messages;
};
#endif

struct value_delay_line
{
  ossia::complex_type type;
  std::vector<value_vector<ossia::value>> data;
};

struct clear_data
{
  void operator()(value_port& p) const
  {
    p.data.clear();
  }

#if defined(OSSIA_PROTOCOL_MIDI)
  void operator()(midi_port& p) const
  {
    p.messages.clear();
  }
#endif

  void operator()(audio_port& p) const
  {
    p.samples.clear();
  }

  void operator()() const
  {
  }
};

struct data_size
{
  std::size_t operator()(const value_delay_line& p) const
  {
    return p.data.size();
  }

#if defined(OSSIA_PROTOCOL_MIDI)
  std::size_t operator()(const midi_delay_line& p) const
  {
    return p.messages.size();
  }
#endif

  std::size_t operator()(const audio_delay_line& p) const
  {
    return p.samples.size();
  }

  std::size_t operator()() const
  {
    return 0;
  }
};


struct mix
{
  void operator()(const value_vector<ossia::value>& out, value_vector<ossia::value>& in)
  {
    for(auto& data : out)
      in.push_back(data);
  }

  void operator()(const audio_vector& src_vec, audio_vector& sink_vec)
  {
    if(sink_vec.size() < src_vec.size())
      sink_vec.resize(src_vec.size());

    for(std::size_t chan = 0; chan < src_vec.size(); chan++)
    {
      auto& src = src_vec[chan];
      auto& sink = sink_vec[chan];
      std::size_t N = std::min(sink.size(), src.size());
      for(std::size_t i = 0; i < N; i++)
        sink[i] += src[i];
    }
  }

#if defined(OSSIA_PROTOCOL_MIDI)
  void operator()(const value_vector<mm::MidiMessage>& out, value_vector<mm::MidiMessage>& in)
  {
    for (const auto& data : out)
      in.push_back(data);
  }
#endif
};

struct copy_data
{
  template <typename T, typename U>
  void operator()(const T&, const U&) const
  {
  }

  void operator()(const value_port& out, value_delay_line& in)
  {
    in.data.push_back(out.data);
  }

  void operator()(const audio_port& out, audio_delay_line& in)
  {
    in.samples.push_back(out.samples);
  }

#if defined(OSSIA_PROTOCOL_MIDI)
  void operator()(const midi_port& out, midi_delay_line& in)
  {
    in.messages.push_back(out.messages);
  }
#endif

  void operator()(const value_port& out, value_port& in)
  {
    mix{}(out.data, in.data);
  }

  void operator()(const audio_port& out, audio_port& in)
  {
    mix{}(out.samples, in.samples);
  }

#if defined(OSSIA_PROTOCOL_MIDI)
  void operator()(const midi_port& out, midi_port& in)
  {
    mix{}(out.messages, in.messages);
  }
#endif
};

struct copy_data_pos
{
  const std::size_t pos;

  template <typename T, typename U>
  void operator()(const T&, const U&) const
  {
  }

  void operator()(const value_delay_line& out, value_port& in)
  {
    if (pos < out.data.size())
    {
      mix{}(out.data[pos], in.data);
    }
  }

  void operator()(const audio_delay_line& out, audio_port& in)
  {
    if (pos < out.samples.size())
    {
      mix{}(out.samples[pos], in.samples);
    }
  }

#if defined(OSSIA_PROTOCOL_MIDI)
  void operator()(const midi_delay_line& out, midi_port& in)
  {
    if (pos < out.messages.size())
    {
      mix{}(out.messages[pos], in.messages);
    }
  }
#endif
};
}
