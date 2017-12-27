//! \file eggs/variant/variant.hpp
// Eggs.Variant
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014-2017
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_VARIANT_VARIANT_HPP
#define EGGS_VARIANT_VARIANT_HPP

#include "detail/apply.hpp"
#include "detail/pack.hpp"
#include "detail/storage.hpp"
#include "detail/utility.hpp"
#include "detail/visitor.hpp"

#include "bad_variant_access.hpp"
#include "in_place.hpp"

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "detail/config/prefix.hpp"

namespace eggs { namespace variants
{
    template <typename ...Ts>
    class variant;

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////
        std::false_type _is_variant(void const*);

        template <typename ...Ts>
        std::true_type _is_variant(variant<Ts...> const*);

        template <typename T>
        struct is_variant
          : decltype(detail::_is_variant(static_cast<T*>(nullptr)))
        {};

        template <typename ...Ts>
        struct is_variant<variant<Ts...>>
          : std::true_type
        {};

        template <typename ...Ts>
        struct is_variant<variant<Ts...> const>
          : std::true_type
        {};

        ///////////////////////////////////////////////////////////////////////
        template <typename T>
        struct checked_is_object
          : std::is_object<T>
        {
            static_assert(
                !std::is_function<T>::value
              , "variant alternative has function type");

            static_assert(
                !std::is_reference<T>::value
              , "variant alternative has reference type");

            static_assert(
                !std::is_void<T>::value
              , "variant alternative has void type");
        };

        ///////////////////////////////////////////////////////////////////////
        template <
            std::size_t I, typename Ts,
            bool Empty = std::is_base_of<empty, at_index<I, Ts>>::value
        >
        struct checked_at_index
          : at_index<I, Ts>
        {};

        template <std::size_t I, typename Ts>
        struct checked_at_index<I, Ts, /*Empty=*/true>
        {
            static_assert(I < Ts::size, "variant index out of range");
        };

        template <
            typename T, typename Ts,
            bool Empty = std::is_base_of<empty, index_of<T, Ts>>::value
        >
        struct checked_index_of
          : index_of<T, Ts>
        {};

        template <typename T, typename Ts>
        struct checked_index_of<T, Ts, /*Empty=*/true>
        {
            EGGS_CXX11_STATIC_CONSTEXPR std::size_t count = count_of<T, Ts>::value;
            static_assert(count != 0, "type not found in variant alternatives");
            static_assert(count <= 1, "type occurs more than once in variant alternatives");
        };

        ///////////////////////////////////////////////////////////////////////
        namespace _best_match
        {
            struct _fallback {};
            struct _ambiguous {};

            template <std::size_t I, typename T>
            struct _overload
            {
                using fun_ptr = index<I>(*)(T);
                operator fun_ptr();
            };

            template <typename Ts, typename Is = index_pack<Ts>>
            struct overloads;

            template <typename ...Ts, std::size_t ...Is>
            struct overloads<pack<Ts...>, pack_c<std::size_t, Is...>>
              : _overload<Is, Ts>...
            {
                using fun_ptr = _fallback(*)(...);
                operator fun_ptr();
            };

            template <std::size_t I, typename T, typename U>
            struct _explicit_overload
            {
                using explicit_fun_ptr = typename std::conditional<
                    std::is_constructible<T, U>::value &&
                   !std::is_convertible<U, T>::value,
                    index<I>(*)(U&&), void
                >::type;
                operator explicit_fun_ptr();
            };

            template <typename Ts, typename U, typename Is = index_pack<Ts>>
            struct explicit_overloads;

            template <typename ...Ts, typename U, std::size_t ...Is>
            struct explicit_overloads<pack<Ts...>, U, pack_c<std::size_t, Is...>>
              : _explicit_overload<Is, Ts, U>...
            {
                using fun_ptr = _fallback(*)(...);
                operator fun_ptr();
            };

            template <typename F, typename T>
            static auto _invoke(int)
             -> decltype(std::declval<F>()(std::declval<T>()));

            template <typename F, typename T>
            static _ambiguous _invoke(...);

            template <
                typename F, typename T
              , typename R = decltype(_best_match::_invoke<F, T>(0))
            >
            struct result_of : R
            {
                EGGS_CXX11_STATIC_CONSTEXPR bool viable = true;
            };

            template <typename F, typename U>
            struct result_of<F, U, _ambiguous>
            {
                EGGS_CXX11_STATIC_CONSTEXPR bool viable = true;
            };

            template <typename F, typename U>
            struct result_of<F, U, _fallback>
            {
                EGGS_CXX11_STATIC_CONSTEXPR bool viable = false;
            };
        }

        template <typename U, typename Ts>
        struct index_of_best_match
          : _best_match::result_of<_best_match::overloads<Ts>, U>
        {};

        template <
            typename U, typename Ts
          , bool ImplicitMatch = index_of_best_match<U, Ts>::viable
        >
        struct index_of_explicit_match
        {};

        template <typename U, typename Ts>
        struct index_of_explicit_match<U, Ts, false>
          : _best_match::result_of<_best_match::explicit_overloads<Ts, U>, U>
        {};

        ///////////////////////////////////////////////////////////////////////
        namespace _relops
        {
            struct _fallback {};

            template <typename T, typename U>
            static auto check_has_equal_to(int)
             -> decltype(std::declval<T>() == std::declval<U>());

            template <typename T, typename U>
            static _fallback check_has_equal_to(...);

            template <
                typename T, typename U
              , typename R = decltype(_relops::check_has_equal_to<T, U>(0))
            >
            struct has_equal_to : std::true_type
            {};

            template <typename T, typename U>
            struct has_equal_to<T, U, _fallback> : std::false_type
            {};

            template <typename T, typename U>
            static auto check_has_not_equal_to(int)
             -> decltype(std::declval<T>() != std::declval<U>());

            template <typename T, typename U>
            static _fallback check_has_not_equal_to(...);

            template <
                typename T, typename U
              , typename R = decltype(_relops::check_has_not_equal_to<T, U>(0))
            >
            struct has_not_equal_to : std::true_type
            {};

            template <typename T, typename U>
            struct has_not_equal_to<T, U, _fallback> : std::false_type
            {};

            template <typename T, typename U>
            static auto check_has_less(int)
             -> decltype(std::declval<T>() < std::declval<U>());

            template <typename T, typename U>
            static _fallback check_has_less(...);

            template <
                typename T, typename U
              , typename R = decltype(_relops::check_has_less<T, U>(0))
            >
            struct has_less : std::true_type
            {};

            template <typename T, typename U>
            struct has_less<T, U, _fallback> : std::false_type
            {};

            template <typename T, typename U>
            static auto check_has_greater(int)
             -> decltype(std::declval<T>() > std::declval<U>());

            template <typename T, typename U>
            static _fallback check_has_greater(...);

            template <
                typename T, typename U
              , typename R = decltype(_relops::check_has_greater<T, U>(0))
            >
            struct has_greater : std::true_type
            {};

            template <typename T, typename U>
            struct has_greater<T, U, _fallback> : std::false_type
            {};

            template <typename T, typename U>
            static auto check_has_less_equal(int)
             -> decltype(std::declval<T>() <= std::declval<U>());

            template <typename T, typename U>
            static _fallback check_has_less_equal(...);

            template <
                typename T, typename U
              , typename R = decltype(_relops::check_has_less_equal<T, U>(0))
            >
            struct has_less_equal : std::true_type
            {};

            template <typename T, typename U>
            struct has_less_equal<T, U, _fallback> : std::false_type
            {};

            template <typename T, typename U>
            static auto check_has_greater_equal(int)
             -> decltype(std::declval<T>() >= std::declval<U>());

            template <typename T, typename U>
            static _fallback check_has_greater_equal(...);

            template <
                typename T, typename U
              , typename R = decltype(_relops::check_has_greater_equal<T, U>(0))
            >
            struct has_greater_equal : std::true_type
            {};

            template <typename T, typename U>
            struct has_greater_equal<T, U, _fallback> : std::false_type
            {};
        }

        template <typename T, typename U = T>
        struct has_equal_to : _relops::has_equal_to<T, U>
        {};

        template <typename T, typename U = T>
        struct has_not_equal_to : _relops::has_not_equal_to<T, U>
        {};

        template <typename T, typename U = T>
        struct has_less : _relops::has_less<T, U>
        {};

        template <typename T, typename U = T>
        struct has_greater : _relops::has_greater<T, U>
        {};

        template <typename T, typename U = T>
        struct has_less_equal : _relops::has_less_equal<T, U>
        {};

        template <typename T, typename U = T>
        struct has_greater_equal : _relops::has_greater_equal<T, U>
        {};

        ///////////////////////////////////////////////////////////////////////
#if EGGS_CXX17_STD_HAS_SWAPPABLE_TRAITS
        using std::is_swappable;
        using std::is_nothrow_swappable;
#else
        namespace _swap
        {
            struct _fallback {};

            template <typename T>
            static typename std::enable_if<
                std::is_move_constructible<T>::value
             && std::is_move_assignable<T>::value
            >::type swap(T&, T&)
#  if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
                noexcept(
                    std::is_nothrow_move_constructible<T>::value
                 && std::is_nothrow_move_assignable<T>::value)
#  endif
                ;

            template <typename T>
            static auto check_swap(int)
             -> decltype(swap(std::declval<T&>(), std::declval<T&>()));

            template <typename T>
            static _fallback check_swap(...);

            template <
                typename T
              , typename R = decltype(_swap::check_swap<T>(0))
            >
            struct is_swappable : std::true_type
            {};

            template <typename T>
            struct is_swappable<T, _fallback> : std::false_type
            {};

            template <typename T, bool Swappable = is_swappable<T>::value>
            struct is_nothrow_swappable
            {
                EGGS_CXX11_STATIC_CONSTEXPR bool value =
                    noexcept(swap(std::declval<T&>(), std::declval<T&>()));
            };

            template <typename T>
            struct is_nothrow_swappable<T, false>
            {
                EGGS_CXX11_STATIC_CONSTEXPR bool value = false;
            };
        }

        template <typename T>
        struct is_swappable : _swap::is_swappable<T>
        {};

        template <typename T>
        struct is_nothrow_swappable
          : std::integral_constant<
                bool
              , _swap::is_nothrow_swappable<T>::value
            >
        {};
#endif

        ///////////////////////////////////////////////////////////////////////
        struct access
        {
            template <typename ...Ts, typename Storage = detail::storage<Ts...>>
            EGGS_CXX14_CONSTEXPR static Storage& storage(
                variant<Ts...>& v) noexcept
            {
                return v._storage;
            }

