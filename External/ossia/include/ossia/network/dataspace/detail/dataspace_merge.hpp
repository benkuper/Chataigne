#pragma once
#include <ossia/network/dataspace/dataspace_visitors.hpp>
#include <ossia/network/dataspace/value_with_unit.hpp>
#include <ossia/network/value/value_conversion.hpp>
#include <ossia/network/value/value_traits.hpp>

namespace ossia
{
template <typename T>
T convert(float f)
{
  return f;
}

namespace detail
{

template <typename T>
struct is_array
{
  static const constexpr bool value
      = value_trait<std::remove_const_t<T>>::is_array;
};

template <typename T, typename U, typename = void>
struct whole_value_merger_helper;

template <typename T>
using strong_value_impl_t = decltype(T::dataspace_value);

template <typename T, typename U>
const constexpr bool both_iterable
    = is_array<strong_value_impl_t<T>>::value&& is_array<U>::value;
template <typename T, typename U>
const constexpr bool first_iterable
    = is_array<strong_value_impl_t<T>>::value && !is_array<U>::value;
template <typename T, typename U>
const constexpr bool second_iterable
    = !is_array<strong_value_impl_t<T>>::value && is_array<U>::value;
template <typename T, typename U>
const constexpr bool neither_iterable
    = !is_array<strong_value_impl_t<T>>::value && !is_array<U>::value;

template <typename T, typename U>
using enable_if_both_iterable = std::enable_if_t<both_iterable<T, U>>;
template <typename T, typename U>
using enable_if_first_iterable = std::enable_if_t<first_iterable<T, U>>;
template <typename T, typename U>
using enable_if_second_iterable = std::enable_if_t<second_iterable<T, U>>;
template <typename T, typename U>
using enable_if_neither_iterable = std::enable_if_t<neither_iterable<T, U>>;

// Case where both T and U are array types, e.g. RGB && List, or CMYK &&
// Vec2f...
template <typename T, typename U>
struct whole_value_merger_helper<T, U, enable_if_both_iterable<T, U>>
{
  ossia::value_with_unit operator()(T value_unit, const U& value)
  {
    // copy the most possible values. T's value will be fixed at compile time.
    const auto n = std::min(value_unit.dataspace_value.size(), value.size());
    for (std::size_t i = 0; i < n; i++)
    {
      value_unit.dataspace_value[i]
          = ossia::convert<std::remove_reference_t<decltype(
              value_unit.dataspace_value[i])>>(value[i]);
    }
    return value_unit;
  }
};

// Case "rgb" and "float" -> does not make sense, we return the input
template <typename T, typename U>
struct whole_value_merger_helper<T, U, enable_if_first_iterable<T, U>>
{
  OSSIA_INLINE ossia::value_with_unit operator()(T value_unit, const U& value)
  {
    return value_unit;
  }
};

// Case "centimeter" and "list" -> does not make sense, we return the input
template <typename T, typename U>
struct whole_value_merger_helper<T, U, enable_if_second_iterable<T, U>>
{
  OSSIA_INLINE ossia::value_with_unit operator()(T value_unit, const U& value)
  {
    return value_unit;
  }
};

// Case "centimeter" and "float"
template <typename T, typename U>
struct whole_value_merger_helper<T, U, enable_if_neither_iterable<T, U>>
{
  OSSIA_INLINE ossia::value_with_unit operator()(T value_unit, const U& value)
  {
    value_unit.dataspace_value = value;
    return value_unit;
  }
};

template <typename T, typename U, typename = void>
struct partial_value_merger_helper;

// T and U are both array types, and we want to merge a single element of U
// into T.
// This is only valid for destination_index with one depth level : e.g. vu[i]
// == v[i]
// since all units only have one depth level.
template <typename T, typename U>
struct partial_value_merger_helper<T, U, enable_if_both_iterable<T, U>>
{
  ossia::value_with_unit
  operator()(T value_unit, const U& value, const ossia::destination_index& idx)
  {
    auto i = idx[0];
    if (value_unit.dataspace_value.size() > i && value.size() > i)
    {
      value_unit.dataspace_value[i]
          = ossia::convert<std::remove_reference_t<decltype(
              value_unit.dataspace_value[i])>>(value[i]);
    }

    return value_unit;
  }

