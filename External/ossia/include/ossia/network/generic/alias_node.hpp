#pragma once
#include <ossia/network/generic/generic_node.hpp>
#include <ossia/network/base/parameter.hpp>
#include <ossia/network/common/path.hpp>
#include <ossia/network/domain/domain_base.hpp>

namespace ossia
{
namespace net
{

/**
 * @brief A type of node that refers to another node
 *
 * Get and set the source with get_origin, set_origin.
 */
class alias_node final : public generic_node_base
{
public:
  using generic_node_base::generic_node_base;

  ~alias_node();

  ossia::net::node_base* get_origin() const;
  void set_origin(ossia::net::node_base* o);

  parameter_base* create_parameter(val_type v) override;
  bool remove_parameter() override;
  parameter_base* get_parameter() const override;

protected:
  std::unique_ptr<node_base> make_child(const std::string& name) override;
  void removing_child(node_base&) override;

private:
  void on_deletion(const ossia::net::node_base& orig);

  ossia::net::node_base* m_origin{};
};

class OSSIA_EXPORT alias_path final :
    public generic_node_base,
    public ossia::net::parameter_base
{
public:
    alias_path(
        std::string name,
        ossia::net::device_base& aDevice,
        ossia::net::node_base& parent);

  ~alias_path();

  const std::vector<ossia::net::node_base*>& get_roots();
  void set_roots(std::vector<ossia::net::node_base*> roots);

  traversal::path get_path() const;
  void set_path(traversal::path o);

private:
  parameter_base* create_parameter(val_type v) override;
  bool remove_parameter() override;
  parameter_base* get_parameter() const override;

  std::unique_ptr<node_base> make_child(const std::string& name) override;
  void removing_child(node_base&) override;

  void pull_value() override;

  std::future<void> pull_value_async() override;
  void request_value() override;
  ossia::value value() const override;
  parameter_base&push_value(const ossia::value&) override;
  parameter_base&push_value(ossia::value&&) override;
  parameter_base&push_value() override;
  parameter_base&set_value(const ossia::value&) override;
  parameter_base&set_value(ossia::value&&) override;
  void set_value_quiet(const ossia::value& v) override;
  void set_value_quiet(ossia::value&& v) override;
  val_type get_value_type() const override;
  parameter_base&set_value_type(val_type) override;
  access_mode get_access() const override;
  parameter_base&set_access(access_mode) override;
  const domain&get_domain() const override;
  parameter_base&set_domain(const domain&) override;
  bounding_mode get_bounding() const override;
  parameter_base&set_bounding(bounding_mode) override;

  template<typename Fun>
  void do_for_nodes(Fun f);

  std::vector<ossia::net::node_base*> m_roots;
  traversal::path m_path;
};
}
}
