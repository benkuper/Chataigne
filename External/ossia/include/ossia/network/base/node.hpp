#pragma once
#include <ossia/network/base/name_validation.hpp>
#include <ossia/network/common/parameter_properties.hpp>

#include <ossia/detail/any_map.hpp>
#include <ossia/detail/callback_container.hpp>
#include <ossia/detail/locked_container.hpp>
#include <ossia/detail/ptr_container.hpp>
#include <ossia/detail/string_view.hpp>

#include <ossia/detail/mutex.hpp>
#include <functional>
#include <memory>
#include <nano_signal_slot.hpp>
#include <ossia_export.h>
#include <string>
#if defined(OSSIA_QT)
class QString;
#endif
namespace ossia
{
namespace net
{
class device_base;
class parameter_base;
class node_base;
/**
 * @brief The node_base class
 *
 * Base class for nodes.
 * A \ref node_base is part of the hierarchy of a \ref device_base.
 *
 * The \ref node_base has ownership of its children.
 *
 * If a node is meant to send and receive data, an \ref parameter_base
 * should be created with node_base::create_parameter.
 *
 * Some device trees may provide immutable node hierarchies :
 * child nodes cannot be added nor removed.
 * This is the case for the \ref midi_device.
 * Other device trees allow the user to create nodes as he sees fit :
 * \ref generic_node is used for this common case.
 *
 * \see generic_node
 * \see device_base
 * \see parameter_base
 */
class OSSIA_EXPORT node_base
{
public:
  using children_t = std::vector<std::unique_ptr<node_base>>;
  node_base() = default;
  node_base(const node_base&) = delete;
  node_base(node_base&&) = delete;
  node_base& operator=(const node_base&) = delete;
  node_base& operator=(node_base&&) = delete;

  virtual ~node_base();

  //! The device in which this node is.
  virtual device_base& get_device() const = 0;

  //! Parent of this node. May be null if it is the device (i.e. root).
  virtual node_base* get_parent() const = 0;

  /**
   * \brief The name of this node, e.g. "foo".
   *
   * \see ossia::net::address_string_from_node
   */
  const std::string& get_name() const
  {
    return m_name;
  }
  virtual node_base& set_name(std::string) = 0;

  //! Allows a node to carry a value
  virtual parameter_base* create_parameter(val_type = val_type::IMPULSE) = 0;
  //! Default implementation does nothing
  virtual void set_parameter(std::unique_ptr<ossia::net::parameter_base>);
  virtual bool remove_parameter() = 0;
  virtual parameter_base* get_parameter() const = 0;

  /** Allows to add arbitrary key-value metadata to nodes.
   * There is a list of pre-defined attributes available in \ref
   * node_attributes.hpp
   */
  const extended_attributes& get_extended_attributes() const;
  void set_extended_attributes(const extended_attributes&);

  /** Get a specific attribute.
   * Usage :
   *
   * \code
   * node.setAttribute("my_int_attribute", 1234);
   * auto attr = node.getAttribute("my_int_attribute");
   * if(int* int_attr = ossia::any_cast<int>(&attr))
   * {
   *   // The attribute exists and is an int.
   * }
   * \endcode
   */
  ossia::any get_attribute(ossia::string_view str) const;

  template <typename T>
  void set(ossia::string_view str, const T& val);
  template <typename T>
  void set(ossia::string_view str, T&& val);

  template <typename T>
  void set(ossia::string_view str, const optional<T>& val);
  template <typename T>
  void set(ossia::string_view str, optional<T>&& val);

  void set(ossia::string_view str, bool value);


  template <typename Attribute, typename T>
  void set(Attribute a, const T& value);
  template <typename Attribute, typename T>
  void set(Attribute a, T& value);
  template <typename Attribute, typename T>
  void set(Attribute a, T&& value);

  /**
   * @brief create_child Adds a sub-child of the given name.
   *
   * @note The name of the child may be modified, so it should be checked after
   * creation.
   *
   * If you need to add multiple childs in one go (for instance
   * `/foo/bar/baz/blop` if this node
   * is `foo`), see ossia::net::find_or_create_node.
   *
   * @return A pointer to the child if it could be created, else nullptr.
   */
  node_base* create_child(std::string name);

  /**
   * @brief Adds a new child if it can be added.
   *
   * For instance if the name is already taken, it won't be added
   * and the returned pointer will be null.
   */
  node_base* add_child(std::unique_ptr<node_base>);

  /**
   * @brief Find a direct child of this node.
   *
   * e.g. `foo.findChild("bar")` will find `/foo/bar`, but not `/foo/blop/bar`.
   *
   * If you need to find a child recursively, see ossia::net::find_node.
   *
   */
  node_base* find_child(ossia::string_view name);
#if defined(OSSIA_QT)
  node_base* find_child(const QString& name);
#endif

  //! Return true if this node is parent of this children
  bool has_child(ossia::net::node_base&);

  bool remove_child(const std::string& name);
  bool remove_child(const node_base& name);

  //! Remove all the children.
  void clear_children();

  operator const extended_attributes&() const
  {
    return m_extended;
  }
  operator extended_attributes&()
  {
    return m_extended;
  }

  locked_container<const children_t> children() const
  {
    return {m_children, m_mutex};
  }

  //! Non mutex-protected version. With great powers, yada yada etc etc
  const auto& unsafe_children() const
  {
    return m_children;
  }

  //! Return a copy of the children vector to iterate without deadlocking.
  std::vector<node_base*> children_copy() const;

  //! A vector with all the names of the children.
  std::vector<std::string> children_names() const;

  //! If childrens are /foo, /bar, bar.1, returns true only for bar.
  bool is_root_instance(const ossia::net::node_base& child) const;

  //! The node subclasses must call this in their destructor.
  mutable Nano::Signal<void(const node_base&)> about_to_be_deleted;

protected:
  //! Should return nullptr if no child is to be added.
  virtual std::unique_ptr<node_base> make_child(const std::string& name) = 0;

  //! Reimplement for a specific removal action.
  virtual void removing_child(node_base& node_base) = 0;

  std::string m_name;
  children_t m_children;
  mutable shared_mutex_t m_mutex;
  extended_attributes m_extended{0};
};

}
}
