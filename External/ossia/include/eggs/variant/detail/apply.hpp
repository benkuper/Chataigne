//! \file eggs/variant/detail/apply.hpp
// Eggs.Variant
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014-2017
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_VARIANT_DETAIL_APPLY_HPP
#define EGGS_VARIANT_DETAIL_APPLY_HPP

#include "pack.hpp"
#include "storage.hpp"
#include "utility.hpp"
#include "visitor.hpp"

#include <cstddef>
#include <type_traits>
#include <utility>

#include "config/prefix.hpp"

namespace eggs { namespace variants { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename F, typename ...Ts>
    EGGS_CXX11_CONSTEXPR auto _invoke(F&& f, Ts&&... vs)
        noexcept(noexcept(detail::forward<F>(f)(detail::forward<Ts>(vs)...)))
     -> decltype(detail::forward<F>(f)(detail::forward<Ts>(vs)...))
    {
        return detail::forward<F>(f)(detail::forward<Ts>(vs)...);
    }

#if EGGS_CXX11_HAS_SFINAE_FOR_EXPRESSIONS
    template <typename F, typename T0, typename ...Ts>
    EGGS_CXX11_CONSTEXPR auto _invoke(F&& f, T0&& v0, Ts&&... vs)
        noexcept(noexcept((v0.*f)(detail::forward<Ts>(vs)...)))
     -> decltype((v0.*f)(detail::forward<Ts>(vs)...))
    {
        return (v0.*f)(detail::forward<Ts>(vs)...);
    }

    template <typename F, typename T0, typename ...Ts>
    EGGS_CXX11_CONSTEXPR auto _invoke(F&& f, T0&& v0, Ts&&... vs)
        noexcept(noexcept(((*v0).*f)(detail::forward<Ts>(vs)...)))
     -> decltype(((*v0).*f)(detail::forward<Ts>(vs)...))
    {
        return ((*v0).*f)(detail::forward<Ts>(vs)...);
    }

    template <typename F, typename T0>
    EGGS_CXX11_CONSTEXPR auto _invoke(F&& f, T0&& v0) noexcept
     -> decltype(v0.*f)
    {
        return v0.*f;
    }

    template <typename F, typename T0>
    EGGS_CXX11_CONSTEXPR auto _invoke(F&& f, T0&& v0) noexcept
     -> decltype((*v0).*f)
    {
        return (*v0).*f;
    }
#endif

    ///////////////////////////////////////////////////////////////////////////
    template <typename R>
    struct _invoke_guard
    {
        template <typename ...Ts>
        EGGS_CXX11_CONSTEXPR R operator()(Ts&&... vs) const
            noexcept(noexcept(detail::_invoke(detail::forward<Ts>(vs)...)))
        {
            return detail::_invoke(detail::forward<Ts>(vs)...);
        }
    };

