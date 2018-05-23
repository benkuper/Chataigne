#pragma once
#include <ossia/network/value/value.hpp>

#include <ossia/network/base/parameter.hpp>
#include <ossia/network/base/node.hpp>

namespace ossia
{

namespace comparisons
{
struct Impulse_T
{
  template <typename T>
  friend bool operator==(const T&, Impulse_T)
  {
    return true;
  }
  template <typename T>
  friend bool operator!=(const T&, Impulse_T)
  {
    return false;
  }
  template <typename T>
  friend bool operator<=(const T&, Impulse_T)
  {
    return true;
  }
  template <typename T>
  friend bool operator>=(const T&, Impulse_T)
  {
    return true;
  }
  template <typename T>
  friend bool operator<(const T&, Impulse_T)
  {
    return false;
  }
  template <typename T>
  friend bool operator>(const T&, Impulse_T)
  {
    return false;
  }
};
struct String_T
{
  template <typename T>
  friend bool operator==(const T&, String_T)
  {
    return false;
  }
  template <typename T>
  friend bool operator!=(const T&, String_T)
  {
    return true;
  }
  template <typename T>
  friend bool operator<=(const T&, String_T)
  {
    return false;
  }
  template <typename T>
  friend bool operator>=(const T&, String_T)
  {
    return false;
  }
  template <typename T>
  friend bool operator<(const T&, String_T)
  {
    return false;
  }
  template <typename T>
  friend bool operator>(const T&, String_T)
  {
    return false;
  }
};

struct NumericValue
{
  template <typename T, typename Fun>
  static bool apply(const T& lhs, const ossia::value& val, Fun fun)
  {
    struct visitor
    {
      const T& lhs;
      Fun fun;

    public:
      bool operator()(impulse) const
      {
        return fun(lhs, Impulse_T{});
      }
      bool operator()(int32_t v) const
      {
        return fun(lhs, v);
      }
      bool operator()(float v) const
      {
        return fun(lhs, v);
      }
      bool operator()(bool v) const
      {
        return fun(lhs, v);
      }
      bool operator()(char v) const
      {
        return fun(lhs, v);
      }
      bool operator()(const std::vector<ossia::value>& v) const
      {
        return (v.size() == 1) && (fun(lhs, v[0]));
      }

      bool operator()(const std::string& v) const
      {
        return fun(lhs, String_T{});
      }
      bool operator()(vec2f v) const
      {
        return false;
      }
      bool operator()(vec3f v) const
      {
        return false;
      }
      bool operator()(vec4f v) const
      {
        return false;
      }

      bool operator()() const
      {
        return false;
      }

    } vis{lhs, fun};

    return val.apply(vis);
  }
};

struct StringValue
{
  template <typename Fun>
  static bool apply(const std::string& lhs, const ossia::value& val, Fun fun)
  {
    struct visitor
    {
      const std::string& lhs;
      Fun fun;

    public:
      bool operator()(impulse) const
      {
        return fun(lhs, Impulse_T{});
      }
      bool operator()(const std::string& v) const
      {
        return fun(lhs, v);
      }
      bool operator()(int32_t v) const
      {
        return fun(v, String_T{});
      }
      bool operator()(float v) const
      {
        return fun(v, String_T{});
      }
      bool operator()(bool v) const
      {
        return fun(v, String_T{});
      }
      bool operator()(char v) const
      {
        return fun(v, String_T{});
      }
      bool operator()(const std::vector<ossia::value>& v) const
      {
        return (v.size() == 1) && (fun(lhs, v[0]));
      }
      bool operator()(vec2f v) const
      {
        return fun(v, String_T{});
      }
      bool operator()(vec3f v) const
      {
        return fun(v, String_T{});
      }
      bool operator()(vec4f v) const
      {
        return fun(v, String_T{});
      }

      bool operator()() const
      {
        return false;
      }

    } vis{lhs, fun};

    return val.apply(vis);
  }
};

template <typename Fun>
struct ListVisitor
{
  const std::vector<ossia::value>& lhs;
  const ossia::value& rhs;
  Fun fun;

public:
  bool operator()(impulse) const
  {
    return fun(lhs, Impulse_T{});
  }
  bool operator()(const std::vector<ossia::value>& t) const
  {
    if (lhs.size() != t.size())
      return false;

    bool result = true;
    auto tit = t.begin();
    for (const auto& val : lhs)
    {
      result &= fun(val, *tit);
      if (!result)
        break;
      tit++;
    }

    return result;
  }

