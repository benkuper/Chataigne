// chobo-vector-ptr v1.02
//
// A non-owning pointer to std::vector which can be used in generic code
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
//  1.01 (2017-04-02) Fixed compilation error on assign and insert with count
//                    and value when count or value is 0
//  1.00 (2016-09-23) First public release
//
//
//                  DOCUMENTATION
//
// Simply include this file wherever you need.
// This is a non-owning pointer class to a std::vector. You can attach it to a
// given vector and make changes to it or just refer to it. Its purpose is to
// be used in generic code which requires a vector.
//
// It is the responsibility of the user to make sure the pointer is attached
// to a valid vector before calling any vector-specific methods or operators.
// Failing to do so will result in null-pointer-dereference crashes.
//
// The library includes two classes, for pointing to const and non-const
// vectors: vector_ptr, and const_vector_ptr. To automatically generate
// the appropriate pointer, use `make_vector_ptr(your_vector)`.
//
// Example:
//
// vector<int> myvec;
// auto vp = make_vector_ptr(myvec);
// vp.resize(4); // myvec has 4 elements now
// vp[0] = 5; // myvec.front() is now 5
//
// Reference:
//
// vector_ptr has all std::vector methods and operators
// const_vector_ptr has all std::vector const methods and operators
//
// Besides those, both classes define a ptr-like interface:
//
// void reset(vector* vec = nullptr) - reset the internal pointer
// operator bool() const - returns whether the internal pointer is valid
// vector* get() noexcept { return m_vector; } - return the internal pointer
//
//
//                  TESTS
//
// The tests are included in the header file and use doctest (https://github.com/onqtam/doctest).
// To run them, define CHOBO_VECTOR_PTR_TEST_WITH_DOCTEST before including
// the header in a file which has doctest.h already included.
//
#pragma once

#include <vector>

namespace chobo
{

template <typename T, typename Alloc = std::allocator<T>>
class vector_ptr
{
public:
    typedef std::vector<T, Alloc> vector;

    typedef T value_type;
    typedef Alloc allocator_type;
    typedef typename vector::size_type size_type;
    typedef typename vector::difference_type difference_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename vector::iterator iterator;
    typedef typename vector::const_iterator const_iterator;
    typedef typename std::reverse_iterator<iterator> reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;

    vector_ptr() = default;

    explicit vector_ptr(vector& vec)
        : m_vector(&vec)
    {}

    vector_ptr(const vector_ptr& other)
        : m_vector(other.m_vector)
    {}

    vector_ptr(vector_ptr&& other)
        : m_vector(other.m_vector)
    {
        other.m_vector = nullptr;
    }

    vector_ptr& operator=(const vector_ptr& other)
    {
        m_vector = other.m_vector;
        return *this;
    }

    vector_ptr& operator=(vector_ptr&& other)
    {
        m_vector = other.m_vector;
        other.m_vector = nullptr;
        return *this;
    }

    void reset(vector* vec = nullptr)
    {
        m_vector = vec;
    }

    explicit operator bool() const
    {
        return !!m_vector;
    }

    vector* get() noexcept { return m_vector; }
    const vector* get() const noexcept { return m_vector; }

    iterator begin() noexcept { return m_vector->begin(); }
    const_iterator begin() const noexcept { return m_vector->begin(); }
    iterator end() noexcept { return m_vector->end(); }
    const_iterator end() const noexcept { return m_vector->end(); }

    reverse_iterator rbegin() noexcept { return m_vector->rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return m_vector->rbegin(); }
    reverse_iterator rend() noexcept { return m_vector->rend(); }
    const_reverse_iterator rend() const noexcept { return m_vector->rend(); }

    const_iterator cbegin() const noexcept { return m_vector->cbegin(); }
    const_iterator cend() const noexcept { return m_vector->cend(); }
    const_reverse_iterator crbegin() const noexcept { return m_vector->crbegin(); }
    const_reverse_iterator crend() const noexcept { return m_vector->crend(); }

    size_type size() const noexcept { return m_vector->size(); }
    size_type max_size() const noexcept { return m_vector->max_size(); }
    void resize(size_type n) { m_vector->resize(n); }
    void resize(size_type n, const value_type& val) { m_vector->resize(n, val); }
    size_type capacity() const noexcept { return m_vector->capacity(); }
    bool empty() const noexcept { return m_vector->empty(); }
    void reserve(size_type n) { m_vector->reserve(n); }
    void shrink_to_fit() { m_vector->shrink_to_fit(); }

    reference operator[] (size_type n) { return m_vector->operator[](n); }
    const_reference operator[] (size_type n) const { return m_vector->operator[](n); }
    reference at(size_type n) { return m_vector->at(n); }
    const_reference at(size_type n) const { return m_vector->at(n); }
    reference front() { return m_vector->front(); }
    const_reference front() const { return m_vector->front(); }
    reference back() { return m_vector->back(); }
    const_reference back() const { return m_vector->back(); }
    value_type* data() noexcept { return m_vector->data(); }
    const value_type* data() const noexcept { return m_vector->data(); }

