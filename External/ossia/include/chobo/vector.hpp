// chobo-vector v1.01
//
// An std::vector-like class with no debug bounds checks which is faster in 
// "Debug" mode
//
// MIT License:
// Copyright(c) 2017 Chobolabs Inc.
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
//  1.01 (2017-04-02) Fixed compilation error on (count, value) constructor and
//                    assign, and insert methods when count or value is 0
//  1.00 (2017-03-10) First public release
//
//
//                  DOCUMENTATION
//
// Simply include this file wherever you need.
// It defines the class chobo::vector, which is a drop-in replacement of
// std::vector. Unlike some standard library releases, most notably
// Dincumware's, it has no debug iterator checks, thus having a much, much
// better performance in "Debug" mode. 
//
//                  TESTS
//
// The tests are included in the header file and use doctest (https://github.com/onqtam/doctest).
// To run them, define CHOBO_VECTOR_TEST_WITH_DOCTEST before including
// the header in a file which has doctest.h already included.
//
#pragma once

#include <type_traits>
#include <cstddef>
#include <memory>

namespace chobo
{

template<typename T, class Alloc = std::allocator<T>>
struct vector
{
public:
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using size_type = typename Alloc::size_type;
    using difference_type = typename Alloc::difference_type;
    using reference = typename Alloc::reference;
    using const_reference = typename Alloc::const_reference;
    using pointer = typename Alloc::pointer;
    using const_pointer = typename Alloc::const_pointer;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    vector()
        : vector(Alloc())
    {}

    vector(const Alloc& alloc)
        : m_capacity(0)
        , m_alloc(alloc)
    {
        m_begin = m_end = nullptr;
    }

    explicit vector(size_t count, const Alloc& alloc = Alloc())
        : vector(alloc)
    {
        resize(count);
    }

    explicit vector(size_t count, const T& value, const Alloc& alloc = Alloc())
        : vector(alloc)
    {
        assign_impl(count, value);
    }

    template <class InputIterator, typename = decltype(*std::declval<InputIterator>())>
    vector(InputIterator first, InputIterator last, const Alloc& alloc = Alloc())
        : vector(alloc)
    {
        assign_impl(first, last);
    }

    vector(std::initializer_list<T> l, const Alloc& alloc = Alloc())
        : vector(alloc)
    {
        assign_impl(l);
    }

    vector(const vector& v)
        : vector(v, std::allocator_traits<Alloc>::select_on_container_copy_construction(v.get_allocator()))
    {}

    vector(const vector& v, const Alloc& alloc)
        : vector(alloc)
    {
        assign_impl(v.begin(), v.end());
    }

    vector(vector&& v)
        : m_capacity(v.m_capacity)
        , m_alloc(std::move(v.m_alloc))
        , m_begin(v.m_begin)
        , m_end(v.m_end)
    {
        v.m_begin = v.m_end = nullptr;
        v.m_capacity = 0;
    }

    ~vector()
    {
        clear();

        if (m_begin)
        {
            m_alloc.deallocate(m_begin, m_capacity);
        }
    }

    vector& operator=(const vector& v)
    {
        if (this == &v)
        {
            // prevent self usurp
            return *this;
        }

        clear();

        assign_impl(v.begin(), v.end());

        return *this;
    }

    vector& operator=(vector&& v)
    {
        clear();

        m_alloc = std::move(v.m_alloc);
        m_capacity = v.m_capacity;
        m_begin = v.m_begin;
        m_end = v.m_end;

        v.m_begin = v.m_end = nullptr;
        v.m_capacity = 0;

        return *this;
    }

    void assign(size_type count, const T& value)
    {
        clear();
        assign_impl(count, value);
    }

    template <class InputIterator, typename = decltype(*std::declval<InputIterator>())>
    void assign(InputIterator first, InputIterator last)
    {
        clear();
        assign_impl(first, last);
    }

    void assign(std::initializer_list<T> ilist)
    {
        clear();
        assign_impl(ilist);
    }

