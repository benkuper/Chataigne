#pragma once
#include <ossia/detail/math.hpp>
#include <cmath>
#include <ossia_export.h>

/**
  \file easing.hpp
  this file contains the implementation of various easing functions, to
  use in \ref automation or \ref mapper.

  Easing functions
  by Kristoffer Gronlund, 2014
  Public domain
  This work is a spiritual descendent (not to say derivative work) of works
  done
  by the following individuals:
  Warren Moore (https://github.com/warrenm)
  Robert Penner (http://www.robertpenner.com/easing/)
  George McGinley Smith (http://gsgd.co.uk/sandbox/jquery/easing/)
  James Padolsey (http://james.padolsey.com/demos/jquery/easing/)
  Authors of jQuery (http://plugins.jquery.com/project/Easing)
  Matt Gallagher
  (http://cocoawithlove.com/2008/09/parametric-acceleration-curves-in-core.html)
  Jesse Crossen
  (http://stackoverflow.com/questions/5161465/how-to-create-custom-easing-function-with-core-animation)
*/

namespace ossia
{
namespace easing
{

struct ease
{
  template <typename T, typename U, typename V>
  constexpr T operator()(T a, U b, V t) const
  {
    return ossia::fma(t, b, ossia::fma(-t, a, a));
  }
};

struct linear
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return t;
  }
};

struct quadraticIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::pow(t, 2);
  }
};

struct quadraticOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return -(t * (t - 2.));
  }
};

struct quadraticInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t < 0.5) ? 2. * t * t : (-2. * t * t) + (4. * t) - 1.;
  }
};

struct cubicIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::pow(t, 3);
  }
};

struct cubicOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::pow(t - 1., 3) + 1.;
  }
};

struct cubicInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t < T(0.5)) ? 4. * std::pow(t, 3)
                        : 0.5 * std::pow((2. * t) - 2, 3) + 1.;
  }
};

struct quarticIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::pow(t, 4);
  }
};

struct quarticOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::pow(t - 1., 3) * (1. - t) + 1.;
  }
};

struct quarticInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t < 0.5) ? 8. * std::pow(t, 4) : -8. * std::pow(t - 1., 4) + 1.;
  }
};

struct quinticIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::pow(t, 5);
  }
};

struct quinticOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::pow(t - 1., 5) + 1.;
  }
};

struct quinticInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t < 0.5) ? 16. * std::pow(t, 5)
                     : 0.5 * std::pow((2. * t) - 2., 5) + 1.;
  }
};

struct sineIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::sin((t - 1.) * half_pi) + 1.;
  }
};

struct sineOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::sin(t * half_pi);
  }
};

struct sineInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return 0.5 * (1. - std::cos(t * pi));
  }
};

struct circularIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return 1. - std::sqrt(1. - (t * t));
  }
};

struct circularOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::sqrt((2. - t) * t);
  }
};

struct circularInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t < 0.5)
               ? 0.5 * (1 - std::sqrt(1 - 4. * (t * t)))
               : 0.5 * (std::sqrt(-((2. * t) - 3.) * ((2. * t) - 1.)) + 1.);
  }
};

struct exponentialIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t <= 0) ? t : ossia::exp2(10. * (t - 1.));
  }
};

struct exponentialOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t >= 1.) ? t : 1. - ossia::exp2(-10. * t);
  }
};

struct exponentialInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t <= 0. || t >= 1.)
               ? t
               : (t < 0.5) ? 0.5 * ossia::exp2((20. * t) - 10.)
                           : -0.5 * ossia::exp2((-20. * t) + 10.) + 1.;
  }
};

struct elasticIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::sin(13. * half_pi * t) * ossia::exp2(10. * (t - 1.));
  }
};

struct elasticOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return sin(-13. * half_pi * (t + 1.)) * ossia::exp2(-10. * t) + 1.;
  }
};

struct elasticInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t < 0.5)
               ? 0.5 * std::sin(13. * half_pi * (2. * t))
                     * ossia::exp2(10. * ((2. * t) - 1.))
               : 0.5 * (std::sin(-13. * half_pi * ((2. * t - 1) + 1))
                            * ossia::exp2(-10. * (2. * t - 1.))
                        + 2.);
  }
};

struct backIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return std::pow(t, 3) - t * std::sin(t * pi);
  }
};

struct backOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return 1. - (std::pow(1. - t, 3) - (1. - t) * std::sin((1. - t) * pi));
  }
};

struct backInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return (t < 0.5)
               ? 0.5 * (std::pow(2. * t, 3)
                        - (2. * t) * std::sin((2. * t) * pi))
               : 0.5 * (1. - (std::pow(1. - (2. * t - 1.), 3)
                              - (1. - (2. * t - 1.))
                                    * std::sin((1. - (2. * t - 1.)) * pi)))
                     + 0.5;
  }
};

struct bounceOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return t < 4. / 11.
               ? (121. * t * t) / 16.
               : (t < 8. / 11.)
                     ? (363. / 40. * t * t) - (99 / 10. * t) + 17 / 5.
                     : (t < 9. / 10.)
                           ? (4356. / 361. * t * t) - (35442. / 1805. * t)
                                 + 16061. / 1805.
                           : (54. / 5. * t * t) - (513. / 25. * t)
                                 + 268. / 25.;
  }
};

struct bounceIn
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return 1. - bounceOut{}(1. - t);
  }
};

struct bounceInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return t < 0.5 ? 0.5 * bounceIn{}(t * 2.)
                   : 0.5 * bounceOut{}(t * 2. - 1.) + 0.5;
  }
};

struct perlinInOut
{
  template <typename T>
  constexpr T operator()(T t) const
  {
    return 6. * std::pow(t, 5) - 15. * std::pow(t, 4) + 10. * std::pow(t, 3);
  }
};
}

template <typename Y, typename Easing>
struct curve_segment_ease
{
  Y operator()(double ratio, Y start, Y end) const
  {
    return easing::ease{}(start, end, Easing{}(ratio));
  }
};
}
