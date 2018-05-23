// chobo-optional v1.02
//
// A value wrapper with an optional invalid state.
//
// MIT License:
// Copyright(c) 2016 Chobolabs Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files(the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and / or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions :
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//
//                  VERSION HISTORY
//
//  1.02 (2017-06-20) Explicit operator bool
//  1.01 (2016-09-27) Qualified operator new
//  1.00 (2016-09-23) First public release
//
//
//                  DOCUMENTATION
//
// Simply include this file wherever you need.
// chobo::optional is similar to boost::optional. It's a value wrapper
// class. It simply holds a value of some type, but has a boolean context
// which gives you an invalid state for the value.
//
// Example:
//
// chobo::optional<string> foo; // no string is constructed
// bool b = foo; // b is false
// std::string xxx = *foo; // ERROR! undefined behavior, foo has no internal string
// foo = "asd"; // a string is constructed within foo
// b = foo; // b is true, foo is valid
// const std::string& asd = *foo; // asd is a reference to the internal string object of foo
// size_t s = foo->length(); // s is 3
//
// chobo::optional<int> i = 0;
// bool b = i; // b is true. Although the internal int is zero, i is a valid optional
// b = *i; // b is false since operator* dereferences optional to its internal object
//
// Reference:
//
// optional() - constructs empty invalid optional
// optional(const optional& o) - copy constructor. Copies the internal value if any.
// optional(optional&& o) - move constructor. Moves the internal value if any.
// optional(const value_type& v) - constructs optional with a copy of v
// optional(value_type&& v) - constructs optional by moving v
// optional(bool condition, const value_type& val) - constructs optional based on
//      condition. If true, copies val, otherwise constructs an invalid optional
//
// optional& operator=(const optional& o)
// optional& operator=(optional&& o)
// optional& operator=(const value_type& t)
// optional& operator=(value_type&& t) - assignment operators. Destroy the previous
//      object if valid.
//
// bool is_valid() const
// operator bool() const - true if the underlying object is in a valid state
//
// template <class ...Args>
// void construct(Args&&... args) - constructs the underlying object by calling
//      its constructor with args. Destroys the previous one if valid.
// void destroy() - destroys the underlying object and switches to invalid state
// void clear() - alias of destroy()
//
// T& get()
// T& get() const
// T& operator*()
// const T& operator*() const - access the underlying object
//
// T& get_or(T& val)
// const T& get_or(const T& val) const - get the underlying object or return val
//      if invalid.
//
// T* operator->()
// const T* operator->() const - call methods of the underlying object
//
// External:
//
// template <typename T>
// bool operator==(const optional<T>& a, const optional<T>& b)
// template <typename T>
// bool operator!=(const optional<T>& a, const optional<T>& b) - compare underlying
//      objects if both are valid, else compare validity
//
// template <typename T>
// bool operator==(const optional<T>& a, const T& b)
// template <typename T>
// bool operator!=(const optional<T>& a, const T& b) - compare underlying objects
//      if a is valid, else check a validity
//
//
//                  TESTS
//
// The tests are included in the header file and use doctest (https://github.com/onqtam/doctest).
// To run them, define CHOBO_OPTIONAL_TEST_WITH_DOCTEST before including
// the header in a file which has doctest.h already included.
//
#pragma once

#include <utility>
#include <type_traits>

namespace chobo
{

template <typename T>
class optional
{
public:
    typedef T value_type;

    optional()
    {}

    optional(const optional& o)
    {
        if (o.is_valid())
        {
            construct(o.get());
        }
    }

    optional(optional&& o)
    {
        if (o.is_valid())
        {
            construct(std::move(o.get()));
        }
    }

    optional(const value_type& v)
    {
        construct(v);
    }

    optional(value_type&& v)
    {
        construct(std::move(v));
    }

    // constructs with val if condition
    optional(bool condition, const value_type& val)
    {
        if (condition)
        {
            construct(val);
        }
    }

    ~optional()
    {
        destroy();
    }

    optional& operator=(const optional& o)
    {
        if (is_valid())
        {
            if (o.is_valid())
            {
                get() = o.get();
            }
            else
            {
                destroy();
            }
        }
        else
        {
            if (o.is_valid())
            {
                construct(o.get());
            }
        }

        return *this;
    }

