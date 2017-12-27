// chobo-vector-view v1.01
//
// A view of a std::vector which makes it look as a vector of another type
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
//  1.01 (2016-09-27) Added checks for unsupported resizes when
//             sizeof(view type) is less than half of sizeof(vec type)
//  1.00 (2016-09-23) First public release
//
//
//                  DOCUMENTATION
//
// Simply include this file wherever you need.
// A vector view is a class which attaches to an existing std::vector
// and provides a view to its data as an alternative type, along
// with a working std::vector-like interface.
//
// THIS IS DANGEROUS, so you must know the risks ot doing so before
// using this library.
//
// The library includes two classes, for viewing const and non-const
// vectors: vector_view, and const_vector_view. To automatically generate
// the appropriate pointer, use `make_vector_view<NewType>(your_vector)`.
//
// Example:
//
// vector<vector2D> geometry;
// ... // fill geometry with data
// auto float_view = make_vector_view<float>(geometry);
// float_view[5] = 8; // equivalent to geometry[2].y = 8;
//
// auto view4d = make_vector_view<vector4D>(geometry);
//
// // add two elements to original array {1,2} and {3,4}
// view4d.push_back(make_vector4D(1, 2, 3, 4));
//
// Reference:
//
// vector_view has the most common std::vector methods and operators
// const_vector_view has the most common std::vector const methods and operators
//
// besides that both have a method vector& vec() for getting the underlying
// std::vector
//
//
//                  Configuration
//
// chobo::vector_view has two configurable settings:
//
//                  Config bounds checks:
//
// By default bounds checks are made in debug mode (via an assert) when accessing
// elements (with `at` or `[]`). Iterators are not checked (yet...)
//
// To disable them, you can define CHOBO_VECTOR_VIEW_NO_DEBUG_BOUNDS_CHECK
// before including the header.
//
//                  Config POD check:
//
// By default the library checks that both the source and the target type of
// the view are PODs. This is a good idea but sometimes a type is almost a POD
// (say constructors to geometry vectors) and is still fit for a view
//
// To disable the POD checks, define CHOBO_VECTOR_VIEW_NO_POD_CHECK
// before including the header.
//
//                  Using vector_view with non-POD types
//
// If you end up using the class with a non-pod type, you should take into
// account that when changing the size of the vector via its view ONLY the
// constructors and destructors of the source type (the one in the std::vector)
// will be called. The target type's constructors and destructors will NEVER be
// called. All assignments happen with its assignment operator (oprator=).
//
//
//                  TESTS
//
// The tests are included in the header file and use doctest (https://github.com/onqtam/doctest).
// To run them, define CHOBO_VECTOR_VIEW_TEST_WITH_DOCTEST before including
// the header in a file which has doctest.h already included.
//
#pragma once

#include <vector>

#if defined(CHOBO_VECTOR_VIEW_NO_DEBUG_BOUNDS_CHECK)
#   define _CHOBO_VECTOR_VIEW_BOUNDS_CHECK(i)
#else
#   include <cassert>
#   define _CHOBO_VECTOR_VIEW_BOUNDS_CHECK(i) assert((i) < this->size())
#endif

#if defined(CHOBO_VECTOR_VIEW_NO_POD_CHECK)
#define _CHOBO_VECTOR_VIEW_POD_CHECK(T)
#else
#include <type_traits>
#define _CHOBO_VECTOR_VIEW_POD_CHECK(T) static_assert(std::is_pod<T>::value, #T " must be a pod");
#endif

namespace chobo
{

template <typename T, typename U, typename Alloc = std::allocator<T>>
class vector_view
{
    _CHOBO_VECTOR_VIEW_POD_CHECK(T)
    _CHOBO_VECTOR_VIEW_POD_CHECK(U)
public:
    typedef std::vector<T, Alloc> vector;

    typedef U value_type;
    typedef T vec_value_type;
    typedef Alloc allocator_type;
    typedef typename vector::size_type size_type;
    typedef typename vector::difference_type difference_type;
    typedef U& reference;
    typedef const U& const_reference;
    typedef U* pointer;
    typedef const U* const_pointer;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit vector_view(vector& vec)
        : m_vector(vec)
    {}

    vector_view(const vector_view& other) = delete;
    vector_view& operator=(const vector_view& other) = delete;

    vector_view(vector_view&& other)
        : m_vector(other.m_vector)
    {} // intentionally don't inavlidate the other view

    vector_view& operator=(vector_view&& other)
    {
        m_vector = std::move(other.m_vector);
    }

    vector& vec()
    {
        return m_vector;
    }

    const vector& vec() const
    {
        return m_vector;
    }

