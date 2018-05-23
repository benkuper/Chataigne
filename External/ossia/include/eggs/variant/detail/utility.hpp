//! \file eggs/variant/detail/utility.hpp
// Eggs.Variant
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014-2017
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_VARIANT_DETAIL_UTILITY_HPP
#define EGGS_VARIANT_DETAIL_UTILITY_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include "config/prefix.hpp"

namespace eggs { namespace variants { namespace detail
{
    template <typename T>
    EGGS_CXX11_CONSTEXPR T&& forward(
        typename std::remove_reference<T>::type& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    EGGS_CXX11_CONSTEXPR T&& forward(
        typename std::remove_reference<T>::type&& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    EGGS_CXX11_CONSTEXPR typename std::remove_reference<T>::type&& move(
        T&& t) noexcept
    {
        return static_cast<typename std::remove_reference<T>::type&&>(t);
    }

    ///////////////////////////////////////////////////////////////////////////
#if EGGS_CXX17_STD_HAS_CONSTEXPR_ADDRESSOF
    using std::addressof;
#else
    namespace _addressof
    {
        struct _fallback {};

        template <typename T>
        static _fallback operator&(T&&);

        template <typename T>
        struct has_addressof_operator
        {
            EGGS_CXX11_STATIC_CONSTEXPR bool value =
                (std::is_class<T>::value || std::is_union<T>::value)
             && !std::is_same<decltype(&std::declval<T&>()), _fallback>::value;
        };
    }

    template <typename T>
    EGGS_CXX11_CONSTEXPR typename std::enable_if<
        !_addressof::has_addressof_operator<T>::value
      , T*
    >::type addressof(T& r) noexcept
    {
        return &r;
    }

    template <typename T>
    typename std::enable_if<
        _addressof::has_addressof_operator<T>::value
      , T*
    >::type addressof(T& r) noexcept
    {
        return std::addressof(r);
    }
#endif
}}}

#include "config/suffix.hpp"

#endif /*EGGS_VARIANT_DETAIL_UTILITY_HPP*/
