#pragma once
#include <ossia/detail/mutex.hpp>
#include <ossia/detail/optional.hpp>
#include <ossia/detail/string_map.hpp>

namespace ossia
{
// MOVEME
template <typename T>
struct locked_map
{
public:
  using map_type = T;
  using key_type = typename map_type::key_type;
  using mapped_type = typename map_type::mapped_type;
  using value_type = typename map_type::value_type;

  optional<mapped_type> find(const key_type& path) const
  {
    lock_t lock(m_mutex);
    auto it = m_map.find(path);
    if (it != m_map.end())
    {
      return it.value();
    }
    else
    {
      return ossia::none;
    }
  }

  optional<mapped_type> find_and_take(const key_type& path)
  {
    lock_t lock(m_mutex);
    auto it = m_map.find(path);
    if (it != m_map.end())
    {
      auto val = std::move(it.value());
      m_map.erase(it);
      return std::move(val);
    }
    else
    {
      return ossia::none;
    }
  }

  void insert(const value_type& m)
  {
    lock_t lock(m_mutex);
    m_map.insert(m);
  }

  void insert(value_type&& m)
  {
    lock_t lock(m_mutex);
    m_map.insert(std::move(m));
  }

  void erase(const key_type& m)
  {
    lock_t lock(m_mutex);
    m_map.erase(m);
  }

private:
  mutable mutex_t m_mutex;
  map_type m_map;
};

namespace net
{
class parameter_base;
using listened_parameters = locked_map<string_map<ossia::net::parameter_base*>>;
}
}