    allocator_type get_allocator() const
    {
        return m_alloc;
    }

    const_reference at(size_type i) const
    {
        return *(m_begin + i);
    }

    reference at(size_type i)
    {
        return *(m_begin + i);
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
        return *(m_end - 1);
    }

    reference back()
    {
        return *(m_end - 1);
    }

    const_pointer data() const noexcept
    {
        return m_begin;
    }

    pointer data() noexcept
    {
        return m_begin;
    }

    // iterators
    iterator begin() noexcept
    {
        return m_begin;
    }

    const_iterator begin() const noexcept
    {
        return m_begin;
    }

    const_iterator cbegin() const noexcept
    {
        return m_begin;
    }

    iterator end() noexcept
    {
        return m_end;
    }

    const_iterator end() const noexcept
    {
        return m_end;
    }

    const_iterator cend() const noexcept
    {
        return m_end;
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
        return m_begin == m_end;
    }

    size_t size() const noexcept
    {
        return m_end - m_begin;
    }

    size_t max_size() const noexcept
    {
        return m_alloc.max_size();
    }


    void reserve(size_t desired_capacity)
    {
        if (desired_capacity <= m_capacity)
        {
            return;
        }

        size_type new_cap = find_capacity(desired_capacity);

        auto new_buf = m_alloc.allocate(new_cap);
        const auto s = size();

        // now we need to transfer the existing elements into the new buffer
        for (size_type i = 0; i < s; ++i)
        {
            m_alloc.construct(new_buf + i, std::move(*(m_begin + i)));
        }

        if (m_begin)
        {
            // free old elements
            for (size_type i = 0; i < s; ++i)
            {
                m_alloc.destroy(m_begin + i);
            }

            m_alloc.deallocate(m_begin, m_capacity);
        }

        m_begin = new_buf;
        m_end = new_buf + s;
        m_capacity = new_cap;
    }

    size_t capacity() const noexcept
    {
        return m_capacity;
    }

    void shrink_to_fit()
    {
        const auto s = size();

        if (s == m_capacity) return;

        if (s == 0)
        {
            m_alloc.deallocate(m_begin, m_capacity);
            m_capacity = 0;
            m_begin = m_end = nullptr;
            return;
        }

        auto new_buf = m_alloc.allocate(s);

        // now we need to transfer the existing elements into the new buffer
        for (size_type i = 0; i < s; ++i)
        {
            m_alloc.construct(new_buf + i, std::move(*(m_begin + i)));
        }

        // free old elements
        for (size_type i = 0; i < s; ++i)
        {
            m_alloc.destroy(m_begin + i);
        }

        m_alloc.deallocate(m_begin, m_capacity);

        m_begin = new_buf;
        m_end = new_buf + s;
        m_capacity = s;
    }

    // modifiers
    void clear() noexcept
    {
        for (auto p = m_begin; p != m_end; ++p)
        {
            m_alloc.destroy(p);
        }

        m_end = m_begin;
    }

    iterator insert(const_iterator position, const value_type& val)
    {
        auto pos = grow_at(position, 1);
        m_alloc.construct(pos, val);
        return pos;
    }

    iterator insert(const_iterator position, value_type&& val)
    {
        auto pos = grow_at(position, 1);
        m_alloc.construct(pos, std::move(val));
        return pos;
    }

    iterator insert(const_iterator position, size_type count, const value_type& val)
    {
        auto pos = grow_at(position, count);
        for (size_type i = 0; i < count; ++i)
        {
            m_alloc.construct(pos + i, val);
        }
        return pos;
    }

    template <typename InputIterator, typename = decltype(*std::declval<InputIterator>())>
    iterator insert(const_iterator position, InputIterator first, InputIterator last)
    {
        auto pos = grow_at(position, last - first);
        size_type i = 0;
        auto np = pos;
        for (auto p = first; p != last; ++p, ++np)
        {
            m_alloc.construct(np, *p);
        }
        return pos;
    }