            EGGS_CXX14_CONSTEXPR static detail::empty_storage storage(
                variant<>& /*v*/) noexcept
            {
                return detail::empty_storage();
            }

            template <typename ...Ts, typename Storage = detail::storage<Ts...>>
            EGGS_CXX11_CONSTEXPR static Storage const& storage(
                variant<Ts...> const& v) noexcept
            {
                return v._storage;
            }

            EGGS_CXX11_CONSTEXPR static detail::empty_storage storage(
                variant<> const& /*v*/) noexcept
            {
                return detail::empty_storage();
            }

            template <typename ...Ts, typename Storage = detail::storage<Ts...>>
            EGGS_CXX14_CONSTEXPR static Storage&& storage(
                variant<Ts...>&& v) noexcept
            {
                return detail::move(v._storage);
            }

            EGGS_CXX14_CONSTEXPR static detail::empty_storage storage(
                variant<>&& /*v*/) noexcept
            {
                return detail::empty_storage();
            }

            template <
                typename ...Ts, size_t I
              , typename T = typename at_index<I, pack<Ts...>>::type
            >
            EGGS_CXX14_CONSTEXPR static T& get(
                variant<Ts...>& v, index<I>) noexcept
            {
                return v._storage.get(index<I + 1>{});
            }

            template <
                typename ...Ts, size_t I
              , typename T = typename at_index<I, pack<Ts...>>::type
            >
            EGGS_CXX11_CONSTEXPR static T const& get(
                variant<Ts...> const& v, index<I>) noexcept
            {
                return v._storage.get(index<I + 1>{});
            }
        };

        ///////////////////////////////////////////////////////////////////////
        template <typename V>
        EGGS_CXX11_CONSTEXPR int throw_if_empty(V const& v)
        {
            return bool(v) ? 0 : detail::throw_bad_variant_access<int>();
        }

        ///////////////////////////////////////////////////////////////////////
        namespace _hash
        {
            struct _fallback {};

            template <typename T, typename Hash = std::hash<T>>
            static auto check_hash(int)
             -> decltype(Hash(), std::declval<Hash const&>()(std::declval<T const&>()));

            template <typename T>
            static _fallback check_hash(...);

            template <
                typename T
              , typename R = decltype(_hash::check_hash<T>(0))
            >
            struct is_hashable : std::true_type
            {};

            template <typename T>
            struct is_hashable<T, _fallback> : std::false_type
            {};

            template <typename T, bool Hashable = is_hashable<T>::value>
            struct is_nothrow_hashable
            {
                using Hash = std::hash<T>;

                EGGS_CXX11_STATIC_CONSTEXPR bool value =
                    noexcept(Hash(), std::declval<Hash const&>()(std::declval<T const&>()));
            };

            template <typename T>
            struct is_nothrow_hashable<T, false>
            {
                EGGS_CXX11_STATIC_CONSTEXPR bool value = false;
            };
        }

        template <typename T>
        struct is_hashable : _hash::is_hashable<T>
        {};

        template <typename T>
        struct is_nothrow_hashable
          : std::integral_constant<
                bool
              , _hash::is_nothrow_hashable<T>::value
            >
        {};

        template <typename Ts, bool Enable>
        struct _std_hash;

        template <typename ...Ts>
        struct _std_hash<pack<Ts...>, false>
        {
            _std_hash() = delete;
            _std_hash(_std_hash const&) = delete;
            _std_hash& operator=(_std_hash const&) = delete;
        };

        template <typename ...Ts>
        struct _std_hash<pack<Ts...>, true>
        {
            std::size_t operator()(variant<Ts...> const& v) const
                noexcept(detail::all_of<detail::pack<
                    detail::is_nothrow_hashable<Ts>...
                >>::value)
            {
                return bool(v)
                  ? detail::hash{}(
                        detail::pack<Ts...>{}, v.which()
                      , v.target()
                    )
                  : 0u;
            }
        };

        template <typename ...Ts>
        using std_hash = _std_hash<
            pack<typename std::remove_const<Ts>::type...>,
            all_of<pack<
                is_hashable<typename std::remove_const<Ts>::type>...>>::value
        >;
    }

    ///////////////////////////////////////////////////////////////////////////
    //! template <class ...Ts> class variant;
    //!
    //! In a `variant`, at most one of the members can be active at any time,
    //! that is, the value of at most one of the members can be stored in a
    //! `variant` at any time.  Implementations are not permitted to use
    //! additional storage, such as dynamic memory, to allocate its contained
    //! value. The contained value shall be allocated in a region of the
    //! `variant<Ts...>` storage suitably aligned for the types `Ts...`.
    //!
    //! All `T` in `Ts...` shall be object types and shall satisfy the
    //! requirements of `Destructible`.
    template <typename ...Ts>
    class variant
    {
        static_assert(
            detail::all_of<detail::pack_c<bool,
                detail::checked_is_object<Ts>::value...>>::value
          , "variant alternatives shall be object types");

    public:
        //! static constexpr std::size_t npos = std::size_t(-1);
        EGGS_CXX11_STATIC_CONSTEXPR std::size_t npos = std::size_t(-1);

    public:
        //! constexpr variant() noexcept;
        //!
        //! \postconditions `*this` does not have an active member.
        //!
        //! \remarks No member is initialized. For every object types `Ts...`
        //!  this constructor shall be a `constexpr` constructor.
        EGGS_CXX11_CONSTEXPR variant() noexcept
          : _storage{}
        {}

        //! constexpr variant(variant const& rhs);
        //!
        //! \effects If `rhs` has an active member of type `T`, initializes
        //!  the active member as if direct-non-list-initializing an object of
        //!  type `T` with the expression `*rhs.target<T>()`; otherwise, no
        //!  member is initialized.
        //!
        //! \postconditions `rhs.which() == this->which()`.
        //!
        //! \throws Any exception thrown by the selected constructor of `T`.
        //!
        //! \remarks This constructor shall be defined as deleted unless
        //!  `std::is_copy_constructible_v<T>` is `true` for all `T` in
        //!  `Ts...`. If `std::is_trivially_copyable_v<T>` is `true` for all
        //!  `T` in `Ts...`, then this copy constructor shall be a trivial
        //!  `constexpr` constructor.
        variant(variant const& rhs) = default;

        //! constexpr variant(variant&& rhs) noexcept(see below);
        //!
        //! \effects If `rhs` has an active member of type `T`, initializes
        //!  the active member as if direct-non-list-initializing an object of
        //!  type `T` with the expression `std::move(*rhs.target<T>())`;
        //!  otherwise, no member is initialized. `bool(rhs)` is unchanged.
        //!
        //! \postconditions `rhs.which() == this->which()`.
        //!
        //! \throws Any exception thrown by the selected constructor of `T`.
        //!
        //! \remarks The expression inside `noexcept` is equivalent to the
        //!  logical AND of `std::is_nothrow_move_constructible_v<Ts>...`.
        //!  This constructor shall not participate in overload resolution
        //!  unless `std::is_move_constructible_v<T>` is `true` for all `T` in
        //!  `Ts...`. If `std::is_trivially_copyable_v<T>` is `true` for all
        //!  `T` in `Ts...`, then this move constructor shall be a trivial
        //!  `constexpr` constructor.
        variant(variant&& rhs) = default;

        //! template <class U>
        //! constexpr variant(U&& v);
        //!
        //! Let `T` be one of the types in `Ts...` for which `std::forward<U>(u)`
        //!  is unambiguously convertible to by overload resolution rules.
        //!
        //! \effects Initializes the active member as if direct-non-list-
        //!  initializing an object of type `T` with the expression
        //!  `std::forward<U>(v)`.
        //!
        //! \postconditions `*this` has an active member.
        //!
        //! \throws Any exception thrown by the selected constructor of `T`.
        //!
        //! \remarks This constructor shall not participate in overload
        //!  resolution unless `std::is_same_v<std::decay_t<U>, variant>` is
        //!  `false`, there is a type `T` in `Ts...` for which `std::forward<U>(u)`
        //!  is unambiguously convertible to by overload resolution rules, and
        //!  `std::is_constructible_v<T, U>` is `true`. If `T`'s selected
        //!  constructor is a `constexpr` constructor, this constructor shall
        //!  be a `constexpr` constructor.
        template <
            typename U
          , typename NoCopy = typename std::enable_if<!std::is_same<
                typename std::decay<U>::type, variant>::value>::type
          , typename NoTag = typename std::enable_if<!detail::is_inplace_tag<
                typename std::decay<U>::type>::value>::type
          , std::size_t I = detail::index_of_best_match<
                U&&, detail::pack<Ts...>>::value
          , typename T = typename detail::at_index<
                I, detail::pack<Ts...>>::type
          , typename std::enable_if<
                std::is_constructible<T, U>::value
             && std::is_convertible<U, T>::value
              , bool>::type = true
        >
        EGGS_CXX11_CONSTEXPR variant(U&& v)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<T, U>::value)
#endif
          : _storage{detail::index<I + 1>{}, detail::forward<U>(v)}
        {}

        //! template <class U>
        //! constexpr explicit variant(U&& v);
        //!
        //! Let `T` be one of the types in `Ts...` for which
        //!  `std::is_constructible_v<T, U>` is `true`.
        //!
        //! \effects Initializes the active member as if direct-non-list-
        //!  initializing an object of type `T` with the expression
        //!  `std::forward<U>(v)`.
        //!
        //! \postconditions `*this` has an active member of type `T`.
        //!
        //! \throws Any exception thrown by the selected constructor of `T`.
        //!
        //! \remarks This constructor shall not participate in overload
        //!  resolution unless `std::is_same_v<std::decay_t<U>, variant>` is
        //!  `false`, `std::forward<U>(u)` is not convertible to any `T` in
        //!  `Ts...`, and there is a single type `T` in `Ts...` for which
        //!  `std::is_constructible_v<T, U>` is `true`. If `T`'s selected
        //!  constructor is a `constexpr` constructor, this constructor shall
        //!  be a `constexpr` constructor.
        template <
            typename U
          , typename NoCopy = typename std::enable_if<!std::is_same<
                typename std::decay<U>::type, variant
            >::value>::type
          , typename NoTag = typename std::enable_if<!detail::is_inplace_tag<
                typename std::decay<U>::type>::value>::type
          , std::size_t I = detail::index_of_explicit_match<
                U&&, detail::pack<Ts...>>::value
          , typename T = typename detail::at_index<
                I, detail::pack<Ts...>>::type
          , typename std::enable_if<
                std::is_constructible<T, U>::value
             && !std::is_convertible<U, T>::value
              , bool>::type = false
        >
        EGGS_CXX11_CONSTEXPR explicit variant(U&& v)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<T, U>::value)
