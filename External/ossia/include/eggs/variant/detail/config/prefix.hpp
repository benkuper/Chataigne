//! \file eggs/variant/detail/config/prefix.hpp
// Eggs.Variant
//
// Copyright Agustin K-ballo Berge, Fusion Fenix 2014-2017
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// no header guards

#if __cplusplus < 201103L
#  if !defined(_MSC_VER) || _MSC_VER < 1900
#    error Eggs.Variant requires compiler and library support for the ISO C++ 2011 standard.
#  endif
#endif

/// constexpr support
#ifndef EGGS_CXX11_HAS_CONSTEXPR
#  if __cpp_constexpr >= 200704L
#    define EGGS_CXX11_HAS_CONSTEXPR 1
#  else
#    define EGGS_CXX11_HAS_CONSTEXPR 1
#  endif
#  define EGGS_CXX11_HAS_CONSTEXPR_DEFINED
#endif

#ifndef EGGS_CXX11_CONSTEXPR
#  if EGGS_CXX11_HAS_CONSTEXPR == 0
#    define EGGS_CXX11_CONSTEXPR
#  else
#    define EGGS_CXX11_CONSTEXPR constexpr
#  endif
#  define EGGS_CXX11_CONSTEXPR_DEFINED
#endif

#ifndef EGGS_CXX11_STATIC_CONSTEXPR
#  if EGGS_CXX11_HAS_CONSTEXPR == 0
#    define EGGS_CXX11_STATIC_CONSTEXPR static const
#  else
#    define EGGS_CXX11_STATIC_CONSTEXPR static constexpr
#  endif
#  define EGGS_CXX11_STATIC_CONSTEXPR_DEFINED
#endif

#ifndef EGGS_CXX14_HAS_CONSTEXPR
#  if EGGS_CXX11_HAS_CONSTEXPR == 0
#    define EGGS_CXX14_HAS_CONSTEXPR 0
#  elif __cpp_constexpr >= 201304L
#    define EGGS_CXX14_HAS_CONSTEXPR 1
#  elif defined(_MSC_VER)
#    if _MSC_VER < 1910
#      define EGGS_CXX14_HAS_CONSTEXPR 0
#    else
#      define EGGS_CXX14_HAS_CONSTEXPR 1
#    endif
#  elif __cplusplus < 201402L
#    define EGGS_CXX14_HAS_CONSTEXPR 0
#  else
#    define EGGS_CXX14_HAS_CONSTEXPR 1
#  endif
#  define EGGS_CXX14_HAS_CONSTEXPR_DEFINED
#endif

#ifndef EGGS_CXX14_CONSTEXPR
#  if EGGS_CXX14_HAS_CONSTEXPR == 0
#    define EGGS_CXX14_CONSTEXPR
#  else
#    define EGGS_CXX14_CONSTEXPR constexpr
#  endif
#  define EGGS_CXX14_CONSTEXPR_DEFINED
#endif

/// RTTI support
#ifndef EGGS_CXX98_HAS_RTTI
#  if __cpp_rtti >= 199711L
#    define EGGS_CXX98_HAS_RTTI 1
#  elif defined(__clang__)
#    define EGGS_CXX98_HAS_RTTI __has_feature(cxx_rtti)
#  elif defined(__GNUC__) && !defined(__GXX_RTTI)
#    define EGGS_CXX98_HAS_RTTI 0
#  elif defined(_MSC_VER) && !defined(_CPPRTTI)
#    define EGGS_CXX98_HAS_RTTI 0
#  else
#    define EGGS_CXX98_HAS_RTTI 1
#  endif
#  define EGGS_CXX98_HAS_RTTI_DEFINED
#endif

