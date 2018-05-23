// chobo-static-vector v1.02
//
// std::vector-like class with a fixed capacity
//
// MIT License:
// Copyright(c) 2016-2017 Chobolabs Inc.
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
//  1.02 (2017-02-05) Added swap to make it a better drop-in replacement of std::vector
//  1.01 (2016-09-27) Qualified operator new. Fixed self usurp on assignment
//  1.00 (2016-09-23) First public release
//
//
//                  DOCUMENTATION
//
// Simply include this file wherever you need.
// It defines the class chobo::static_vector, which is almost a drop-in
// replacement of std::vector, but has a fixed capacity as a template argument.
// It gives you the benefits of using std::array (cache-locality) with the
// flexibility of std::vector - dynamic size.
// However the size may never exceed its initially set capacity.
//
// Example:
//
// chobo::static_vector<int, 4> myvec; // a static vector of size 0 and capacity 4
// myvec.resize(2); // vector is {0,0}
// myvec[1] = 11; // vector is {0,11}
// myvec.push_back(7); // vector is {0,11,7}
// myvec.insert(myvec.begin() + 1, 3); // vector is {0,3,11,7}
// myvec.push_back(5); // runtime error
// myvec.erase(myvec.begin());  // vector is {3,11,7}
// myvec.resize(5); // runtime error
//
//                  Configuration
//
// The library has two configuration options. They can be set as #define-s
// before including the header file, but it is recommended to change the code
// of the library itself with the values you want, especially if you include
// the library in many compilation units (as opposed to, say, a precompiled
// header or a central header).
//
//                  Config out of range error handling
//
// An out of range error is a runtime error which is triggered when the vector
// needs to be resized with a size higher than its capacity.
// For example: chobo::static_vector<int, 100> v(101);
//
// This is set by defining CHOBO_STATIC_VECTOR_ERROR_HANDLING to one of the
// following values:
// * CHOBO_STATIC_VECTOR_ERROR_HANDLING_NONE - no error handling. Crashes WILL
//      ensue if the error is triggered.
// * CHOBO_STATIC_VECTOR_ERROR_HANDLING_THROW - std::out_of_range is thrown.
// * CHOBO_STATIC_VECTOR_ERROR_HANDLING_ASSERT - asserions are triggered.
// * CHOBO_STATIC_VECTOR_ERROR_HANDLING_RESCUE - the error is ignored but
//      sanity is (somewhat) preserved. Functions which trigger the error will
//      just bail without changing the vector.
// * CHOBO_STATIC_VECTOR_ERROR_HANDLING_ASSERT_AND_THROW - combines assert and
//      throw to catch errors more easily in debug mode
// * CHOBO_STATIC_VECTOR_ERROR_HANDLING_ASSERT_AND_RESCUE - combines assert and
//      rescue to catch errors in debug mode and silently bail in release mode.
//
// To set this setting by editing the file change the line:
// ```
// #   define CHOBO_STATIC_VECTOR_ERROR_HANDLING CHOBO_STATIC_VECTOR_ERROR_HANDLING_THROW
// ```
// to the default setting of your choice
//
//                  Config bounds checks:
//
// By default bounds checks are made in debug mode (via an asser) when accessing
// elements (with `at` or `[]`). Iterators are not checked (yet...)
//
// To disable them, you can define CHOBO_STATIC_VECTOR_NO_DEBUG_BOUNDS_CHECK
// before including the header.
//
//
//                  MISC
//
// * There is an unused (and unusable) allocator class defined inside
// static_vector. It's point is to be a sham for templates which refer to
// container::allocator. It also allows it to work with chobo::flat_map
//
//                  TESTS
//
// The tests are included in the header file and use doctest (https://github.com/onqtam/doctest).
// To run them, define CHOBO_STATIC_VECTOR_TEST_WITH_DOCTEST before including
// the header in a file which has doctest.h already included.
//
#pragma once

#include <type_traits>
#include <cstddef>
#include <iterator>