#endif
          : _storage{detail::index<I + 1>{}, std::forward<U>(v)}
        {}

        //! template <std::size_t I, class ...Args>
        //! constexpr explicit variant(in_place_index_t<I>, Args&&... args);
        //!
        //! Let `T` be the `I`th element in `Ts...`, where indexing is
        //! zero-based.
        //!
        //! \effects Initializes the active member as if direct-non-list-
        //!  initializing an object of type `T` with the arguments
        //!  `std::forward<Args>(args)...`.
        //!
        //! \postconditions `*this` has an active member of type `T`.
        //!
        //! \throws Any exception thrown by the selected constructor of `T`.
        //!
        //! \remarks This constructor shall not participate in overload
        //!  resolution unless `I < sizeof...(Ts)`, and
        //!  `std::is_constructible_v<T, Args...>` is `true`. If `T`'s
        //!  selected constructor is a `constexpr` constructor, this
        //!  constructor shall be a `constexpr` constructor.
        template <
            std::size_t I, typename ...Args
          , typename T = typename detail::at_index<
                I, detail::pack<Ts...>>::type
          , typename Enable = typename std::enable_if<
                std::is_constructible<T, Args...>::value>::type
        >
        EGGS_CXX11_CONSTEXPR explicit variant(
            in_place_index_t<I>, Args&&... args)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<T, Args...>::value)
#endif
          : _storage{detail::index<I + 1>{}, detail::forward<Args>(args)...}
        {}

        //! template <std::size_t I, class U, class ...Args>
        //! constexpr explicit variant(in_place_index_t<I>, std::initializer_list<U> il, Args&&... args);
        //!
        //! Let `T` be the `I`th element in `Ts...`, where indexing is
        //! zero-based.
        //!
        //! \effects Initializes the active member as if direct-non-list-
        //!  initializing an object of type `T` with the arguments `il,
        //!  std::forward<Args>(args)...`.
        //!
        //! \postconditions `*this` has an active member of type `T`.
        //!
        //! \throws Any exception thrown by the selected constructor of `T`.
        //!
        //! \remarks This constructor shall not participate in overload
        //!  resolution unless `I < sizeof...(Ts)`, and
        //!  `std::is_constructible_v<T, std::initializer_list<U>&, Args...>`
        //!  is `true`. If `T`'s selected constructor is a `constexpr`
        //!  constructor, this constructor shall be a `constexpr` constructor.
        template <
            std::size_t I, typename U, typename ...Args
          , typename T = typename detail::at_index<
                I, detail::pack<Ts...>>::type
          , typename Enable = typename std::enable_if<std::is_constructible<
                T, std::initializer_list<U>&, Args...>::value>::type
        >
        EGGS_CXX11_CONSTEXPR explicit variant(
            in_place_index_t<I>, std::initializer_list<U> il, Args&&... args)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<
                T, std::initializer_list<U>&, Args...>::value)
#endif
          : _storage{detail::index<I + 1>{}, il, detail::forward<Args>(args)...}
        {}

        //! template <class T, class ...Args>
        //! constexpr explicit variant(in_place_type_t<T>, Args&&... args);
        //!
        //! \effects Equivalent to `variant(in_place<I>,
        //!  std::forward<Args>(args)...)` where `I` is the zero-based index
        //!  of `T` in `Ts...`.
        //!
        //! \remarks This constructor shall not participate in overload
        //!  resolution unless there is exactly one occurrence of `T` in
        //!  `Ts...`, and `std::is_constructible_v<T, Args...>` is `true`. If
        //!  `T`'s selected constructor is a `constexpr` constructor, this
        //!  constructor shall be a `constexpr` constructor.
        template <
            typename T, typename ...Args
          , std::size_t I = detail::index_of<
                T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
          , typename Enable = typename std::enable_if<std::is_constructible<
                T, Args...>::value>::type
        >
        EGGS_CXX11_CONSTEXPR explicit variant(
            in_place_type_t<T>, Args&&... args)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<T, Args...>::value)
#endif
          : _storage{detail::index<I + 1>{}, detail::forward<Args>(args)...}
        {}

        //! template <class T, class U, class ...Args>
        //! constexpr explicit variant(in_place_type_t<T>, std::initializer_list<U> il, Args&&... args);
        //!
        //! \effects Equivalent to `variant(in_place<I>, il,
        //!  std::forward<Args>(args)...)` where `I` is the zero-based index
        //!  of `T` in `Ts...`.
        //!
        //! \remarks This constructor shall not participate in overload
        //!  resolution unless there is exactly one occurrence of `T` in
        //!  `Ts...`, and `std::is_constructible_v<T, std::initializer_list<U>&,
        //!  Args&&...>` is `true`. If `T`'s selected constructor is a
        //!  `constexpr` constructor, this constructor shall be a `constexpr`
        //!  constructor.
        template <
            typename T, typename U, typename ...Args
          , std::size_t I = detail::index_of<
                T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
          , typename Enable = typename std::enable_if<std::is_constructible<
                T, std::initializer_list<U>&, Args...>::value>::type
        >
        EGGS_CXX11_CONSTEXPR explicit variant(
            in_place_type_t<T>, std::initializer_list<U> il, Args&&... args)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<
                T, std::initializer_list<U>&, Args...>::value)
#endif
          : _storage{detail::index<I + 1>{}, il, detail::forward<Args>(args)...}
        {}

        //! ~variant();
        //!
        //! \effects If `*this` has an active member of type `T`, destroys the
        //!  active member as if by calling `target<T>()->~T()`.
        //!
        //! \remarks If `std::is_trivially_destructible_v<T>` is `true` for all
        //!  `T` in `Ts...`, then this destructor shall be trivial.
        ~variant() = default;

        //! constexpr variant& operator=(variant const& rhs);
        //!
        //! \effects
        //!  - If both `*this` and `rhs` have an active member of type `T`,
        //!    assigns to the active member the expression `*rhs.target<T>()`;
        //!
        //!  - otherwise, if `*this` has an active member of type `U`, destroys
        //!    the active member by calling `target<U>()->~U()`. Then, if `rhs`
        //!    has an active member of type `T`, initializes the active member
        //!    as if direct-non-list-initializing an object of type `T` with
        //!    the expression `*rhs.target<T>()`.
        //!
        //! \returns `*this`.
        //!
        //! \postconditions `rhs.which() == this->which()`.
        //!
        //! \remarks If an exception is thrown during the call to `T`'s copy
        //!  assignment, the state of the active member is as defined by the
        //!  exception safety guarantee of `T`'s copy assignment. If an
        //!  exception is thrown during the call to `T`'s copy constructor,
        //!  `*this` has no active member, and the previous active member (if
        //!  any) has been destroyed. This function shall be defined as
        //!  deleted unless `std::is_copy_assignable_v<T> &&
        //!  std::is_copy_constructible_v<T>` is `true` for all `T` in `Ts...`.
        //!  If `std::is_trivially_copyable_v<T>` is `true` for all `T` in
        //!  `Ts...`, then this copy assignment operator shall be a trivial
        //!  `constexpr` assignment operator.
        variant& operator=(variant const& rhs) = default;

        //! constexpr variant& operator=(variant&& rhs) noexcept(see below);
        //!
        //! \effects
        //!  - If both `*this` and `rhs` have an active member of type `T`,
        //!    assigns to the active member the expression
        //!    `std::move(*rhs.target<T>())`;
        //!
        //!  - otherwise, if `*this` has an active member of type `U`, destroys
        //!    the active member by calling `target<U>()->~U()`. Then, if `rhs`
        //!    has an active member of type `T`, initializes the active member
        //!    as if direct-non-list-initializing an object of type `T` with
        //!    the expression `std::move(*rhs.target<T>())`.
        //!
        //!  `bool(rhs)` is unchanged.
        //!
        //! \returns `*this`.
        //!
        //! \postconditions `rhs.which() == this->which()`.
        //!
        //! \remarks If an exception is thrown during the call to `T`'s move
        //!  assignment, the state of both active members is determined by the
        //!  exception safety guarantee of `T`'s move assignment. If an
        //!  exception is thrown during the call to `T`'s move constructor,
        //!  `*this` has no active member, the previous active member (if any)
        //!  has been destroyed, and the state of the active member of `rhs`
        //!  is determined by the exception safety guarantee of `T`'s move
        //!  constructor. The expression inside `noexcept` is equivalent to
        //!  the logical AND of `std::is_nothrow_move_assignable_v<Ts>...` and
        //!  `std::is_nothrow_move_constructible_v<Ts>...`. This function
        //!  shall not participate in overload resolution unless
        //!  `std::is_move_assignable_v<T> && std::is_move_constructible_v<T>`
        //!  is `true` for all `T` in `Ts...`. If `std::is_trivially_copyable_v<T>`
        //!  is `true` for all `T` in `Ts...`, then this move assignment
        //!  operator shall be a trivial `constexpr` assignment operator.
        variant& operator=(variant&& rhs) = default;

        //! template <class U>
        //! constexpr variant& operator=(U&& v);
        //!
        //! Let `T` be one of the types in `Ts...` for which `std::forward<U>(u)`
        //!  is unambiguously convertible to by overload resolution rules.
        //!
        //! \effects
        //!  - If `*this` has an active member of type `T`, assigns to the
        //!    active member the expression `std::forward<U>(v)`;
        //!
        //!  - otherwise, calls `*this = {}`. Then, initializes the active
        //!    member as if direct-non-list-initializing an object of
        //!    type `T` with the expression `std::forward<U>(v)`.
        //!
        //! \returns `*this`.
        //!
        //! \postconditions `*this` has an active member.
        //!
        //! \remarks If an exception is thrown during the call to `T`'s
        //!  assignment, the state of the active member is as defined by the
        //!  exception safety guarantee of `T`'s assignment. If an exception
        //!  is thrown during the call to `T`'s constructor, `*this` has no
        //!  active member, and the previous active member (if any) has been
        //!  destroyed. This function shall not participate in overload
        //!  resolution unless `std::is_same_v<std::decay_t<U>, variant>` is
        //!  `false`, there is a type `T` in `Ts...` for which `std::forward<U>(u)`
        //!  is unambiguously convertible to by overload resolution rules, and
        //!  `std::is_constructible_v<T, U>` is `true`. This function shall be
        //!  defined as deleted unless `std::is_assignable_v<T&, U>` is `true`.
        //!  If `std::is_trivially_copyable_v<T>` is `true` for all `T` in
        //!  `Ts...` and `T`'s selected constructor is a `constexpr`
        //!  constructor, then this function shall be a `constexpr` function.
        template <
            typename U
          , typename NoCopy = typename std::enable_if<!std::is_same<
                typename std::decay<U>::type, variant
            >::value>::type
          , std::size_t I = detail::index_of_best_match<
                U&&, detail::pack<Ts...>>::value
          , typename T = typename detail::at_index<
                I, detail::pack<Ts...>>::type
          , typename std::enable_if<
                std::is_assignable<T&, U>::value
             && std::is_constructible<T, U>::value, bool>::type = true
        >
        EGGS_CXX14_CONSTEXPR variant& operator=(U&& v)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_assignable<T&, U>::value
                  && std::is_nothrow_constructible<T, U>::value)
