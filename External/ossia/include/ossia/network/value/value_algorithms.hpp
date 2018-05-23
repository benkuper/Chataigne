#pragma once
#include <ossia/network/value/value.hpp>

namespace ossia
{
// SourcePrecedence causes the merging of values
// to occur only if the destination value is null
// (it won't replace an existing value).

template <bool SourcePrecedence = true>
struct value_merger
{
  template <typename Value_T>
  static void merge_value(ossia::value& dest, Value_T&& src)
  {
    if (!dest.valid())
    {
      dest = src;
      return;
    }
    else
    {
      if (dest.valid() && src.valid())
      {
        switch (src.getType())
        {
          case ossia::val_type::LIST:
          {
            auto& src_vec = src.template get<std::vector<ossia::value>>();
            switch (dest.getType())
            {
              case ossia::val_type::LIST:
                merge_list(dest.get<std::vector<ossia::value>>(), src_vec);
                break;
              case ossia::val_type::VEC2F:
                merge_list(dest.get<ossia::vec2f>(), src_vec);
                break;
              case ossia::val_type::VEC3F:
                merge_list(dest.get<ossia::vec3f>(), src_vec);
                break;
              case ossia::val_type::VEC4F:
                merge_list(dest.get<ossia::vec4f>(), src_vec);
                break;
              default:
                break;
            }
            break;
          }
          case ossia::val_type::VEC2F:
          {
            auto& src_vec = src.template get<ossia::vec2f>();
            switch (dest.getType())
            {
              case ossia::val_type::LIST:
                merge_list(dest.get<std::vector<ossia::value>>(), src_vec);
                break;
              case ossia::val_type::VEC2F:
                merge_list(dest.get<ossia::vec2f>(), src_vec);
                break;
              case ossia::val_type::VEC3F:
                merge_list(dest.get<ossia::vec3f>(), src_vec);
                break;
              case ossia::val_type::VEC4F:
                merge_list(dest.get<ossia::vec4f>(), src_vec);
                break;
              default:
                break;
            }
            break;
          }
          case ossia::val_type::VEC3F:
          {
            auto& src_vec = src.template get<ossia::vec3f>();
            switch (dest.getType())
            {
              case ossia::val_type::LIST:
                merge_list(dest.get<std::vector<ossia::value>>(), src_vec);
                break;
              case ossia::val_type::VEC2F:
                merge_list(dest.get<ossia::vec2f>(), src_vec);
                break;
              case ossia::val_type::VEC3F:
                merge_list(dest.get<ossia::vec3f>(), src_vec);
                break;
              case ossia::val_type::VEC4F:
                merge_list(dest.get<ossia::vec4f>(), src_vec);
                break;
              default:
                break;
            }
            break;
          }
          case ossia::val_type::VEC4F:
          {
            auto& src_vec = src.template get<ossia::vec4f>();
            switch (dest.getType())
            {
              case ossia::val_type::LIST:
                merge_list(dest.get<std::vector<ossia::value>>(), src_vec);
                break;
              case ossia::val_type::VEC2F:
                merge_list(dest.get<ossia::vec2f>(), src_vec);
                break;
              case ossia::val_type::VEC3F:
                merge_list(dest.get<ossia::vec3f>(), src_vec);
                break;
              case ossia::val_type::VEC4F:
                merge_list(dest.get<ossia::vec4f>(), src_vec);
                break;
              default:
                break;
            }
            break;
          }
          default:
          {
            switch (dest.getType())
            {
              case ossia::val_type::LIST:
                set_first_value(
                    dest.get<std::vector<ossia::value>>(),
                    std::forward<Value_T>(src));
                break;
              case ossia::val_type::VEC2F:
                set_first_value(
                    dest.get<ossia::vec2f>(), std::forward<Value_T>(src));
                break;
              case ossia::val_type::VEC3F:
                set_first_value(
                    dest.get<ossia::vec3f>(), std::forward<Value_T>(src));
                break;
              case ossia::val_type::VEC4F:
                set_first_value(
                    dest.get<ossia::vec4f>(), std::forward<Value_T>(src));
                break;
              default:
              {
                // src overwrites dest
                if (SourcePrecedence)
                  dest = std::forward<Value_T>(src);
                return;
              }
            }
            break;
          }
        }
        return;
      }
      else
      {
        if (auto dest_list_ptr = dest.target<std::vector<ossia::value>>())
        {
          // Merge a single value in a list
          set_first_value(*dest_list_ptr, std::forward<Value_T>(src));
          return;
        }
        else if (
            auto src_list_ptr
            = src.template target<std::vector<ossia::value>>())
        {
          // If one of the two values is invalid, we always keep the other
          if (src_list_ptr->empty())
          {
            std::vector<ossia::value> t{dest};
            dest = std::move(t);
            return;
          }
          else if (!(*src_list_ptr)[0].valid())
          {
            std::vector<ossia::value> t = *src_list_ptr;
            t[0] = dest;
            dest = std::move(t);
            return;
          }
          else
          {
            // src overwrites dest
            if (src.valid() && SourcePrecedence)
              dest = std::forward<Value_T>(src);
            return;
          }
        }
        else
        {
          // src overwrites dest
          if (src.valid() && SourcePrecedence)
            dest = std::forward<Value_T>(src);
          return;
        }
      }
    }
  }

