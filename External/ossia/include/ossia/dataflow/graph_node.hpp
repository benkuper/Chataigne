#pragma once
#include <ossia/dataflow/execution_state.hpp>
#include <ossia/dataflow/graph_edge.hpp>
#include <ossia/dataflow/port.hpp>
#include <ossia/editor/scenario/time_value.hpp>

namespace ossia
{

class OSSIA_EXPORT graph_node
{
public:
  graph_node();
  virtual ~graph_node();

  bool enabled() const;
  void set_enabled(bool b);

  bool executed() const;
  void set_executed(bool b);

  virtual bool consumes(const std::string&) const;
  virtual bool consumes(const destination&) const;
  virtual bool consumes(const execution_state&) const;

  virtual void run(execution_state&);

  void set_date(ossia::time_value d, double pos);
  void set_date(int64_t d, double pos)
  {
    set_date(ossia::time_value(d), pos);
  }
  ossia::time_value time() const
  {
    return m_date;
  }
  double position() const
  {
    return m_position;
  }

  bool can_execute(const execution_state&) const;

  bool has_port_inputs() const;
  bool has_global_inputs() const;
  bool has_local_inputs(const execution_state&) const;

  const inlets& inputs() const;
  const outlets& outputs() const;

  void clear();

  // incremented for each process
  int64_t temporal_counter{};
  std::vector<int64_t> temporal_priority;
  std::vector<int64_t> custom_priority;

protected:
  inlets m_inlets;
  outlets m_outlets;

  double m_position{};
  ossia::time_value m_prev_date{};
  ossia::time_value m_date{};

  bool m_enabled{};
  bool m_executed{};
};
}
