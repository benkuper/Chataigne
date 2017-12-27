#pragma once
#include <ossia/dataflow/connection.hpp>
#include <ossia/dataflow/port.hpp>
#include <ossia/detail/apply.hpp>

namespace ossia
{
struct init_delay_line
{
  delay_line_type& delay_line;
  void operator ()(const audio_port&) { delay_line = audio_delay_line{}; }
  void operator ()(const value_port&) { delay_line = value_delay_line{}; }
#if defined(OSSIA_PROTOCOL_MIDI)
  void operator ()(const midi_port&) { delay_line = midi_delay_line{}; }
#endif
  void operator ()() { }
};

// A pure dependency edge does not have in/out ports set
struct graph_edge
{
  graph_edge(
      connection c, outlet_ptr pout, inlet_ptr pin, node_ptr pout_node,
      node_ptr pin_node)
      : con{c}
      , out{std::move(pout)}
      , in{std::move(pin)}
      , out_node{std::move(pout_node)}
      , in_node{std::move(pin_node)}
  {
    if (in && out)
    {
      out->connect(this);
      in->connect(this);

      if (auto delay = con.target<delayed_glutton_connection>())
      {
        ossia::apply(init_delay_line{delay->buffer}, out->data);
      }
      else if (auto sdelay = con.target<delayed_strict_connection>())
      {
        ossia::apply(init_delay_line{sdelay->buffer}, out->data);
      }
    }
  }

  ~graph_edge()
  {
    clear();
  }

  void clear()
  {
    if (in && out)
    {
      out->disconnect(this);
      in->disconnect(this);
    }

    con = connection{};
    out.reset();
    in.reset();
    out_node.reset();
    in_node.reset();
  }

  connection con;
  outlet_ptr out;
  inlet_ptr in;
  node_ptr out_node;
  node_ptr in_node;
};

template <typename... Args>
auto make_edge(Args&&... args)
{
  return std::make_shared<ossia::graph_edge>(std::forward<Args>(args)...);
}
}
