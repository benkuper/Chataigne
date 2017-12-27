#pragma once
#include <ossia/network/value/value.hpp>

#define FAST_COMPILES
namespace ossia
{
// For clamp, wrap, fold...
template <typename TernaryFun>
struct apply_ternary_fun_visitor
{
  template <typename T, typename U, typename V>
  OSSIA_INLINE ossia::value
  operator()(const T& val, const U& min, const V& max)
  {
    return val;
  }
#if !defined(FAST_COMPILES)
  template <typename U, typename V>
  OSSIA_INLINE ossia::value operator()(List&& val, const U& min, const V& max)
  {
    return std::move(val);
  }
  template <typename U, typename V>
  OSSIA_INLINE ossia::value
  operator()(const List& val, const U& min, const V& max)
  {
    return val;
  }
  template <typename U, typename V>
  OSSIA_INLINE ossia::value
  operator()(String&& val, const U& min, const V& max)
  {
    return std::move(val);
  }
  template <typename U, typename V>
  OSSIA_INLINE ossia::value
  operator()(const String& val, const U& min, const V& max)
  {
    return val;
  }
#endif

  OSSIA_INLINE ossia::value operator()(int32_t val, int32_t min, int32_t max)
  {
    return TernaryFun::compute(val, min, max);
  }
  OSSIA_INLINE ossia::value operator()(float val, float min, float max)
  {
    return TernaryFun::compute(val, min, max);
  }
  OSSIA_INLINE ossia::value operator()(char val, char min, char max)
  {
    return (char)TernaryFun::compute((int32_t)val, (int32_t)min, (int32_t)max);
  }
  OSSIA_INLINE ossia::value operator()(bool val, bool min, bool max)
  {
    return (bool)TernaryFun::compute((int32_t)val, (int32_t)min, (int32_t)max);
  }

  template <std::size_t N>
  ossia::value operator()(
      std::array<float, N> val, const std::array<float, N>& min,
      const std::array<float, N>& max)
  {
    for (std::size_t i = 0; i < N; i++)
    {
      val[i] = TernaryFun::compute(val[i], min[i], max[i]);
    }
    return val;
  }

  template <std::size_t N>
  ossia::value operator()(std::array<float, N> val, float min, float max)
  {
    for (std::size_t i = 0; i < N; i++)
    {
      val[i] = TernaryFun::compute(val[i], min, max);
    }
    return val;
  }

  // TODO handle clamping of List between two values of other types
  // (see apply_domain.hpp)
  ossia::value operator()(
      const std::vector<ossia::value>& incoming,
      const std::vector<ossia::value>& min,
      const std::vector<ossia::value>& max)
  {
    std::vector<ossia::value> val;
    const auto N = incoming.size();
    const auto nmin = min.size();
    const auto nmax = max.size();
    if (N == nmin && N == nmax)
    {
      val.reserve(N);
      for (std::size_t i = 0; i < N; i++)
      {
        val.push_back(ossia::apply(*this, incoming[i].v, min[i].v, max[i].v));
      }
    }
    return val;
  }

  ossia::value operator()(
      std::vector<ossia::value>&& val, const std::vector<ossia::value>& min,
      const std::vector<ossia::value>& max)
  {
    const auto N = val.size();
    const auto nmin = min.size();
    const auto nmax = max.size();
    if (N == nmin && N == nmax)
    {
      for (std::size_t i = 0; i < N; i++)
      {
        val[i] = ossia::apply(*this, std::move(val)[i].v, min[i].v, max[i].v);
      }
    }
    return std::move(val);
  }

  ossia::value operator()(
      const std::vector<ossia::value>& incoming, const ossia::value& min,
      const ossia::value& max)
  {
    std::vector<ossia::value> val;
    const auto N = incoming.size();
    val.reserve(N);
    for (std::size_t i = 0; i < N; i++)
    {
      val.push_back(ossia::apply(*this, incoming[i].v, min.v, max.v));
    }
    return val;
  }

  ossia::value operator()(
      std::vector<ossia::value>&& val, const ossia::value& min,
      const ossia::value& max)
  {
    const auto N = val.size();
    for (std::size_t i = 0; i < N; i++)
    {
      val[i] = ossia::apply(*this, std::move(val)[i].v, min.v, max.v);
    }
    return std::move(val);
  }

