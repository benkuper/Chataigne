#pragma once
#include <ossia/detail/config.hpp>
#include <eggs/variant.hpp>
/**
 * \file state_element_fwd.hpp
 */
namespace ossia
{
struct message;
struct piecewise_message;
template <std::size_t N>
struct piecewise_vec_message;
class state;
class custom_state;

/**
 * \typedef state_element
 *
 * A variant of elements that are meant to be triggered during
 * execution of a score.
 *
 * \see \ref message
 * \see \ref state
 * \see \ref custom_state
 * \see state_element.hpp
 */
using state_element = eggs::
    variant<message, state, custom_state, piecewise_message, piecewise_vec_message<2>, piecewise_vec_message<3>, piecewise_vec_message<4>>;

template <std::size_t N>
using state_element_by_index =
    typename eggs::variants::variant_element<N, ossia::state_element>::type;
}
