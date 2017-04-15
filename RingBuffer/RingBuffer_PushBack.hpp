#include "RingBuffer.h"

namespace Details {
        // copy
    
    template<class T, class Alloc, bool copy>
    struct rb_help_push_back_copy_full_imp
    {
        void operator()(RingBuffer<T, Alloc> &buffer, const T& value)
        {
            buffer.push_back_full_copy_imp(value);
        }
    };
    
    template<class T, class Alloc>
    struct rb_help_push_back_copy_full_imp<T, Alloc, false>
    {
        void operator()(RingBuffer<T, Alloc> &buffer, const T& value)
        {
            buffer.push_back_destruct_construct_full_imp(value);
        }
    };
    
    // move
    
    template<class T, class Alloc, bool move>
    struct rb_help_push_back_move_full_imp
    {
        void operator()(RingBuffer<T, Alloc> &buffer, T&& value)
        {
            buffer.push_back_full_move_imp(std::move(value));
        }
    };
    
    template<class T, class Alloc>
    struct rb_help_push_back_move_full_imp<T, Alloc, false>
    {
        void operator()(RingBuffer<T, Alloc> &buffer, T&& value)
        {
            buffer.push_back_destruct_construct_full_imp(std::move(value));
        }
    };
    
    
}