  ossia::value operator()(
      const ossia::value& val, const ossia::value& min,
      const ossia::value& max)
  {
    return ossia::apply(*this, val.v, min.v, max.v);
  }

  ossia::value operator()(
      ossia::value&& val, const ossia::value& min, const ossia::value& max)
  {
    return ossia::apply(*this, std::move(val.v), min.v, max.v);
  }
};

// For clamp_min, clamp_max...
template <typename BinaryFun>
struct apply_binary_fun_visitor
{
  template <typename T, typename U>
  OSSIA_INLINE ossia::value operator()(const T& val, const U& min)
  {
    return val;
  }

#if !defined(FAST_COMPILES)
  template <typename U>
  OSSIA_INLINE ossia::value operator()(List&& val, const U& min)
  {
    return std::move(val);
  }
  template <typename U>
  OSSIA_INLINE ossia::value operator()(const List& val, const U& min)
  {
    return val;
  }
  template <typename U>
  OSSIA_INLINE ossia::value operator()(String&& val, const U& min)
  {
    return std::move(val);
  }
  template <typename U>
  OSSIA_INLINE ossia::value operator()(const String& val, const U& min)
  {
    return val;
  }
#endif

  OSSIA_INLINE ossia::value operator()(int32_t val, int32_t min)
  {
    return int32_t{BinaryFun::compute(val, min)};
  }
  OSSIA_INLINE ossia::value operator()(float val, float min)
  {
    return float{BinaryFun::compute(val, min)};
  }
  OSSIA_INLINE ossia::value operator()(char val, char min)
  {
    return (char)BinaryFun::compute((int32_t)val, (int32_t)min);
  }
  OSSIA_INLINE ossia::value operator()(bool val, bool min)
  {
    return (bool)BinaryFun::compute((int32_t)val, (int32_t)min);
  }

  template <std::size_t N>
  ossia::value
  operator()(std::array<float, N> val, const std::array<float, N>& min)
  {
    for (std::size_t i = 0; i < N; i++)
    {
      val[i] = BinaryFun::compute(val[i], min[i]);
    }
    return val;
  }

  template <std::size_t N>
  ossia::value operator()(std::array<float, N> val, float min)
  {
    for (std::size_t i = 0; i < N; i++)
    {
      val[i] = BinaryFun::compute(val[i], min);
    }
    return val;
  }

  // TODO handle clamping of List between two values of other types
  // (see apply_domain.hpp)
  ossia::value operator()(
      const std::vector<ossia::value>& incoming,
      const std::vector<ossia::value>& min)
  {
    std::vector<ossia::value> val;
    const auto N = incoming.size();
    const auto nmin = min.size();
    if (N == nmin)
    {
      val.reserve(N);
      for (std::size_t i = 0; i < N; i++)
      {
        val.push_back(ossia::apply(*this, incoming[i].v, min[i].v));
      }
    }
    return val;
  }

  ossia::value operator()(
      std::vector<ossia::value>&& val, const std::vector<ossia::value>& min)
  {
    const auto N = val.size();
    const auto nmin = min.size();
    if (N == nmin)
    {
      for (std::size_t i = 0; i < N; i++)
      {
        val[i] = ossia::apply(*this, std::move(val)[i].v, min[i].v);
      }
    }
    return std::move(val);
  }

  ossia::value operator()(
      const std::vector<ossia::value>& incoming, const ossia::value& min)
  {
    std::vector<ossia::value> val;
    const auto N = incoming.size();
    val.reserve(N);
    for (std::size_t i = 0; i < N; i++)
    {
      val.push_back(ossia::apply(*this, incoming[i].v, min.v));
    }
    return val;
  }

  ossia::value
  operator()(std::vector<ossia::value>&& val, const ossia::value& min)
  {
    const auto N = val.size();
    for (std::size_t i = 0; i < N; i++)
    {
      val[i] = ossia::apply(*this, std::move(val)[i].v, min.v);
    }
    return std::move(val);
  }

  ossia::value operator()(const ossia::value& val, const ossia::value& min)
  {
    return ossia::apply(*this, val.v, min.v);
  }

  ossia::value operator()(ossia::value&& val, const ossia::value& min)
  {
    return ossia::apply(*this, std::move(val.v), min.v);
  }
};
}
