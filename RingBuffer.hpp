#include "RingBuffer.h"
#include <stdexcept>

#define RB_IMP RingBuffer<T, Alloc>

template<class T, class Alloc>
RingBuffer<T, Alloc>::RingBuffer(size_type capacity, const Alloc &alloc)
    : m_capacity(capacity)
    , m_allocator(alloc)
    , m_size(0)
    , m_start(0)
{
    m_data = m_allocator.allocate(m_capacity);
}

template<class T, class Alloc>
typename RB_IMP::reference RingBuffer<T, Alloc>::front()
{
    if (empty())
        throw std::range_error("ring buffer is empty");
    
    return m_data[m_start];
}

template<class T, class Alloc>
typename RB_IMP::const_reference RingBuffer<T, Alloc>::front() const
{
    if (empty())
        throw std::range_error("ring buffer is empty");
    
    return m_data[m_start];
}

template<class T, class Alloc>
typename RB_IMP::reference RingBuffer<T, Alloc>::back()
{
    if (empty())
        throw std::range_error("ring buffer is empty");
    
    auto end_pos = (m_start + m_size - 1) % m_capacity;
    return m_data[end_pos];
}

template<class T, class Alloc>
typename RB_IMP::const_reference RingBuffer<T, Alloc>::back() const
{
    if (empty())
        throw std::range_error("ring buffer is empty");
    
    auto end_pos = (m_start + m_size - 1) % m_capacity;
    return m_data[end_pos];
}

template<class T, class Alloc>
typename RB_IMP::reference RingBuffer<T, Alloc>::operator[](size_type pos)
{
    pos = (m_start + pos - 1) % m_capacity;
    return m_data[pos];
}

template<class T, class Alloc>
typename RB_IMP::const_reference RingBuffer<T, Alloc>::operator[](size_type pos) const
{
    pos = (m_start + pos - 1) % m_capacity;
    return m_data[pos];
}

template<class T, class Alloc>
void RingBuffer<T, Alloc>::push_back(const T& value)
{
    push_back_imp(value);
}

template<class T, class Alloc>
void RingBuffer<T, Alloc>::push_back(T&& value)
{
    push_back_imp(std::move(value));
}

template<class T, class Alloc>
template<class... Args>
void RingBuffer<T, Alloc>::emplace_back(Args&&... args)
{
    push_back_imp(std::forward<Args>(args)...);
}

template<class T, class Alloc>
void RingBuffer<T, Alloc>::swap(RingBuffer& other) noexcept
{
    std::swap(m_data, other.m_data);
    std::swap(m_start, other.m_start);
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

template<class T, class Alloc>
template<class... Args>
void RingBuffer<T, Alloc>::push_back_imp(Args&&... args)
{
    if (m_size == m_capacity)
    {
        std::allocator_traits<Alloc>::construct
        (
            m_allocator
            , m_data + m_start
            , std::forward<Args>(args)...
        );
        m_start = (m_start + 1) % m_capacity;
    }
    else
    {
        std::allocator_traits<Alloc>::construct
        (
            m_allocator
            , m_data + ((m_start + m_size) % m_capacity)
            , std::forward<Args>(args)...
        );
        ++m_size;
    }
}

// swap
template <class T, class Alloc>
void swap(RingBuffer<T, Alloc>& left, RingBuffer<T, Alloc>& right) noexcept
{
    left.swap(right);
}