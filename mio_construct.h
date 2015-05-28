#ifndef MIO_CONSTRUCT_H_INCLUDED
#define MIO_CONSTRUCT_H_INCLUDED

#include <new>

template <typename T1, typename T2>
inline void construct(T1 *p, const T2 &value)
{
    new(p) T1(value);  // placement new
}

template <typename T>
inline void destory(T *pointer)
{
    pointer->~T();
}


#endif // MIO_CONSTRUCT_H_INCLUDED