    template <class InputIterator, typename = decltype(*std::declval<InputIterator>())>
    void assign(InputIterator first, InputIterator last) { m_vector->assign(first, last); }
    void assign(size_type n, const value_type& val) { m_vector->assign(n, val); }
    void assign(std::initializer_list<value_type> il) { m_vector->asign(il); }

    void push_back(const value_type& val) { m_vector->push_back(val); }
    void push_back(value_type&& val) { m_vector->push_back(std::forward<value_type>(val)); }

    void pop_back() { m_vector->pop_back(); }

    iterator insert(const_iterator position, const value_type& val) { return m_vector->insert(position, val); }
    iterator insert(const_iterator position, size_type n, const value_type& val) { return m_vector->insert(position, n, val); }
    template <class InputIterator, typename = decltype(*std::declval<InputIterator>())>
    iterator insert(const_iterator position, InputIterator first, InputIterator last) { return m_vector->insert(position, first, last); }
    iterator insert(const_iterator position, value_type&& val) { return m_vector->insert(position, std::forward<value_type>(val)); }
    iterator insert(const_iterator position, std::initializer_list<value_type> il) { return m_vector->insert(il); }

    iterator erase(const_iterator position) { return m_vector->erase(position); }
    iterator erase(const_iterator first, const_iterator last) { return m_vector->erase(first, last); }

    void swap(vector_ptr& other) { m_vector->swap(other.m_vector); }
    void swap(vector& other) { m_vector->swap(other); }

    void clear() noexcept { m_vector->clear(); }

    template <class... Args>
    iterator emplace(const_iterator position, Args&&... args) { return m_vector->emplace(position, std::forward<Args>(args)...); }
    template <class... Args>
    void emplace_back(Args&&... args) { m_vector->emplace_back(std::forward<Args>(args)...); }

    allocator_type get_allocator() const noexcept { return m_vector->get_allocator(); }

private:
    vector* m_vector = nullptr;
};

template <class T, class Alloc>
bool operator== (const vector_ptr<T, Alloc>& lhs, const vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector == rhs.m_vector;
}
template <class T, class Alloc>
bool operator!= (const vector_ptr<T, Alloc>& lhs, const vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector != rhs.m_vector;
}
template <class T, class Alloc>
bool operator<  (const vector_ptr<T, Alloc>& lhs, const vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector < rhs.m_vector;
}
template <class T, class Alloc>
bool operator<= (const vector_ptr<T, Alloc>& lhs, const vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector <= rhs.m_vector;
}
template <class T, class Alloc>
bool operator>(const vector_ptr<T, Alloc>& lhs, const vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector > rhs.m_vector;
}
template <class T, class Alloc>
bool operator>= (const vector_ptr<T, Alloc>& lhs, const vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector >= rhs.m_vector;
}

///////////////////////////////////////////////////////////////////////////////


template <typename T, typename Alloc = std::allocator<T>>
class const_vector_ptr
{
public:
    typedef std::vector<T, Alloc> vector;

    typedef T value_type;
    typedef Alloc allocator_type;
    typedef typename vector::size_type size_type;
    typedef typename vector::difference_type difference_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename vector::iterator iterator;
    typedef typename vector::const_iterator const_iterator;
    typedef typename std::reverse_iterator<iterator> reverse_iterator;
    typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;

    const_vector_ptr() = default;

    explicit const_vector_ptr(const vector& vec)
        : m_vector(&vec)
    {}

    const_vector_ptr(const const_vector_ptr& other)
        : m_vector(other.m_vector)
    {}

    const_vector_ptr(const_vector_ptr&& other)
        : m_vector(other.m_vector)
    {
        other.m_vector = nullptr;
    }

    const_vector_ptr& operator=(const const_vector_ptr& other)
    {
        m_vector = other.m_vector;
        return *this;
    }

    const_vector_ptr& operator=(const_vector_ptr&& other)
    {
        m_vector = other.m_vector;
        other.m_vector = nullptr;
        return *this;
    }

    void reset(const vector* vec = nullptr)
    {
        m_vector = vec;
    }

    explicit operator bool() const
    {
        return !!m_vector;
    }

    const vector* get() const noexcept { return m_vector; }

    const_iterator begin() const noexcept { return m_vector->begin(); }
    const_iterator end() const noexcept { return m_vector->end(); }

    const_reverse_iterator rbegin() const noexcept { return m_vector->rbegin(); }
    const_reverse_iterator rend() const noexcept { return m_vector->rend(); }

    const_iterator cbegin() const noexcept { return m_vector->cbegin(); }
    const_iterator cend() const noexcept { return m_vector->cend(); }
    const_reverse_iterator crbegin() const noexcept { return m_vector->crbegin(); }
    const_reverse_iterator crend() const noexcept { return m_vector->crend(); }

    size_type size() const noexcept { return m_vector->size(); }
    size_type max_size() const noexcept { return m_vector->max_size(); }
    size_type capacity() const noexcept { return m_vector->capacity(); }
    bool empty() const noexcept { return m_vector->empty(); }

