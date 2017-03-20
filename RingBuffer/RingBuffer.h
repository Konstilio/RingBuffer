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

template <class T, class Alloc = std::allocator<T> >
bool operator==(const RingBuffer<T,Alloc> &, const RingBuffer<T,Alloc>&);

template <class T, class Alloc = std::allocator<T> >
bool operator!=(const RingBuffer<T,Alloc> &, const RingBuffer<T,Alloc>&);

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
    
    explicit RingBuffer(size_type capacity, const Alloc &alloc = Alloc());
    RingBuffer(const RingBuffer &other);
    RingBuffer &operator=(const RingBuffer &other);
    RingBuffer(RingBuffer &&other);
    RingBuffer &operator=(RingBuffer &&other);
    ~RingBuffer();
    
    friend bool operator== <> (const RingBuffer &, const RingBuffer &);
    friend bool operator!= <> (const RingBuffer &, const RingBuffer &);
    
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    reference operator[](size_type);
    const_reference operator[](size_type) const;
    
    void clear();
    void push_back(const T&);
    void push_back(T&&);
    template<class ...Args>
    void emplace_back(Args&&...);
    void pop_front();
    
    void swap(RingBuffer& other) noexcept;
    size_type size() const;
    size_type capacity() const;
    bool empty() const;
    
    template<class Pointer, class Reference>
    class iteratorImp {
    public:
        typedef typename Alloc::difference_type difference_type;
        typedef typename Alloc::value_type value_type;
        typedef Reference reference;
        typedef Pointer pointer;
        typedef std::random_access_iterator_tag iterator_category;
        friend class RingBuffer<T, Alloc>;
        
        iteratorImp();
    private:
        iteratorImp
            (
                T *data
                , size_type start
                , size_type capacity
                , size_type current = 0
            );
    public:

        bool operator==(const iteratorImp&) const;
        bool operator!=(const iteratorImp&) const;
        bool operator<(const iteratorImp&) const;
        bool operator>(const iteratorImp&) const;
        bool operator<=(const iteratorImp&) const;
        bool operator>=(const iteratorImp&) const;
        
        iteratorImp& operator++();
        iteratorImp operator++(int);
        iteratorImp& operator--();
        iteratorImp operator--(int);
        iteratorImp& operator+=(size_type);
        iteratorImp operator+(size_type) const;
        
        friend iteratorImp operator+(size_type pos, const iteratorImp& it)
        {
            return iteratorImp(it.m_rbData, it.m_rbStart, it.m_rbCapacity, it.m_current + pos);
        }
        
        iteratorImp& operator-=(size_type); //optional
        iteratorImp operator-(size_type) const; //optional
        difference_type operator-(iteratorImp) const; //optional
        
        Reference operator*() const;
        Pointer operator->() const;
        Reference operator[](size_type) const; //optional
        
    private:
        Pointer m_rbData;
        
        size_type m_rbStart;
        size_type m_rbCapacity;
        size_type m_current;
    };
    typedef iteratorImp<T*, reference> iterator;
    
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
#include "RingBufferIterator.hpp"

#endif /* RingBuffer_h */
