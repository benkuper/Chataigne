#pragma once
#include <ossia/dataflow/data.hpp>
#include <ossia/dataflow/execution_state.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

namespace ossia
{
struct port
{
  data_type data;

  enum scope_t
  {
    none = 1 << 0,
    local = 1 << 1,
    global = 1 << 2,
    both = local | global
  };

  scope_t scope{scope_t::both};

protected:
  port(data_type d) : data{std::move(d)}
  {
  }

  port() = delete;
  port(const port&) = delete;
  port(port&&) = delete;
  port& operator=(const port&) = delete;
  port& operator=(port&&) = delete;
};

struct inlet : public port
{
  inlet(data_type d) : port{std::move(d)}
  {
  }
  inlet(data_type d, destination_t dest)
      : port{std::move(d)}, address{std::move(dest)}
  {
  }

  inlet(data_type d, ossia::net::parameter_base& addr)
      : port{std::move(d)}, address{&addr}
  {
  }

  inlet(data_type d, graph_edge& edge) : port{std::move(d)}
  {
    sources.push_back(&edge);
  }

  void connect(graph_edge* e)
  {
    auto it = ossia::find(sources, e);
    if (it == sources.end())
      sources.push_back(e);
  }

  void disconnect(graph_edge* e)
  {
    boost::remove_erase(sources, e);
  }

  destination_t address;
  std::vector<graph_edge*> sources;
};

struct outlet : public port
{
  outlet(data_type d) : port{std::move(d)}
  {
  }
  outlet(data_type d, destination_t dest)
      : port{std::move(d)}, address{std::move(dest)}
  {
  }

  outlet(data_type d, ossia::net::parameter_base& addr)
      : port{std::move(d)}, address{&addr}
  {
  }

  outlet(data_type d, graph_edge& edge) : port{std::move(d)}
  {
    targets.push_back(&edge);
  }

  void connect(graph_edge* e)
  {
    auto it = ossia::find(targets, e);
    if (it == targets.end())
      targets.push_back(e);
  }

  void disconnect(graph_edge* e)
  {
    boost::remove_erase(targets, e);
  }

  void write(execution_state& e);

  destination_t address;
  std::vector<graph_edge*> targets;
};

template <typename T, typename... Args>
inlet_ptr make_inlet(Args&&... args)
{
  return std::make_shared<inlet>(T{}, std::forward<Args>(args)...);
}
template <typename T, typename... Args>
outlet_ptr make_outlet(Args&&... args)
{
  return std::make_shared<outlet>(T{}, std::forward<Args>(args)...);
}

using ports = std::vector<std::shared_ptr<port>>;
using inlets = std::vector<inlet_ptr>;
using outlets = std::vector<outlet_ptr>;
}
