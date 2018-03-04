/* Copyright (c) 2010-2017, Cedric Stalder <cedric.stalder@gmail.com>
 *                          Stefan Eilemann <eile@eyescale.ch>
 *                          Daniel Nachbaur <danielnachbaur@gmail.com>
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

#ifndef SERVUS_UINT128_H
#define SERVUS_UINT128_H

#include <servus/api.h>
#include <servus/types.h>

#include <sstream>
#ifdef _MSC_VER
// Don't include <servus/types.h> to be minimally intrusive for apps
// using uint128_t
#include <basetsd.h>
typedef UINT64 uint64_t;
#define snprintf _snprintf
#else
#include <stdint.h>
#endif

namespace servus
{
class uint128_t;
std::ostream& operator<<(std::ostream& os, const uint128_t& id);

/**
 * A base type for 128 bit unsigned integer values.
 *
 * Example: @include tests/uint128_t.cpp
 */
class uint128_t
{
public:
    /**
     * Construct a new 128 bit integer with a default value.
     */
    explicit uint128_t(const unsigned long long low_ = 0)
        : _high(0)
        , _low(low_)
    {
    }

    /**
     * Construct a new 128 bit integer with a default value.
     */
    explicit uint128_t(const unsigned long low_)
        : _high(0)
        , _low(low_)
    {
    }

    /**
     * Construct a new 128 bit integer with a default value.
     */
    explicit uint128_t(const int low_)
        : _high(0)
        , _low(low_)
    {
    }

    /**
     * Construct a new 128 bit integer with default values.
     **/
    uint128_t(const uint64_t high_, const uint64_t low_)
        : _high(high_)
        , _low(low_)
    {
    }

    /**
     * Construct a new 128 bit integer from a string representation.
     **/
    explicit uint128_t(const std::string& string)
        : _high(0)
        , _low(0)
    {
        *this = string;
    }

    /**
     * @return true if the uint128_t is a generated universally unique
     *         identifier.
     */
    bool isUUID() const { return high() != 0; }
    /** Assign another 128 bit value. */
    uint128_t& operator=(const servus::uint128_t& rhs)
    {
        _high = rhs._high;
        _low = rhs._low;
        return *this;
    }

    /** Assign another 64 bit value. */
    uint128_t& operator=(const uint64_t rhs)
    {
        _high = 0;
        _low = rhs;
        return *this;
    }

    /** Assign an integer value. */
    uint128_t& operator=(const int rhs)
    {
        _high = 0;
        _low = rhs;
        return *this;
    }

    /** Assign an 128 bit value from a std::string. */
    SERVUS_API uint128_t& operator=(const std::string& from);

    /**
     * @return true if the values are equal, false if not.
     **/
    bool operator==(const servus::uint128_t& rhs) const
    {
        return _high == rhs._high && _low == rhs._low;
    }

    /**
     * @return true if the values are different, false otherwise.
     **/
    bool operator!=(const servus::uint128_t& rhs) const
    {
        return _high != rhs._high || _low != rhs._low;
    }

    /**
     * @return true if the values are equal, false otherwise.
     **/
    bool operator==(const unsigned long long& low_) const
    {
        return *this == uint128_t(low_);
    }

    /**
     * @return true if the values are different, false otherwise.
     **/
    bool operator!=(const unsigned long long& low_) const
    {
        return *this != uint128_t(low_);
    }

    /**
     * @return true if this value is smaller than the RHS value.
     **/
    bool operator<(const servus::uint128_t& rhs) const
    {
        if (_high < rhs._high)
            return true;
        if (_high > rhs._high)
            return false;
        return _low < rhs._low;
    }

    /**
     * @return true if this value is bigger than the rhs value.
     */
    bool operator>(const servus::uint128_t& rhs) const
    {
        if (_high > rhs._high)
            return true;
        if (_high < rhs._high)
            return false;
        return _low > rhs._low;
    }

    /**
     * @return true if this value is smaller or equal than the
     *         RHS value.
     */
    bool operator<=(const servus::uint128_t& rhs) const
    {
        if (_high < rhs._high)
            return true;
        if (_high > rhs._high)
            return false;
        return _low <= rhs._low;
    }

    /**
     * @return true if this value is smaller or equal than the
     *         RHS value.
     */
    bool operator>=(const servus::uint128_t& rhs) const
    {
        if (_high > rhs._high)
            return true;
        if (_high < rhs._high)
            return false;
        return _low >= rhs._low;
    }