#define CHOBO_STATIC_VECTOR_ERROR_HANDLING_NONE  0
#define CHOBO_STATIC_VECTOR_ERROR_HANDLING_THROW 1
#define CHOBO_STATIC_VECTOR_ERROR_HANDLING_ASSERT 2
#define CHOBO_STATIC_VECTOR_ERROR_HANDLING_RESCUE 3
#define CHOBO_STATIC_VECTOR_ERROR_HANDLING_ASSERT_AND_THROW 4
#define CHOBO_STATIC_VECTOR_ERROR_HANDLING_ASSERT_AND_RESCUE 5

#if !defined(CHOBO_STATIC_VECTOR_ERROR_HANDLING)
#   define CHOBO_STATIC_VECTOR_ERROR_HANDLING CHOBO_STATIC_VECTOR_ERROR_HANDLING_THROW
#endif

#if CHOBO_STATIC_VECTOR_ERROR_HANDLING == CHOBO_STATIC_VECTOR_ERROR_HANDLING_NONE
#   define _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(cond, rescue_return)
#elif CHOBO_STATIC_VECTOR_ERROR_HANDLING == CHOBO_STATIC_VECTOR_ERROR_HANDLING_THROW
#   include <stdexcept>
#   define _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(cond, rescue_return) if (cond) throw std::out_of_range("chobo::static_vector out of range")
#elif CHOBO_STATIC_VECTOR_ERROR_HANDLING == CHOBO_STATIC_VECTOR_ERROR_HANDLING_ASSERT
#   include <cassert>
#   define _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(cond, rescue_return) assert(!(cond) && "chobo::static_vector out of range")
#elif CHOBO_STATIC_VECTOR_ERROR_HANDLING == CHOBO_STATIC_VECTOR_ERROR_HANDLING_RESCUE
#   define _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(cond, rescue_return) if (cond) return rescue_return
#elif CHOBO_STATIC_VECTOR_ERROR_HANDLING == CHOBO_STATIC_VECTOR_ERROR_HANDLING_ASSERT_AND_THROW
#   include <stdexcept>
#   include <cassert>
#   define _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(cond, rescue_return) \
    do { if (cond) { assert(false && "chobo::static_vector out of range"); throw std::out_of_range("chobo::static_vector out of range"); } } while(false)
#elif CHOBO_STATIC_VECTOR_ERROR_HANDLING == CHOBO_STATIC_VECTOR_ERROR_HANDLING_ASSERT_AND_RESCUE
#   include <cassert>
#   define _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(cond, rescue_return) \
    do { if (cond) { assert(false && "chobo::static_vector out of range"); return rescue_return; } } while(false)
#else
#error "Unknown CHOBO_STATIC_VECTOR_ERRROR_HANDLING"
#endif


#if defined(CHOBO_STATIC_VECTOR_NO_DEBUG_BOUNDS_CHECK)
#   define _CHOBO_STATIC_VECTOR_BOUNDS_CHECK(i)
#else
#   include <cassert>
#   define _CHOBO_STATIC_VECTOR_BOUNDS_CHECK(i) assert((i) < this->size())
#endif

namespace chobo
{

template<typename T, size_t Capacity>
struct static_vector
{
    struct fake_allocator
    {
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
    };

public:
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using allocator_type = fake_allocator;

    static_vector() = default;

    explicit static_vector(size_t count)
    {
        resize(count);
    }

    static_vector(size_t count, const T& value)
    {
        _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(count > Capacity, );

        for (size_t i = 0; i < count; ++i)
        {
            push_back(value);
        }
    }

    static_vector(std::initializer_list<T> l)
    {
        _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(l.size() > Capacity, );

        for (auto&& i : l)
        {
            push_back(i);
        }
    }

    static_vector(const static_vector& v)
    {
        for (const auto& i : v)
        {
            push_back(i);
        }
    }

