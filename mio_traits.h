#ifndef MIO_TRAITS_H_INCLUDED
#define MIO_TRAITS_H_INCLUDED

/// 此文件尚未完成，无法使用

#include <cstddef>

namespace mio
{

template <typename T>
class IterTraits
{
public:
    typedef typename T::value_type value_type;
    typedef typename T::difference_type difference_type;
    typedef typename T::pointer pointer;
    typedef typename T::reference reference;
    typedef typename T::iterator_category iterator_category;
};

template <typename T>
class IterTraits<T *>
{
public:
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef typename T::random_access_iterator_tag iterator_category;
};

template <typename T>
class IterTraits<const T *>
{
public:
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef typename T::random_access_iterator_tag iterator_category;
};

}

#endif // MIO_TRAITS_H_INCLUDED
