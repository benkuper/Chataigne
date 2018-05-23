//! \file eggs/variant/detail/pack.hpp
// Eggs.Variant
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014-2017
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_VARIANT_DETAIL_PACK_HPP
#define EGGS_VARIANT_DETAIL_PACK_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

#include "config/prefix.hpp"

namespace eggs { namespace variants { namespace detail
{
    struct empty
    {
        EGGS_CXX11_CONSTEXPR empty() noexcept {}
        EGGS_CXX11_CONSTEXPR bool operator==(empty) const { return true; }
        EGGS_CXX11_CONSTEXPR bool operator!=(empty) const { return false; }
        EGGS_CXX11_CONSTEXPR bool operator<(empty) const { return false; }
        EGGS_CXX11_CONSTEXPR bool operator>(empty) const { return false; }
        EGGS_CXX11_CONSTEXPR bool operator<=(empty) const { return true; }
        EGGS_CXX11_CONSTEXPR bool operator>=(empty) const { return true; }
    };

    template <typename T>
    struct identity
    {
        using type = T;
    };

    template <std::size_t I>
    struct index
    {
        EGGS_CXX11_STATIC_CONSTEXPR std::size_t value = I;
    };

    template <typename ...Ts>
    struct pack
    {
        using type = pack;
        EGGS_CXX11_STATIC_CONSTEXPR std::size_t size = sizeof...(Ts);
    };

    template <typename T, T ...Vs>
    struct pack_c
    {
        using type = pack_c;
        EGGS_CXX11_STATIC_CONSTEXPR std::size_t size = sizeof...(Vs);
    };

    template <typename ...Ts>
    static EGGS_CXX11_CONSTEXPR int swallow_pack(Ts const&...) noexcept
    {
        return 0;
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Is, bool Odd>
    struct _make_index_pack_twice;

    template <std::size_t ...Is>
    struct _make_index_pack_twice<
        pack_c<std::size_t, Is...>
      , false
    > : pack_c<std::size_t, Is..., (sizeof...(Is) + Is)...>
    {};

    template <std::size_t ...Is>
    struct _make_index_pack_twice<
        pack_c<std::size_t, Is...>
      , true
    > : pack_c<std::size_t, Is..., (sizeof...(Is) + Is)..., sizeof...(Is) * 2>
    {};

    template <std::size_t N>
    struct _make_index_pack
      : _make_index_pack_twice<
            typename _make_index_pack<N / 2>::type
          , N % 2 != 0
        >
    {};

    template <>
    struct _make_index_pack<1>
      : pack_c<std::size_t, 0>
    {};

    template <>
    struct _make_index_pack<0>
      : pack_c<std::size_t>
    {};

    template <std::size_t N>
    using make_index_pack = typename _make_index_pack<N>::type;

    template <typename Ts>
    struct _index_pack;

    template <typename ...Ts>
    struct _index_pack<pack<Ts...>>
      : _make_index_pack<sizeof...(Ts)>
    {};

    template <typename Ts>
    using index_pack = typename _index_pack<Ts>::type;

    ///////////////////////////////////////////////////////////////////////////
    template <typename Vs>
    struct _make_typed_pack;

    template <std::size_t ...Vs>
    struct _make_typed_pack<pack_c<std::size_t, Vs...>>
      : pack<index<Vs>...>
    {};

    template <typename Ts>
    struct _typed_index_pack
      : _make_typed_pack<index_pack<Ts>>
    {};

    template <typename Ts>
    using typed_index_pack = typename _typed_index_pack<Ts>::type;

    ///////////////////////////////////////////////////////////////////////////
    template <typename Vs>
    struct all_of;

    template <bool ...Vs>
    struct all_of<pack_c<bool, Vs...>>
      : std::integral_constant<
            bool
          , std::is_same<
                pack_c<bool, Vs...>
              , pack_c<bool, (Vs || true)...> // true...
            >::value
        >
    {};

    template <typename ...Ts>
    using _always_true = std::true_type;

    template <typename ...Ts>
    static std::false_type _all_of(...);

    template <typename ...Ts>
    static auto _all_of(int) -> _always_true<
        typename std::enable_if<bool(Ts::value)>::type...>;

    template <typename ...Ts>
    struct all_of<pack<Ts...>>
      : decltype(detail::_all_of<Ts...>(0))
    {};

    ///////////////////////////////////////////////////////////////////////////
    template <typename ...Vs>
    struct any_of;

    template <bool ...Vs>
    struct any_of<pack_c<bool, Vs...>>
      : std::integral_constant<
            bool
          , !std::is_same<
                pack_c<bool, Vs...>
              , pack_c<bool, (Vs && false)...> // false...
            >::value
        >
    {};

    template <typename ...Ts>
    using _always_false = std::false_type;

    template <typename ...Ts>
    static std::true_type _any_of(...);

    template <typename ...Ts>
    static auto _any_of(int) -> _always_false<
        typename std::enable_if<!bool(Ts::value)>::type...>;

    template <typename ...Ts>
    struct any_of<pack<Ts...>>
      : decltype(detail::_any_of<Ts...>(0))
    {};

    ///////////////////////////////////////////////////////////////////////////
    template <std::size_t I, typename T>
    struct _indexed {};

    template <typename Ts, typename Is = index_pack<Ts>>
    struct _indexer;

    template <typename ...Ts, std::size_t ...Is>
    struct _indexer<pack<Ts...>, pack_c<std::size_t, Is...>>
      : _indexed<Is, Ts>...
    {};

    template <std::size_t I>
    static empty _at_index(...);

    template <std::size_t I, typename T>
    static identity<T> _at_index(_indexed<I, T> const&);

    template <std::size_t I, typename Ts>
    struct at_index
      : decltype(detail::_at_index<I>(_indexer<Ts>{}))
    {};

    template <typename T, typename Ts>
    struct count_of;

    template <typename T>
    struct count_of<T, pack<>>
    {
        EGGS_CXX11_STATIC_CONSTEXPR std::size_t value = 0;
    };

    template <typename T, typename H, typename ...Ts>
    struct count_of<T, pack<H, Ts...>>
    {
        EGGS_CXX11_STATIC_CONSTEXPR std::size_t value =
            count_of<T, pack<Ts...>>::value
          + (std::is_same<T, H>::value ? 1 : 0);
    };

    template <typename T>
    static empty _index_of(...);

    template <typename T, std::size_t I>
    static index<I> _index_of(_indexed<I, T> const&);

    template <typename T, typename Ts>
    struct index_of
#if 0
        // most compilers get this wrong in the case of duplicated types
      : decltype(detail::_index_of<T>(_indexer<Ts>{}))
#else
      : std::conditional<
            count_of<T, Ts>::value == 1
          , decltype(detail::_index_of<T>(_indexer<Ts>{}))
          , empty
        >::type
#endif
    {};
}}}

#include "config/suffix.hpp"

#endif /*EGGS_VARIANT_DETAIL_PACK_HPP*/
