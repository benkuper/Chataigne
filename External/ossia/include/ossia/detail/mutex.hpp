#pragma once
#include <ossia/detail/config.hpp>
#include <mutex>
#if !defined(__APPLE__)
#define OSSIA_HAS_SHARED_MUTEX
#include <shared_mutex>
#else
#include <Availability.h>
#include <AvailabilityMacros.h>
#if defined(MAC_OS_X_VERSION_10_12) \
    && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_12)
#include <shared_mutex>
#define OSSIA_HAS_SHARED_MUTEX
#endif
#endif
namespace ossia
{
#if defined(OSSIA_HAS_SHARED_MUTEX)
using mutex_t = std::mutex;
using lock_t = std::lock_guard<mutex_t>;

using shared_mutex_t = std::shared_timed_mutex;
using write_lock_t = std::lock_guard<shared_mutex_t>;
using read_lock_t = std::shared_lock<shared_mutex_t>;
#else
using mutex_t = std::mutex;
using shared_mutex_t = std::mutex;
using lock_t = std::lock_guard<mutex_t>;
using write_lock_t = std::lock_guard<mutex_t>;
using read_lock_t = std::lock_guard<mutex_t>;
#endif
}
