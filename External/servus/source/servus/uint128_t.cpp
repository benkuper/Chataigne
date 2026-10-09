
/* Copyright (c) 2011-2016, Stefan Eilemann <eile@eyescale.ch>
 *
 * This file is part of Servus <https://github.com/HBPVIS/Servus>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "uint128_t.h"
#include "md5/md5.hh"

#include <mutex>
#include <random>

#include <cassert>
#include <cstdlib> // for strtoull
#include <cstring> // for strcmp

#ifdef _MSC_VER
#define strtoull _strtoui64
#endif

using ScopedLock = std::unique_lock<std::mutex>;
namespace chrono = std::chrono;

namespace servus
{
uint128_t& uint128_t::operator=(const std::string& from)
{
    if (from.empty())
    {
        _high = 0;
        _low = 0;
        return *this;
    }

    char* next = 0;
    _high = ::strtoull(from.c_str(), &next, 16);
    assert(next != from.c_str());

    if (*next == '\0') // short representation, high was 0
    {
        _low = _high;
        _high = 0;
    }
    else
    {
        if (strncmp(next, "\\058" /* utf-8 ':' */, 4) == 0)
            next += 4;
        else
        {
#ifndef NDEBUG
            if (*next != ':')
                std::cerr << from << ", " << next << std::endl;
#endif
            assert(*next == ':');
            ++next;
        }
        _low = ::strtoull(next, 0, 16);
    }
    return *this;
}

uint128_t make_uint128(const char* string)
{
    const md5::MD5 md5((unsigned char*)string);
    uint128_t value;
    md5.raw_digest(value.high(), value.low());
    return value;
}

uint128_t make_UUID()
{
    uint128_t value;
    while (value.high() == 0)
    {
        static std::random_device device;
        static std::mt19937_64 engine(device());
        static std::uniform_int_distribution<uint64_t> generator(
            0, std::numeric_limits<uint64_t>::max());
#ifndef _MSC_VER
        // The static state is hidden to users, so the function is made
        // thread safe for robustness over performance.
        // Also, creating a new generator each call seems to increases the
        // chances of collissions up to a noticeable level.

        // http://stackoverflow.com/questions/14711263
        static std::mutex mutex;
        ScopedLock lock(mutex);
#endif
        value.high() = generator(engine);
        value.low() = generator(engine);
    }
    return value;
}
}
