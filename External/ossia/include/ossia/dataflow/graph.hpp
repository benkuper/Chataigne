#pragma once
#include <ossia/dataflow/graph_edge.hpp>
#include <ossia/dataflow/graph_node.hpp>
#include <ossia/editor/scenario/time_value.hpp>
#include <boost/bimap.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/range/adaptors.hpp>
class DataflowTest;
namespace ossia
{

using graph_t = boost::
    adjacency_list<boost::vecS, boost::vecS, boost::directedS, node_ptr, std::shared_ptr<graph_edge>>;

using graph_vertex_t = graph_t::vertex_descriptor;
using graph_edge_t = graph_t::edge_descriptor;

template <typename T, typename U>
using bimap = boost::bimap<T, U>;
using node_bimap = bimap<graph_vertex_t, node_ptr>;
using edge_bimap = bimap<graph_edge_t, std::shared_ptr<graph_edge>>;
using node_bimap_v = node_bimap::value_type;
using edge_bimap_v = edge_bimap::value_type;

using edge_map_t
    = std::unordered_map<std::pair<graph_node*, graph_node*>, graph_edge*>;

enum class node_ordering
{
  topological, temporal, hierarchical
};

class OSSIA_EXPORT graph
{
public:
  ~graph();

  void add_node(node_ptr n);
  void remove_node(const node_ptr& n);

  void enable(graph_node& n);
  void disable(graph_node& n);

  void connect(const std::shared_ptr<graph_edge>& edge);
  void disconnect(const std::shared_ptr<graph_edge>& edge);

  void clear();

  void state();
  void state(execution_state& e);

  static set<graph_node*>
  disable_strict_nodes(const set<graph_node*>& enabled_nodes);
  static set<graph_node*> disable_strict_nodes(const set<node_ptr>& n);

  void disable_strict_nodes_rec(set<graph_node*>& cur_enabled_node);

  static void copy_from_local(const data_type& out, inlet& in);
  static void copy(const delay_line_type& out, std::size_t pos, inlet& in);
  static void copy(const outlet& out, inlet& in);
  static void copy_to_local(
      const data_type& out, const destination& d, execution_state& in);
  static void copy_to_global(
      const data_type& out, const destination& d, execution_state& in);
  static void pull_from_parameter(inlet& in, execution_state& e);

  void init_node(graph_node& n, execution_state& e);
  void teardown_node(graph_node& n, execution_state& e);

  node_bimap m_nodes;
  edge_bimap m_edges;

  set<graph_node*> m_user_enabled_nodes;

  graph_t m_graph;

  edge_map_t m_edge_map;

  time_value m_time{};

  node_ordering m_ordering{node_ordering::topological};

  friend struct init_node_visitor;
  friend struct inlet;
  friend struct outlet;
  friend class ::DataflowTest;
};
}
