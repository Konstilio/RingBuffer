#include "RingBuffer.h"

template<class T, class Alloc>
RingBuffer<T, Alloc>::RingBuffer(size_type capacity, const Alloc &alloc)
    : m_capacity(capacity)
    , m_allocator(alloc)
{
    
}