/// RTTI constexpr support
#ifndef EGGS_CXX11_HAS_CONSTEXPR_RTTI
#  if EGGS_CXX98_HAS_RTTI == 0 || EGGS_CXX11_HAS_CONSTEXPR == 0
#    define EGGS_CXX11_HAS_CONSTEXPR_RTTI 0
#  elif defined(_MSC_VER)
#    define EGGS_CXX11_HAS_CONSTEXPR_RTTI 0
#  else
#    define EGGS_CXX11_HAS_CONSTEXPR_RTTI 1
#  endif
#  define EGGS_CXX11_HAS_CONSTEXPR_RTTI_DEFINED
#endif

/// exception support
#ifndef EGGS_CXX98_HAS_EXCEPTIONS
#  if __cpp_exceptions >= 199711L
#    define EGGS_CXX98_HAS_EXCEPTIONS 1
#  elif defined(__clang__)
#    define EGGS_CXX98_HAS_EXCEPTIONS __has_feature(cxx_exceptions)
#  elif defined(__GNUC__) && !defined(__EXCEPTIONS)
#    define EGGS_CXX98_HAS_EXCEPTIONS 0
#  elif defined(_MSC_VER) && !defined(_CPPUNWIND)
#    define EGGS_CXX98_HAS_EXCEPTIONS 0
#  else
#    define EGGS_CXX98_HAS_EXCEPTIONS 1
#  endif
#  define EGGS_CXX98_HAS_EXCEPTIONS_DEFINED
#endif

/// noreturn support
#ifndef EGGS_CXX11_NORETURN
#  if defined(__has_cpp_attribute)
#    if __has_cpp_attribute(noreturn)
#      define EGGS_CXX11_NORETURN [[noreturn]]
#    else
#      define EGGS_CXX11_NORETURN
#    endif
#  elif defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8)) && !defined(__clang__)
#    define EGGS_CXX11_NORETURN __attribute__ ((__noreturn__))
#  elif defined(_MSC_VER)  && _MSC_VER < 1900
#    define EGGS_CXX11_NORETURN __declspec(noreturn)
#  else
#    define EGGS_CXX11_NORETURN [[noreturn]]
#  endif
#  define EGGS_CXX11_NORETURN_DEFINED
#endif

/// sfinae for expressions support
#ifndef EGGS_CXX11_HAS_SFINAE_FOR_EXPRESSIONS
#  if defined(_MSC_VER) && _MSC_VER < 1910
#    define EGGS_CXX11_HAS_SFINAE_FOR_EXPRESSIONS 0
#  elif defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 9)) && !defined(__clang__)
#    define EGGS_CXX11_HAS_SFINAE_FOR_EXPRESSIONS 0
#  else
#    define EGGS_CXX11_HAS_SFINAE_FOR_EXPRESSIONS 1
#  endif
#  define EGGS_CXX11_HAS_SFINAE_FOR_EXPRESSIONS_DEFINED
#endif

/// variable templates support
#ifndef EGGS_CXX14_HAS_VARIABLE_TEMPLATES
#  if __cpp_variable_templates >= 201304L
#    define EGGS_CXX14_HAS_VARIABLE_TEMPLATES 1
#  elif defined(_MSC_VER)
#    define EGGS_CXX14_HAS_VARIABLE_TEMPLATES 1
#  elif __cplusplus < 201402L
#    define EGGS_CXX14_HAS_VARIABLE_TEMPLATES 0
#  elif defined(__GNUC__) && __GNUC__ < 5 && !defined(__clang__)
#    define EGGS_CXX14_HAS_VARIABLE_TEMPLATES 0
#  else
#    define EGGS_CXX14_HAS_VARIABLE_TEMPLATES 1
#  endif
#  define EGGS_CXX14_HAS_VARIABLE_TEMPLATES_DEFINED
#endif

