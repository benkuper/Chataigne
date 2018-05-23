//! \file eggs/variant/detail/visitor.hpp
// Eggs.Variant
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014-2017
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_VARIANT_DETAIL_VISITOR_HPP
#define EGGS_VARIANT_DETAIL_VISITOR_HPP

#include "pack.hpp"
#include "utility.hpp"

#include <cassert>
#include <cstddef>
#include <exception>
#include <functional>
#include <new>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "config/prefix.hpp"

namespace eggs { namespace variants { namespace detail
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename F, typename Sig>
    struct visitor;

    template <typename F, typename R, typename ...Args>
    struct visitor<F, R(Args...)>
    {
        template <typename ...Ts>
        struct _table
        {
            static EGGS_CXX11_CONSTEXPR R (*value[pack<Ts...>::size])(Args...)
#if EGGS_CXX11_HAS_CONSTEXPR
                = {&F::template call<Ts>...};
#else
                ;
#endif
        };

#if defined(NDEBUG)
        static EGGS_CXX11_CONSTEXPR int _assert_in_range(
            std::size_t /*index*/, std::size_t /*size*/)
        {
            return 0;
        }
#else
        static int _assert_in_range_failure(std::size_t index, std::size_t size)
        {
            assert(index < size && "discriminator out of range");
            return 0;
        }

        static EGGS_CXX11_CONSTEXPR int _assert_in_range(
            std::size_t index, std::size_t size)
        {
            return index < size ? 0 : _assert_in_range_failure(index, size);
        }
#endif

        template <typename ...Ts>
        EGGS_CXX11_CONSTEXPR R operator()(pack<Ts...>, std::size_t which,
            Args&&... args) const
        {
            return _assert_in_range(which, sizeof...(Ts)),
                _table<Ts...>::value[which](detail::forward<Args>(args)...);
        }

        EGGS_CXX11_NORETURN R operator()(pack<>, std::size_t, Args&&...) const
        {
            std::terminate();
        }
    };

    template <typename F, typename R, typename ...Args>
    template <typename ...Ts>
    EGGS_CXX11_CONSTEXPR R (*visitor<F, R(Args...)>::_table<Ts...>::
        value[pack<Ts...>::size])(Args...)
#if EGGS_CXX11_HAS_CONSTEXPR
        ;
#else
        = {&F::template call<Ts>...};
#endif

    ///////////////////////////////////////////////////////////////////////////
    struct copy_construct
      : visitor<copy_construct, void(void*, void const*)>
    {
        template <typename T>
        static void call(void* ptr, void const* other)
        {
            ::new (ptr) T(*static_cast<T const*>(other));
        }
    };

    struct move_construct
      : visitor<move_construct, void(void*, void*)>
    {
        template <typename T>
        static void call(void* ptr, void* other)
        {
            ::new (ptr) T(detail::move(*static_cast<T*>(other)));
        }
    };

    struct copy_assign
      : visitor<copy_assign, void(void*, void const*)>
    {
        template <typename T>
        static void call(void* ptr, void const* other)
        {
            *static_cast<T*>(ptr) = *static_cast<T const*>(other);
        }
    };

    struct move_assign
      : visitor<move_assign, void(void*, void*)>
    {
        template <typename T>
        static void call(void* ptr, void* other)
        {
            *static_cast<T*>(ptr) = detail::move(*static_cast<T*>(other));
        }
    };

    struct destroy
      : visitor<destroy, void(void*)>
    {
        template <typename T>
        static void call(void* ptr)
        {
            static_cast<T*>(ptr)->~T();
        }
    };

    struct swap
      : visitor<swap, void(void*, void*)>
    {
        template <typename T>
        static void call(void* ptr, void* other)
        {
            using std::swap;
            swap(*static_cast<T*>(ptr), *static_cast<T*>(other));
        }
    };

#if EGGS_CXX98_HAS_RTTI
    struct type_id
      : visitor<type_id, std::type_info const&()>
    {
        template <typename T>
        static EGGS_CXX11_CONSTEXPR std::type_info const& call()
        {
            return typeid(T);
        }
    };
#endif

    template <typename Union>
    struct equal_to
      : visitor<equal_to<Union>, bool(Union const&, Union const&)>
    {
        template <typename I>
        static EGGS_CXX11_CONSTEXPR bool call(Union const& lhs, Union const& rhs)
        {
            return lhs.get(I{}) == rhs.get(I{});
        }
    };

    template <typename Union>
    struct not_equal_to
      : visitor<not_equal_to<Union>, bool(Union const&, Union const&)>
    {
        template <typename I>
        static EGGS_CXX11_CONSTEXPR bool call(Union const& lhs, Union const& rhs)
        {
            return lhs.get(I{}) != rhs.get(I{});
        }
    };

    template <typename Union>
    struct less
      : visitor<less<Union>, bool(Union const&, Union const&)>
    {
        template <typename I>
        static EGGS_CXX11_CONSTEXPR bool call(Union const& lhs, Union const& rhs)
        {
            return lhs.get(I{}) < rhs.get(I{});
        }
    };

    template <typename Union>
    struct greater
      : visitor<greater<Union>, bool(Union const&, Union const&)>
    {
        template <typename I>
        static EGGS_CXX11_CONSTEXPR bool call(Union const& lhs, Union const& rhs)
        {
            return lhs.get(I{}) > rhs.get(I{});
        }
    };

    template <typename Union>
    struct less_equal
      : visitor<less_equal<Union>, bool(Union const&, Union const&)>
    {
        template <typename I>
        static EGGS_CXX11_CONSTEXPR bool call(Union const& lhs, Union const& rhs)
        {
            return lhs.get(I{}) <= rhs.get(I{});
        }
    };

    template <typename Union>
    struct greater_equal
      : visitor<greater_equal<Union>, bool(Union const&, Union const&)>
    {
        template <typename I>
        static EGGS_CXX11_CONSTEXPR bool call(Union const& lhs, Union const& rhs)
        {
            return lhs.get(I{}) >= rhs.get(I{});
        }
    };

    struct hash
      : visitor<hash, std::size_t(void const*)>
    {
        template <typename T>
        static std::size_t call(void const* ptr)
        {
            std::hash<typename std::remove_const<T>::type> h;
            return h(*static_cast<T const*>(ptr));
        }
    };

    template <typename T, typename Union>
    struct target
      : visitor<target<T, Union>, T*(Union&)>
    {
        static EGGS_CXX11_CONSTEXPR T* _impl(T* ptr)
        {
            return ptr;
        }

        static EGGS_CXX11_CONSTEXPR T* _impl(void const*)
        {
            return nullptr;
        }

        template <typename I>
        static EGGS_CXX11_CONSTEXPR T* call(Union& u)
        {
            return target::_impl(detail::addressof(u.get(I{})));
        }
    };
}}}

#include "config/suffix.hpp"

#endif /*EGGS_VARIANT_DETAIL_VISITOR_HPP*/