  template <std::size_t N>
  ossia::value_with_unit
  operator()(T value_unit, const U& value, const std::bitset<N>& idx)
  {
    if (handle_vec(value_unit.dataspace_value, value, idx))
      return value_unit;
    return {};
  }

  template <std::size_t N>
  bool handle_vec(
      std::array<float, N>& src, const std::array<float, N>& incoming,
      const std::bitset<N>& idx)
  {
    for (std::size_t i = 0; i < N; i++)
    {
      if (idx.test(i))
      {
        src[i] = incoming[i];
      }
    }
    return true;
  }

  template <typename... Args>
  OSSIA_INLINE bool handle_vec(Args&&...)
  {
    return false;
  }
};

template <typename T, typename U>
struct partial_value_merger_helper<T, U, enable_if_first_iterable<T, U>>
{
  ossia::value_with_unit
  operator()(T value_unit, const U& value, const ossia::destination_index& idx)
  {
    auto i = idx[0];
    value_unit.dataspace_value[i]
        = ossia::convert<std::remove_reference_t<decltype(
            value_unit.dataspace_value[i])>>(value);
    return value_unit;
  }

  template <std::size_t N>
  OSSIA_INLINE ossia::value_with_unit
  operator()(T value_unit, const U& value, const std::bitset<N>& idx)
  {
    return {};
  }
};

template <typename T, typename U>
struct partial_value_merger_helper<T, U, enable_if_second_iterable<T, U>>
{
  OSSIA_INLINE ossia::value_with_unit
  operator()(T value_unit, const U& value, const ossia::destination_index& idx)
  {
    return value_unit;
  }

  template <std::size_t N>
  OSSIA_INLINE ossia::value_with_unit
  operator()(T value_unit, const U& value, const std::bitset<N>& idx)
  {
    return {};
  }
};

template <typename T, typename U>
struct partial_value_merger_helper<T, U, enable_if_neither_iterable<T, U>>
{
  OSSIA_INLINE ossia::value_with_unit
  operator()(T value_unit, const U& value, const ossia::destination_index& idx)
  {
    return value_unit;
  }

  template <std::size_t N>
  OSSIA_INLINE ossia::value_with_unit
  operator()(T value_unit, const U& value, const std::bitset<N>& idx)
  {
    return {};
  }
};

struct value_merger
{
  const ossia::destination_index& index;

  template <typename T, typename U>
  ossia::value_with_unit
  operator()(const strong_value<T>& value_unit, const U& value)
  {
    auto n = index.size();
    switch (n)
    {
      case 0:
        return whole_value_merger_helper<strong_value<T>, U>{}(
            value_unit, value);
      case 1:
        // The index is where the value should go
        return partial_value_merger_helper<strong_value<T>, U>{}(
            value_unit, value, index);
      default:
        // Cannot merge anything
        return value_unit;
    }
  }

  template <typename T>
  OSSIA_INLINE ossia::value_with_unit
  operator()(const strong_value<T>& value_unit, impulse value)
  {
    return value_unit;
  }
  template <typename T>
  OSSIA_INLINE ossia::value_with_unit
  operator()(const strong_value<T>& value_unit, const std::string& value)
  {
    return value_unit;
  }
};

template <std::size_t N>
struct vec_value_merger
{
  const std::bitset<N>& index;

  template <typename T, typename U>
  OSSIA_INLINE ossia::value_with_unit
  operator()(const strong_value<T>& value_unit, const U& value)
  {
    return {};
  }

  template <typename T>
  ossia::value_with_unit operator()(
      const strong_value<T>& value_unit, const std::array<float, N>& value)
  {
    if (index.all())
      return whole_value_merger_helper<strong_value<T>, std::array<float, N>>{}(
          value_unit, value);
    else if (index.any())
      return partial_value_merger_helper<strong_value<T>, std::array<float, N>>{}(
          value_unit, value, index);
    else
      return value_unit;
  }
};
}
}