#endif
        {
            if (_storage.which() == I + 1)
            {
                _storage.get(detail::index<I + 1>{}) = detail::forward<U>(v);
            } else {
                _storage.emplace(detail::index<I + 1>{}, detail::forward<U>(v));
            }
            return *this;
        }

        template <
            typename U
          , typename NoCopy = typename std::enable_if<!std::is_same<
                typename std::decay<U>::type, variant
            >::value>::type
          , std::size_t I = detail::index_of_best_match<
                U&&, detail::pack<Ts...>>::value
          , typename T = typename detail::at_index<
                I, detail::pack<Ts...>>::type
          , typename std::enable_if<
                !std::is_assignable<T&, U>::value, bool>::type = false
        >
        EGGS_CXX14_CONSTEXPR variant& operator=(U&& v) = delete;

        //! template <std::size_t I, class ...Args>
        //! constexpr T& emplace(Args&&... args);
        //!
        //! Let `T` be the `I`th element in `Ts...`, where indexing is
        //! zero-based.
        //!
        //! \requires `I < sizeof...(Ts)`.
        //!
        //! \effects Calls `*this = {}`. Then, initializes the active member
        //!  as if direct-non-list-initializing  an object of type `T` with
        //!  the arguments `std::forward<Args>(args)...`.
        //!
        //! \postconditions `*this` has an active member of type `T`.
        //!
        //! \returns  A reference to the new contained value.
        //!
        //! \throws Any exception thrown by the selected constructor of `T`.
        //!
        //! \remarks If an exception is thrown during the call to `T`'s
        //!  constructor, `*this` has no active member, and the previous
        //!  active member (if any) has been destroyed. This function shall
        //!  not participate in overload resolution unless
        //!  `std::is_constructible_v<T, Args...>` is `true`.  If
        //!  `std::is_trivially_copyable_v<T> && std::is_copy_assignable_v<T>`
        //!  is `true` for all `T` in `Ts...` and `T`'s selected constructor
        //!  is a `constexpr` constructor, then this function shall be a
        //!  `constexpr` function.
        template <
            std::size_t I, typename ...Args
          , typename T = typename detail::checked_at_index<
                I, detail::pack<Ts...>>::type
          , typename Enable = typename std::enable_if<
                std::is_constructible<T, Args...>::value>::type
        >
        EGGS_CXX14_CONSTEXPR T& emplace(Args&&... args)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<T, Args...>::value)
#endif
        {
            return _storage.emplace(
                detail::index<I + 1>{}, detail::forward<Args>(args)...);
        }

        //! template <std::size_t I, class U, class ...Args>
        //! constexpr T& emplace(std::initializer_list<U> il, Args&&... args);
        //!
        //! Let `T` be the `I`th element in `Ts...`, where indexing is
        //! zero-based.
        //!
        //! \requires `I < sizeof...(Ts)`.
        //!
        //! \effects Calls `*this = {}`. Then, initializes the active member
        //!  as if direct-non-list-initializing an object of type `T` with
        //!  the arguments `il, std::forward<Args>(args)...`.
        //!
        //! \postconditions `*this` has an active member of type `T`.
        //!
        //! \returns  A reference to the new contained value.
        //!
        //! \throws Any exception thrown by the selected constructor of `T`.
        //!
        //! \remarks If an exception is thrown during the call to `T`'s
        //!  constructor, `*this` has no active member, and the previous
        //!  active member (if any) has been destroyed. This function shall
        //!  not participate in overload resolution unless
        //!  `std::is_constructible_v<T, std::initializer_list<U>&, Args...>`
        //!  is `true`. If `std::is_trivially_copyable_v<T> &&
        //!  std::is_copy_assignable_v<T>` is `true` for all `T` in `Ts...`
        //!  and `T`'s selected constructor is a `constexpr` constructor, then
        //!  this function shall be a `constexpr` function.
        template <
            std::size_t I, typename U, typename ...Args
          , typename T = typename detail::checked_at_index<
                I, detail::pack<Ts...>>::type
          , typename Enable = typename std::enable_if<std::is_constructible<
                T, std::initializer_list<U>&, Args...>::value>::type
        >
        EGGS_CXX14_CONSTEXPR T& emplace(std::initializer_list<U> il, Args&&... args)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<
                T, std::initializer_list<U>&, Args...>::value)
#endif
        {
            return _storage.emplace(
                detail::index<I + 1>{}, il, detail::forward<Args>(args)...);
        }

        //! template <class T, class ...Args>
        //! constexpr T& emplace(Args&&... args);
        //!
        //! \requires `T` shall occur exactly once in `Ts...`.
        //!
        //! \effects Equivalent to `emplace<I>(std::forward<Args>(args)...)`
        //!  where `I` is the zero-based index of `T` in `Ts...`.
        //!
        //! \remarks This function shall not participate in overload resolution
        //!  unless `std::is_constructible_v<T, Args...>` is `true`. If
        //!  `std::is_trivially_copyable_v<T> && std::is_copy_assignable_v<T>`
        //!  is `true` for all `T` in `Ts...` and `T`'s selected constructor
        //!  is a `constexpr` constructor, then this function shall be a
        //!  `constexpr` function.
        template <
            typename T, typename ...Args
          , std::size_t I = detail::checked_index_of<
                T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
          , typename Enable = typename std::enable_if<
                std::is_constructible<T, Args...>::value>::type
        >
        EGGS_CXX14_CONSTEXPR T& emplace(Args&&... args)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<T, Args...>::value)
#endif
        {
            return _storage.emplace(
                detail::index<I + 1>{}, detail::forward<Args>(args)...);
        }

        //! template <class T, class U, class ...Args>
        //! constexpr T& emplace(std::initializer_list<U> il, Args&&... args);
        //!
        //! \requires `T` shall occur exactly once in `Ts...`.
        //!
        //! \effects Equivalent to `emplace<I>(il, std::forward<Args>(args)...)`
        //!  where `I` is the zero-based index of `T` in `Ts...`.
        //!
        //! \remarks This function shall not participate in overload resolution
        //!  unless `std::is_constructible_v<T, std::initializer_list<U>&,
        //!  Args...>` is `true`. If `std::is_trivially_copyable_v<T> &&
        //!  std::is_copy_assignable_v<T>` is `true` for all `T` in `Ts...` and
        //!  `T`'s selected constructor is a `constexpr` constructor, then this
        //!  function shall be a `constexpr` function.
        template <
            typename T, typename U, typename ...Args
          , std::size_t I = detail::checked_index_of<
                T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
          , typename Enable = typename std::enable_if<std::is_constructible<
                T, std::initializer_list<U>&, Args...>::value>::type
        >
        EGGS_CXX14_CONSTEXPR T& emplace(std::initializer_list<U> il, Args&&... args)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(std::is_nothrow_constructible<
                T, std::initializer_list<U>&, Args...>::value)
#endif
        {
            return _storage.emplace(
                detail::index<I + 1>{}, il, detail::forward<Args>(args)...);
        }

        //! constexpr void swap(variant& rhs) noexcept(see below);
        //!
        //! \requires Lvalues of `T` shall be swappable and
        //!  `std::is_move_constructible_v<T>` is `true` for all `T` in
        //!  `Ts...`.
        //!
        //! \effects
        //!  - If both `*this` and `rhs` have an active member of type `T`,
        //!    calls `swap(*this->target<T>(), *rhs.target<T>())`;
        //!
        //!  - otherwise, calls `std::swap(*this, rhs)`.
        //!
        //! \remarks If an exception is thrown during the call to function
        //!  `swap` the state of the active members of type `T` is determined
        //!  by the exception safety guarantee of `swap` for lvalues of `T`.
        //!  If an exception is thrown during the call to a move constructor,
        //!  the state of `*this` and `rhs` is unspecified. The expression
        //!  inside `noexcept` is equivalent to the logical AND of
        //!  `std::is_nothrow_move_constructible_v<T> &&
        //!  std::is_nothrow_swappable_v<T>` for all `T` in `Ts...`. If
        //!  `std::is_trivially_copyable_v<T> && std::is_copy_assignable_v<T>`
        //!  is  `true` for all `T` in `Ts...`, then this function shall be a
        //!  `constexpr` function.
        EGGS_CXX14_CONSTEXPR void swap(variant& rhs)
#if EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
            noexcept(detail::all_of<detail::pack<
                detail::is_nothrow_swappable<Ts>...
              , std::is_nothrow_move_constructible<Ts>...
            >>::value)
#endif
        {
            _storage.swap(rhs._storage);
        }

        //! constexpr explicit operator bool() const noexcept;
        //!
        //! \returns `true` if and only if `*this` has an active member.
        //!
        //! \remarks This function shall be a `constexpr` function.
        EGGS_CXX11_CONSTEXPR explicit operator bool() const noexcept
        {
            return _storage.which() != 0;
        }

        //! constexpr std::size_t which() const noexcept;
        //!
        //! \returns The zero-based index of the active member if `*this` has
        //!  one. Otherwise, returns `npos`.
        //!
        //! \remarks This function shall be a `constexpr` function.
        EGGS_CXX11_CONSTEXPR std::size_t which() const noexcept
        {
            return _storage.which() != 0 ? _storage.which() - 1 : npos;
        }

#if EGGS_CXX98_HAS_RTTI
        //! constexpr std::type_info const& target_type() const noexcept;
        //!
        //! \returns If `*this` has an active member of type `T`, `typeid(T)`;
        //!  otherwise `typeid(void)`.
        //!
        //! \remarks This function shall be a `constexpr` function.
        EGGS_CXX11_CONSTEXPR std::type_info const& target_type() const noexcept
        {
            return _storage.which() != 0
              ? detail::type_id{}(
                    detail::pack<Ts...>{}, _storage.which() - 1
                )
              : typeid(void);
        }
