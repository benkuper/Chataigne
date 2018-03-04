/* Copyright (c) 2013-2015, Stefan.Eilemann@epfl.ch
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

#ifndef SERVUS_RESULT_H
#define SERVUS_RESULT_H

#include <servus/api.h>
#include <servus/types.h>

#include <iostream>

namespace servus
{
/** A result returns an error code and behaves like a boolean. */
class Result
{
    typedef void (Result::*bool_t)() const;
    void bool_true() const {}
public:
    static const int32_t SUCCESS = 0;

    /** Construct a new result. */
    explicit Result(const int32_t code)
        : code_(code)
    {
    }

    /** Destruct the result. */
    virtual ~Result() {}
    /** @return true if no error occured, false otherwise. */
    operator bool_t() const
    {
        return code_ == SUCCESS ? &Result::bool_true : 0;
    }

    /** @return true if an error occured, false otherwise. */
    bool operator!() const { return code_ != SUCCESS; }
    /** @return true if the result is equal to the given value. */
    bool operator==(const int32_t code) const { return code_ == code; }
    /** @return true if the result is not equal to the rhs. */
    bool operator!=(const int32_t code) const { return code != code_; }
    /** @return the result code. */
    int32_t getCode() const { return code_; }
    /** @return the result string. */
    virtual std::string getString() const
    {
        return code_ == SUCCESS ? "success" : "result";
    }

protected:
    int32_t code_;
};

inline std::ostream& operator<<(std::ostream& os, const Result& result)
{
    return os << result.getString() << " (" << result.getCode() << ")";
}
}
#endif // SERVUS_RESULT_H
