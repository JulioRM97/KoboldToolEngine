#pragma once

#include <vector>

#define MEMORY_REGION_SIZE 100

namespace kte
{

////////////////////////////////////////////////////////////////
template<typename T, typename R>
class MemoryRegionIterator : public std::iterator<std::input_iterator_tag, T>
{
 public:
  MemoryRegionIterator(const R* memory_region, size_t idx)
      : memory_region_(memory_region),
        idx_(idx) {}

  MemoryRegionIterator& operator++();

  MemoryRegionIterator operator++(int);

  bool operator ==(const MemoryRegionIterator<T, R>& other) const;

  bool operator !=(const MemoryRegionIterator<T, R>& other) const;

  T& operator *() const;

 private:
  const R* memory_region_;

  size_t idx_;
};

////////////////////////////////////////////////////////////////
template<typename T, typename R>
MemoryRegionIterator<T, R>& MemoryRegionIterator<T, R>::operator++()
{
  for (idx_++; idx_ < MEMORY_REGION_SIZE; idx_++)
  {
    if (memory_region_->used[idx_])
    {
      return *this;
    }
  }

  idx_ = MEMORY_REGION_SIZE;
  return *this;
}

////////////////////////////////////////////////////////////////
template<typename T, typename R>
MemoryRegionIterator<T, R> MemoryRegionIterator<T, R>::operator++(int)
{
  auto retval = *this;
  ++(*this);
  return retval;
}

////////////////////////////////////////////////////////////////
template<typename T, typename R>
bool MemoryRegionIterator<T, R>::operator ==(const MemoryRegionIterator<T, R>& other) const
{
  return memory_region_ == other.memory_region_ && idx_ == other.idx_;
}

////////////////////////////////////////////////////////////////
template<typename T, typename R>
bool MemoryRegionIterator<T, R>::operator !=(const MemoryRegionIterator<T, R>& other) const
{
  return memory_region_ != other.memory_region_ || idx_ != other.idx_;
}

////////////////////////////////////////////////////////////////
template<typename T, typename R>
T& MemoryRegionIterator<T, R>::operator *() const
{
  return memory_region_->elements[idx_];
}

////////////////////////////////////////////////////////////////
template<typename T>
class MemoryRegion
{
 public:
  typedef MemoryRegionIterator<T, MemoryRegion<T>> iterator;

  typedef MemoryRegionIterator<const T, MemoryRegion<T>> const_iterator;

  MemoryRegion();

  ~MemoryRegion();

  iterator begin() { return iterator(this, 0); }

  iterator end() { return iterator(this, MEMORY_REGION_SIZE); }

  const_iterator begin() const { return const_iterator(this, 0); }

  const_iterator end() const { return const_iterator(this, MEMORY_REGION_SIZE); }

  T* elements = nullptr;

  std::vector<const T*> parents;

  std::vector<bool> used;

 private:
  std::allocator<T> alloc_;
};

////////////////////////////////////////////////////////////////
template<typename T>
MemoryRegion<T>::MemoryRegion()
{
  elements = alloc_.allocate(sizeof(T) * MEMORY_REGION_SIZE);
  parents.resize(MEMORY_REGION_SIZE);
  used.resize(MEMORY_REGION_SIZE);
}

////////////////////////////////////////////////////////////////
template<typename T>
MemoryRegion<T>::~MemoryRegion()
{
  if (elements)
  {
    alloc_.deallocate(elements, sizeof(T) * MEMORY_REGION_SIZE);
    elements = nullptr;
  }
}

} // end namespace kte