#endif

        //! constexpr void* target() noexcept;
        //!
        //! \returns If `*this` has an active member, a pointer to the active
        //!  member; otherwise a null pointer.
        //!
        //! \remarks This function shall be a `constexpr` function.
        EGGS_CXX14_CONSTEXPR void* target() noexcept
        {
            return _storage.which() != 0 ? _storage.target() : nullptr;
        }

        //! constexpr void const* target() const noexcept;
        //!
        //! \returns If `*this` has an active member, a pointer to the active
        //!  member; otherwise a null pointer.
        //!
        //! \remarks This function shall be a `constexpr` function.
        EGGS_CXX11_CONSTEXPR void const* target() const noexcept
        {
            return _storage.which() != 0 ? _storage.target() : nullptr;
        }

        //! template <class T>
        //! constexpr T* target() noexcept;
        //!
        //! \returns If `*this` has an active member of type `T` or of a type
        //!  of which `T` is an unambiguous and accessible base class, a
        //!  pointer to the active member; otherwise a null pointer.
        //!
        //! \remarks This function shall be a `constexpr` function.
        template <typename T>
        EGGS_CXX14_CONSTEXPR T* target() noexcept
        {
            return _storage.which() != 0
              ? detail::target<T, detail::storage<Ts...>>{}(
                    detail::typed_index_pack<detail::pack<detail::empty, Ts...>>{}
                  , _storage.which(), _storage
                )
              : nullptr;
        }

        //! template <class T>
        //! constexpr T const* target() const noexcept;
        //!
        //! \returns If `*this` has an active member of type `T` or of a type
        //!  of which `T` is an unambiguous and accessible base class, a
        //!  pointer to the active member; otherwise a null pointer.
        //!
        //! \remarks This function shall be a `constexpr` function.
        template <typename T>
        EGGS_CXX11_CONSTEXPR T const* target() const noexcept
        {
            return _storage.which() != 0
              ? detail::target<T const, detail::storage<Ts...> const>{}(
                    detail::typed_index_pack<detail::pack<detail::empty, Ts...>>{}
                  , _storage.which(), _storage
                )
              : nullptr;
        }

    private:
        friend struct detail::access;
        detail::storage<Ts...> _storage;
    };

    template <>
    class variant<>
    {
    public:
        EGGS_CXX11_STATIC_CONSTEXPR std::size_t npos = std::size_t(-1);

    public:
        EGGS_CXX11_CONSTEXPR variant() noexcept {}
        variant(variant const&) = default;
        variant(variant&&) = default;

        variant& operator=(variant const&) = default;
        variant& operator=(variant&&) = default;

        EGGS_CXX14_CONSTEXPR void swap(variant&) noexcept {}

        EGGS_CXX11_CONSTEXPR explicit operator bool() const noexcept { return false; }
        EGGS_CXX11_CONSTEXPR std::size_t which() const noexcept { return npos; }
#if EGGS_CXX98_HAS_RTTI
        EGGS_CXX11_CONSTEXPR std::type_info const& target_type() const noexcept { return typeid(void); }
#endif
        EGGS_CXX14_CONSTEXPR void* target() noexcept { return nullptr; }
        EGGS_CXX11_CONSTEXPR void const* target() const noexcept { return nullptr; }
        template <typename T>
        EGGS_CXX14_CONSTEXPR T* target() noexcept { return nullptr; }
        template <typename T>
        EGGS_CXX11_CONSTEXPR T const* target() const noexcept { return nullptr; }
    };

    template <typename ...Ts>
    std::size_t const variant<Ts...>::npos;

    ///////////////////////////////////////////////////////////////////////////
    //! template <class T>
    //! struct variant_size;
    //!
    //! \remarks All specializations of `variant_size<T>` shall meet the
    //!  `UnaryTypeTrait` requirements with a `BaseCharacteristic` of
    //!  `std::integral_constant<std::size_t, N>` for some `N` if `T` is a
    //!  variant-like type; otherwise it shall be empty.
    template <typename T>
    struct variant_size
    {};

    //! template <class ...Ts>
    //! struct variant_size<variant<Ts...>>;
    //!
    //! \remarks Has a `BaseCharacteristic` of `std::integral_constant<
    //!  std::size_t, sizeof...(Ts)>`.
    template <typename ...Ts>
    struct variant_size<variant<Ts...>>
      : std::integral_constant<std::size_t, sizeof...(Ts)>
    {};

    //! template <class T>
    //! struct variant_size<T const>;
    //!
    //! \remarks Let `VS` denote `variant_size<T>` of the cv-unqualified type
    //!  `T`. Has a `BaseCharacteristic` of `std::integral_constant<
    //!  std::size_t, VS::value>` if `T` is a variant-like type; otherwise
    //!  it is empty.
    template <typename T>
    struct variant_size<T const>
      : variant_size<T>
    {};

#if EGGS_CXX14_HAS_VARIABLE_TEMPLATES
    //! template <class T>
    //! constexpr std::size_t variant_size_v = variant_size<T>::value;
    template <typename T>
    EGGS_CXX11_CONSTEXPR std::size_t variant_size_v = variant_size<T>::value;