    /** Increment the value. */
    uint128_t& operator++()
    {
        ++_low;
        if (!_low)
            ++_high;

        return *this;
    }

    /** Decrement the value. */
    uint128_t& operator--()
    {
        if (!_low)
            --_high;
        --_low;
        return *this;
    }

    /** Add value and return the new value. */
    uint128_t& operator+=(const servus::uint128_t& rhs)
    {
        const uint64_t oldLow = _low;
        _low += rhs._low;
        if (_low < oldLow) // overflow
            _high += rhs._high + 1;
        else
            _high += rhs._high;
        return *this;
    }

    /** @return the reference to the lower 64 bits of this 128 bit value. */
    const uint64_t& low() const { return _low; }
    /** @return the reference to the high 64 bits of this 128 bit value. */
    const uint64_t& high() const { return _high; }
    /** @return the reference to the lower 64 bits of this 128 bit value. */
    uint64_t& low() { return _low; }
    /** @return the reference to the high 64 bits of this 128 bit value. */
    uint64_t& high() { return _high; }
    /** @return a short, but not necessarily unique, string of the value. */
    std::string getShortString() const
    {
        std::stringstream stream;
        stream << std::hex << _high << _low;
        const std::string str = stream.str();
        return str.substr(0, 3) + ".." +
               str.substr(str.length() - 3, std::string::npos);
    }

    /** @return the full string representation of the value. */
    std::string getString() const
    {
        // OPT: snprintf is faster than using std::stringstream
        char buffer[34] /* 2 x 16 bytes + : + \0 */;
        snprintf(buffer, 34, "%llx:%llx", ull_t(high()), ull_t(low()));
        return std::string(buffer);
    }

    /** Serialize this object to a boost archive. */
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar& low();
        ar& high();
    }

private:
    uint64_t _high;
    uint64_t _low;
};

/** ostream operator for 128 bit unsigned integers. */
inline std::ostream& operator<<(std::ostream& os, const uint128_t& id)
{
    if (id.high() == 0)
        os << std::hex << id.low() << std::dec;
    else
        os << std::hex << id.high() << ':' << id.low() << std::dec;
    return os;
}

/** istream operator for 128 bit unsigned integers. */
inline std::istream& operator>>(std::istream& is, uint128_t& id)
{
    std::string str;
    is >> str;
    id = str;
    return is;
}

/** Add a 64 bit value to a 128 bit value. */
inline uint128_t operator+(const servus::uint128_t& a, const uint64_t& b)
{
    uint128_t result = a;
    result.low() += b;
    if (result.low() < a.low())
        ++result.high();
    return result;
}

/** Add two 128 bit values. */
inline uint128_t operator+(const servus::uint128_t& a,
                           const servus::uint128_t& b)
{
    uint128_t result = a;
    result += b;
    return result;
}

/** Subtract a 64 bit value from a 128 bit value. */
inline uint128_t operator-(const servus::uint128_t& a, const uint64_t& b)
{
    uint128_t result = a;
    result.low() -= b;
    if (result.low() > a.low())
        --result.high();
    return result;
}

/** Bitwise and operation on two 128 bit values. */
inline uint128_t operator&(const servus::uint128_t& a,
                           const servus::uint128_t& b)
{
    uint128_t result = a;
    result.high() &= b.high();
    result.low() &= b.low();
    return result;
}

/** Bitwise or operation on two 128 bit values. */
inline uint128_t operator|(const servus::uint128_t& a,
                           const servus::uint128_t& b)
{
    uint128_t result = a;
    result.high() |= b.high();
    result.low() |= b.low();
    return result;
}

/**
 * Create a 128 bit integer based on a string.
 *
 * The MD5 hash of the given text is used to form the uint128_t.
 *
 * @param string the string to form the uint128_t from.
 */
SERVUS_API uint128_t make_uint128(const char* string);

/** Create a 128 bit integer based on a string. */
inline uint128_t make_uint128(const std::string& string)
{
    return make_uint128(string.c_str());
}

/**
 * Construct a new 128 bit integer with a generated universally unique
 * identifier.
 */
SERVUS_API uint128_t make_UUID();
}

namespace std
{
template <>
struct hash<servus::uint128_t>
{
    typedef size_t result_type;

    result_type operator()(const servus::uint128_t& in) const
    {
        hash<uint64_t> forward;
        return forward(in.high()) ^ forward(in.low());
    }
};

inline string to_string(const servus::uint128_t& value)
{
    return value.getString();
}
}

#endif // SERVUS_UINT128_H
