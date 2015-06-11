#ifndef MIO_VECTOR_H_INCLUDED
#define MIO_VECTOR_H_INCLUDED

#include "mio_alloc.h"
#include "mio_construct.h"
#include <bits/stl_uninitialized.h>

namespace mio
{

typedef default_alloc alloc;

template <class T, class Alloc = alloc>
class vector
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef value_type& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

protected:
    typedef simple_alloc<value_type, Alloc> data_allocator;
    iterator start;           // 表示使用空间的头
    iterator finish;          // 表示使用空间的尾
    iterator endOfStorage;  // 表示可用空间的尾

    void insert_aux(iterator position, const T& x);

    void deallocate()
    {
        if(start)
        {
            data_allocator(start, endOfStorage - start);
        }
    }

    void fill_initialize(size_type n, const T& value)
    {
        start = allocate_and_fill(n, value);
        finish = start + n;
        endOfStorage = finish;
    }

    iterator allocate_and_fill(size_type n, const T& x)
    {
        iterator result = data_allocator::allocate(n);
        std::uninitialized_fill_n(result, n, x);

        return result;
    }

public:
    iterator begin()
    {
        return start;
    }

    iterator end()
    {
        return finish;
    }

    size_type size() const
    {
        return static_cast<size_type>(finish - start);
    }

    size_type capacity() const
    {
        return static_cast<size_type>(endOfStorage - start);
    }

    bool empty() const
    {
        return start == finish;
    }

    reference operator[](size_type n)
    {
        return *(start + n);
    }

    // constructors
    vector():
        start(nullptr), finish(nullptr), endOfStorage(nullptr)
    {
    }

    vector(size_type n, const T &value)
    {
        fill_initialize(n, value);
    }

    vector(int n, const T &value)
    {
        fill_initialize(n, value);
    }

    vector(long n, const T &value)
    {
        fill_initialize(n, value);
    }

    explicit vector(size_type n)
    {
        fill_initialize(n, value_type());
    }

    ~vector()
    {
        //TODO:
    }

    reference front()
    {
        return *start;
    }

    reference back()
    {
        return *finish;
    }

    void push_back(const T &x)
    {
        if(finish != endOfStorage)
        {
            construct(finish, x);
            ++finish;
        }
        else
        {
            insert_aux(finish, x);
        }
    }

    void pop_back()
    {
        --finish;
        destory(finish);
    }

    iterator erase(iterator position)
    {
        if(position + 1 != finish)
        {
            copy(position + 1, finish, position);
            --finish;
            destory(finish);

            return position;
        }
    }

    void resize(size_type newSize, const T &x)
    {
        if(newSize < size())
        {
            erase(begin() + newSize, end());
        }
        else
        {
            insert(end(), newSize - size(), x);
        }
    }

    void resize(size_type newSize)
    {
        resize(newSize, value_type());
    }

    void clear()
    {
        erase(begin(), end());
    }
};

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T& x)
{
    if(finish != endOfStorage)
    {
        // 有备用空间

        // 向后移动数据，插入x
        construct(finish, *(finish - 1));
        ++finish;
        // copy [position, finish - 2) to [?, finish - 1)
        std::copy_backward(position, finish - 2, finish - 1);
        *position = x;
    }
    else
    {
        // 无备用空间

        const size_type oldSize = size();
        const size_type len = oldSize ? 2 * oldSize : 1;

        iterator newStart = data_allocator::allocate(len);
        iterator newFinish = newStart;

        try
        {
            newFinish = uninitialized_copy();
        }
        catch(...)
        {

        }
    }
}

}
#endif // MIO_VECTOR_H_INCLUDED