#endif

    //! template <std::size_t I, class T>
    //! struct variant_element; // undefined
    //!
    //! \remarks All specializations of `variant_element<I, T>` shall meet the
    //!  `TransformationTrait` requirements with a member typedef `type` that
    //!  names the `I`th member of `T`, where indexing is zero-based.
    template <std::size_t I, typename T>
    struct variant_element; // undefined

    //! template <std::size_t I, class ...Ts>
    //! struct variant_element<I, variant<Ts...>>;
    //!
    //! \requires `I < sizeof...(Ts)`. Otherwise, the program is ill-formed.
    //!
    //! \remarks The member typedef `type` shall name the type of the `I`th
    //!  element of `Ts...`, where indexing is zero-based.
    template <std::size_t I, typename ...Ts>
    struct variant_element<I, variant<Ts...>>
      : detail::at_index<I, detail::pack<Ts...>>
    {
        static_assert(I < sizeof...(Ts), "variant_element index out of range");
    };

    //! template <std::size_t I, class T>
    //! struct variant_element<I, T const>;
    //!
    //! \remarks Let `VE` denote `variant_element<I, T>` of the cv-unqualified
    //!  type `T`. The member typedef `type` names `std::add_const_t<
    //!  typename VE::type>`.
    template <std::size_t I, typename T>
    struct variant_element<I, T const>
      : std::add_const<typename variant_element<I, T>::type>
    {};

    //! template <std::size_t I, class T>
    //! using variant_element_t = class variant_element<I, T>::type;
    template <std::size_t I, typename T>
    using variant_element_t = typename variant_element<I, T>::type;

    ///////////////////////////////////////////////////////////////////////////
    //! template <std::size_t I, class ...Ts>
    //! constexpr variant_element_t<I, variant<Ts...>>& get(variant<Ts...>& v);
    //!
    //! \requires `I < sizeof...(Ts)`. Otherwise, the program is ill-formed.
    //!
    //! \returns A reference to the `I`th member of `v` if it is active, where
    //!  indexing is zero-based.
    //!
    //! \throws `bad_variant_access` if the `I`th member of `v` is not active.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        std::size_t I, typename ...Ts
      , typename T = typename detail::checked_at_index<
            I, detail::pack<Ts...>>::type
    >
    EGGS_CXX14_CONSTEXPR T& get(variant<Ts...>& v)
    {
        return v.which() == I
          ? detail::access::get(v, detail::index<I>{})
          : detail::throw_bad_variant_access<T&>();
    }

    //! template <std::size_t I, class ...Ts>
    //! constexpr variant_element_t<I, variant<Ts...>> const& get(variant<Ts...> const& v);
    //!
    //! \requires `I < sizeof...(Ts)`. Otherwise, the program is ill-formed.
    //!
    //! \returns A const reference to the `I`th member of `v` if it is active,
    //!  where indexing is zero-based.
    //!
    //! \throws `bad_variant_access` if the `I`th member of `v` is not active.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        std::size_t I, typename ...Ts
      , typename T = typename detail::checked_at_index<
            I, detail::pack<Ts...>>::type
    >
    EGGS_CXX11_CONSTEXPR T const& get(variant<Ts...> const& v)
    {
        return v.which() == I
          ? detail::access::get(v, detail::index<I>{})
          : detail::throw_bad_variant_access<T const&>();
    }

    //! template <std::size_t I, class ...Ts>
    //! constexpr variant_element_t<I, variant<Ts...>>&& get(variant<Ts...>&& v);
    //!
    //! \requires `I < sizeof...(Ts)`. Otherwise, the program is ill-formed.
    //!
    //! \effects Equivalent to return `std::forward<variant_element_t<I,
    //!  variant<Ts...>>>(get<I>(v))`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        std::size_t I, typename ...Ts
      , typename T = typename detail::checked_at_index<
            I, detail::pack<Ts...>>::type
    >
    EGGS_CXX14_CONSTEXPR T&& get(variant<Ts...>&& v)
    {
        return detail::forward<T>(variants::get<I>(v));
    }

    //! template <std::size_t I, class ...Ts>
    //! constexpr variant_element_t<I, variant<Ts...>> const&& get(variant<Ts...> const&& v);
    //!
    //! \requires `I < sizeof...(Ts)`. Otherwise, the program is ill-formed.
    //!
    //! \effects Equivalent to return `std::forward<variant_element_t<I,
    //!  variant<Ts...>> const>(get<I>(v))`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        std::size_t I, typename ...Ts
      , typename T = typename detail::checked_at_index<
            I, detail::pack<Ts...>>::type
    >
    EGGS_CXX14_CONSTEXPR T const&& get(variant<Ts...> const&& v)
    {
        return detail::forward<T const>(variants::get<I>(v));
    }

    //! template <class T, class ...Ts>
    //! constexpr T& get(variant<Ts...>& v);
    //!
    //! \requires The type `T` occurs exactly once in `Ts...`. Otherwise, the
    //!  program is ill-formed.
    //!
    //! \returns A reference to the active member of `v` if it is of type `T`.
    //!
    //! \throws `bad_variant_access` if the active member of `v` is not of
    //!  type `T`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        typename T, typename ...Ts
      , std::size_t I = detail::checked_index_of<
            T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
    >
    EGGS_CXX14_CONSTEXPR T& get(variant<Ts...>& v)
    {
        return v.which() == I
          ? detail::access::get(v, detail::index<I>{})
          : detail::throw_bad_variant_access<T&>();
    }

    //! template <class T, class ...Ts>
    //! constexpr T const& get(variant<Ts...> const& v);
    //!
    //! \requires The type `T` occurs exactly once in `Ts...`. Otherwise, the
    //!  program is ill-formed.
    //!
    //! \returns A const reference to the active member of `v` if it is of
    //!  type `T`.
    //!
    //! \throws `bad_variant_access` if the active member of `v` is not of
    //!  type `T`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        typename T, typename ...Ts
      , std::size_t I = detail::checked_index_of<
            T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
    >
    EGGS_CXX11_CONSTEXPR T const& get(variant<Ts...> const& v)
    {
        return v.which() == I
          ? detail::access::get(v, detail::index<I>{})
          : detail::throw_bad_variant_access<T const&>();
    }

    //! template <class T, class ...Ts>
    //! constexpr T&& get(variant<Ts...>&& v);
    //!
    //! \requires The type `T` occurs exactly once in `Ts...`. Otherwise, the
    //!  program is ill-formed.
    //!
    //! \effects Equivalent to return `std::forward<T>(get<T>(v))`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        typename T, typename ...Ts
      , std::size_t I = detail::checked_index_of<
            T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
    >
    EGGS_CXX14_CONSTEXPR T&& get(variant<Ts...>&& v)
    {
        return detail::forward<T>(variants::get<T>(v));
    }

    //! template <class T, class ...Ts>
    //! constexpr T const&& get(variant<Ts...> const&& v);
    //!
    //! \requires The type `T` occurs exactly once in `Ts...`. Otherwise, the
    //!  program is ill-formed.
    //!
    //! \effects Equivalent to return `std::forward<T const>(get<T>(v))`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        typename T, typename ...Ts
      , std::size_t I = detail::checked_index_of<
            T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
    >
    EGGS_CXX14_CONSTEXPR T const&& get(variant<Ts...> const&& v)
    {
        return detail::forward<T const>(variants::get<T>(v));
    }

    ///////////////////////////////////////////////////////////////////////////
    //! template <std::size_t I, class ...Ts>
    //! constexpr variant_element_t<I, variant<Ts...>>* get_if(variant<Ts...>* v) noexcept;
    //!
    //! \requires `I < sizeof...(Ts)`. Otherwise, the program is ill-formed.
    //!
    //! \returns If `v != nullptr`, a pointer to the `I`th member of `*v` if
    //!  it is active, where indexing is zero-based; otherwise, `nullptr`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        std::size_t I, typename ...Ts
      , typename T = typename detail::checked_at_index<
            I, detail::pack<Ts...>>::type
    >
    EGGS_CXX14_CONSTEXPR T* get_if(variant<Ts...>* v) noexcept
    {
        return v && v->which() == I
          ? detail::addressof(detail::access::get(*v, detail::index<I>{}))
          : nullptr;
    }

    //! template <std::size_t I, class ...Ts>
    //! constexpr variant_element_t<I, variant<Ts...>> const* get_if(variant<Ts...> const* v) noexcept;
    //!
    //! \requires `I < sizeof...(Ts)`. Otherwise, the program is ill-formed.
    //!
    //! \returns If `v != nullptr`, a const pointer to the `I`th member of `*v`
    //!  if it is active, where indexing is zero-based; otherwise, `nullptr`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        std::size_t I, typename ...Ts
      , typename T = typename detail::checked_at_index<
            I, detail::pack<Ts...>>::type
    >
    EGGS_CXX11_CONSTEXPR T const* get_if(variant<Ts...> const* v) noexcept
    {
        return v && v->which() == I
          ? detail::addressof(detail::access::get(*v, detail::index<I>{}))
          : nullptr;
    }

    //! template <class T, class ...Ts>
    //! constexpr T* get_if(variant<Ts...>* v) noexcept;
    //!
    //! \requires The type `T` occurs exactly once in `Ts...`. Otherwise, the
    //!  program is ill-formed.
    //!
    //! \returns If `v != nullptr`, a pointer to the active member of `*v` if
    //!  it is of type `T`; otherwise,`nullptr`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        typename T, typename ...Ts
      , std::size_t I = detail::checked_index_of<
            T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
    >
    EGGS_CXX14_CONSTEXPR T* get_if(variant<Ts...>* v) noexcept
    {
        return v && v->which() == I
          ? detail::addressof(detail::access::get(*v, detail::index<I>{}))
          : nullptr;
    }

    //! template <class T, class ...Ts>
    //! constexpr T const* get_if(variant<Ts...> const* v) noexcept;
    //!
    //! \requires The type `T` occurs exactly once in `Ts...`. Otherwise, the
    //!  program is ill-formed.
    //!
    //! \returns If `v != nullptr`, a const pointer to the active member of
    //!  `*v` if it is of type `T`; otherwise,`nullptr`.
    //!
    //! \remarks This function shall be a `constexpr` function.
    template <
        typename T, typename ...Ts
      , std::size_t I = detail::checked_index_of<
            T, detail::pack<typename std::remove_cv<Ts>::type...>>::value
    >
    EGGS_CXX11_CONSTEXPR T const* get_if(variant<Ts...> const* v) noexcept
    {
        return v && v->which() == I
          ? detail::addressof(detail::access::get(*v, detail::index<I>{}))
          : nullptr;
    }

    ///////////////////////////////////////////////////////////////////////////
    //! template <class ...Ts>
    //! constexpr bool operator==(variant<Ts...> const& lhs, variant<Ts...> const& rhs);
    //!
    //! \requires The expression `*lhs.target<T>() == *rhs.target<T>()` shall
    //!  be convertible to `bool` for all `T` in `Ts...`.
    //!
    //! \returns If `lhs.which() != rhs.which()`, `false`; otherwise, if
    //!  `!bool(lhs)`, `true`; otherwise, `*lhs.target<T>() == *rhs.target<T>()`
    //!  where `T` is the type of the active member of both `lhs` and `rhs`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless the expression `*lhs.target<T>() == *rhs.target<T>()` is
    //!  well-formed for all `T` in `Ts...`. This function shall be a
    //!  `constexpr` function unless `lhs.which() == rhs.which()` and
    //!  `*lhs.target<T>() == *rhs.target<T>()` where `T` is the type of the
    //!  active member of both `lhs` and `rhs` is not a constant expression.
    template <
        typename ...Ts
      , typename Enable = typename std::enable_if<detail::all_of<detail::pack<
            detail::has_equal_to<Ts>...>>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator==(
        variant<Ts...> const& lhs, variant<Ts...> const& rhs)
    {
        return lhs.which() == rhs.which()
          ? detail::equal_to<detail::storage<Ts...>>{}(
                detail::typed_index_pack<detail::pack<detail::empty, Ts...>>{}
              , lhs.which() + 1
              , detail::access::storage(lhs), detail::access::storage(rhs)
            )
          : false;
    }

    EGGS_CXX11_CONSTEXPR inline bool operator==(
        variant<> const& /*lhs*/, variant<> const& /*rhs*/)
    {
        return true;
    }

    //! template <class ...Ts>
    //! constexpr bool operator!=(variant<Ts...> const& lhs, variant<Ts...> const& rhs);
    //!
    //! \requires The expression `*lhs.target<T>() != *rhs.target<T>()` shall
    //!  be convertible to `bool` for all `T` in `Ts...`.
    //!
    //! \returns If `lhs.which() != rhs.which()`, `true`; otherwise, if
    //!  `!bool(lhs)`, `false`; otherwise, `*lhs.target<T>() != *rhs.target<T>()`
    //!  where `T` is the type of the active member of both `lhs` and `rhs`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless the expression `*lhs.target<T>() != *rhs.target<T>()` is
    //!  well-formed for all `T` in `Ts...`.  This function shall be a
    //!  `constexpr` function unless `lhs.which() == rhs.which()` and
    //!  `*lhs.target<T>() != *rhs.target<T>()` where `T` is the type of the
    //!  active member of both `lhs` and `rhs` is not a constant expression.
    template <
        typename ...Ts
      , typename Enable = typename std::enable_if<detail::all_of<detail::pack<
            detail::has_not_equal_to<Ts>...>>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator!=(
        variant<Ts...> const& lhs, variant<Ts...> const& rhs)
    {
        return lhs.which() == rhs.which()
          ? detail::not_equal_to<detail::storage<Ts...>>{}(
                detail::typed_index_pack<detail::pack<detail::empty, Ts...>>{}
              , lhs.which() + 1
              , detail::access::storage(lhs), detail::access::storage(rhs)
            )
          : true;
    }

    EGGS_CXX11_CONSTEXPR inline bool operator!=(
        variant<> const& /*lhs*/, variant<> const& /*rhs*/)
    {
        return false;
    }

    //! template <class ...Ts>
    //! constexpr bool operator<(variant<Ts...> const& lhs, variant<Ts...> const& rhs);
    //!
    //! \requires The expression `*lhs.target<T>() < *rhs.target<T>()` shall
    //!  be convertible to `bool` for all `T` in `Ts...`.
    //!
    //! \returns If `!bool(rhs)`, `false`; otherwise, if `!bool(lhs)`, `true`;
    //!  otherwise, if `lhs.which() == rhs.which()`, `*lhs.target<T>() <
    //!  *rhs.target<T>()` where `T` is the type of the active member of both
    //!  `lhs` and `rhs`; otherwise, `lhs.which() < rhs.which()`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless the expression `*lhs.target<T>() < *rhs.target<T>()` is
    //!  well-formed for all `T` in `Ts...`. This function shall be a
    //!  `constexpr` function unless `lhs.which() == rhs.which()` and
    //!  `*lhs.target<T>() < *rhs.target<T>()` where `T` is the type of the
    //!  active member of both `lhs` and `rhs` is not a constant expression.
    template <
        typename ...Ts
      , typename Enable = typename std::enable_if<detail::all_of<detail::pack<
            detail::has_less<Ts>...>>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator<(
        variant<Ts...> const& lhs, variant<Ts...> const& rhs)
    {
        return lhs.which() == rhs.which()
          ? detail::less<detail::storage<Ts...>>{}(
                detail::typed_index_pack<detail::pack<detail::empty, Ts...>>{}
              , lhs.which() + 1
              , detail::access::storage(lhs), detail::access::storage(rhs)
            )
          : bool(lhs) == bool(rhs)
              ? lhs.which() < rhs.which()
              : bool(rhs);
    }

    EGGS_CXX11_CONSTEXPR inline bool operator<(
        variant<> const& /*lhs*/, variant<> const& /*rhs*/)
    {
        return false;
    }

    //! template <class ...Ts>
    //! constexpr bool operator>(variant<Ts...> const& lhs, variant<Ts...> const& rhs);
    //!
    //! \requires The expression `*lhs.target<T>() > *rhs.target<T>()` shall
    //!  be convertible to `bool` for all `T` in `Ts...`.
    //!
    //! \returns If `!bool(lhs)`, `false`; otherwise, if `!bool(rhs)`, `true`;
    //!  otherwise, if `lhs.which() == rhs.which()`, `*lhs.target<T>() >
    //!  *rhs.target<T>()` where `T` is the type of the active member of both
    //!  `lhs` and `rhs`; otherwise, `lhs.which() > rhs.which()`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless the expression `*lhs.target<T>() > *rhs.target<T>()` is
    //!  well-formed for all `T` in `Ts...`. This function shall be a
    //!  `constexpr` function unless `lhs.which() == rhs.which()` and
    //!  `*lhs.target<T>() > *rhs.target<T>()` where `T` is the type of the
    //!  active member of both `lhs` and `rhs` is not a constant expression.
    template <
        typename ...Ts
      , typename Enable = typename std::enable_if<detail::all_of<detail::pack<
            detail::has_greater<Ts>...>>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator>(
        variant<Ts...> const& lhs, variant<Ts...> const& rhs)
    {
        return lhs.which() == rhs.which()
          ? detail::greater<detail::storage<Ts...>>{}(
                detail::typed_index_pack<detail::pack<detail::empty, Ts...>>{}
              , lhs.which() + 1
              , detail::access::storage(lhs), detail::access::storage(rhs)
            )
          : bool(lhs) == bool(rhs)
              ? lhs.which() > rhs.which()
              : bool(lhs);
    }

    EGGS_CXX11_CONSTEXPR inline bool operator>(
        variant<> const& /*lhs*/, variant<> const& /*rhs*/)
    {
        return false;
    }

    //! template <class ...Ts>
    //! constexpr bool operator<=(variant<Ts...> const& lhs, variant<Ts...> const& rhs);
    //!
    //! \requires The expression `*lhs.target<T>() <= *rhs.target<T>()` shall
    //!  be convertible to `bool` for all `T` in `Ts...`.
    //!
    //! \returns If `!bool(lhs)`, `true`; otherwise, if `!bool(rhs)`, `false`;
    //!  otherwise, if `lhs.which() == rhs.which()`, `*lhs.target<T>() <=
    //!  *rhs.target<T>()` where `T` is the type of the active member of both
    //!  `lhs` and `rhs`; otherwise, `lhs.which() < rhs.which()`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless the expression `*lhs.target<T>() <= *rhs.target<T>()` is
    //!  well-formed for all `T` in `Ts...`. This function shall be a
    //!  `constexpr` function unless `lhs.which() == rhs.which()` and
    //!  `*lhs.target<T>() <= *rhs.target<T>()` where `T` is the type of the
    //!  active member of both `lhs` and `rhs` is not a constant expression.
    template <
        typename ...Ts
      , typename Enable = typename std::enable_if<detail::all_of<detail::pack<
            detail::has_less_equal<Ts>...>>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator<=(
        variant<Ts...> const& lhs, variant<Ts...> const& rhs)
    {
        return lhs.which() == rhs.which()
          ? detail::less_equal<detail::storage<Ts...>>{}(
                detail::typed_index_pack<detail::pack<detail::empty, Ts...>>{}
              , lhs.which() + 1
              , detail::access::storage(lhs), detail::access::storage(rhs)
            )
          : bool(lhs) == bool(rhs)
              ? lhs.which() < rhs.which()
              : bool(rhs);
    }

    EGGS_CXX11_CONSTEXPR inline bool operator<=(
        variant<> const& /*lhs*/, variant<> const& /*rhs*/)
    {
        return true;
    }

    //! template <class ...Ts>
    //! constexpr bool operator>=(variant<Ts...> const& lhs, variant<Ts...> const& rhs);
    //!
    //! \requires The expression `*lhs.target<T>() >= *rhs.target<T>()` shall
    //!  be convertible to `bool` for all `T` in `Ts...`.
    //!
    //! \returns If `!bool(rhs)`, `true`; otherwise, if `!bool(lhs)`, `false`;
    //!  otherwise, if `lhs.which() == rhs.which()`, `*lhs.target<T>() >=
    //!  *rhs.target<T>()` where `T` is the type of the active member of both
    //!  `lhs` and `rhs`; otherwise, `lhs.which() > rhs.which()`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless the expression `*lhs.target<T>() >= *rhs.target<T>()` is
    //!  well-formed for all `T` in `Ts...`. This function shall be a
    //!  `constexpr` function unless `lhs.which() == rhs.which()` and
    //!  `*lhs.target<T>() >= *rhs.target<T>()` where `T` is the type of the
    //!  active member of both `lhs` and `rhs` is not a constant expression.
    template <
        typename ...Ts
      , typename Enable = typename std::enable_if<detail::all_of<detail::pack<
            detail::has_greater_equal<Ts>...>>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator>=(
        variant<Ts...> const& lhs, variant<Ts...> const& rhs)
    {
        return lhs.which() == rhs.which()
          ? detail::greater_equal<detail::storage<Ts...>>{}(
                detail::typed_index_pack<detail::pack<detail::empty, Ts...>>{}
              , lhs.which() + 1
              , detail::access::storage(lhs), detail::access::storage(rhs)
            )
          : bool(lhs) == bool(rhs)
              ? lhs.which() > rhs.which()
              : bool(lhs);
    }

    EGGS_CXX11_CONSTEXPR inline bool operator>=(
        variant<> const& /*lhs*/, variant<> const& /*rhs*/)
    {
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////
    //! template <class ...Ts, class T>
    //! constexpr bool operator==(variant<Ts...> const& lhs, U const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `*lhs.target<T>() == rhs` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `lhs` has an active member of type `T`,
    //!  `*lhs.target<T>() == rhs`; otherwise, `false`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `*lhs.target<T>() == rhs` is well-formed. This function
    //!  shall be a `constexpr` function unless `lhs` has an active member of
    //!  type `T` and `*lhs.target<T>() == rhs` is not a constant expression.
    template <
        typename ...Ts, typename U
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_equal_to<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator==(
        variant<Ts...> const& lhs, U const& rhs)
    {
        return lhs.which() == I
          ? *lhs.template target<T>() == rhs
          : false;
    }

    //! template <class T, class ...Ts>
    //! constexpr bool operator==(U const& lhs, variant<Ts...> const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `lhs == *rhs.target<T>()` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `rhs` has an active member of type `T`,
    //!  `lhs == *rhs.target<T>()`; otherwise, `false`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `lhs == *rhs.target<T>()' is well-formed. This function
    //!  shall be a `constexpr` function unless `rhs` has an active member of
    //!  type `T` and `lhs == *rhs.target<T>()` is not a constant expression.
    template <
        typename U, typename ...Ts
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_equal_to<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator==(
        U const& lhs, variant<Ts...> const& rhs)
    {
        return rhs.which() == I
          ? lhs == *rhs.template target<T>()
          : false;
    }

    //! template <class ...Ts, class T>
    //! constexpr bool operator!=(variant<Ts...> const& lhs, U const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `*lhs.target<T>() != rhs` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `lhs` has an active member of type `T`,
    //!  `*lhs.target<T>() != rhs`; otherwise, `true`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `*lhs.target<T>() != rhs` is well-formed. This function
    //!  shall be a `constexpr` function unless `lhs` has an active member of
    //!  type `T` and `*lhs.target<T>() != rhs` is not a constant expression.
    template <
        typename ...Ts, typename U
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_not_equal_to<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator!=(
        variant<Ts...> const& lhs, U const& rhs)
    {
        return lhs.which() == I
          ? *lhs.template target<T>() != rhs
          : true;
    }

    //! template <class T, class ...Ts>
    //! constexpr bool operator!=(U const& lhs, variant<Ts...> const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `lhs != *rhs.target<T>()` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `rhs` has an active member of type `T`,
    //!  `lhs != *rhs.target<T>()`; otherwise, `true`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `lhs != *rhs.target<T>()' is well-formed. This function
    //!  shall be a `constexpr` function unless `rhs` has an active member of
    //!  type `T` and `lhs != *rhs.target<T>()` is not a constant expression.
    template <
        typename U, typename ...Ts
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_not_equal_to<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator!=(
        U const& lhs, variant<Ts...> const& rhs)
    {
        return rhs.which() == I
          ? lhs != *rhs.template target<T>()
          : true;
    }

    //! template <class ...Ts, class T>
    //! constexpr bool operator<(variant<Ts...> const& lhs, U const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `*lhs.target<T>() < rhs` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `!bool(lhs)`, `true`; otherwise, if `lhs` has an active
    //!  member of type `T`, `*lhs.target<T>() < rhs`; otherwise,
    //!  `lhs.which() < I` where `I` is the zero-based index of `T` in `Ts...`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `*lhs.target<T>() < rhs` is well-formed. This function
    //!  shall be a `constexpr` function unless `lhs` has an active member of
    //!  type `T` and `*lhs.target<T>() < rhs` is not a constant expression.
    template <
        typename ...Ts, typename U
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_less<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator<(
        variant<Ts...> const& lhs, U const& rhs)
    {
        return lhs.which() == I
          ? *lhs.template target<T>() < rhs
          : bool(lhs)
              ? lhs.which() < I
              : true;
    }

    //! template <class T, class ...Ts>
    //! constexpr bool operator<(U const& lhs, variant<Ts...> const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `lhs < *rhs.target<T>()` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `!bool(rhs)`, `false`; otherwise, if `rhs` has an active
    //!  member of type `T`, `lhs < *rhs.target<T>()`; otherwise,
    //!  `I < rhs.which()` where `I` is the zero-based index of `T` in `Ts...`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `lhs < *rhs.target<T>()' is well-formed. This function
    //!  shall be a `constexpr` function unless `rhs` has an active member of
    //!  type `T` and `lhs < *rhs.target<T>()` is not a constant expression.
    template <
        typename U, typename ...Ts
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_less<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator<(
        U const& lhs, variant<Ts...> const& rhs)
    {
        return rhs.which() == I
          ? lhs < *rhs.template target<T>()
          : bool(rhs)
              ? I < rhs.which()
              : false;
    }

    //! template <class ...Ts, class T>
    //! constexpr bool operator>(variant<Ts...> const& lhs, U const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `*lhs.target<T>() > rhs` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `!bool(lhs)`, `false`; otherwise, if `lhs` has an active
    //!  member of type `T`, `*lhs.target<T>() > rhs`; otherwise,
    //!  `lhs.which() > I` where `I` is the zero-based index of `T` in `Ts...`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `*lhs.target<T>() > rhs` is well-formed. This function
    //!  shall be a `constexpr` function unless `lhs` has an active member of
    //!  type `T` and `*lhs.target<T>() > rhs` is not a constant expression.
    template <
        typename ...Ts, typename U
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_greater<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator>(
        variant<Ts...> const& lhs, U const& rhs)
    {
        return lhs.which() == I
          ? *lhs.template target<T>() > rhs
          : bool(lhs)
              ? lhs.which() > I
              : false;
    }

    //! template <class T, class ...Ts>
    //! constexpr bool operator>(U const& lhs, variant<Ts...> const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `lhs > *rhs.target<T>()` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `!bool(rhs)`, `true`; otherwise, if `rhs` has an active
    //!  member of type `T`, `lhs > *rhs.target<T>()`; otherwise,
    //!  `I > rhs.which()` where `I` is the zero-based index of `T` in `Ts...`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `lhs > *rhs.target<T>()' is well-formed. This function
    //!  shall be a `constexpr` function unless `rhs` has an active member of
    //!  type `T` and `lhs > *rhs.target<T>()` is not a constant expression.
    template <
        typename U, typename ...Ts
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_greater<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator>(
        U const& lhs, variant<Ts...> const& rhs)
    {
        return rhs.which() == I
          ? lhs > *rhs.template target<T>()
          : bool(rhs)
              ? I > rhs.which()
              : true;
    }

    //! template <class ...Ts, class T>
    //! constexpr bool operator<=(variant<Ts...> const& lhs, U const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `*lhs.target<T>() <= rhs` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `!bool(lhs)`, `true`; otherwise, if `lhs` has an active
    //!  member of type `T`, `*lhs.target<T>() <= rhs`; otherwise,
    //!  `lhs.which() < I` where `I` is the zero-based index of `T` in `Ts...`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `*lhs.target<T>() <= rhs` is well-formed. This function
    //!  shall be a `constexpr` function unless `lhs` has an active member of
    //!  type `T` and `*lhs.target<T>() <= rhs` is not a constant expression.
    template <
        typename ...Ts, typename U
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_less_equal<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator<=(
        variant<Ts...> const& lhs, U const& rhs)
    {
        return lhs.which() == I
          ? *lhs.template target<T>() <= rhs
          : bool(lhs)
              ? lhs.which() < I
              : true;
    }

    //! template <class T, class ...Ts>
    //! constexpr bool operator<=(U const& lhs, variant<Ts...> const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `lhs <= *rhs.target<T>()` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `!bool(rhs)`, `false`; otherwise, if `rhs` has an active
    //!  member of type `T`, `lhs <= *rhs.target<T>()`; otherwise,
    //!  `I < rhs.which()` where `I` is the zero-based index of `T` in `Ts...`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `lhs <= *rhs.target<T>()' is well-formed. This function
    //!  shall be a `constexpr` function unless `rhs` has an active member of
    //!  type `T` and `lhs <= *rhs.target<T>()` is not a constant expression.
    template <
        typename U, typename ...Ts
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_less_equal<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator<=(
        U const& lhs, variant<Ts...> const& rhs)
    {
        return rhs.which() == I
          ? lhs <= *rhs.template target<T>()
          : bool(rhs)
              ? I < rhs.which()
              : false;
    }

    //! template <class ...Ts, class T>
    //! constexpr bool operator>=(variant<Ts...> const& lhs, U const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `*lhs.target<T>() >= rhs` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `!bool(lhs)`, `false`; otherwise, if `lhs` has an active
    //!  member of type `T`, `*lhs.target<T>() >= rhs`; otherwise,
    //!  `lhs.which() > I` where `I` is the zero-based index of `T` in `Ts...`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `rhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `*lhs.target<T>() >= rhs` is well-formed. This function
    //!  shall be a `constexpr` function unless `lhs` has an active member of
    //!  type `T` and `*lhs.target<T>() >= rhs` is not a constant expression.
    template <
        typename ...Ts, typename U
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_greater_equal<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator>=(
        variant<Ts...> const& lhs, U const& rhs)
    {
        return lhs.which() == I
          ? *lhs.template target<T>() >= rhs
          : bool(lhs)
              ? lhs.which() > I
              : false;
    }

    //! template <class T, class ...Ts>
    //! constexpr bool operator>=(U const& lhs, variant<Ts...> const& rhs);
    //!
    //! Let `T` be one of the types in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules.
    //!
    //! \requires The expression `lhs >= *rhs.target<T>()` shall be
    //!  convertible to `bool`.
    //!
    //! \returns If `!bool(rhs)`, `true`; otherwise, if `rhs` has an active
    //!  member of type `T`, `lhs >= *rhs.target<T>()`; otherwise,
    //!  `I > rhs.which()` where `I` is the zero-based index of `T` in `Ts...`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless there is a type `T` in `Ts...` for which `lhs` is
    //!  unambiguously convertible to by overload resolution rules, and the
    //!  expression `lhs >= *rhs.target<T>()' is well-formed. This function
    //!  shall be a `constexpr` function unless `rhs` has an active member of
    //!  type `T` and `lhs >= *rhs.target<T>()` is not a constant expression.
    template <
        typename U, typename ...Ts
      , std::size_t I = detail::index_of_best_match<
            U const&, detail::pack<Ts...>>::value
      , typename T = typename detail::at_index<
            I, detail::pack<Ts...>>::type
      , typename Enable = typename std::enable_if<
            detail::has_greater_equal<T const&, U const&>::value>::type
    >
    EGGS_CXX11_CONSTEXPR bool operator>=(
        U const& lhs, variant<Ts...> const& rhs)
    {
        return rhs.which() == I
          ? lhs >= *rhs.template target<T>()
          : bool(rhs)
              ? I > rhs.which()
              : true;
    }

    ///////////////////////////////////////////////////////////////////////////
    //! template <class R, class F, class ...Vs>
    //! constexpr R apply(F&& f, Vs&&... vs);
    //!
    //! Let `Vi` be the `i`-th type in `Vs...`, where all indexing is
    //!  zero-based.
    //!
    //! \requires For all `i`, `Vi` shall be either a specialization of
    //!  `variant` or publicly and unambiguously derived, directly or
    //!  indirectly, from one. Let `Ui` be the `i`-th `variant` specialization.
    //!  `INVOKE(std::forward<F>(f), get<Is>(std::forward<Vs>(vs))..., R)`
    //!  shall be a valid expression for all `Is...` in the range `[0u,
    //!  variant_size_v<Ui>)...`.
    //!
    //! \effects Equivalent to `INVOKE(std::forward<F>(f), get<Is>(
    //!  std::forward<Vs>(vs))...), R)` where `Is...` are the zero-based
    //!  indices of the active members of `vs...`.
    //!
    //! \throws `bad_variant_access` if any of `vs...` has no active member.
    //!
    //! \remarks If the selected function is a constant expression, then this
    //!  function shall be a `constexpr` function.
    template <
        typename R, typename F, typename ...Vs
      , typename Enable = typename std::enable_if<
            detail::all_of<detail::pack<
                detail::is_variant<typename std::remove_reference<Vs>::type>...
            >>::value
        >::type
    >
    EGGS_CXX11_CONSTEXPR R apply(F&& f, Vs&&... vs)
    {
        return (void)detail::swallow_pack(detail::throw_if_empty(vs)...),
            detail::apply<R>(detail::forward<F>(f),
                detail::access::storage(detail::forward<Vs>(vs))...);
    }

    //! template <class F, class ...Vs>
    //! constexpr R apply(F&& f, Vs&&... vs);
    //!
    //! Let `Ri...` be the return types of every potentially evaluated
    //!  `INVOKE` expression; if every `Ri...` is the same type, then let `R`
    //!  be that type.
    //!
    //! \effects Equivalent to `apply<R>(std::forward<F>(f),
    //!  std::forward<Vs>(vs)...)`.
    //!
    //! \remarks This function shall not participate in overload resolution
    //!  unless the return type of every potentially evaluated `INVOKE`
    //!  expression is the same type. If the selected function is a constant
    //!  expression, then this function shall be a `constexpr` function.
    template <
        int DeductionGuard = 0, typename F, typename ...Vs
      , typename Enable = typename std::enable_if<
            detail::all_of<detail::pack<
                detail::is_variant<typename std::remove_reference<Vs>::type>...
            >>::value
        >::type
      , typename R = typename detail::apply_result<F, detail::pack<
            decltype(detail::access::storage(std::declval<Vs>()))...>>::type
    >
    EGGS_CXX11_CONSTEXPR R apply(F&& f, Vs&&... vs)
    {
        return variants::apply<R>(
            detail::forward<F>(f), detail::forward<Vs>(vs)...);
    }

    ///////////////////////////////////////////////////////////////////////////
    //! template <class ...Ts>
    //! constexpr void swap(variant<Ts...>& x, variant<Ts...>& y)
    //!   noexcept(noexcept(x.swap(y))
    //!
    //! \effects Calls `x.swap(y)`.
    //!
    //! \remarks This function shall be defined as deleted unless
    //!  `std::is_move_constructible_v<T> && std::is_swappable_v<T>` is `true`
    //!  for all `T` in `Ts...`. If `std::is_trivially_copyable_v<T> &&
    //!  std::is_copy_assignable_v<T>` is `true` for all `T` in `Ts...`, then
    //!  this function shall be a `constexpr` function.
    template <
        typename ...Ts
      , typename std::enable_if<detail::all_of<detail::pack<
            detail::is_swappable<Ts>...
          , std::is_move_constructible<Ts>...
        >>::value, bool>::type = true
    >
    EGGS_CXX14_CONSTEXPR void swap(variant<Ts...>& x, variant<Ts...>& y)
        noexcept(noexcept(x.swap(y)))
    {
        x.swap(y);
    }

    template <
        typename ...Ts
      , typename std::enable_if<!detail::all_of<detail::pack<
            detail::is_swappable<Ts>...
          , std::is_move_constructible<Ts>...
        >>::value, bool>::type = false
    >
    EGGS_CXX14_CONSTEXPR void swap(variant<Ts...>& x, variant<Ts...>& y) = delete;
}}

namespace std
{
    //! template <class ...Ts>
    //! struct hash<::eggs::variants::variant<Ts...>>;
    //!
    //! The specialization `std::hash<variant<Ts...>>` is enabled if and only if
    //!  every specialization in `std::hash<std::remove_const_t<Ts>>...` is
    //!  enabled. When enabled, for an object `v` of type `variant<Ts...>`, if
    //!  `v` has an active member of type `T`, `std::hash<variant<Ts...>>()(v)`
    //!  shall evaluate to the same value as `std::hash<T>()(*v.target<T>())`;
    //!  otherwise it evaluates to an unspecified value. The member functions
    //!  are guaranteed to be `noexcept` if the member functions of every
    //!  specialization in `std::hash<std::remove_const_t<Ts>>...` are
    //!  `noexcept`.
    template <typename ...Ts>
    struct hash< ::eggs::variants::variant<Ts...>>
      : ::eggs::variants::detail::std_hash<Ts...>
    {};
}

#include "detail/config/suffix.hpp"

#endif /*EGGS_VARIANT_VARIANT_HPP*/
