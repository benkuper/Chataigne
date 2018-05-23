//! \file eggs/variant/in_place.hpp
// Eggs.Variant
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014-2017
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_VARIANT_IN_PLACE_HPP
#define EGGS_VARIANT_IN_PLACE_HPP

#include <cstddef>
#include <type_traits>

#include "detail/config/prefix.hpp"

namespace eggs { namespace variants
{
    namespace detail
    {
        template <std::size_t I>
        struct in_place_index_tag {};

        template <typename T>
        struct in_place_type_tag {};
    }

    ///////////////////////////////////////////////////////////////////////////
    //! The template aliases `in_place_index_t` and `in_place_type_t` are used
    //! as unique types to disambiguate constructor and function overloading.
    //! Specifically, `variant<Ts...>` has a constructor with
    //! `in_place_type_t<T>`as the first parameter, followed by a parameter
    //! pack; this indicates that `T` should be constructed in-place (as if
    //! by a call to a placement new expression) with the forwarded pack
    //! expansion as arguments for the initialization of `T`.

    //! template <std::size_t I>
    //! using in_place_index_t = void(*)(unspecified<I>);
    template <std::size_t I>
    using in_place_index_t = void(*)(detail::in_place_index_tag<I>);

    //! template <class T>
    //! using in_place_type_t = void(*)(unspecified<T>);
    template <typename T>
    using in_place_type_t = void(*)(detail::in_place_type_tag<T>);

    //! template <std::size_t I>
    //! void in_place(unspecified<I>);
    template <std::size_t I>
    inline void in_place(detail::in_place_index_tag<I> = {}) {}

    //! template <class T>
    //! void in_place(unspecified<T>);
    template <typename T>
    inline void in_place(detail::in_place_type_tag<T> = {}) {}

    ///////////////////////////////////////////////////////////////////////////
    namespace detail
    {
        template <typename T>
        struct is_inplace_tag
          : std::false_type
        {};

        template <std::size_t I>
        struct is_inplace_tag<in_place_index_t<I>>
          : std::true_type
        {};

        template <typename T>
        struct is_inplace_tag<in_place_type_t<T>>
          : std::true_type
        {};
    }
}}

#include "detail/config/suffix.hpp"

#endif /*EGGS_VARIANT_IN_PLACE_HPP*/
