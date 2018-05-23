#pragma once
#include <ossia-config.hpp>
#include <type_traits>

#if defined(__cplusplus) \
    && ((__cplusplus >= 201103L) || (defined(_MSC_VER) && _MSC_VER >= 1900))
#define OSSIA_CXX11 1
#endif
#if defined(__cplusplus) \
    && ((__cplusplus >= 201403L) || (defined(_MSC_VER) && _MSC_VER >= 1900))
#define OSSIA_CXX14 1
#endif
#if defined(__cplusplus) \
    && ((__cplusplus >= 201703L) || (defined(_MSC_VER) && _MSC_VER >= 1900))
#define OSSIA_CXX17 1
#endif

/// Constexpr support ///
#define OSSIA_DECL_RELAXED_CONSTEXPR constexpr

/// Inline support ///
#if defined(__GNUC__)
#define OSSIA_INLINE __attribute__((always_inline))
#elif defined(__clang__)
#define OSSIA_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER)
#define OSSIA_INLINE __forceinline
#else
#define OSSIA_INLINE
#endif

// Remove with C++17
#if defined(_MSC_VER) || defined(__cpp_lib_void_t)
namespace ossia
{
template <typename... Args>
using void_t = std::void_t<Args...>;
}
#else
namespace ossia
{
template <typename... Args>
using void_t = void;
}
#endif

#define BOOST_MATH_DISABLE_FLOAT128

#define SPDLOG_NO_DATETIME
#define SPDLOG_NO_THREAD_ID
#define SPDLOG_NO_NAME

#define SPDLOG_DEBUG_ON
#define SPDLOG_TRACE_ON

#if !defined(SPDLOG_FMT_EXTERNAL)
#define SPDLOG_FMT_EXTERNAL
#endif

#if !defined(FMT_HEADER_ONLY)
#define FMT_HEADER_ONLY
#endif

#if !defined(RAPIDJSON_HAS_STDSTRING)
#define RAPIDJSON_HAS_STDSTRING 1
#endif

#define BOOST_ERROR_CODE_HEADER_ONLY 1
#define BOOST_SYSTEM_NO_DEPRECATED

#define _WEBSOCKETPP_CPP11_STRICT_ 1

#if !defined(ASIO_STANDALONE)
#define ASIO_STANDALONE 1
#endif

#if !defined(BOOST_REGEX_NO_LIB)
#define BOOST_REGEX_NO_LIB
#endif

#if !defined(BOOST_DATE_TIME_NO_LIB)
#define BOOST_DATE_TIME_NO_LIB
#endif
