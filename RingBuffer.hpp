#include "RingBuffer.h"
#include <stdexcept>

#define RB_IMP RingBuffer<T, Alloc>

template<class T, class Alloc>
RingBuffer<T, Alloc>::RingBuffer(size_type capacity, const Alloc &alloc)
    : m_capacity(capacity)
    , m_allocator(alloc)
    , m_size(0)
{
    m_data = m_allocator.allocate(m_capacity);
    m_start = m_data;
    m_end = m_data;
}

template<class T, class Alloc>
typename RB_IMP::reference RingBuffer<T, Alloc>::front()
{
    if (empty())
        throw std::range_error("ring buffer is empty");
    
    return *m_start;
}

template<class T, class Alloc>
typename RB_IMP::const_reference RingBuffer<T, Alloc>::front() const
{
    if (empty())
        throw std::range_error("ring buffer is empty");
    
    return *m_start;
}

template<class T, class Alloc>
void RingBuffer<T, Alloc>::swap(RingBuffer& other)
{
    std::swap(m_data, other.m_data);
    std::swap(m_start, other.m_start);
    std::swap(m_end, other.m_end);
    std::swap(m_capacity, other.m_capacity);
    std::swap(m_size, other.m_size);
}

template<class T, class Alloc>
typename RB_IMP::size_type RingBuffer<T, Alloc>::size() const
{
    return m_size;
}

template<class T, class Alloc>
typename RB_IMP::size_type RingBuffer<T, Alloc>::capacity() const
{
    return m_capacity;
}

template<class T, class Alloc>
bool RingBuffer<T, Alloc>::empty() const
{
    return m_size == 0;
}

// swap
template <class T, class Alloc>
void swap(RingBuffer<T, Alloc>& left, RingBuffer<T, Alloc>& right)
{
    left.swap(right);
}