    iterator insert(const_iterator position, std::initializer_list<T> ilist)
    {
        auto pos = grow_at(position, ilist.size());
        size_type i = 0;
        for (auto& elem : ilist)
        {
            m_alloc.construct(pos + i, elem);
            ++i;
        }
        return pos;
    }

    template<typename... Args>
    iterator emplace(const_iterator position, Args&&... args)
    {
        auto pos = grow_at(position, 1);
        m_alloc.construct(pos, std::forward<Args>(args)...);
        return pos;
    }

    iterator erase(const_iterator position)
    {
        return shrink_at(position, 1);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        return shrink_at(first, last - first);
    }

    void push_back(const_reference val)
    {
        auto pos = grow_at(m_end, 1);
        m_alloc.construct(pos, val);
    }

    void push_back(T&& val)
    {
        auto pos = grow_at(m_end, 1);
        m_alloc.construct(pos, std::move(val));
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        auto pos = grow_at(m_end, 1);
        m_alloc.construct(pos, std::forward<Args>(args)...);
    }

    void pop_back()
    {
        shrink_at(m_end - 1, 1);
    }

    void resize(size_type n, const value_type& v)
    {
        if (n <= m_capacity)
        {
            const auto new_end = m_begin + n;

            while (m_end > new_end)
            {
                m_alloc.destroy(--m_end);
            }

            while (new_end > m_end)
            {
                m_alloc.construct(m_end++, v);
            }
        }
        else
        {
            reserve(n);

            const auto new_end = m_begin + n;

            while (new_end > m_end)
            {
                m_alloc.construct(m_end++, v);
            }
        }
    }

    void resize(size_type n)
    {
        if (n <= m_capacity)
        {
            const auto new_end = m_begin + n;

            while (m_end > new_end)
            {
                m_alloc.destroy(--m_end);
            }

            while (new_end > m_end)
            {
                m_alloc.construct(m_end++);
            }
        }
        else
        {
            reserve(n);

            const auto new_end = m_begin + n;

            while (new_end > m_end)
            {
                m_alloc.construct(m_end++);
            }
        }
    }

private:

    size_type find_capacity(size_type desired_capacity) const
    {
        if (m_capacity == 0)
        {
            return desired_capacity;
        }
        else
        {
            auto new_cap = m_capacity;

            while (new_cap < desired_capacity)
            {
                // grow by roughly 1.5
                new_cap *= 3;
                ++new_cap;
                new_cap /= 2;
            }

            return new_cap;
        }
    }

    // increase the size by splicing the elements in such a way that
    // a hole of uninitialized elements is left at position, with size num
    // returns the (potentially new) address of the hole
    T* grow_at(const T* cp, size_t num)
    {
        auto position = const_cast<T*>(cp);

        const auto s = size();

        if (s + num <= m_capacity)
        {
            m_end = m_begin + s + num;

            for (auto p = m_end - num - 1; p >= position; --p)
            {
                m_alloc.construct(p + num, std::move(*p));
                m_alloc.destroy(p);
            }

            return position;
        }
        else
        {
            // we need to transfer the elements into the a buffer
            size_type new_cap = find_capacity(s + num);

            auto new_buf = m_alloc.allocate(new_cap);

            position = new_buf + (position - m_begin);

            auto p = m_begin;
            auto np = new_buf;

            for (; np != position; ++p, ++np)
            {
                m_alloc.construct(np, std::move(*p));
            }

            np += num;
            for (; p != m_end; ++p, ++np)
            {
                m_alloc.construct(np, std::move(*p));
            }

            // destroy old
            if (m_begin)
            {
                for (p = m_begin; p != m_end; ++p)
                {
                    m_alloc.destroy(p);
                }

                m_alloc.deallocate(m_begin, m_capacity);
            }
            
            m_capacity = new_cap;
            m_begin = new_buf;
            m_end = new_buf + s + num;

            return position;
        }
    }

