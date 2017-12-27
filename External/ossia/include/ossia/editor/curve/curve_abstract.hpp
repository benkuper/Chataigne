#pragma once
#include <memory>
#include <ossia_export.h>
#include <utility>
/**
 * \file curve_abstract.hpp
 */
namespace ossia
{
/**
 * @brief The curve_segment_type enum
 *
 * Represents the result type of either the X or Y
 * parameter of a curve.
 */
enum class curve_segment_type
{
  INT,
  FLOAT,
  DOUBLE,
  BOOL,
  ANY
};
/**
 * \typedef curve_type A pair that contains the types of a curve segment
 *
 * curve_type::first is the abscissa
 * curve_type::second is the ordinate
 */
using curve_type
    = std::pair<ossia::curve_segment_type, ossia::curve_segment_type>;

/**
 * @brief The curve_abstract class
 *
 * Base class for curves.
 *
 * \see \ref curve \ref curve_segment
 */
class OSSIA_EXPORT curve_abstract
{
public:
  virtual ~curve_abstract();

  /*! get the type of both coordinates */
  virtual curve_type get_type() const = 0;

  /*! Clear internal data structures of the curve.
   * Call before a new execution. */
  virtual void reset() = 0;
};

using curve_ptr = std::shared_ptr<ossia::curve_abstract>;
}