/// constexpr std::addressof support
#ifndef EGGS_CXX17_STD_HAS_CONSTEXPR_ADDRESSOF
#  if EGGS_CXX11_HAS_CONSTEXPR == 0
#    define EGGS_CXX17_STD_HAS_CONSTEXPR_ADDRESSOF 0
#  elif __cpp_lib_addressof_constexpr >= 201603L
#    define EGGS_CXX17_STD_HAS_CONSTEXPR_ADDRESSOF 1
#  elif defined(_MSC_VER)
#    if _MSC_VER < 1910
#      define EGGS_CXX17_STD_HAS_CONSTEXPR_ADDRESSOF 0
#    else
#      define EGGS_CXX17_STD_HAS_CONSTEXPR_ADDRESSOF 1
#    endif
#  elif __cplusplus < 201703L
#    define EGGS_CXX17_STD_HAS_CONSTEXPR_ADDRESSOF 0
#  else
#    define EGGS_CXX17_STD_HAS_CONSTEXPR_ADDRESSOF 1
#  endif
#  define EGGS_CXX17_STD_HAS_CONSTEXPR_ADDRESSOF_DEFINED
#endif

/// std::is_nothrow_* support
#ifndef EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS
#  if defined(__GLIBCXX__) && !defined(_GLIBCXX_NOEXCEPT)
#    define EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS 0
#  else
#    define EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS 1
#  endif
#  define EGGS_CXX11_STD_HAS_IS_NOTHROW_TRAITS_DEFINED
#endif

/// std::is_[nothrow_]swappable support
#ifndef EGGS_CXX17_STD_HAS_SWAPPABLE_TRAITS
#  if __cpp_lib_is_swappable >= 201603L
#    define EGGS_CXX17_STD_HAS_SWAPPABLE_TRAITS 1
#  elif defined(_MSC_VER)
#    if _MSC_VER < 1910 || !_HAS_CXX17
#      define EGGS_CXX17_STD_HAS_SWAPPABLE_TRAITS 0
#    else
#      define EGGS_CXX17_STD_HAS_SWAPPABLE_TRAITS 1
#    endif
#  elif __cplusplus < 201703L
#    define EGGS_CXX17_STD_HAS_SWAPPABLE_TRAITS 0
#  else
#    define EGGS_CXX17_STD_HAS_SWAPPABLE_TRAITS 1
#  endif
#  define EGGS_CXX17_STD_HAS_SWAPPABLE_TRAITS_DEFINED
#endif

/// std::is_trivially_copyable support
#ifndef EGGS_CXX11_STD_HAS_IS_TRIVIALLY_COPYABLE
#  if defined(__GLIBCXX__)
#    if defined(__has_include)
#      define EGGS_CXX11_STD_HAS_IS_TRIVIALLY_COPYABLE __has_include(<codecvt>) // 5.0+
#    else
#      define EGGS_CXX11_STD_HAS_IS_TRIVIALLY_COPYABLE 0
#    endif
#  elif defined(_MSC_VER) && _MSC_VER < 1910
#    define EGGS_CXX11_STD_HAS_IS_TRIVIALLY_COPYABLE 0
#  else
#    define EGGS_CXX11_STD_HAS_IS_TRIVIALLY_COPYABLE 1
#  endif
#  define EGGS_CXX11_STD_HAS_IS_TRIVIALLY_COPYABLE_DEFINED
#endif

/// std::is_trivially_destructible support
#ifndef EGGS_CXX11_STD_HAS_IS_TRIVIALLY_DESTRUCTIBLE
#  if defined(__GLIBCXX__)
#    if defined(__has_include)
#      define EGGS_CXX11_STD_HAS_IS_TRIVIALLY_DESTRUCTIBLE __has_include(<ext/cmath>) // 4.8+
#    else
#      define EGGS_CXX11_STD_HAS_IS_TRIVIALLY_DESTRUCTIBLE 0
#    endif
#  else
#    define EGGS_CXX11_STD_HAS_IS_TRIVIALLY_DESTRUCTIBLE 1
#  endif
#  define EGGS_CXX11_STD_HAS_IS_TRIVIALLY_DESTRUCTIBLE_DEFINED
#endif

#if defined(_MSC_VER)
#  pragma warning(push)
/// destructor was implicitly defined as deleted because a base class
/// destructor is inaccessible or deleted
#  pragma warning(disable: 4624)
#endif