  template <typename T>
  bool operator()(const T& v) const
  {
    if (lhs.size() == 1)
      return fun(lhs[0], rhs);

    return false;
  }

  bool operator()() const
  {
    return false;
  }
};
template <typename Fun>
auto make_list_visitor(
    const std::vector<ossia::value>& lhs, const ossia::value& val, Fun f)
{
  return ListVisitor<Fun>{lhs, val, f};
}

struct ListValue
{
  template <typename Fun>
  static bool
  apply(const std::vector<ossia::value>& lhs, const ossia::value& val, Fun fun)
  {
    auto vis = make_list_visitor(lhs, val, fun);

    return val.apply(vis);
  }
};

template <typename Fun>
struct DestinationVisitor
{
  const destination& lhs;
  const ossia::value& rhs;
  Fun fun;

public:
  bool operator()(impulse) const
  {
    return fun(lhs.value.get(), Impulse_T{});
  }

  template <typename T>
  bool operator()(const T& v) const
  {
    return fun(lhs.address().value(lhs.index), rhs);
  }

  bool operator()() const
  {
    return false;
  }
};

template <typename Fun>
auto make_destination_visitor(
    const destination& lhs, const ossia::value& val, Fun f)
{
  return DestinationVisitor<Fun>{lhs, val, f};
}

struct DestinationValue
{
  template <typename Fun>
  static bool apply(const destination& lhs, const ossia::value& val, Fun fun)
  {
    return val.apply(make_destination_visitor(lhs, val, fun));
  }
};

template <std::size_t N, typename Fun>
struct VecVisitor
{
  const std::array<float, N>& lhs;
  Fun fun;

public:
  bool operator()(impulse) const
  {
    return fun(lhs, Impulse_T{});
  }
  bool operator()(const std::array<float, N>& d) const
  {
    return fun(lhs, d);
  }

  template <typename T>
  bool operator()(const T& v) const
  {
    return false;
  }

  bool operator()() const
  {
    return false;
  }
};

template <typename Vec_T, typename Fun>
auto make_vec_visitor(const Vec_T& lhs, Fun f)
{
  return VecVisitor<Vec_T::size_value, Fun>{lhs, f};
}

struct VecValue
{
  template <typename Vec_T, typename Fun>
  static bool apply(const Vec_T& lhs, const ossia::value& val, Fun fun)
  {
    return val.apply(make_vec_visitor(lhs, fun));
  }
};
}
/*
template <typename T, std::size_t N>
OSSIA_EXPORT bool Vec<T, N>::operator==(const ossia::value& v) const
{
  return comparisons::VecValue::apply(*this, v, std::equal_to<>{});
}
template <typename T, std::size_t N>
OSSIA_EXPORT bool Vec<T, N>::operator!=(const ossia::value& v) const
{
  return !comparisons::VecValue::apply(*this, v, std::equal_to<>{});
}

template <typename T, std::size_t N>
OSSIA_EXPORT bool Vec<T, N>::operator>(const ossia::value& v) const
{
  return comparisons::VecValue::apply(*this, v, std::greater<>{});
}
template <typename T, std::size_t N>
OSSIA_EXPORT bool Vec<T, N>::operator>=(const ossia::value& v) const
{
  return comparisons::VecValue::apply(*this, v, std::greater_equal<>{});
}

template <typename T, std::size_t N>
OSSIA_EXPORT bool Vec<T, N>::operator<(const ossia::value& v) const
{
  return comparisons::VecValue::apply(*this, v, std::less<>{});
}
template <typename T, std::size_t N>
OSSIA_EXPORT bool Vec<T, N>::operator<=(const ossia::value& v) const
{
  return comparisons::VecValue::apply(*this, v, std::less_equal<>{});
}
*/
}