    T* shrink_at(const T* cp, size_t num)
    {
        auto position = const_cast<T*>(cp);

        const auto s = size();
        if (s - num == 0)
        {
            clear();
            return m_end;
        }

        for (auto p = position, np = position + num; np != m_end; ++p, ++np)
        {
            m_alloc.destroy(p);
            m_alloc.construct(p, std::move(*np));
        }

        for (auto p = m_end - num; p != m_end; ++p)
        {
            m_alloc.destroy(p);
        }

        m_end -= num; 

        return ++position;
    }

    // grows buffer only on empty vectors
    void safe_grow(size_t capacity)
    {
        assert(m_begin == m_end);

        if (capacity <= m_capacity)
            return;

        if (m_begin)
        {
            m_alloc.deallocate(m_begin, m_capacity);
        }

        m_capacity = find_capacity(capacity);
        m_begin = m_end = m_alloc.allocate(m_capacity);
    }

    void assign_impl(size_type count, const T& value)
    {
        safe_grow(count);

        for (size_type i = 0; i < count; ++i)
        {
            m_alloc.construct(m_end, value);
            ++m_end;
        }
    }

    template <class InputIterator>
    void assign_impl(InputIterator first, InputIterator last)
    {
        auto isize = last - first;
        safe_grow(isize);

        for (auto p = first; p != last; ++p)
        {
            m_alloc.construct(m_end, *p);
            ++m_end;
        }
    }

    void assign_impl(std::initializer_list<T> ilist)
    {
        safe_grow(ilist.size());

        for (auto& elem : ilist)
        {
            m_alloc.construct(m_end, elem);
            ++m_end;
        }
    }
    
    pointer m_begin;
    pointer m_end;

    size_t m_capacity;

    Alloc m_alloc;
};

template<typename T, class Alloc>
bool operator==(const vector<T, Alloc>& a, const vector<T, Alloc>& b)
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

template<typename T, class Alloc>
bool operator!=(const vector<T, Alloc>& a, const vector<T, Alloc>& b)
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


#if defined(CHOBO_VECTOR_TEST_WITH_DOCTEST)

#include <string>
#include <utility>

namespace chobo_vector_test
{

size_t allocations = 0;
size_t deallocations = 0;
size_t allocated_bytes = 0;
size_t deallocated_bytes = 0;
size_t constructions = 0;
size_t destructions = 0;

template <typename T>
class counting_allocator : public std::allocator<T>
{
public:
    typedef std::allocator<T> super;

    T* allocate(size_t n, std::allocator<void>::const_pointer hint = 0)
    {
        ++allocations;
        allocated_bytes += n * sizeof(T);
        return super::allocate(n, hint);
    }

    void deallocate(T* p, size_t n)
    {
        ++deallocations;
        deallocated_bytes += n * sizeof(T);
        return super::deallocate(p, n);
    }

    template< class U, class... Args >
    void construct(U* p, Args&&... args)
    {
        ++constructions;
        return super::construct(p, std::forward<Args>(args)...);
    }