    template <size_t CapacityB>
    static_vector(const static_vector<T, CapacityB>& v)
    {
        _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(v.size() > Capacity, );

        for (const auto& i : v)
        {
            push_back(i);
        }
    }

    static_vector(static_vector&& v)
    {
        for (auto i = v.begin(); i != v.end(); ++i)
        {
            emplace_back(std::move(*i));
        }
        v.clear();
    }

    ~static_vector()
    {
        clear();
    }

    static_vector& operator=(const static_vector& v)
    {
        if (this == &v)
        {
            // prevent self usurp
            return *this;
        }

        clear();
        for (auto& elem : v)
        {
            push_back(elem);
        }

        return *this;
    }

    static_vector& operator=(static_vector&& v)
    {
        clear();
        for (auto i = v.begin(); i!=v.end(); ++i)
        {
            emplace_back(std::move(*i));
        }

        v.clear();
        return *this;
    }

    const_reference at(size_type i) const
    {
        _CHOBO_STATIC_VECTOR_BOUNDS_CHECK(i);
        return *reinterpret_cast<const T*>(m_data + i);
    }

    reference at(size_type i)
    {
        _CHOBO_STATIC_VECTOR_BOUNDS_CHECK(i);
        return *reinterpret_cast<T*>(m_data + i);
    }

    const_reference operator[](size_type i) const
    {
        return at(i);
    }

    reference operator[](size_type i)
    {
        return at(i);
    }

    const_reference front() const
    {
        return at(0);
    }

    reference front()
    {
        return at(0);
    }

    const_reference back() const
    {
        return at(m_size - 1);
    }

    reference back()
    {
        return at(m_size - 1);
    }

    const_pointer data() const noexcept
    {
        return reinterpret_cast<const T*>(m_data);
    }

    pointer data() noexcept
    {
        return reinterpret_cast<T*>(m_data);
    }

    // iterators
    iterator begin() noexcept
    {
        return data();
    }

    const_iterator begin() const noexcept
    {
        return data();
    }

    const_iterator cbegin() const noexcept
    {
        return data();
    }

    iterator end() noexcept
    {
        return data() + m_size;
    }

    const_iterator end() const noexcept
    {
        return data() + m_size;
    }

    const_iterator cend() const noexcept
    {
        return data() + m_size;
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    // capacity
    bool empty() const noexcept
    {
        return m_size == 0;
    }

    size_t size() const noexcept
    {
        return m_size;
    }

    size_t max_size() const noexcept
    {
        return Capacity;
    }

    size_t capacity() const noexcept
    {
        return Capacity;
    }

    // modifiers
    void pop_back()
    {
        reinterpret_cast<const T*>(m_data + m_size - 1)->~T();
        --m_size;
    }

    void clear() noexcept
    {
        while (!empty())
        {
            pop_back();
        }
    }

    void push_back(const_reference v)
    {
        _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(size() >= Capacity, );

        ::new(m_data + m_size) T(v);
        ++m_size;
    }

    void push_back(T&& v)
    {
        _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(size() >= Capacity, );

        ::new(m_data + m_size) T(std::move(v));
        ++m_size;
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(size() >= Capacity, );

        ::new(m_data + m_size) T(std::forward<Args>(args)...);
        ++m_size;
    }

    iterator insert(iterator position, const value_type& val)
    {
        _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(size() >= Capacity, position);

        if (position == end())
        {
            emplace_back(val);
            return position;
        }

        emplace_back(std::move(back()));

        for (auto i = end() - 2; i != position; --i)
        {
            *i = std::move(*(i - 1));
        }

        *position = val;

        return position;
    }

    template<typename... Args>
    iterator emplace(iterator position, Args&&... args)
    {
        _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(size() >= Capacity, position);

        if (position == end())
        {
            emplace_back(std::forward<Args>(args)...);
            return position;
        }

        emplace_back(std::move(back()));

        for (auto i = end() - 2; i != position; --i)
        {
            *i = std::move(*(i - 1));
        }

        position->~T();
        ::new (position) T(std::forward<Args>(args)...);

        return position;
    }

    iterator erase(iterator position)
    {
        auto dist = position - begin();
        position->~T();

        for (auto i = position + 1; i != end(); ++i)
        {
            *(i - 1) = std::move(*i);
        }

        resize(size() - 1);
        return begin() + dist;
    }

    void resize(size_type n)
    {
        _CHOBO_STATIC_VECTOR_OUT_OF_RANGE_IF(n > Capacity, );

        while (m_size > n)
        {
            pop_back();
        }

        while (n > m_size)
        {
            emplace_back();
        }
    }

    void swap(static_vector& v)
    {
        static_vector* longer;
        static_vector* shorter;

        if (v.m_size > m_size)
        {
            longer = &v;
            shorter = this;
        }
        else
        {
            longer = this;
            shorter = &v;
        }

        for (size_t i = 0; i < shorter->size(); ++i)
        {
            std::swap(shorter->at(i), longer->at(i));
        }

        auto short_size = shorter->m_size;

        for (size_t i = shorter->size(); i < longer->size(); ++i)
        {
            shorter->emplace_back(std::move(longer->at(i)));
            longer->at(i).~T();
        }

        longer->m_size = short_size;
    }

private:
    typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type m_data[Capacity];
    size_t m_size = 0;
};

template <typename T, size_t CapacityA, size_t CapacityB>
bool operator==(const static_vector<T, CapacityA>& a, const static_vector<T, CapacityB>& b)
{
    if (a.size() != b.size())
    {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i])
            return false;
    }