    template <typename UAlloc>
    vector_view& operator=(const std::vector<U, UAlloc>& other)
    {
        size_type n = other.size();
        resize(n);
        for (size_type i = 0; i < n; ++i)
        {
            this->at(i) = other[i];
        }
    }

    iterator begin() noexcept
    {
        return reinterpret_cast<iterator>(m_vector.data());
    }

    const_iterator begin() const noexcept
    {
        return reinterpret_cast<const_iterator>(m_vector.data());
    }

    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    iterator end() noexcept
    {
        return begin() + size();
    }

    const_iterator end() const noexcept
    {
        return begin() + size();
    }

    const_iterator cend() const noexcept
    {
        return begin() + size();
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

    size_type size() const noexcept
    {
        return (m_vector.size() * sizeof(vec_value_type)) / sizeof(value_type);
    }

    size_type capacity() const noexcept
    {
        return (m_vector.capacity() * sizeof(vec_value_type)) / sizeof(value_type);
    }

    size_type max_size() const noexcept
    {
        return (m_vector.max_size() * sizeof(vec_value_type)) / sizeof(value_type);
    }

    void resize(size_type n)
    {
        size_type s = (n * sizeof(value_type) + sizeof(vec_value_type) - 1) / sizeof(vec_value_type);
        m_vector.resize(s);

#if !defined CHOBO_VECTOR_VIEW_NO_RESIZE_CHECK
        // Is this assert fails, here's what has happened:
        // The size of the type of the view is smaller than the half of the size of the vector.
        // Since the vector's number of elements must be a whole number, we cannot resize it to
        // hold cerrtain numbers of the type of the view (ie not multiples of the sizeof(vec_type)/sizeof(view_type))
        //
        // In theory this could be supported if we add a custom size variable for the view class
        // However, besides the increase in complexity, this will cause us to lose a cruicial
        // feature - having changes to the vector from outside be automatically reflected on the view
        // because it's only a view, and has no state of its own.
        // Adding such size will be a state for this class.
        // Perhaps if there is need such a feature could be implemented but in a class
        // with another name, where it's clear that persisting it would be hurtful.
        //
        // So to avoid potential bugs this assertion, as well as the static assertions in
        // push_back and pop_back have been added.
        assert(size() == n && "unsupported resize");
#endif
    }

    void resize(size_type n, const value_type& val)
    {
        size_type prev_size = size();
        resize(n);
        for (iterator i = begin() + prev_size; i != end(); ++i)
        {
            *i = val;
        }
    }

    bool empty() const noexcept
    {
        return m_vector.size() * sizeof(vec_value_type) < sizeof(value_type);
    }

    void reserve(size_type n)
    {
        n = (n * sizeof(value_type) + sizeof(vec_value_type) - 1) / sizeof(vec_value_type);
        m_vector.reserve(n);
    }

    void shrink_to_fit()
    {
        m_vector.shrink_to_fit();
    }

    const_reference at(size_type i) const
    {
        _CHOBO_VECTOR_VIEW_BOUNDS_CHECK(i);
        return *(begin() + i);
    }

    reference at(size_type i)
    {
        _CHOBO_VECTOR_VIEW_BOUNDS_CHECK(i);
        return *(begin() + i);
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
        return *(end() - 1);
    }

    reference back()
    {
        return *(end() - 1);
    }

    const_pointer data() const noexcept
    {
        return begin();
    }

    pointer data() noexcept
    {
        return begin();
    }

    void push_back(const value_type& val)
    {
        // see comment in resize for an explanation
        static_assert(sizeof(value_type) > sizeof(vec_value_type) / 2,
            "vector_view::push_back is not supported for value_type with size smaller than half of the viewed type");
        resize(size() + 1, val);
    }

    void push_back(value_type&& val)
    {
        // see comment in resize for an explanation
        static_assert(sizeof(value_type) > sizeof(vec_value_type) / 2,
            "vector_view::push_back is not supported for value_type with size smaller than half of the viewed type");
        resize(size() + 1);
        back() = std::move(val);
    }

    void pop_back()
    {
        // see comment in resize for an explanation
        static_assert(sizeof(value_type) > sizeof(vec_value_type) / 2,
            "vector_view::pop_back is not supported for value_type with size smaller than half of the viewed type");
        resize(size() - 1);
    }

    void clear() noexcept
    {
        m_vector.clear();
    }

private:
    vector& m_vector;
};

///////////////////////////////////////////////////////////////////////////////

template <typename T, typename U, typename Alloc = std::allocator<T>>
class const_vector_view
{
    _CHOBO_VECTOR_VIEW_POD_CHECK(T)
    _CHOBO_VECTOR_VIEW_POD_CHECK(U)
public:
    typedef std::vector<T, Alloc> vector;

