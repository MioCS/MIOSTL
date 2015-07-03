#ifndef MIO_DEQUE_H_INCLUDED
#define MIO_DEQUE_H_INCLUDED

#include "mio_alloc.h"
#include <iterator>
#include <bits/stl_construct.h>

namespace mio
{

inline size_t __deque_buf_size(size_t n, size_t sz)
{
    return n ? n : (sz < 512 ? static_cast<size_t>(512 / sz) : size_t(1));
}

template <class T, class Ref, class Ptr, size_t BufSize>
class __deque_iterator
{
public:
    typedef __deque_iterator<T, T&, T*, BufSize> iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSize> iterator;

    static size_t buffer_size()
    {
        return __deque_buf_size(BufSize, sizeof(T));
    }

    typedef std::random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T** map_pointer;  // 主控
    typedef __deque_iterator self;

    // 保持与容器的联结
    T *cur;    // 此迭代器所指之缓冲区的当前元素
    T *first;  // 此迭代器所指之缓冲区的头
    T *last;   // 此迭代器所指之缓冲区的尾
    map_pointer node;  // 指向管控中心

    // 改变当前迭代器指向的缓冲区
    void set_node(map_pointer new_node)
    {
        node = new_node;
        first = *new_node;
        last = first + static_cast<difference_type>(buffer_size());
    }

    reference operator*() const
    {
        return *cur;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    // 当前迭代器位于x之后
    difference_type operator-(const self &x) const
    {
        return static_cast<difference_type>(buffer_size()) * (node - x.node - 1) +
               (x.last - x.cur) + (cur - first);
    }

    self& operator++()
    {
        if(++cur == last)  // 向后移动，如果到达缓冲区尾端，切换至下一个缓冲区
        {
            set_node(node + 1);
            cur = first;
        }

        return *this;
    }

    self& operator++(int)
    {
        self tmp = *this;

        ++*this;

        return temp;
    }


};

}  // end of namespace mio



#endif // MIO_DEQUE_H_INCLUDED