    return true;
}

template <typename T, size_t CapacityA, size_t CapacityB>
bool operator!=(const static_vector<T, CapacityA>& a, const static_vector<T, CapacityB>& b)
{
    if (a.size() != b.size())
    {
        return true;
    }

    for (size_t i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i])
            return true;
    }

    return false;
}

}


#if defined(CHOBO_STATIC_VECTOR_TEST_WITH_DOCTEST)

#include <string>

TEST_CASE("[static_vector] test")
{
    using namespace chobo;
    using namespace std;

    static_vector<int, 10> ivec;

    static_assert(sizeof(ivec) == sizeof(int) * 10 + sizeof(size_t), "static_vector must have size of N*t + size_t");

    CHECK(ivec.size() == 0);
    CHECK(ivec.capacity() == 10);
    CHECK(ivec.max_size() == 10);
    CHECK(ivec.begin() == ivec.end());
    CHECK(ivec.cbegin() == ivec.cend());
    CHECK(ivec.empty());

    ivec.push_back(5);
    CHECK(ivec.size() == 1);
    CHECK(ivec[0] == 5);
    auto it = ivec.begin();
    CHECK(it == ivec.data());
    CHECK(it == ivec.cbegin());
    CHECK(*it == 5);
    ++it;
    CHECK(it == ivec.end());
    CHECK(it == ivec.cend());

    ivec.emplace_back(3);
    CHECK(ivec.size() == 2);
    auto rit = ivec.rbegin();
    CHECK(*rit == 3);
    ++rit;
    *rit = 12;
    ++rit;
    CHECK(rit == ivec.rend());
    CHECK(rit == ivec.crend());
    CHECK(ivec.front() == 12);
    CHECK(ivec.back() == 3);

    ivec.insert(ivec.begin(), 53);
    ivec.insert(ivec.begin() + 2, 90);
    ivec.insert(ivec.begin() + 4, 17);
    ivec.insert(ivec.end(), 6);

    int ints[] = { 53, 12, 90, 3, 17, 6 };
    CHECK(ivec.size() == 6);
    CHECK(memcmp(ivec.data(), ints, sizeof(ints)) == 0);

    ivec.pop_back();
    CHECK(ivec.size() == 5);
    CHECK(memcmp(ivec.data(), ints, sizeof(ints) - sizeof(int)) == 0);

    ivec.resize(6);
    CHECK(ivec.size() == 6);
    ints[5] = 0;
    CHECK(memcmp(ivec.data(), ints, sizeof(ints)) == 0);

    const static_vector<int, 5> ivec2 = { 1, 2, 3, 4 };
    CHECK(ivec2.size() == 4);
    CHECK(*ivec2.begin() == 1);
    CHECK(ivec2[1] == 2);
    CHECK(ivec2.at(2) == 3);
    CHECK(*ivec2.rbegin() == 4);

    ivec.erase(ivec.begin());
    CHECK(ivec.size() == 5);
    CHECK(ivec.front() == 12);
    CHECK(memcmp(ivec.data(), ints + 1, ivec.size() * sizeof(int)) == 0);

    ivec.erase(ivec.begin() + 2);
    CHECK(ivec.size() == 4);
    CHECK(ivec[2] == 17);

    static_vector<string, 11> svec = { "as", "df" };
    CHECK(svec.size() == 2);
    string s1 = "the quick brown fox jumped over the lazy dog 1234567890";
    svec.emplace_back(s1);
    CHECK(svec.back() == s1);

    auto svec1 = svec;
    static_assert(sizeof(svec1) == sizeof(string) * 11 + sizeof(size_t), "static_vector must have size of N*t + size_t");
    CHECK(svec1 == svec);

    const void* cstr = svec.back().c_str();
    auto svec2 = std::move(svec);
    CHECK(svec2.size() == 3);
    CHECK(svec2.back() == s1);

    CHECK(svec.empty());
    CHECK(svec2.back().c_str() == cstr);

    svec = std::move(svec2);
    CHECK(svec2.empty());
    CHECK(svec.back().c_str() == cstr);

    svec2 = svec;
    CHECK(svec2.back() == s1);
    CHECK(svec.back() == s1);
    CHECK(svec == svec2);

    svec.insert(svec.begin(), s1);
    CHECK(svec.size() == 4);
    CHECK(svec.back().c_str() == cstr);
    CHECK(svec.front() == svec.back());

    cstr = s1.c_str();
    svec.emplace(svec.begin() + 2, std::move(s1));
    CHECK(svec.size() == 5);
    CHECK(svec.front() == svec[2]);
    CHECK(svec[2].c_str() == cstr);

    svec.clear();
    CHECK(svec.empty());
    svec2.clear();
    CHECK(svec2.empty());
    CHECK(svec == svec2);

    svec.resize(svec.capacity());
    CHECK(svec.size() == svec.capacity());

    for (auto& s : svec)
    {
        CHECK(s.empty());
    }

#if !defined(__EMSCRIPTEN__) || !defined(NDEBUG) // emscripten allows exceptions with -O0
    CHECK_THROWS_AS(svec.push_back("asd"), std::out_of_range);
    CHECK(svec.size() == svec.capacity());
    CHECK_THROWS_AS(svec.resize(55), std::out_of_range);
    CHECK(svec.size() == svec.capacity());
    CHECK_THROWS_AS(svec.insert(svec.begin(), "55"), std::out_of_range);
    CHECK(svec.size() == svec.capacity());
    CHECK_THROWS_AS(svec.emplace(svec.begin(), "55"), std::out_of_range);
    CHECK(svec.size() == svec.capacity());
#endif

    // self usurp
    svec = svec;
    CHECK(svec.size() == svec.capacity());

    // swap
    svec = { "1", "2", "3" };
    svec2 = { "4", "5", "6", "7" };

    svec.swap(svec2);

    CHECK(svec.size() == 4);
    CHECK(svec2.size() == 3);
    CHECK(svec2.front() == "1");
    CHECK(svec.back() == "7");

    svec = { "a", "b", "c" };
    svec2.swap(svec);

    CHECK(svec2.size() == svec.size());
    CHECK(svec2.back() == "c");
    CHECK(svec.front() == "1");
}

#endif

