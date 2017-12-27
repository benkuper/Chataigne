#ifndef OSSIA_CPP98_HPP_2017_07_07
#define OSSIA_CPP98_HPP_2017_07_07
#include <ossia_export.h>
#include <string>
#include <vector>
#if (__cplusplus >= 201103L) || (defined(_MSC_VER) && (_MSC_VER >= 1900))
#define OSSIA_CPP_CXX11
#endif
namespace ossia
{
class value;
namespace net
{
class node_base;
class parameter_base;
class device_base;
}
}
namespace opp
{
enum access_mode
{
  Get,
  Set,
  Bi
};

enum bounding_mode
{
  Free,
  Clip,
  Wrap,
  Fold,
  Low,
  High
};

class node;
class oscquery_server;
class oscquery_mirror;
class OSSIA_EXPORT value
{
public:
  value();
  ~value();
  value(const value& v);
  value(int v);
  value(bool v);
  value(float v);
  value(double v);
  value(std::vector<opp::value> v);
  value(std::string v);
  value(const char* v);

  bool is_impulse() const;
  bool is_int() const;
  bool is_float() const;
  bool is_bool() const;
  bool is_list() const;
  bool is_string() const;

  int to_int() const;
  float to_float() const;
  bool to_bool() const;
  std::vector<opp::value> to_list() const;
  std::string to_string() const;

  value& operator=(const value& v);
  value& operator=(int v);
  value& operator=(float v);
  value& operator=(bool v);
  value& operator=(std::vector<opp::value> v);
  value& operator=(std::string v);

  void set_impulse();
  void set_int(int v);
  void set_float(float v);
  void set_bool(bool v);
  void set_list(std::vector<opp::value> v);
  void set_string(std::string v);

#if defined(OSSIA_CPP_CXX11)
  value(value&& v);
  value& operator=(value&& v);
#endif
private:
  friend class node;
  value(const ossia::value& v);
  template<typename T>
  value(const T* v);
  ossia::value* m_val;
};

typedef void (*value_callback)(void*, const opp::value&);

class OSSIA_EXPORT node
{
public:
  node();
  node(const node&);
  node& operator=(const node&);
  ~node();

  bool valid() const;

  std::string get_name() const;
  void set_name(std::string s);

  std::string get_parameter() const;

  std::vector<node> get_children() const;

  node find_child(std::string addr);
  void remove_child(std::string addr);
  node create_child(std::string addr);

  node create_impulse(std::string addr);
  node create_int(std::string addr);
  node create_float(std::string addr);
  node create_bool(std::string addr);
  node create_list(std::string addr);
  node create_string(std::string addr);

  node create_buffer(std::string addr);
  node create_filepath(std::string addr);

  node create_rgb(std::string addr);
  node create_rgba(std::string addr);
  node create_argb(std::string addr);
  node create_argb8(std::string addr);
  node create_hsv(std::string addr);

  node create_cart2D(std::string addr);
  node create_cart3D(std::string addr);
  node create_polar(std::string addr);
  node create_spherical(std::string addr);
  node create_opengl(std::string addr);
  node create_cylindrical(std::string addr);

  node create_quaternion(std::string addr);
  node create_euler(std::string addr);
  node create_axis(std::string addr);

  node create_decibel(std::string addr);
  node create_midigain(std::string addr);

  node& set_value(opp::value v);
  opp::value get_value() const;
  opp::value fetch_value() const;
  void set_value_callback(value_callback c, void* context);

  node& set_min(opp::value min);
  opp::value get_min() const;

  node& set_max(opp::value max);
  opp::value get_max() const;

  node& set_accepted_values(std::vector<opp::value> v);
  std::vector<opp::value> get_accepted_values() const;

  node& set_unit(std::string v);
  std::string get_unit() const;

  node& set_access(access_mode v);
  access_mode get_access() const;

  node& set_bounding(bounding_mode v);
  bounding_mode get_bounding() const;

  node& set_default_value(value v);
  value get_default_value();

  node& set_description(std::string v);
  std::string get_description() const;

  node& set_tags(std::vector<std::string> v);
  std::vector<std::string> get_tags() const;

  node& set_instance_bounds(int min, int max);
  node& unset_instance_bounds();
  std::pair<int, int> get_instance_bounds() const;

  node& set_priority(float v);
  node& unset_priority();
  float get_priority();

  node& set_refresh_rate(int v);
  node& unset_refresh_rate();
  int get_refresh_rate();

  node& set_value_step_size(double v);
  node& unset_value_step_size();
  double get_value_step_size();

  node& set_hidden(bool v);
  bool get_hidden() const;

  node& set_disabled(bool v);
  bool get_disabled() const;

  node& set_critical(bool v);
  bool get_critical() const;

  bool get_zombie() const;

  node& set_muted(bool v);
  bool get_muted() const;

  node& set_repetition_filter(bool v);
  bool get_repetition_filter() const;

#if defined(OSSIA_CPP_CXX11)
  node(node&&) = default;
  node& operator=(node&&) = default;
#endif

private:
  friend class oscquery_mirror;
  friend class oscquery_server;
  node(ossia::net::node_base* b);
  node(ossia::net::node_base* b, ossia::net::parameter_base* addr);
  void init();
  void cleanup(const ossia::net::node_base&);
  void cleanup_parameter(const ossia::net::parameter_base&);

  ossia::net::node_base* m_node;
  ossia::net::parameter_base* m_addr;
};

class OSSIA_EXPORT oscquery_server
{
public:
  oscquery_server(std::string name, int wsPort = 5678, int oscPort = 1234);
  ~oscquery_server();
  node get_root_node() const;

private:
  ossia::net::device_base* m_dev;
};

class OSSIA_EXPORT oscquery_mirror
{
public:
  oscquery_mirror(std::string name, std::string host = "ws://127.0.0.1:5678");
  ~oscquery_mirror();

  node get_root_node() const;
  void refresh();
  void reconnect(std::string name, std::string host);

private:
  ossia::net::device_base* m_dev;
};
}
#endif