    template< class U >
    void destroy(U* p)
    {
        ++destructions;
        return super::destroy(p);
    }
};
}

TEST_CASE("[vector] test")
{
    using namespace chobo;
    using namespace chobo_vector_test;
    {
        vector<int, counting_allocator<int>> ivec;
        CHECK(ivec.size() == 0);
        CHECK(ivec.capacity() == 0);
        CHECK(!ivec.begin());
        CHECK(ivec.begin() == ivec.end());
        CHECK(!ivec.cbegin());
        CHECK(ivec.cbegin() == ivec.cend());
        CHECK(ivec.empty());

        auto d = ivec.data();
        ivec.reserve(2);
        CHECK(ivec.capacity() == 2);
        CHECK(d != ivec.data());
        CHECK(allocations == 1);

        ivec.resize(3, 8);
        CHECK(ivec.capacity() == 3);
        CHECK(ivec.size() == 3);
        CHECK(ivec.front() == 8);
        CHECK(ivec.back() == 8);
        CHECK(d != ivec.data());
        CHECK(allocations == 2);

        ivec.clear();
        CHECK(ivec.size() == 0);
        CHECK(ivec.capacity() == 3);
        CHECK(d != ivec.data());
        CHECK(ivec.begin() == ivec.end());
        CHECK(ivec.cbegin() == ivec.cend());
        CHECK(ivec.empty());

        ivec.shrink_to_fit();
        CHECK(ivec.size() == 0);
        CHECK(ivec.capacity() == 0);
        CHECK(!ivec.begin());
        CHECK(ivec.begin() == ivec.end());
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
        CHECK(ivec.capacity() == 3);

        ivec.insert(ivec.begin() + 2, 90);
        ivec.insert(ivec.begin() + 4, 17);
        ivec.insert(ivec.end(), 6);
        ivec.insert(ivec.begin(), { 1, 2 });

        int ints[] = { 1, 2, 53, 12, 90, 3, 17, 6 };
        CHECK(ivec.capacity() >= 8);
        CHECK(ivec.size() == 8);
        CHECK(memcmp(ivec.data(), ints, sizeof(ints)) == 0);

        ivec.pop_back();
        CHECK(ivec.size() == 7);
        CHECK(memcmp(ivec.data(), ints, sizeof(ints) - sizeof(int)) == 0);

        CHECK(ivec.capacity() > 7);
        ivec.shrink_to_fit();
        CHECK(ivec.capacity() == 7);

        ivec.resize(8);
        CHECK(ivec.size() == 8);
        ints[7] = 0;
        CHECK(memcmp(ivec.data(), ints, sizeof(ints)) == 0);

        const vector<int, counting_allocator<int>> ivec2 = { 1, 2, 3, 4 };
        CHECK(ivec2.size() == 4);
        CHECK(*ivec2.begin() == 1);
        CHECK(ivec2[1] == 2);
        CHECK(ivec2.at(2) == 3);
        CHECK(*ivec2.rbegin() == 4);

        ivec.erase(ivec.begin());
        CHECK(ivec.size() == 7);
        CHECK(ivec.front() == 2);
        CHECK(memcmp(ivec.data(), ints + 1, ivec.size() * sizeof(int)) == 0);

        ivec.erase(ivec.begin() + 2, ivec.begin() + 4);
        CHECK(ivec.size() == 5);
        CHECK(ivec[3] == 17);

        vector<std::string, counting_allocator<std::string>> svec;
        svec.assign({ "as", "df" });
        CHECK(svec.size() == 2);
        std::string s1 = "the quick brown fox jumped over the lazy dog 1234567890";
        svec.emplace_back(s1);
        CHECK(svec.back() == s1);

        auto svec1 = svec;
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

        s1 = "asdf";
        vector<char, counting_allocator<char>> cvec(s1.begin(), s1.end());
        CHECK(cvec.size() == 4);
        CHECK(cvec.front() == 'a');
        CHECK(cvec.back() == 'f');

        cvec.clear();
        CHECK(cvec.size() == 0);
        CHECK(cvec.empty());

        s1 = "baz";
        cvec.assign(s1.begin(), s1.end());
        CHECK(cvec.size() == 3);
        CHECK(cvec.front() == 'b');
        CHECK(cvec.back() == 'z');

        // 0 is implicitly castable to nullptr_t which can be an iterator in our case
        vector<int> nullptr_test(2, 0);
        CHECK(nullptr_test.size() == 2);
        CHECK(nullptr_test.front() == 0);
        CHECK(nullptr_test.back() == 0);

        nullptr_test.assign(3, 0);
        CHECK(nullptr_test.size() == 3);
        CHECK(nullptr_test.front() == 0);
        CHECK(nullptr_test.back() == 0);
        
        nullptr_test.insert(nullptr_test.begin(), 1, 0);
        CHECK(nullptr_test.size() == 4);
        CHECK(nullptr_test.front() == 0);
    }

    CHECK(allocations == deallocations);
    CHECK(allocated_bytes == deallocated_bytes);
    CHECK(constructions == destructions);

    allocations = deallocations = allocated_bytes = deallocated_bytes = constructions = destructions = 0;
}

#endif