  template <typename Value_T>
  static void insert_in_list(
      std::vector<ossia::value>& t, Value_T&& v,
      const ossia::destination_index& idx)
  {
    std::vector<ossia::value>* cur_ptr = &t;
    for (auto it = idx.begin(); it != idx.end();)
    {
      auto& cur = *cur_ptr;
      std::size_t pos = *it;
      if (cur.size() < pos + 1)
      {
        cur.resize(pos + 1);
      }

      if (++it == idx.end())
      {
        // We're at the deepest index position :
        // we add the value at the current place.
        merge_value(cur[pos], std::forward<Value_T>(v));
      }
      else
      {
        // We go through another depth layer.
        if (auto sub_list = cur[pos].target<std::vector<ossia::value>>())
        {
          cur_ptr = sub_list;
        }
        else
        {
          // We put the current value at cur[pos] at index 0 of the
          // newly-created sub-list.
          std::vector<ossia::value> sub{std::move(cur[pos])};
          cur[pos] = std::move(sub);

          // And use it for the next iteration
          cur_ptr = cur[pos].target<std::vector<ossia::value>>();
        }
      }
    }
  }

  static void write_float(const ossia::value& val, float& f)
  {
    if (val.valid())
    {
      switch (val.getType())
      {
        case ossia::val_type::INT:
          f = (float)val.template get<int>();
          break;
        case ossia::val_type::FLOAT:
          f = (float)val.template get<float>();
          break;
        case ossia::val_type::CHAR:
          f = (float)val.template get<char>();
          break;
        case ossia::val_type::BOOL:
          f = (float)val.template get<bool>();
          break;
        default:
          break;
      }
    }
  }

  template <std::size_t N, typename Value_T>
  static void set_first_value(std::array<float, N>& t, Value_T&& val)
  {
    write_float(val, t[0]);
  }

  template <typename Value_T>
  static void set_first_value(std::vector<ossia::value>& t, Value_T&& val)
  {
    if (t.empty())
    {
      t.push_back(std::forward<Value_T>(val));
    }
    else
    {
      merge_value(t[0], std::forward<Value_T>(val));
    }
  }

  static void merge_list(
      std::vector<ossia::value>& lhs, const std::vector<ossia::value>& rhs)
  {
    std::size_t n = rhs.size();
    if (lhs.size() < n)
    {
      lhs.resize(n);
    }

    for (std::size_t i = 0u; i < n; i++)
    {
      merge_value(lhs[i], rhs[i]);
    }
  }
  static void
  merge_list(std::vector<ossia::value>& lhs, std::vector<ossia::value>&& rhs)
  {
    std::size_t n = rhs.size();
    if (lhs.size() < n)
    {
      lhs.resize(n);
    }

    for (std::size_t i = 0u; i < n; i++)
    {
      merge_value(lhs[i], std::move(rhs)[i]);
    }
  }

  template <std::size_t N>
  static void
  merge_list(std::vector<ossia::value>& lhs, const std::array<float, N>& rhs)
  {
    if (lhs.size() < N)
    {
      lhs.resize(N);
    }

    for (std::size_t i = 0u; i < N; i++)
    {
      lhs[i] = rhs[i];
    }
  }

  template <std::size_t N>
  static void
  merge_list(std::array<float, N>& lhs, const std::vector<ossia::value>& rhs)
  {
    const std::size_t n = std::min(N, rhs.size());
    for (std::size_t i = 0u; i < n; i++)
    {
      write_float(rhs[i], lhs[i]);
    }
  }

  template <std::size_t N, std::size_t M>
  static void
  merge_list(std::array<float, N>& lhs, const std::array<float, M>& rhs)
  {
    const std::size_t n = std::min(N, M);
    for (std::size_t i = 0u; i < n; i++)
    {
      lhs[i] = rhs[i];
    }
  }
};

namespace detail
{
/**
 * @brief The destination_index_retriever struct
 * Get the value associated with an index in a list.
 * If the index cannot be reached, an exception is thrown.
 *
 * @todo testme
 */
struct destination_index_retriever
{
  const ossia::destination_index& index;
  ossia::destination_index::const_iterator it;

  ossia::value operator()(const std::vector<ossia::value>& t)
  {
    if (it == index.end())
    {
      return t;
    }
    else if (t.size() > *it)
    {
      auto& val = t[*it];
      ++it;
      return val.apply(*this);
    }
    else
    {
      return {};
    }
  }

  template <typename T>
  ossia::value operator()(const T& t)
  {
    if (it == index.end())
    {
      return t;
    }
    else
    {
      return {};
    }
  }

  template <std::size_t N>
  ossia::value operator()(const std::array<float, N>& t)
  {
    if (it == index.end())
    {
      return t;
    }
    else if (t.size() > *it)
    {
      if (it + 1 == index.end())
        return float{t[*it]};
    }

    return {};
  }

  ossia::value operator()()
  {
    return {};
  }
};
}
}
