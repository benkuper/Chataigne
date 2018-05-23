#pragma once
#include <ossia/network/value/value.hpp>
#include <ossia/network/base/parameter.hpp>
#include <ModernMIDI/midi_message.h>
#include <boost/container/flat_set.hpp>
#include <boost/functional/hash.hpp>
#include <eggs/variant.hpp>
#include <ossia/network/common/path.hpp>

#include <array>
#include <deque>
#include <vector>
namespace std
{
template <typename T>
class hash<std::pair<T*, T*>>
{
public:
  std::size_t operator()(const std::pair<T*, T*>& p) const
  {
    std::size_t seed = 0;
    boost::hash_combine(seed, p.first);
    boost::hash_combine(seed, p.second);
    return seed;
  }
};
}

namespace ossia
{

template <typename T>
using set = boost::container::flat_set<T>;

using destination_t = eggs::variant<
ossia::net::parameter_base*,
ossia::traversal::path
>;
class graph_node;
struct graph_edge;
using node_ptr = std::shared_ptr<graph_node>;
using edge_ptr = std::shared_ptr<graph_edge>;
struct port;
struct outlet;
struct inlet;
using inlet_ptr = std::shared_ptr<inlet>;
using outlet_ptr = std::shared_ptr<outlet>;
struct base_pair
{
  graph_edge* edge{};
};
using inlet_pair = base_pair;
using outlet_pair = base_pair;
inline bool operator==(base_pair lhs, base_pair rhs)
{
  return lhs.edge == rhs.edge;
}

struct audio_port;
#if defined(OSSIA_PROTOCOL_MIDI)
struct midi_port;
#endif
struct value_port;

struct audio_delay_line;
#if defined(OSSIA_PROTOCOL_MIDI)
struct midi_delay_line;
#endif
struct value_delay_line;

#if defined(OSSIA_PROTOCOL_MIDI)
using data_type = eggs::variant<audio_port, midi_port, value_port>;
#else
using data_type = eggs::variant<audio_port, value_port>;
#endif

#if defined(OSSIA_PROTOCOL_MIDI)
using delay_line_type = eggs::variant<audio_delay_line, midi_delay_line, value_delay_line>;
#else 
using delay_line_type = eggs::variant<audio_delay_line, value_delay_line>;
#endif
}
