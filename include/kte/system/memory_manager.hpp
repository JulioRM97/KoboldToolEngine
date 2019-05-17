#pragma once

#include <cstdint>
#include <deque>
#include <vector>

#include "memory_region.hpp"

#define MEMORY_REGION_SIZE 100

namespace kte
{

////////////////////////////////////////////////////////////////
template<typename T, typename R, typename D>
class MemoryManagerIterator : public std::iterator<std::input_iterator_tag, T>
{
 public:
  MemoryManagerIterator(D& memory_regions,
                        MemoryRegionIterator<T, R> element_iterator)
      : memory_region_(memory_regions.begin()),
        memory_region_end_(memory_regions.end()),
        element_iterator_(element_iterator) {}

  MemoryManagerIterator operator ++();

  bool operator !=(const MemoryManagerIterator<T, R, D>& other) const;

  T& operator *() const;

 private:
  decltype(std::declval<D>().begin()) memory_region_;

  const decltype(std::declval<D>().end()) memory_region_end_;

  MemoryRegionIterator<T, R> element_iterator_;
};

////////////////////////////////////////////////////////////////
template<typename T, typename R, typename D>
MemoryManagerIterator<T, R, D> MemoryManagerIterator<T, R, D>::operator ++()
{
  element_iterator_++;

  if (element_iterator_ == (*memory_region_).end())
  {
    memory_region_++;
    if (memory_region_ != memory_region_end_)
    {
      element_iterator_ = (*memory_region_).begin();
    }
  }

  return *this;
}

////////////////////////////////////////////////////////////////
template<typename T, typename R, typename D>
bool MemoryManagerIterator<T, R, D>::operator !=(const MemoryManagerIterator<T, R, D>& other) const
{
  return element_iterator_ != other.element_iterator_;
}

////////////////////////////////////////////////////////////////
template<typename T, typename R, typename D>
T& MemoryManagerIterator<T, R, D>::operator *() const
{
  return *element_iterator_;
}

////////////////////////////////////////////////////////////////
template<typename T, typename R = MemoryRegion<T>>
class MemoryManager
{
  static_assert(std::is_base_of<MemoryRegion<T>, R>::value,
                "R must derive from BaseMemoryRegion");

 public:
  typedef MemoryManagerIterator<T, R, std::deque<R>> iterator;

  typedef MemoryManagerIterator<const T, R, const std::deque<R>> const_iterator;

  T& emplace() { emplace(nullptr); }

  template<typename... Args>
  T& emplace(const T* parent, Args&& ... args);

  void remove(T* element);

  iterator begin()
  {
    return iterator(memory_regions_, memory_regions_.front().begin());
  }

  iterator end()
  {
    return iterator(memory_regions_, memory_regions_.back().end());
  }

  const_iterator begin() const
  {
    return const_iterator(memory_regions_, memory_regions_.front().begin());
  }

  const_iterator end() const
  {
    return const_iterator(memory_regions_, memory_regions_.back().end());
  }

 private:
  R* regionFor(const T* element) const;

  std::pair<R*, size_t> nextFree(const T* parent = nullptr);

  std::deque<R> memory_regions_;
};

////////////////////////////////////////////////////////////////
template<typename T, typename R>
template<typename... Args>
T& MemoryManager<T, R>::emplace(const T* parent, Args&& ... args)
{
  auto pair = nextFree(parent);
  auto& region = *pair.first;
  auto idx = pair.second;
  region.used[idx] = true;
  region.parents[idx] = parent;
  return *new(region.elements + idx) T(std::forward<Args>(args)...);
}

////////////////////////////////////////////////////////////////
template<typename T, typename R>
void MemoryManager<T, R>::remove(T* element)
{
  // TODO check for null
  auto& region = *regionFor(element);

  // TODO remove also children!
  for (size_t idx = 0; idx < MEMORY_REGION_SIZE; idx++)
  {
    if (!region.used[idx])
    {
      region.used[idx] = false;
      region.parents[idx] = nullptr;
      region.elements[idx]->~T();
    }
  }
}

////////////////////////////////////////////////////////////////
template<typename T, typename R>
R* MemoryManager<T, R>::regionFor(const T* element) const
{
  for (auto& region : memory_regions_)
  {
    ptrdiff_t diff = element - region.elements;
    if (diff > 0 && diff < MEMORY_REGION_SIZE)
    {
      return region;
    }
  }

  return nullptr;
}

////////////////////////////////////////////////////////////////
template<typename T, typename R>
std::pair<R*, size_t> MemoryManager<T, R>::nextFree(const T* parent)
{
  bool memory_region_found = !parent;
  size_t idx = 0;

  for (auto& region : memory_regions_)
  {
    if (!memory_region_found)
    {
      ptrdiff_t diff = parent - region.elements;
      if (diff >= 0 && diff < MEMORY_REGION_SIZE)
      {
        memory_region_found = true;
        idx = static_cast<size_t>(diff) + 1;
      }
    }

    if (memory_region_found)
    {
      for (; idx < MEMORY_REGION_SIZE; idx++)
      {
        if (!region.used[idx])
        {
          return std::make_pair(&region, idx);
        }
      }
    }
  }

  memory_regions_.emplace_back();
  idx = 0;

  return std::make_pair(&memory_regions_.back(), idx);
}

} // end namespace kte

