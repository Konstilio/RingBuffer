#include "RingBuffer.h"
#include <stdexcept>
#include <cassert>

#define RB_IMP RingBuffer<T, Alloc>
#define RB_IMP_IT typename RingBuffer<T, Alloc>::iterator
#define RB_IMP_CIT typename RingBuffer<T, Alloc>::const_iterator

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
        push_back(other[pos]);
    
}

template<class T, class Alloc>
RB_IMP &RingBuffer<T, Alloc>::operator=(const RingBuffer &other)
{
    auto temp = other;
    swap(temp);
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
RB_IMP &RingBuffer<T, Alloc>::operator=(RingBuffer &&other)
{
    auto temp = std::move(other);
    swap(temp);
}


template<class T, class Alloc>
RingBuffer<T, Alloc>::~RingBuffer()
{
    clear();
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
void RingBuffer<T, Alloc>::reallocate(size_type capacity)
{
    RingBuffer temp(capacity);
    swap(temp);
}

template<class T, class Alloc>
void RingBuffer<T, Alloc>::clear()
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
    
    m_size = 0;
}

template<class T, class Alloc>
void RingBuffer<T, Alloc>::push_back(const T& value)
{
    if (m_size < m_capacity)
        push_back_non_full_imp(value);
    else
        push_back_full_imp(value);
}

template<class T, class Alloc>
void RingBuffer<T, Alloc>::push_back(T&& value)
{
    if (m_size < m_capacity)
        push_back_non_full_imp(value);
}

template<class T, class Alloc>
template<class... Args>
void RingBuffer<T, Alloc>::emplace_back(Args&&... args)
{
    if (m_size < m_capacity)
        push_back_non_full_imp(std::forward<Args>(args)...);
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
RB_IMP_IT RingBuffer<T, Alloc>::begin()
{
    return iterator(m_data, m_start, m_capacity, 0);
}

template<class T, class Alloc>
RB_IMP_CIT RingBuffer<T, Alloc>::begin() const
{
    return const_iterator(m_data, m_start, m_capacity, 0);
}

template<class T, class Alloc>
RB_IMP_CIT RingBuffer<T, Alloc>::cbegin() const
{
    return begin();
}

template<class T, class Alloc>
RB_IMP_IT RingBuffer<T, Alloc>::end()
{
    return iterator(m_data, m_start, m_capacity, m_size);
}

template<class T, class Alloc>
RB_IMP_CIT RingBuffer<T, Alloc>::end() const
{
    return const_iterator(m_data, m_start, m_capacity, m_size);
}

template<class T, class Alloc>
RB_IMP_CIT RingBuffer<T, Alloc>::cend() const
{
    return end();
}

template<class T, class Alloc>
template<class... Args>
void RingBuffer<T, Alloc>::push_back_non_full_imp(Args&&... args)
{
    assert(m_size < m_capacity);
    std::allocator_traits<Alloc>::construct
    (
        m_allocator
        , m_data + ((m_start + m_size) % m_capacity)
        , std::forward<Args>(args)...
    );
    ++m_size;
}

template<class T, class Alloc>
template<class... Args>
void RingBuffer<T, Alloc>::push_back_full_construct_destruct_imp(Args&&... args)
{
    assert(m_size < m_capacity);
    std::allocator_traits<Alloc>::destroy
    (
        m_allocator
        , m_data + m_start
    );
    --m_size;
    
    std::allocator_traits<Alloc>::construct
    (
        m_allocator
        , m_data + m_start
        , std::forward<Args>(args)...
    );
    ++m_size;
    
    m_start = (m_start + 1) % m_capacity;  ++m_size;
}

template<class T, class Alloc>
template<bool copy_assignable>
void RingBuffer<T, Alloc>::push_back_full_imp(const T& value)
{
    if (copy_assignable)
    {
        m_data[m_start] = value;
        m_start = (m_start + 1) % m_capacity;
    }
    else
       push_back_full_construct_destruct_imp(value);
}

// swap
template <class T, class Alloc>
void swap(RingBuffer<T, Alloc>& left, RingBuffer<T, Alloc>& right) noexcept
{
    left.swap(right);
}

// relations operators
template<class T, class Alloc>
bool operator==(const RingBuffer<T, Alloc>& left, const RingBuffer<T, Alloc>& right)
{
    if (left.m_size != right.m_size)
        return false;
    
    for (typename RingBuffer<T, Alloc>::size_type pos = 0; pos < left.m_size; ++pos)
    {
        auto left_pos = (left.m_start + pos) % left.m_capacity;
        auto right_pos = (right.m_start + pos) % right.m_capacity;
        if (!(left.m_data[left_pos] == right.m_data[right_pos]))
            return false;
    }
    
    return true;
}

template<class T, class Alloc>
bool operator!=(const RingBuffer<T, Alloc>& left, const RingBuffer<T, Alloc>& right)
{
    return !(left == right);
}

#undef RB_IMP