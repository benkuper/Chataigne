#pragma once
#include <ossia/detail/config.hpp>
#include <cmath>
/**
 * \file math.hpp
 *
 * This file contains various mathematical constants and utilities
 */

namespace ossia
{
#if defined(__ANDROID_API__)
using ::remainder;
using ::exp2;
using ::log1p;
using ::asinh;
using ::fma;
using ::llround;
#else
using std::remainder;
using std::exp2;
using std::log1p;
using std::asinh;
using std::fma;
using std::llround;
#endif
const OSSIA_DECL_RELAXED_CONSTEXPR auto pi
    = 3.141592653589793238462643383279502884;
const OSSIA_DECL_RELAXED_CONSTEXPR auto two_pi
    = 6.283185307179586476925286766559005768;
const OSSIA_DECL_RELAXED_CONSTEXPR auto half_pi
    = 1.570796326794896619231321691639751442;
const OSSIA_DECL_RELAXED_CONSTEXPR auto rad_to_deg
    = 5.729577951308232087679815481410517033e+01;
const OSSIA_DECL_RELAXED_CONSTEXPR auto deg_to_rad
    = 1.745329251994329576923690768488612713e-02;
const OSSIA_DECL_RELAXED_CONSTEXPR auto ln_2
    = 6.931471805599453094172321214581765680e-01;

template <typename T>
auto norm(T x, T y)
{
  return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
}

template <typename T>
auto norm(T x, T y, T z)
{
  return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}

/**
 * @brief clamp_min Returns the value bounded by a min
 */
template <class T>
OSSIA_INLINE OSSIA_DECL_RELAXED_CONSTEXPR T clamp_min(T d, const T min)
{
  return d < min ? min : d;
}

/**
 * @brief clamp_max Returns the value bounded by a max
 */
template <class T>
OSSIA_INLINE OSSIA_DECL_RELAXED_CONSTEXPR T clamp_max(T d, const T max)
{
  return d > max ? max : d;
}

/**
 * @brief clamp Returns the value bounded by a min and a max
 */
template <class T>
OSSIA_INLINE OSSIA_DECL_RELAXED_CONSTEXPR T
clamp(T d, const T min, const T max)
{
  return clamp_min(clamp_max(d, max), min);
}

// Wrap & Fold code taken from Jamoma TTLimits.h.
// Credits : Nils Peters, Nov. 2008
template <class T>
OSSIA_INLINE OSSIA_DECL_RELAXED_CONSTEXPR T
wrap(const T val, const T low, const T high)
{
  if ((val >= low) && (val < high))
    return val;
  else if (val >= low)
    return low + std::fmod(val - low, std::fabs(low - high));
  else
    return high - std::fmod(low - val, std::fabs(low - high));
}

template <class T>
OSSIA_INLINE OSSIA_DECL_RELAXED_CONSTEXPR T
fold(const T val, const T low, const T high)
{
  if ((val >= low) && (val <= high))
    return val;
  else
  {
    return low +
        std::fabs(
          ossia::remainder(val - low, 2. * std::fabs(low - high)));
  }
}
}
