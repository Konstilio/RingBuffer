#include "RingBuffer.h"

template<class T, class Alloc, bool copy>
struct rb_help_push_back_full_imp
{
    void operator()(RingBuffer<T, Alloc> &buffer, const T& value)
    {
        buffer.m_data[buffer.m_start] = value;
        buffer.m_start = (buffer.m_start + 1) % buffer.m_capacity;
    }
};

template<class T, class Alloc>
struct rb_help_push_back_full_imp<T, Alloc, false>
{
    void operator()(RingBuffer<T, Alloc> &buffer, const T& value)
    {
        std::allocator_traits<Alloc>::destroy
        (
         buffer.m_allocator
         , buffer.m_data + buffer.m_start
         );
        --buffer.m_size;
        
        std::allocator_traits<Alloc>::construct
        (
         buffer.m_allocator
         , buffer.m_data + buffer.m_start
         , value
        //, std::forward<Args>(args)...
        );
        ++buffer.m_size;
        
        buffer.m_start = (buffer.m_start + 1) % buffer.m_capacity;
    }
};