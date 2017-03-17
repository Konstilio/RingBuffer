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
    m_data = std::allocator_traits<Alloc>::allocate
    (
        m_allocator
        , m_capacity
    );
}

template<class T, class Alloc>
RingBuffer<T, Alloc>::RingBuffer(const RingBuffer &other)
    : m_capacity(other.m_capacity)
    , m_allocator(std::allocator_traits<Alloc>::select_on_container_copy_construction(other.m_allocator))
    , m_size(0)
    , m_start(other.m_start)
{
    m_data = std::allocator_traits<Alloc>::allocate
    (
        m_allocator
        , m_capacity
    );
    
    for (size_type pos = 0; pos < other.m_size; ++pos)
        push_back_imp(other[pos]);
    
}

template<class T, class Alloc>
RingBuffer<T, Alloc>::RingBuffer(RingBuffer &&other)
    : m_capacity(other.m_capacity)
    , m_allocator(std::move(other.m_allocator))
    , m_size(other.m_size)
    , m_start(other.m_start)
    , m_data(other.m_data)

{
    other.m_data = nullptr;
    other.m_start = 0;
    other.m_capacity = 0;
    other.m_size = 0;
}


template<class T, class Alloc>
RingBuffer<T, Alloc>::~RingBuffer()
{
    for (size_type pos = 0; pos < m_size; ++pos)
    {
        auto real_pos = (m_start + pos) % m_capacity;
        std::allocator_traits<Alloc>::destroy
        (
            m_allocator
            , m_data + real_pos
        );
    }
    
    std::allocator_traits<Alloc>::deallocate
    (
        m_allocator
        , m_data
        , m_capacity
    );
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
    pos = (m_start + pos) % m_capacity;
    return m_data[pos];
}

template<class T, class Alloc>
typename RB_IMP::const_reference RingBuffer<T, Alloc>::operator[](size_type pos) const
{
    pos = (m_start + pos) % m_capacity;
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
void RingBuffer<T, Alloc>::pop_front()
{
    if (empty())
        throw std::range_error("ring buffer is empty");
    
    std::allocator_traits<Alloc>::destroy
    (
        m_allocator
        , m_data + m_start
    );
    m_start = (m_start + 1) % m_capacity;
    --m_size;
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
    // TODO make exception contract
    if (m_size == m_capacity)
    {
        std::allocator_traits<Alloc>::destroy
        (
            m_allocator
            , m_data + m_start
        );
        
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