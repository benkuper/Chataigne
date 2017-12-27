#pragma once
#include <ossia/detail/config.hpp>
#include <chobo/static_vector.hpp>
#include <iterator>

/**
 * \file destination_index.hpp
 */

namespace ossia
{
/**
 * Identifies an element in a nested list.
 * For instance :
 *
 * \code
 * List_A{
 *  List_B{1, 2, 3},
 *  List_C{
 *    List_D{4},
 *    List_E{5}
 *  }
 * }
 *
 * destination_index{}        => List_A
 * destination_index{0}       => List_B
 * destination_index{0, 1}    => '1'
 * destination_index{0, 2}    => '2'
 * destination_index{0, 3}    => '3'
 * destination_index{1}       => List_C
 * destination_index{1, 0}    => List_D
 * destination_index{1, 0, 0} => '4'
 * destination_index{1, 1}    => List_E
 * destination_index{1, 1, 0} => '5'
 * \endcode
 *
 * There are at most 8 nested depth level.
 */
using destination_index = chobo::static_vector<uint8_t, 8>;
}