    optional& operator=(optional&& o)
    {
        if (is_valid())
        {
            if (o.is_valid())
            {
                get() = std::move(o.get());
            }
            else
            {
                destroy();
            }
        }
        else
        {
            if (o.is_valid())
            {
                construct(std::move(o.get()));
            }
        }

        return *this;
    }

    optional& operator=(const value_type& t)
    {
        if (is_valid())
        {
            get() = t;
        }
        else
        {
            construct(t);
        }

        return *this;
    }

    optional& operator=(value_type&& t)
    {
        if (is_valid())
        {
            get() = std::move(t);
        }
        else
        {
            construct(std::move(t));
        }

        return *this;
    }

    template <class ...Args>
    void construct(Args&&... args)
    {
        destroy();
        void* p = &m_data;
        ::new (p) value_type(std::forward<Args>(args)...);
        m_is_valid = true;
    }

    void destroy()
    {
        if (is_valid())
        {
            get().~T();
            m_is_valid = false;
        }
    }

    void clear()
    {
        destroy();
    }

    T& get()
    {
        return reinterpret_cast<value_type&>(m_data);
    }

    const T& get() const
    {
        return reinterpret_cast<const value_type&>(m_data);
    }

    T& operator*()
    {
        return get();
    }

    const T& operator*() const
    {
        return get();
    }

    T* operator->()
    {
        return &get();
    }

    const T* operator->() const
    {
        return &get();
    }

    T& get_or(T& val)
    {
        if (is_valid())
        {
            return get();
        }
        else
        {
            return val;
        }
    }

    const T& get_or(const T& val) const
    {
        if (is_valid())
        {
            return get();
        }
        else
        {
            return val;
        }
    }

    bool is_valid() const
    {
        return m_is_valid;
    }

    explicit operator bool() const
    {
        return is_valid();
    }

private:
    typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type m_data;
    bool m_is_valid = false;
};

template <typename T>
bool operator==(const optional<T>& a, const optional<T>& b)
{
    if (a.is_valid())
    {
        if (b.is_valid())
        {
            return a.get() == b.get();
        }

        return false;
    }
    else
    {
        return !b.is_valid();
    }
}

template <typename T>
bool operator!=(const optional<T>& a, const optional<T>& b)
{
    if (a.is_valid())
    {
        if (b.is_valid())
        {
            return a.get() != b.get();
        }

        return true;
    }
    else
    {
        return b.is_valid();
    }
}

template <typename T>
bool operator==(const optional<T>& a, const T& b)
{
    return a.is_valid() && a.get() == b;
}

template <typename T>
bool operator!=(const optional<T>& a, const T& b)
{
    return !a.is_valid() || a.get() != b;
}


}

#if defined(CHOBO_OPTIONAL_TEST_WITH_DOCTEST)

#include <vector>

TEST_CASE("[optional] test")
{
    using namespace chobo;
    using namespace std;

    optional<int> iopt;
    optional<int> iinvalid;
    CHECK(!iopt);
    CHECK(iopt != 5);
    CHECK(iopt == iinvalid);
    iopt = 0;
    CHECK(iopt);
    CHECK(!*iopt);

    auto iopt2 = iopt;
    CHECK(iopt2);
    CHECK(iopt2 == iopt);

    iopt = 5;
    CHECK(iopt != iopt2);
    CHECK(iopt == 5);
    CHECK(iopt != 0);

    iopt2 = iopt;
    CHECK(iopt2 == 5);
    CHECK(iopt2.get_or(15) == 5);

    iopt2.clear();
    CHECK(!iopt2);
    CHECK(iopt2 != iopt);

    CHECK(iopt2.get_or(15) == 15);

    vector<int> foo = { 1, 2, 3 };
    optional<vector<int>> vopt = foo;
    CHECK(vopt);
    CHECK(vopt == foo);

    auto vopt2 = std::move(vopt);
    CHECK(vopt);
    CHECK(vopt2);
    CHECK(vopt2->size() == 3);
    CHECK(vopt->empty());

    vopt = std::move(vopt2);
    CHECK(vopt);
    CHECK(vopt2);
    CHECK(vopt->size() == 3);
    CHECK(vopt2->empty());


}

#endif
