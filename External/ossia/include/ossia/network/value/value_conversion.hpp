#pragma once
#include <ossia/network/common/parameter_properties.hpp>
#include <ossia_export.h>
#include <vector>

/**
 * \file value_conversion.hpp
 */
namespace ossia
{
class value;
/**
 * @brief convert Convert a value to the given type.
 * @param val A value
 * @return The closest possible conversion of the value to T, else a
 * default-constructed T.
 */
template <typename T>
OSSIA_EXPORT T convert(const ossia::value& val);

/**
 * @brief convert Convert lists to array<float, 2/3/4>
 */
template <typename T>
OSSIA_EXPORT T convert(const std::vector<ossia::value>& val);

OSSIA_EXPORT ossia::value
convert(const ossia::value& val, ossia::val_type newtype);
OSSIA_EXPORT ossia::value
convert(const ossia::value& val, const ossia::value& cur);
}
