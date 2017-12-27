#pragma once

#include <ossia/detail/optional.hpp>
#include <ossia/detail/ptr_container.hpp>
#include <ossia/editor/curve/curve_abstract.hpp>
#include <ossia/editor/curve/curve_segment.hpp>
#include <ossia/editor/curve/curve_segment/easing.hpp>
#include <ossia/network/value/destination.hpp>
#include <ossia/network/value/value.hpp>
#include <ossia/network/base/parameter.hpp>
#include <ossia/network/base/node.hpp>
#include <boost/container/flat_map.hpp>
#include <functional>
#include <map>
#include <memory>
#include <ossia_export.h>
#include <utility>
#include <vector>

/**
 * \file curve.hpp
 */

namespace ossia
{
class destination;

template <typename T>
const constexpr std::nullptr_t curve_segment_type_map{};
template <>
const constexpr ossia::curve_segment_type
    curve_segment_type_map<int> = ossia::curve_segment_type::INT;
template <>
const constexpr ossia::curve_segment_type
    curve_segment_type_map<float> = ossia::curve_segment_type::FLOAT;
template <>
const constexpr ossia::curve_segment_type
    curve_segment_type_map<double> = ossia::curve_segment_type::DOUBLE;
template <>
const constexpr ossia::curve_segment_type
    curve_segment_type_map<bool> = ossia::curve_segment_type::BOOL;
template <>
const constexpr ossia::curve_segment_type
    curve_segment_type_map<ossia::value> = ossia::curve_segment_type::ANY;

template <typename... Args>
using curve_map = boost::container::flat_map<Args...>;

template <typename X, typename Y>
/**
 * @brief The curve class
 *
 * A curve is a succession of \ref curve_segment.
 * It is used in a \ref Behavior to drive \ref automation or \ref mapper.
 */
class OSSIA_EXPORT curve final : public curve_abstract
{
public:
  using abscissa_type = X;
  using ordinate_type = Y;
  using curve_type = curve<X, Y>;
  using map_type = curve_map<X, std::pair<Y, ossia::curve_segment<Y>>>;

  curve() = default;
  curve(const curve&) = delete;
  curve(curve&&) = delete;
  curve& operator=(const curve&) = delete;
  curve& operator=(curve&&) = delete;
  /*! destructor */
  virtual ~curve() = default;

  void reset() override;

  /*! add a segment to reach a target point to the curve
 \param std::shared_ptr<#CurveSegment<Y>> segment to target point
 \param X target point abscissa
 \param Y target point ordinate
 \return bool */
  bool add_point(ossia::curve_segment<Y> segment, X abscissa, Y value);

  /*! remove a point from the curve
 \param X point abscissa
 \return bool */
  bool remove_point(X abscissa);

  /*! get value at an abscissa
 \param X abscissa.
 \return Y ordinate */
  Y value_at(X abscissa) const;

  ossia::curve_type get_type() const override;

  /*! get initial point abscissa
 \details if there is an initial abcissa destination, it will return the value
 of the address
 \return X value */
  X get_x0() const;

  /*! get initial point ordinate
 \details if there is an initial ordinate destination, it will return the value
 of the address
 \return Y value */
  Y get_y0() const;

  /*! set initial point abscissa
 \details if there is an initial abscissa destination, this accessor is useless
 \param X abscissa */
  void set_x0(X value);

  /*! set initial point ordinate
 \details if there is an initial ordinate destination, this accessor is useless
 \param Y ordinate */
  void set_y0(Y value);

  /*! get initial point abscissa destination
 \return const Destination* */
  ossia::optional<destination> get_x0_destination() const;

  /*! get initial point ordinate destination
 \return const Destination* */
  ossia::optional<destination> get_y0_destination() const;

  /*! set initial curve abscissa using a Destination
 \param const Destination* */
  void set_x0_destination(const ossia::destination& destination);

  /*! set initial curve ordinate using a Destination
 \param const Destination* */
  void set_y0_destination(const ossia::destination& destination);

  /*! get points of the curve
  \return std::map<X, pair<Y, CurveSegment<Y>>> map of {abscissa, {value,
          previous segment}
  */
  map_type get_points() const;

  /**
   * @brief set_scale_bounds
   *
   * Use this if the curve ordinate is given between [0; 1] and has to be
   * rescaled to
   * the correct bounds when the first value is received.
   *
   * The arguments are the known bounds of the points at the time of creation.
   */
  void set_scale_bounds(Y min, Y max, Y end);

  static Y convert_to_template_type_value(
      const ossia::value& value, ossia::destination_index::const_iterator idx);

private:
  mutable X m_x0;
  mutable Y m_y0;
  mutable ossia::optional<ossia::destination> m_x0_destination;
  mutable ossia::optional<ossia::destination> m_y0_destination;

  mutable map_type m_points;

  mutable Y m_y0_cache;
  struct scale_info
  {
    Y min;
    Y max;
    Y start;
    Y end;
  };
  mutable optional<scale_info> m_scaleBounds;
  mutable optional<map_type> m_originalPoints;

  mutable bool m_y0_cacheUsed = false;
};

/**
 * @brief The constant_curve class
 * A curve that always return a same single value.
 *
 * It is useful if for instance one wants to automate on the
 * first value of the following list :
 *
 * [ 1, "a string", [ "another", 'c' ] ]
 *
 * while keeping the reste of the list intact.
 */
class constant_curve final : public curve_abstract
{
public:
  constant_curve(ossia::value v) : mValue{std::move(v)}
  {
  }
  constant_curve() = default;
  constant_curve(const constant_curve&) = delete;
  constant_curve(constant_curve&&) = delete;
  constant_curve& operator=(const constant_curve&) = delete;
  constant_curve& operator=(constant_curve&&) = delete;

  ossia::value value() const
  {
    return mValue;
  }

  curve_type get_type() const override
  {
    return std::make_pair(
        ossia::curve_segment_type::DOUBLE, ossia::curve_segment_type::ANY);
  }

  void reset() override
  {
  }

private:
  const ossia::value mValue;
};
}
