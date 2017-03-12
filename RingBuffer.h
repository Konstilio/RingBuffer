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
    
private:
    T* m_data;
    size_type m_capacity;
    Alloc m_allocator;
    
};

#include "RingBuffer.hpp"

#endif /* RingBuffer_h */
