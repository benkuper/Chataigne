#pragma once
#include <ossia/detail/destination_index.hpp>
#include <ossia/network/exceptions.hpp>
#include <ossia/network/value/value_base.hpp>
#include <ossia/network/common/parameter_properties.hpp>
#include <ossia/detail/string_view.hpp>
#include <limits>
#include <ossia_export.h>
#include <string>
#include <vector>

/**
 * \file value.hpp
 */
namespace fmt
{
template <typename T>
class BasicWriter;
}

namespace ossia
{
namespace detail
{
template <typename T>
struct dummy
{
  using type = T;
};
}

OSSIA_EXPORT std::string
to_pretty_string(const ossia::destination_index& index);

class value;

/*!
 * \brief getValueAsString Returns a string corresponding to the value
 * \param val a valid value
 * \return a string in the format : "type: value".
 *
 * ex. "int: 3"
 *     "string: tutu"
 *     "list: [ int: 2, float: 3 ]"
 * etc...
 *
 */
OSSIA_EXPORT std::string value_to_pretty_string(const ossia::value& val);
OSSIA_EXPORT ossia::value parse_pretty_value(ossia::string_view str);

#include <ossia/network/value/value_variant_impl.hpp>

using value_variant = value_variant_type;

/**
 * @brief The value class
 *
 * Core type of the ossia API.
 * A value is a variant of multiple fundamental types.
 *
 * A value can be in an "unset" state; it has to be checked for this.
 * e.g. :
 * \code
 * void f(ossia::value& v) {
 *   if(v.valid())
 *   {
 *     auto maybe_int = v.target<Int>();
 *     if(maybe_int)
 *       std::cout << maybe_int->value;
 *     else // we know for some reason that it is a float
 *       std::cout << v.get<Float>().value;
 *   }
 * }
 * \endcode
 *
 * A generic operation can be applied safely to a value with a visitor.
 * See for instance \ref value_to_pretty_string.
 */
class OSSIA_EXPORT value
{
public:
  using value_type = value_variant;

  value_type v;

  // Construction
  template <typename T>
  value(T*) = delete;
  value(const char* txt) : v{std::string(txt)}
  {
  }

  value(impulse val) noexcept : v{val}
  {
  }

  value(bool val) noexcept : v{val}
  {
  }
  value(int val) noexcept : v{val}
  {
  }
  value(long val) noexcept : v{(int)val}
  {
  }
  value(char val) noexcept : v{val}
  {
  }
  value(float val) noexcept : v{val}
  {
  }
  value(double val) noexcept : v{(float)val}
  {
  }
  value(const std::string& val) noexcept : v{val}
  {
  }
  value(const std::vector<ossia::value>& val) noexcept : v{val}
  {
  }
  value(std::array<float, 2> val) noexcept : v{val}
  {
  }
  value(std::array<float, 3> val) noexcept : v{val}
  {
  }
  value(std::array<float, 4> val) noexcept : v{val}
  {
  }

  explicit value(std::string&& val) noexcept : v{std::move(val)}
  {
  }
  explicit value(std::vector<ossia::value>&& val) noexcept : v{std::move(val)}
  {
  }

  template <typename T, typename... Args>
  value(detail::dummy<T> t, Args&&... args) noexcept
      : v{T{std::forward<Args>(args)...}}
  {
  }

  // To initialize a value directly with correct arguments
  template <typename T, typename... Args>
  static ossia::value make(Args&&... args) noexcept
  {
    return ossia::value{detail::dummy<T>{}, std::forward<Args>(args)...};
  }

  // Assignment
  value& operator=(ossia::impulse val) noexcept
  {
    v = val;
    return *this;
  }
  value& operator=(const char* c) noexcept
  {
    v = std::string(c);
    return *this;
  }
  value& operator=(bool val) noexcept
  {
    v = val;
    return *this;
  }
  value& operator=(int val) noexcept
  {
    v = val;
    return *this;
  }
  value& operator=(float val) noexcept
  {
    v = val;
    return *this;
  }
  value& operator=(char val) noexcept
  {
    v = val;
    return *this;
  }
  value& operator=(const std::string& val) noexcept
  {
    v = val;
    return *this;
  }
  value& operator=(const std::vector<ossia::value>& val) noexcept
  {
    v = val;
    return *this;
  }
  value& operator=(std::array<float, 2> val) noexcept
  {
    v = val;
    return *this;
  }
  value& operator=(std::array<float, 3> val) noexcept
  {
    v = val;
    return *this;
  }
  value& operator=(std::array<float, 4> val) noexcept
  {
    v = val;
    return *this;
  }

  value& operator=(std::string&& val) noexcept
  {
    v = std::move(val);
    return *this;
  }
  value& operator=(std::vector<ossia::value>&& val) noexcept
  {
    v = std::move(val);
    return *this;
  }

