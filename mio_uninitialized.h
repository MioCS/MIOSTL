#ifndef MIO_UNINITIALIZED_H_INCLUDED
#define MIO_UNINITIALIZED_H_INCLUDED

#include "mio_construct.h"
#include <type_traits>  // __true_type
#include <exception>

namespace mio {

// 以下为uninitialized_fill_n函数实现

// POD类型构造
template <class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, std::__true_type)
{
    return std::fill_n(first, n, x);
}

// 非POD类型构造
template <class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, std::__false_type)
{
    ForwardIterator cur = first;
    Size index = n;

    try
    {
        for(; n > 0; --n, ++cur)
        {
            construct(&*cur, x);
        }
    }
    catch (std::exception)
    {
        ++n, --cur;

        for(; n <= index; ++n, --cur)
        {
            destory(&*cur);
        }
    }

    return cur;
}

template <class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator
__uninitialized_fill_n(ForwardIterator first, Size n, const T&x, T1*)
{
    // 判断T的类型是否为Plain Old Data
    typedef typename __type_traits<T1>::is_POD_type is_POD;

    return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template <class ForwardIterator first, class Size, const T &x>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator first, Size n, const T &x)
{
    return __uninitialized_fill_n(first, n, x, value_type(x));
}

/*
// 以下为uninitialized_copy函数实现

template <class InputIterator, class ForwardIterator>
ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last,
                         ForwardIterator result, __true_type)
{
    return std::copy(firs, last, result);
}

template <class InputIterator, class ForwardIterator>
ForwardIterator
__uninitialized_copy_aux(InputIterator first, InputIterator last,
                         ForwardIterator result, __false_type)
{
    InputIterator indexIter = first;
    ForwardIterator cur = result;

    try
    {
        for(;first != last; ++first, ++cur)
        {
            construct(&*cur, *first);
        }
    }
    catch (exception)
    {
        --first, --cur;

        for(; first != indexIter; --first, --cur)
        {
            destory(&*cur);
        }
    }
}

template <class InputIterator, class ForwardIterator, class T>
ForwardIterator
__uninitialized_copy(InputIterator first, InputIterator last,
                   ForwardIterator result, T*)
{
    typedef typename __type_traits<T>::is_POD_type is_POD;

    return __uninitialized_copy_aux(first, last, result, is_POD);
}

template <class InputIterator, class ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last,
                   ForwardIterator result)
{
    return __uninitialized_copy(first, last, result, value_type(result));
}

// 以下为uninitialized_fill函数实现

template <class InputIterator, class T>
ForwardIterator
__uninitialized_fill_aux(InputIterator first, InputIterator last,
                         const T &x, __true_type)
{
    return std::fill(firs, last, x);
}

template <class InputIterator, class T>
ForwardIterator
__uninitialized_fill_aux(InputIterator first, InputIterator last,
                         const T &x, __false_type)
{
    InputIterator cur = first;

    try
    {
        for(; cur != last; ++cur)
        {
            construct(&*cur, x);
        }
    }
    catch (exception)
    {
        --cur;

        for(; cur != firs; --cur)
        {
            destory(&*cur);
        }
    }
}

template <class InputIterator, class T, class T1>
ForwardIterator
__uninitialized_fill(InputIterator first, InputIterator last,
                     const T &x, T1*)
{
    typedef typename __type_traits<T1>::is_POD_type is_POD;

    return __uninitialized_fill_aux(first, last, x, is_POD);
}

template <class InputIterator, class ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last,
                   const T &x)
{
    return __uninitialized_fill(first, last, result, value_type(first));
}*/

}  // end of namespace mio
#endif // MIO_UNINITIALIZED_H_INCLUDED
