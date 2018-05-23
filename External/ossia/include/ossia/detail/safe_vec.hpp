#pragma once
#include <ossia/detail/algorithms.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <hopscotch_set.h>
#include <vector>

namespace ossia
{
template <typename T>
class safe_vector
{
public:
  safe_vector()
  {
    static_assert(sizeof(T) <= 64, "use this only for small types");
  }

  safe_vector(const safe_vector&) = delete;
  safe_vector(safe_vector&&) = delete;
  safe_vector& operator=(const safe_vector&) = delete;
  safe_vector& operator=(safe_vector&&) = delete;

  bool contains(T t)
  {
    return ossia::contains(impl, t);
  }

  void clear()
  {
    impl.clear();
  }

  void push_back(T t)
  {
    impl.push_back(t);
  }

  void remove_all(T t)
  {
    boost::remove_erase(impl, t);
  }

  void reserve(std::size_t N)
  {
    impl.reserve(N);
  }

  auto copy() const
  {
    return impl;
  }

  auto& reference()
  {
    return impl;
  }

  auto& reference() const
  {
    return impl;
  }

private:
  std::vector<T> impl;
};

template <typename T>
class safe_set
{
public:
  safe_set()
  {
    static_assert(sizeof(T) <= 64, "use this only for small types");
  }

  safe_set(const safe_set&) = delete;
  safe_set(safe_set&&) = delete;
  safe_set& operator=(const safe_set&) = delete;
  safe_set& operator=(safe_set&&) = delete;

  bool contains(T t)
  {
    return ossia::contains(impl, t);
  }

  void clear()
  {
    impl.clear();
  }

  void push_back(T t)
  {
    impl.insert(t);
  }

  void remove_all(T t)
  {
    impl.erase(t);
  }

  void reserve(std::size_t N)
  {
    impl.reserve(N);
  }

  auto copy() const
  {
    return impl;
  }

  auto& reference()
  {
    return impl;
  }

  auto& reference() const
  {
    return impl;
  }

  auto size() const
  {
    return impl.size();
  }

  // https://stackoverflow.com/q/20953390/1495627
  template<typename Tval>
  struct EgurHash {
      size_t operator()(const Tval val) const {
          static const size_t shift = (size_t)log2(1 + sizeof(Tval));
          return (size_t)(val) >> shift;
      }
  };
private:
  tsl::hopscotch_set<T, EgurHash<T>> impl;
};
}
