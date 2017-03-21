#include "RingBuffer.h"
#include <cassert>

#define RB_IMP RingBuffer<T, Alloc>
#define RB_IT_IMP RingBuffer<T, Alloc>::iteratorImp<Pointer, Reference>
#define RB_IMP_DIFF typename RingBuffer<T, Alloc>::difference_type

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP::iteratorImp
()
    : m_rbData(nullptr)
    , m_rbStart(0)
    , m_rbCapacity(1)
    , m_current(0)
{
}

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP::iteratorImp
    (
        T *data
        , RB_IMP::size_type start
        , RB_IMP::size_type capacity
        , RB_IMP::size_type current
    )
    : m_rbData(data)
    , m_rbStart(start)
    , m_rbCapacity(capacity)
    , m_current(current)
{
    
}

template <class T, class Alloc>
template<class Pointer, class Reference>
bool RB_IT_IMP::operator==(const iteratorImp &other) const
{
    if (m_rbData != other.m_rbData)
        return false;
    
    assert(m_rbStart == other.m_rbStart);
    return m_current == other.m_current;
}

template <class T, class Alloc>
template<class Pointer, class Reference>
bool RB_IT_IMP::operator!=(const iteratorImp &other) const
{
    return !(operator==(other));
}

template <class T, class Alloc>
template<class Pointer, class Reference>
bool RB_IT_IMP::operator<(const iteratorImp &other) const
{
    if (m_rbData != other.m_rbData)
        return m_rbData < other.m_rbData;
    
    assert(m_rbStart == other.m_rbStart);
    return m_current < other.m_current;
}

template <class T, class Alloc>
template<class Pointer, class Reference>
bool RB_IT_IMP::operator>(const iteratorImp &other) const
{
    return other < *this;
}

template <class T, class Alloc>
template<class Pointer, class Reference>
bool RB_IT_IMP::operator<=(const iteratorImp &other) const
{
    return !(operator>(other));
}

template <class T, class Alloc>
template<class Pointer, class Reference>
bool RB_IT_IMP::operator>=(const iteratorImp &other) const
{
    return !(operator<(other));
}

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP &RB_IT_IMP::operator++()
{
    ++m_current;
    return *this;
}

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP RB_IT_IMP::operator++(int)
{
    auto temp = *this;
    ++m_current;
    return temp;
}

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP &RB_IT_IMP::operator--()
{
    --m_current;
    return *this;
}

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP RB_IT_IMP::operator--(int)
{
    auto temp = *this;
    --m_current;
    return temp;
}

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP &RB_IT_IMP::operator+=(RB_IMP::size_type pos)
{
    m_current += pos;
    return *this;
}

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP RB_IT_IMP::operator+(RB_IMP::size_type pos) const
{
    return iteratorImp(m_rbData, m_rbStart, m_rbCapacity, m_current + pos);
}

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP &RB_IT_IMP::operator-=(RB_IMP::size_type pos)
{
    m_current -= pos;
    return *this;
}

template <class T, class Alloc>
template<class Pointer, class Reference>
RB_IT_IMP RB_IT_IMP::operator-(RB_IMP::size_type pos) const
{
    return iteratorImp(m_rbData, m_rbStart, m_rbCapacity, m_current - pos);
}

template <class T, class Alloc>
template <class Pointer, class Reference>
RB_IMP_DIFF RB_IT_IMP::operator-(const iteratorImp &other) const
{
    return (other.m_rbData - m_rbData) + other.m_current - m_current;
}

template <class T, class Alloc>
template <class Pointer, class Reference>
Reference RB_IT_IMP::operator*() const
{
    auto pos = (m_rbStart + m_current) % m_rbCapacity;
    return m_rbData[pos];
}

template <class T, class Alloc>
template <class Pointer, class Reference>
Pointer RB_IT_IMP::operator->() const
{
    auto pos = (m_rbStart + m_current) % m_rbCapacity;
    return m_rbData + pos;
}

template <class T, class Alloc>
template <class Pointer, class Reference>
Reference RB_IT_IMP::operator[](RB_IMP::size_type pos) const
{
    pos = (m_rbStart + m_current + pos) % m_rbCapacity;
    return m_rbData[pos];
}



















