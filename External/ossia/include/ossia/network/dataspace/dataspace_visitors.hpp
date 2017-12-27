#pragma once
#include <ossia/detail/destination_index.hpp>
#include <ossia/detail/string_view.hpp>
#include <ossia/network/value/vec.hpp>
#include <ossia/network/common/parameter_properties.hpp>
#include <bitset>
#include <ossia_export.h>
#include <type_traits>

/**
 * \file dataspace_visitors.hpp
 */
namespace ossia
{
struct unit_t;
class value;
struct value_with_unit;

/**
 * @brief Check if two units are convertible into each other
 *
 * i.e. if they are in the same dataspace
 */
OSSIA_EXPORT
bool check_units_convertible(const ossia::unit_t&, const ossia::unit_t&);

/**
 * @brief get_dataspace_text
 * @return Text corresponding to the dataspace. For instance "color" or "gain".
 */
OSSIA_EXPORT
ossia::string_view get_dataspace_text(const ossia::unit_t&);

/**
 * @brief get_unit_text
 * @return Text corresponding to the unit. For instance "rgb" or "cartesian3D".
 */
OSSIA_EXPORT
ossia::string_view get_unit_text(const ossia::unit_t&);

/**
 * @brief get_unit_accessors
 * @return If the unit has an array-like type (e.g. RGB color, XY point),
 *         this returns a string of array attributes. Else an empty string.
 */
OSSIA_EXPORT
ossia::string_view get_unit_accessors(const ossia::unit_t&);

/**
 * @brief get_pretty_unit_text Unit for human readability and debugging
 *
 * Format is dataspace.unit, e.g. distance.m/s or color.rgb
 */
OSSIA_EXPORT
std::string get_pretty_unit_text(const ossia::unit_t&);

/**
 * @brief get_unit_accessor Return the character corresponding to the accessor
 *
 * e.g. for unit == rgb, n == 1, the function returns 'g'.
 * @note Will return 0 (as in "(char)0") if anything is invalid.
 */
OSSIA_EXPORT
char get_unit_accessor(const ossia::unit_t&, uint8_t n);

/**
 * @brief parse_dataspace
 * @param text Something like color, etc.
 *
 * @return A value corresponding to the dataspace, without any specific unit.
 */
OSSIA_EXPORT
ossia::unit_t parse_dataspace(ossia::string_view text);

/**
 * @brief parse_unit Takes a text and a dataspace and gives a corresponding
 * unit.
 * @param text The text to parse
 * @param dataspace A dataspace such as ossia::color_u, ossia::distance_u...
 *
 * @return Corresponding unit, or the same unit as input if it was not found.
 */
OSSIA_EXPORT
ossia::unit_t
parse_unit(ossia::string_view text, const ossia::unit_t& dataspace);

/**
 * @brief parse_pretty_unit Create a unit from a valid unit in the format
 * of get_pretty_unit_text
 */
OSSIA_EXPORT
ossia::unit_t parse_pretty_unit(ossia::string_view text);

/**
 * @brief parse_unit Alternative if we already have a dataspace type at hand.
 * @note T should be a dataspace, such as ossia::color_u.
 * @todo In 2020 enforce this with concepts...
 */
template <typename T>
OSSIA_EXPORT ossia::unit_t parse_unit(ossia::string_view text, T dataspace);

/**
 * @brief make_value Create a value_with_unit
 * @param v A value
 * @param u A unit
 *
 * @return The same value with the unit attached if the types were correct.
 */
OSSIA_EXPORT
value_with_unit make_value(const ossia::value& v, const ossia::unit_t& u);

/**
 * @brief get_unit Create an unit from indexes in the variant
 * @param dataspace Identifier in the first variant
 * @param unit Optional identifier in the second variant
 *
 * Ex. : position.opengl_u == (1, 4).
 * This is useful for concise serialization / deserialization.
 *
 * @return A corresponding unit
 */
OSSIA_EXPORT
ossia::unit_t make_unit(uint64_t dataspace, uint64_t unit);

/**
 * @brief underlying_type Get the implementation type of an unit
 * @param u An unit
 *
 * Will return Impulse if the unit is invalid.
 *
 * @return The matching type. For instance vec3f for position.cart3D.
 */
OSSIA_EXPORT
ossia::val_type matching_type(const ossia::unit_t& u);

inline ossia::val_type matching_type(ossia::unit_t&& u)
{
  return matching_type(static_cast<const ossia::unit_t&>(u));
}

/**
 * @brief convert Convert a value to another unit in the same dataspace
 * @param v A value
 * @param t An unit of the same dataspace than the value.
 *
 * E.g. you can convert 1cm in millimeters, but not 1cm in RGB colors.
 *
 * @return The converted value if both value and unit are valid
 * and convertible, else the input value
 */
OSSIA_EXPORT
ossia::value_with_unit
convert(const ossia::value_with_unit& v, const ossia::unit_t& t);

/**
 * @brief convert Convert a value to another unit.
 * @param v A value.
 * @param source_unit The unit of v.
 * @param destination_unit The unit in which v shall be converted.
 *
 * Dataspaces ought to be similar.
 *
 * @return Converted value, else the input value.
 */
OSSIA_EXPORT
ossia::value convert(
    const ossia::value& v, const ossia::unit_t& source_unit,
    const ossia::unit_t& destination_unit);

/**
 * @brief convert Convert a value + unit to a simple value
 * @param v Value to convert
 * @return Corresponding ossia::value
 */
OSSIA_EXPORT
ossia::value to_value(const ossia::value_with_unit& v);

/**
 * @brief to_unit Get the unit part of a value_with_unit
 */
OSSIA_EXPORT
ossia::unit_t to_unit(const ossia::value_with_unit& v);

/**
 * @brief to_pretty_string Pretty string of unit & value
 *
 * @return For example "[0.2, 1.6, 3.4] rgb"
 */
OSSIA_EXPORT
std::string to_pretty_string(const ossia::value_with_unit& v);

/**
 * @brief merge Merge a computed value in a unit-aware value
 *
 * The computed value is assumed to be of the same dataspace.
 * For instance if v is a HSV color, then t can also be an HSV color,
 * or just "h", "s", or "v" component, but not a "r" color.
 *
 * @param The value with an unit
 * @param The value to merge
 * @param Optional destination index or bitset to merge with
 * @return
 */
OSSIA_EXPORT
ossia::value_with_unit merge(
    const ossia::value_with_unit&, const ossia::value&,
    ossia::destination_index);

OSSIA_EXPORT
ossia::value_with_unit
merge(const ossia::value_with_unit&, const ossia::vec2f&, std::bitset<2>);

OSSIA_EXPORT
ossia::value_with_unit
merge(const ossia::value_with_unit&, const ossia::vec3f&, std::bitset<3>);

OSSIA_EXPORT
ossia::value_with_unit
merge(const ossia::value_with_unit&, const ossia::vec4f&, std::bitset<4>);
}