    const_reference operator[] (size_type n) const { return m_vector->operator[](n); }
    const_reference at(size_type n) const { return m_vector->at(n); }
    const_reference front() const { return m_vector->front(); }
    const_reference back() const { return m_vector->back(); }
    const value_type* data() const noexcept { return m_vector->data(); }

    allocator_type get_allocator() const noexcept { return m_vector->get_allocator(); }

private:
    const vector* m_vector = nullptr;
};

template <class T, class Alloc>
bool operator== (const const_vector_ptr<T, Alloc>& lhs, const const_vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector == rhs.m_vector;
}
template <class T, class Alloc>
bool operator!= (const const_vector_ptr<T, Alloc>& lhs, const const_vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector != rhs.m_vector;
}
template <class T, class Alloc>
bool operator<  (const const_vector_ptr<T, Alloc>& lhs, const const_vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector < rhs.m_vector;
}
template <class T, class Alloc>
bool operator<= (const const_vector_ptr<T, Alloc>& lhs, const const_vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector <= rhs.m_vector;
}
template <class T, class Alloc>
bool operator>(const const_vector_ptr<T, Alloc>& lhs, const const_vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector > rhs.m_vector;
}
template <class T, class Alloc>
bool operator>= (const const_vector_ptr<T, Alloc>& lhs, const const_vector_ptr<T, Alloc>& rhs)
{
    return lhs.m_vector >= rhs.m_vector;
}

///////////////////////////////////////////////////////////////////////////////

template <class T, class Alloc>
vector_ptr<T, Alloc> make_vector_ptr(std::vector<T, Alloc>& vec)
{
    return vector_ptr<T, Alloc>(vec);
}

template <class T, class Alloc>
const_vector_ptr<T, Alloc> make_vector_ptr(const std::vector<T, Alloc>& vec)
{
    return const_vector_ptr<T, Alloc>(vec);
}

}

#if defined(CHOBO_VECTOR_PTR_TEST_WITH_DOCTEST)

#include <type_traits>

TEST_CASE("[vector_ptr] test")
{
    using namespace chobo;

    std::vector<int> vec;

    vector_ptr<int> p1;
    CHECK(!p1);

    auto p2 = make_vector_ptr(vec);
    static_assert(std::is_same<vector_ptr<int>, decltype(p2)>::value, "p2 must be vector_ptr");
    CHECK(p2);
    CHECK(p2.size() == 0);

    p1.reset(&vec);
    CHECK(p1);

    p1.resize(3);
    CHECK(vec.size() == 3);
    CHECK(p2.size() == 3);

    for (auto i : p2)
    {
        CHECK(i == 0);
    }

    p2.reset();
    CHECK(!p2);

    p2 = p1;
    CHECK(p2);

    p2[1] = 5;
    CHECK(p1.at(1) == 5);

    auto p3 = p1;
    CHECK(p3);
    CHECK(p3.size() == 3);
    CHECK(*(p3.begin() + 1) == 5);

    auto p4 = std::move(p1);
    CHECK(!p1);
    CHECK(p4);
    CHECK(p4.size() == 3);
    CHECK(*(p4.rbegin() + 1) == 5);

    CHECK(p2);
    p2 = p1;
    CHECK(!p2);

    p2 = std::move(p4);
    CHECK(!p4);
    CHECK(p2);
    CHECK(p2.size() == 3);

    p2.assign(3, 0);    
    CHECK(vec.size() == 3);
    CHECK(vec.front() == 0);
    CHECK(vec.back() == 0);

    p2.insert(p2.begin(), 1, 0);
    CHECK(vec.size() == 4);
    CHECK(vec.front() == 0);
}

TEST_CASE("[const_vector_ptr] test")
{
    using namespace chobo;

    std::vector<int> ncvec;
    ncvec.resize(3);

    const auto& vec = ncvec;

    const_vector_ptr<int> p1;
    CHECK(!p1);

    auto p2 = make_vector_ptr(vec);
    static_assert(std::is_same<const_vector_ptr<int>, decltype(p2)>::value, "p2 must be const_vector_ptr");
    CHECK(p2);
    CHECK(p2.size() == 3);

    p1.reset(&vec);
    CHECK(p1);

    CHECK(p1.size() == 3);

    for (auto i : p2)
    {
        CHECK(i == 0);
    }

    p2.reset();
    CHECK(!p2);

    p2 = p1;
    CHECK(p2);

    ncvec[1] = 5;
    CHECK(p1.at(1) == 5);

    auto p3 = p1;
    CHECK(p3);
    CHECK(p3.size() == 3);
    CHECK(*(p3.begin() + 1) == 5);

    auto p4 = std::move(p1);
    CHECK(!p1);
    CHECK(p4);
    CHECK(p4.size() == 3);
    CHECK(*(p4.rbegin() + 1) == 5);

    CHECK(p2);
    p2 = p1;
    CHECK(!p2);

    p2 = std::move(p4);
    CHECK(!p4);
    CHECK(p2);
    CHECK(p2.size() == 3);
}

#endif