  value() noexcept
  {
  }
  ~value() noexcept;
  value(const value& other) noexcept : v{other.v}
  {
  }
  value(value&& other) noexcept : v{std::move(other.v)}
  {
  }
  value& operator=(const value& other) noexcept
  {
    v = other.v;
    return *this;
  }
  value& operator=(value&& other) noexcept
  {
    v = std::move(other.v);
    return *this;
  }

  operator value_type&()
  {
    return v;
  }
  operator const value_type&() const
  {
    return v;
  }

  // Operations
  template <typename T>
  const T& get() const
  {
    return v.get<T>();
  }

  template <typename T>
  T& get()
  {
    return v.get<typename std::remove_const<T>::type>();
  }

  template <typename T>
  const T* target() const noexcept
  {
    using type = typename std::remove_const<T>::type;
    static_assert(!std::is_same<type, ossia::value>::value, "");
    return v.target<type>();
  }

  template <typename T>
  T* target() noexcept
  {
    using type = typename std::remove_const<T>::type;
    static_assert(!std::is_same<type, ossia::value>::value, "");
    return v.target<type>();
  }

  ossia::val_type getType() const
  {
    auto t = v.which();
    if (t == v.npos)
    {
      throw ossia::invalid_value_type_error{
          "value::getType: "
          "called with no value"};
      return {};
    }

    return static_cast<ossia::val_type>(t);
  }

  bool valid() const noexcept
  {
    return bool(v);
  }

  void reset() noexcept
  {
    v = value_type{};
  }

  template <typename Visitor>
  auto apply(Visitor&& vis) -> decltype(auto)
  {
    return ossia::apply(std::forward<Visitor>(vis), this->v);
  }

  template <typename Visitor>
  auto apply(Visitor&& vis) const -> decltype(auto)
  {
    return ossia::apply(std::forward<Visitor>(vis), this->v);
  }

  friend OSSIA_EXPORT bool operator==(const value& lhs, const value& rhs);
  friend OSSIA_EXPORT bool operator!=(const value& lhs, const value& rhs);
  friend OSSIA_EXPORT bool operator>(const value& lhs, const value& rhs);
  friend OSSIA_EXPORT bool operator>=(const value& lhs, const value& rhs);
  friend OSSIA_EXPORT bool operator<(const value& lhs, const value& rhs);
  friend OSSIA_EXPORT bool operator<=(const value& lhs, const value& rhs);

  template <typename ostream_t>
  friend ostream_t& operator<<(ostream_t& os, const ossia::value& c)
  {
    // TODO OPTIMIZEME
    return os << value_to_pretty_string(c);
  }
  template <typename istream_t>
  friend istream_t& operator>>(istream_t& is, const ossia::value& c)
  {
    // TODO
    return is;
  }
};

inline ossia::value init_value(ossia::val_type type)
{
  switch (type)
  {
    case val_type::IMPULSE:
      return ossia::impulse{};
    case val_type::BOOL:
      return bool{};
    case val_type::INT:
      return int32_t{};
    case val_type::FLOAT:
      return float{};
    case val_type::CHAR:
      return char{};
    case val_type::STRING:
      return std::string{};
    case val_type::LIST:
      return std::vector<ossia::value>{};
    case val_type::VEC2F:
      return vec2f{};
    case val_type::VEC3F:
      return vec3f{};
    case val_type::VEC4F:
      return vec4f{};
    case val_type::NONE:
    default:
      break;
  }

  throw invalid_value_type_error("init_value: Invalid type");
  return {};
}

/**
 * @brief get_value_at_index
 *
 * for ossia::list{{1, 2}, 3} and index {}, return {{1, 2}, 3}.
 * for ossia::list{{1, 2}, 3} and index {0}, return {1, 2}.
 * for ossia::list{{1, 2}, 3} and index {0, 0}, return 1.
 * for ossia::list{{1, 2}, 3} and index {0, 1}, return 2.
 * for ossia::list{{1, 2}, 3} and index {0, 2}, return {}.
 * for ossia::list{{1, 2}, 3} and index {1}, return 3.
 * for ossia::list{{1, 2}, 3} and index {1, 0}, return {}.
 * for ossia::list{{1, 2}, 3} and index {2}, return {}.
 *
 * @see @ref destination_index
 * @return The value if it is found, else nothing.
 */
ossia::value get_value_at_index(
    const ossia::value& val, const ossia::destination_index& idx);
}

namespace std
{
OSSIA_EXPORT std::ostream&
operator<<(std::ostream& s, const std::vector<ossia::value>& list);
OSSIA_EXPORT std::ostream&
operator<<(std::ostream& s, const std::array<float, 2ul>& vec);
OSSIA_EXPORT std::ostream&
operator<<(std::ostream& s, const std::array<float, 3ul>& vec);
OSSIA_EXPORT std::ostream&
operator<<(std::ostream& s, const std::array<float, 4ul>& vec);
OSSIA_EXPORT std::ostream&
operator<<(std::ostream&, const std::vector<std::string>& list);
OSSIA_EXPORT std::istream&
operator>>(std::istream&, std::vector<std::string>& list);
}
