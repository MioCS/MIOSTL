#ifndef MIO_ALLOCATOR_H_INCLUDED
#define MIO_ALLOCATOR_H_INCLUDED

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

using namespace std;

namespace mio
{

void out_of_memory()
{
    cerr << "out of memory" << endl;
    exit(1);
}

template <class T>
inline T* _allocate(ptrdiff_t size, T*)
{
    set_new_handler(out_of_memory);
    T* temp = (T *)(::operator new(static_cast<size_t>(size * sizeof(T))));

    return temp;
}

template <class T>
inline void _deallocate(T *buff)
{
    ::operator delete(buff);
}

template <class T1, class T2>
inline void _construct(T1 *p, const T2& value)
{
    new(p) T1(value);  // placement new
}

template <class T>
inline void _destory(T *ptr)
{
    ptr->~T;
}

template <class T>
class allocator
{
public:
    typedef T         value_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;

    template <class U>
    struct rebind
    {
        typedef allocator<U> other;
    };

    pointer allocate(size_type n, const void* hint = 0)
    {
        return _allocate(static_cast<difference_type>(n),
                         static_cast<pointer>(0));
    }

    void deallocate(pointer p, size_type n)
    {
        _deallocate(p);
    }

    void construc(pointer p, const_reference value)
    {
        _construct(p, value);
    }

    void destory(pointer p)
    {
        _destory(p);
    }

    pointer address(reference x)
    {
        return static_cast<pointer>(&x);
    }

    size_type max_size() const
    {
        return size_type(UINT_MAX / sizeof(T));
    }
};

}  // end of namespace mio


#endif // MIO_ALLOCATOR_H_INCLUDED