    typedef U value_type;
    typedef T vec_value_type;
    typedef Alloc allocator_type;
    typedef typename vector::size_type size_type;
    typedef typename vector::difference_type difference_type;
    typedef U& reference;
    typedef const U& const_reference;
    typedef U* pointer;
    typedef const U* const_pointer;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit const_vector_view(const vector& vec)
        : m_vector(vec)
    {}

    const_vector_view(const const_vector_view& other) = delete;
    const_vector_view& operator=(const const_vector_view& other) = delete;

    const_vector_view(const_vector_view&& other)
        : m_vector(other.m_vector)
    {} // intentionally don't inavlidate the other view

    const_vector_view& operator=(const_vector_view&& other) = delete;

    const vector& vec() const
    {
        return m_vector;
    }

    const_iterator begin() const noexcept
    {
        return reinterpret_cast<const_iterator>(m_vector.data());
    }

    const_iterator cbegin() const noexcept
    {
        return begin();
    }

    const_iterator end() const noexcept
    {
        return begin() + size();
    }

    const_iterator cend() const noexcept
    {
        return begin() + size();
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    size_type size() const noexcept
    {
        return (m_vector.size() * sizeof(vec_value_type)) / sizeof(value_type);
    }

    size_type capacity() const noexcept
    {
        return (m_vector.capacity() * sizeof(vec_value_type)) / sizeof(value_type);
    }

    size_type max_size() const noexcept
    {
        return (m_vector.max_size() * sizeof(vec_value_type)) / sizeof(value_type);
    }

    bool empty() const noexcept
    {
        return m_vector.size() * sizeof(vec_value_type) < sizeof(value_type);
    }

    const_reference at(size_type i) const
    {
        _CHOBO_VECTOR_VIEW_BOUNDS_CHECK(i);
        return *(begin() + i);
    }

    const_reference operator[](size_type i) const
    {
        return at(i);
    }

    const_reference front() const
    {
        return at(0);
    }

    const_reference back() const
    {
        return *(end() - 1);
    }

    const_pointer data() const noexcept
    {
        return begin();
    }

private:
    const vector& m_vector;
};

///////////////////////////////////////////////////////////////////////////////

template <typename U, typename T, typename Alloc>
vector_view<T, U, Alloc> make_vector_view(std::vector<T, Alloc>& vec)
{
    return vector_view<T, U, Alloc>(vec);
}

template <typename U, typename T, typename Alloc>
const_vector_view<T, U, Alloc> make_vector_view(const std::vector<T, Alloc>& vec)
{
    return const_vector_view<T, U, Alloc>(vec);
}

}


#if defined(CHOBO_VECTOR_VIEW_TEST_WITH_DOCTEST)

namespace chobo_vector_view_test
{

struct vector3D
{
    int x, y, z;
};

struct vector4D
{
    int x, y, z, w;
};

struct chobo_vector3D
{
    int a, b, c;
};

}

TEST_CASE("[vector_view] test")
{
    using namespace chobo;
    using namespace chobo_vector_view_test;

    std::vector<int> vec;
    auto v3dview = make_vector_view<vector3D>(vec);

    CHECK(v3dview.empty());
    CHECK(v3dview.size() == 0);
    vec.push_back(5);
    CHECK(v3dview.empty());
    CHECK(v3dview.size() == 0);
    vec.push_back(10);
    CHECK(v3dview.empty());
    CHECK(v3dview.size() == 0);
    vec.push_back(15);
    CHECK(!v3dview.empty());
    CHECK(v3dview.size() == 1);

    CHECK(v3dview.front().x == 5);
    CHECK(v3dview.front().y == 10);
    CHECK(v3dview.front().z == 15);

    CHECK(v3dview.back().x == 5);
    CHECK(v3dview.back().y == 10);
    CHECK(v3dview.back().z == 15);

    v3dview.resize(2);
    CHECK(vec.size() == 6);
    v3dview[1].x = 2;
    v3dview.at(1).y = 4;
    (v3dview.begin() + 1)->z = 6;
    CHECK(vec[3] == 2);
    CHECK(vec[4] == 4);
    CHECK(vec[5] == 6);
    CHECK((v3dview.rend() - 2)->z == 6);

    vector3D foo = { 1,3,5 };
    v3dview.resize(4, foo);
    CHECK(vec.size() == 12);

    CHECK(vec.back() == 5);
    CHECK(vec[6] == 1);
    CHECK(v3dview.rbegin()->y == 3);

    v3dview.resize(3);
    CHECK(vec.size() == 9);
    CHECK(v3dview.crbegin()->z == 5);

    v3dview.push_back(foo);
    CHECK(vec.size() == 12);
    CHECK(vec[9] == 1);
    CHECK(vec[10] == 3);
    CHECK(vec[11] == 5);

    v3dview.pop_back();
    CHECK(vec.size() == 9);

    v3dview.clear();
    CHECK(vec.empty());

    vec.resize(5);
    CHECK(v3dview.size() == 1);

    v3dview.resize(2);
    CHECK(vec.size() == 6);

    void* data = vec.data();
    CHECK(v3dview.data() == data);

    // same
    std::vector<chobo_vector3D> vec3d;
    auto v3dview_chobo = make_vector_view<vector3D>(vec3d);
    vec3d.resize(5);
    CHECK(v3dview_chobo.size() == 5);

    vec3d[2].a = 7;
    vec3d[2].b = 8;
    vec3d[2].c = 9;
    CHECK(v3dview_chobo[2].x == 7);
    CHECK(v3dview_chobo.at(2).y == 8);
    CHECK((v3dview_chobo.begin() + 2)->z == 9);

    // unequal
    std::vector<vector4D> vec4d;
    auto v3dview4d = make_vector_view<vector3D>(vec4d);

    vec4d.resize(1);
    CHECK(v3dview4d.size() == 1);

    v3dview4d.resize(2);
    CHECK(v3dview4d.size() == 2);
    CHECK(vec4d.size() == 2);

    // smaller
    auto iview = make_vector_view<int>(vec4d);
    CHECK(iview.size() == 8);
    iview.resize(12);
    CHECK(vec4d.size() == 3);
}

TEST_CASE("[const_vector_view] test")
{
    using namespace chobo;
    using namespace chobo_vector_view_test;

    std::vector<int> vvec;
    const auto& vec = vvec;
    auto v3dview = make_vector_view<vector3D>(vec);

    CHECK(v3dview.empty());
    CHECK(v3dview.size() == 0);
    vvec.push_back(5);
    CHECK(v3dview.empty());
    CHECK(v3dview.size() == 0);
    vvec.push_back(10);
    CHECK(v3dview.empty());
    CHECK(v3dview.size() == 0);
    vvec.push_back(15);
    CHECK(!v3dview.empty());
    CHECK(v3dview.size() == 1);

    CHECK(v3dview.front().x == 5);
    CHECK(v3dview.front().y == 10);
    CHECK(v3dview.front().z == 15);

    CHECK(v3dview.back().x == 5);
    CHECK(v3dview.back().y == 10);
    CHECK(v3dview.back().z == 15);

    vvec.resize(6);
    CHECK(v3dview.size() == 2);
    vvec[3] = 2;
    vvec[4] = 4;
    vvec[5] = 6;
    CHECK(v3dview[1].x == 2);
    CHECK(v3dview.at(1).y == 4);
    CHECK((v3dview.begin() + 1)->z == 6);
    CHECK((v3dview.rend() - 2)->z == 6);

    vvec.resize(12);
    CHECK(v3dview.size() == 4);

    vvec[10] = 3;
    CHECK(v3dview.rbegin()->y == 3);

    vvec.resize(9);
    CHECK(v3dview.size() == 3);

    vvec.clear();
    CHECK(v3dview.empty());

    vvec.resize(5);
    CHECK(v3dview.size() == 1);

    const void* data = vec.data();
    CHECK(v3dview.data() == data);

    // same
    std::vector<chobo_vector3D> vvec3d;
    const auto& vec3d = vvec3d;
    auto v3dview_chobo = make_vector_view<vector3D>(vec3d);
    vvec3d.resize(5);
    CHECK(v3dview_chobo.size() == 5);

    vvec3d[2].a = 7;
    vvec3d[2].b = 8;
    vvec3d[2].c = 9;
    CHECK(v3dview_chobo[2].x == 7);
    CHECK(v3dview_chobo.at(2).y == 8);
    CHECK((v3dview_chobo.begin() + 2)->z == 9);

    // unequal
    std::vector<vector4D> vvec4d;
    const auto& vec4d = vvec4d;
    auto v3dview4d = make_vector_view<vector3D>(vec4d);

    vvec4d.resize(1);
    CHECK(v3dview4d.size() == 1);

    vvec4d.resize(2);
    CHECK(v3dview4d.size() == 2);

    vvec4d.resize(3);
    CHECK(v3dview4d.size() == 4);

    // smaller
    auto iview = make_vector_view<int>(vec4d);
    CHECK(iview.size() == 12);
}

#endif