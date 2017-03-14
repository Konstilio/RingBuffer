//
//  RingBuffer.h
//  RingBuffer
//
//  Created by Aleksander Konstantinov on 3/12/17.
//  Copyright © 2017 Aleksander Konstantinov. All rights reserved.
//

#ifndef RingBuffer_h
#define RingBuffer_h

#include <memory>

template<class T, class Alloc = std::allocator<T>>
class RingBuffer;

template <class T, class Alloc = std::allocator<T> >
void swap(RingBuffer<T,Alloc>&, RingBuffer<T,Alloc>&) noexcept;

template<class T, class Alloc>
class RingBuffer
{
public:
    typedef Alloc allocator_type;
    typedef typename Alloc::value_type value_type;
    typedef typename Alloc::reference reference;
    typedef typename Alloc::const_reference const_reference;
    typedef typename Alloc::difference_type difference_type;
    typedef typename Alloc::size_type size_type;
    
    RingBuffer(size_type capacity, const Alloc &alloc = Alloc());
    
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    reference operator[](size_type);
    const_reference operator[](size_type) const;
    
    void push_back(const T&);
    void push_back(T&&);
    template<class ...Args>
    void emplace_back(Args&&...);
    
    void swap(RingBuffer& other) noexcept;
    size_type size() const;
    size_type capacity() const;
    bool empty() const;
    
private:
    template<class... Args>
    void push_back_imp(Args&&... args);
    
    T* m_data;

    size_type m_start;
    size_type m_capacity;
    size_type m_size;
    Alloc m_allocator;
    
};



#include "RingBuffer.hpp"

#endif /* RingBuffer_h */