    template <>
    struct _invoke_guard<void>
    {
        template <typename ...Ts>
        EGGS_CXX14_CONSTEXPR void operator()(Ts&&... vs) const
            noexcept(noexcept(detail::_invoke(detail::forward<Ts>(vs)...)))
        {
            detail::_invoke(detail::forward<Ts>(vs)...);
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename F, typename Ts>
    struct _result_of;

    template <typename F, typename ...Ts>
    struct _result_of<F, pack<Ts...>>
    {
        using type = decltype(detail::_invoke(
            std::declval<F>(), std::declval<Ts>()...));
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename T, typename Is = make_index_pack<T::size>>
    struct _make_apply_pack;

    template <typename T, std::size_t I, std::size_t ...Is>
    struct _make_apply_pack<T, pack_c<std::size_t, I, Is...>>
    {
        using type = pack<index<Is>...>;
    };

    template <typename T>
    using _apply_pack = typename _make_apply_pack<T>::type;

    ///////////////////////////////////////////////////////////////////////////
    template <typename V, typename I>
    struct _apply_get
    {
        using type = decltype(std::declval<V>().get(I{}));

        type EGGS_CXX11_CONSTEXPR operator()(V& v) const
        {
            return v.get(I{});
        }
    };

    template <typename V, typename I>
    struct _apply_get<V&&, I>
    {
        using type = typename std::remove_reference<
            decltype(std::declval<V>().get(I{}))>::type;

        EGGS_CXX11_CONSTEXPR type operator()(V& v) const
        {
            return detail::move(v.get(I{}));
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename R, typename F, typename Ms, typename Vs>
    struct _apply;

    template <
        typename R, typename F, typename ...Ms, typename V
    >
    struct _apply<R, F, pack<Ms...>, pack<V>>
      : visitor<
            _apply<R, F, pack<Ms...>, pack<V>>
          , R(F&&, Ms..., V&&)
        >
    {
        template <typename I>
        static EGGS_CXX11_CONSTEXPR R call(F&& f, Ms... ms, V&& v)
        {
            return _invoke_guard<R>{}(
                detail::forward<F>(f), detail::forward<Ms>(ms)...
              , _apply_get<V, I>{}(v));
        }
    };

    template <
        typename R, typename F, typename ...Ms
      , typename V0, typename V1, typename ...Vs
    >
    struct _apply<R, F, pack<Ms...>, pack<V0, V1, Vs...>>
      : visitor<
            _apply<R, F, pack<Ms...>, pack<V0, V1, Vs...>>
          , R(F&&, Ms..., V0&&, V1&&, Vs&&...)
        >
    {
        template <typename I>
        static EGGS_CXX11_CONSTEXPR R call(F&& f, Ms... ms, V0&& v0,
            V1&& v1, Vs&&... vs)
        {
            using T = typename _apply_get<V0, I>::type;
            return _apply<R, F, pack<Ms..., T>, pack<V1, Vs...>>{}(
                    _apply_pack<typename std::decay<V1>::type>{}, v1.which() - 1
                  , detail::forward<F>(f)
                  , detail::forward<Ms>(ms)..., _apply_get<V0, I>{}(v0)
                  , detail::forward<V1>(v1), detail::forward<Vs>(vs)...);
        }
    };

    template <typename R, typename F>
    EGGS_CXX11_CONSTEXPR R apply(F&& f)
    {
        return _invoke_guard<R>{}(detail::forward<F>(f));
    }

    template <typename R, typename F, typename V, typename ...Vs>
    EGGS_CXX11_CONSTEXPR R apply(F&& f, V&& v, Vs&&... vs)
    {
        return _apply<R, F, pack<>, pack<V&&, Vs&&...>>{}(
                _apply_pack<typename std::decay<V>::type>{}, v.which() - 1
              , detail::forward<F>(f)
              , detail::forward<V>(v), detail::forward<Vs>(vs)...);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename R, typename ...Rs>
    struct _apply_result_combine
      : std::enable_if<
            all_of<pack<std::is_same<R, Rs>...>>::value
          , R
        >
    {};

    template <
        typename F, typename Ms, typename V, typename Vs,
        typename Is = _apply_pack<typename std::decay<V>::type>
    >
    struct _apply_result_expand;

    template <
        typename F, typename ...Ms, typename V,
        typename ...Is
    >
    struct _apply_result_expand<
        F, pack<Ms...>, V, pack<>
      , pack<Is...>
    > : _apply_result_combine<
            typename _result_of<
                F, pack<Ms..., typename _apply_get<V, Is>::type>
            >::type...
        >
    {};

    template <
        typename F, typename ...Ms, typename V0, typename V1, typename ...Vs,
        typename ...Is
    >
    struct _apply_result_expand<
        F, pack<Ms...>, V0, pack<V1, Vs...>
      , pack<Is...>
    > : _apply_result_combine<
            typename _apply_result_expand<
                F, pack<Ms..., typename _apply_get<V0, Is>::type>
              , V1, pack<Vs...>
            >::type...
        >
    {};

    template <typename F, typename Vs>
    struct apply_result;

    template <typename F>
    struct apply_result<F, pack<>>
      : _result_of<F, pack<>>
    {};

    template <typename F, typename V, typename ...Vs>
    struct apply_result<F, pack<V, Vs...>>
      : _apply_result_expand<F, pack<>, V, pack<Vs...>>
    {};
}}}

#include "config/suffix.hpp"

#endif /*EGGS_VARIANT_DETAIL_APPLY_HPP*/
