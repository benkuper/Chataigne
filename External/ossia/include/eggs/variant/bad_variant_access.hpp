//! \file eggs/variant/bad_variant_access.hpp
// Eggs.Variant
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014-2017
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef EGGS_VARIANT_BAD_VARIANT_ACCESS_HPP
#define EGGS_VARIANT_BAD_VARIANT_ACCESS_HPP

#include <exception>

#include "detail/config/prefix.hpp"

namespace eggs { namespace variants
{
    ///////////////////////////////////////////////////////////////////////////
    //! class bad_variant_access : public std::exception
    //!
    //! Objects of type `bad_variant_access` are thrown to report attempts to
    //! access an inactive member of a `variant` object.
    class bad_variant_access
      : public std::exception
    {
    public:
        //! bad_variant_access() noexcept;
        //!
        //! \effects Constructs a `bad_variant_access` object.
        bad_variant_access() noexcept
        {}

        //! char const* what() const noexcept override;
        //!
        //! \returns An implementation-defined NTBS.
        char const* what() const noexcept /*override*/
        {
            return "bad_variant_access";
        }
    };

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////
        template <typename T>
        EGGS_CXX11_NORETURN inline T throw_bad_variant_access()
        {
#if EGGS_CXX98_HAS_EXCEPTIONS
            throw bad_variant_access{};
#else
            std::terminate();
#endif
        }
    }
}}

#include "detail/config/suffix.hpp"

#endif /*EGGS_VARIANT_BAD_VARIANT_ACCESS_HPP*/
